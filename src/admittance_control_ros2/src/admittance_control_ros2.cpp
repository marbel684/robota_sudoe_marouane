#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/wrench_stamped.hpp>
#include <std_msgs/msg/float64_multi_array.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include <tf2/LinearMath/Matrix3x3.h>
#include <urdf/model.h>
#include <kdl_parser/kdl_parser.hpp>
#include <kdl/chain.hpp>
#include <kdl/chainjnttojacsolver.hpp>
#include <kdl/jntarray.hpp>
#include <kdl/jacobian.hpp>
#include <Eigen/Dense>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <map>
#include <memory>
#include <string>
#include <vector>

class AdmittanceControlROS2 : public rclcpp::Node {
public:
  AdmittanceControlROS2() : Node("admittance_control") {
    sensor_ = declare_parameter<std::string>("sensor", "ethercat");
    tool_frame_ = declare_parameter<std::string>("tool_frame", "tool0");
    base_frame_ = declare_parameter<std::string>("base_frame", "base_link");

    // Virtual mass/inertia. SI units: kg and kg.m^2.
    M_trans_ = declare_parameter<double>("M_trans", 12.0);
    M_rot_ = declare_parameter<double>("M_rot", 0.10);

    // Direct damping parameters. SI units:
    // B_trans: N.s/m
    // B_rot: N.m.s/rad
    B_trans_ = declare_parameter<double>("B_trans", 8.31);
    B_rot_ = declare_parameter<double>("B_rot", 0.76);

    lambda_ = declare_parameter<double>("lambda_dls", 0.02);
    F_alpha_ = declare_parameter<double>("F_alpha", 0.08);
    V_alpha_ = declare_parameter<double>("V_alpha", 0.25);
    dead_cart_ = declare_parameter<double>("dead_cart", 1.5);
    dead_rot_ = declare_parameter<double>("dead_rot", 0.08);

    freq_ = declare_parameter<double>("freq", 250.0);

    max_linear_speed_ =
      declare_parameter<double>("max_linear_speed", 0.20);
    max_angular_speed_ =
      declare_parameter<double>("max_angular_speed", 0.35);
    max_joint_speed_ =
      declare_parameter<double>("max_joint_speed", 0.60);

    max_linear_accel_ =
      declare_parameter<double>("max_linear_accel", 0.50);
    max_angular_accel_ =
      declare_parameter<double>("max_angular_accel", 0.80);

    force_timeout_ =
      declare_parameter<double>("force_timeout", 0.20);

    if (M_trans_ <= 0.0 || M_rot_ <= 0.0 ||
        B_trans_ <= 0.0 || B_rot_ <= 0.0 ||
        freq_ <= 0.0) {
      throw std::runtime_error("Invalid admittance parameters");
    }

    M_.resize(6);
    B_.resize(6);
    M_ << M_trans_, M_trans_, M_trans_,
          M_rot_, M_rot_, M_rot_;
    B_ << B_trans_, B_trans_, B_trans_,
          B_rot_, B_rot_, B_rot_;

    v_.setZero(6);
    v_previous_.setZero(6);
    force_filter_.setZero(6);
    qdot_filter_.setZero(6);

    // The Bota frame is mechanically mounted with the rotation documented
    // by the original project. This must be validated experimentally.
    const double zcorr =
      (sensor_ == "ethercat" || sensor_ == "serial_rkb")
        ? M_PI / 2.0 : 0.0;

    const double c = std::cos(zcorr);
    const double s = std::sin(zcorr);
    correction_ << c, -s, 0.0,
                    s,  c, 0.0,
                    0.0, 0.0, 1.0;

    tf_buffer_ = std::make_shared<tf2_ros::Buffer>(get_clock());
    tf_listener_ =
      std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

    auto client =
      std::make_shared<rclcpp::SyncParametersClient>(
        this, "robot_state_publisher");

    std::string urdf;
    if (client->wait_for_service(std::chrono::seconds(5))) {
      auto params = client->get_parameters({"robot_description"});
      if (!params.empty()) {
        urdf = params[0].as_string();
      }
    }

    if (urdf.empty()) {
      throw std::runtime_error("robot_description unavailable");
    }

    urdf::Model model;
    if (!model.initString(urdf)) {
      throw std::runtime_error("URDF parse error");
    }

    KDL::Tree tree;
    if (!kdl_parser::treeFromUrdfModel(model, tree)) {
      throw std::runtime_error("KDL tree error");
    }

    if (!tree.getChain(base_frame_, tool_frame_, chain_)) {
      throw std::runtime_error("KDL chain error");
    }

    jacobian_solver_ =
      std::make_unique<KDL::ChainJntToJacSolver>(chain_);

    joint_names_ = {
      "shoulder_pan_joint",
      "shoulder_lift_joint",
      "elbow_joint",
      "wrist_1_joint",
      "wrist_2_joint",
      "wrist_3_joint"
    };

    const std::string wrench_topic =
      sensor_ == "UR"
        ? "/force_sensor_ur_zeroed"
        : "/force_sensor_eth";

    wrench_sub_ =
      create_subscription<geometry_msgs::msg::WrenchStamped>(
        wrench_topic, rclcpp::QoS(10),
        [this](const geometry_msgs::msg::WrenchStamped::SharedPtr msg) {
          last_wrench_ = *msg;
          last_force_time_ = now();
          has_wrench_ = true;
        });

    joint_sub_ =
      create_subscription<sensor_msgs::msg::JointState>(
        "/joint_states", rclcpp::QoS(10),
        [this](const sensor_msgs::msg::JointState::SharedPtr msg) {
          last_joint_ = *msg;
          has_joint_ = true;
        });

    velocity_pub_ =
      create_publisher<std_msgs::msg::Float64MultiArray>(
        "/forward_velocity_controller/commands", rclcpp::QoS(10));

    timer_ = create_wall_timer(
      std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::duration<double>(1.0 / freq_)),
      std::bind(&AdmittanceControlROS2::loop, this));

    RCLCPP_INFO(
      get_logger(),
      "Admittance V3 ready: sensor=%s, %.1f Hz",
      sensor_.c_str(), freq_);
  }

private:
  static double clamp_abs(double x, double limit) {
    return std::clamp(x, -limit, limit);
  }

  static Eigen::Vector3d apply_dead_zone(
    const Eigen::Vector3d &x, double threshold) {
    const double norm = x.norm();
    if (norm <= threshold || norm < 1e-12) {
      return Eigen::Vector3d::Zero();
    }

    // Preserve direction while removing only the threshold magnitude.
    return x * ((norm - threshold) / norm);
  }

  void publish_zero_velocity() {
    std_msgs::msg::Float64MultiArray cmd;
    cmd.data.assign(chain_.getNrOfJoints(), 0.0);
    velocity_pub_->publish(cmd);

    v_.setZero();
    v_previous_.setZero();
    qdot_filter_.setZero();
  }

  bool compute_jacobian(Eigen::MatrixXd &J) {
    std::map<std::string, size_t> index;

    for (size_t i = 0; i < last_joint_.name.size(); ++i) {
      index[last_joint_.name[i]] = i;
    }

    KDL::JntArray q(chain_.getNrOfJoints());

    for (size_t j = 0; j < joint_names_.size(); ++j) {
      const auto it = index.find(joint_names_[j]);

      if (it == index.end() ||
          it->second >= last_joint_.position.size()) {
        return false;
      }

      q(j) = last_joint_.position[it->second];
    }

    KDL::Jacobian jac(chain_.getNrOfJoints());

    if (jacobian_solver_->JntToJac(q, jac) < 0) {
      return false;
    }

    J.resize(6, chain_.getNrOfJoints());

    for (unsigned r = 0; r < 6; ++r) {
      for (unsigned c = 0;
           c < chain_.getNrOfJoints(); ++c) {
        J(r, c) = jac(r, c);
      }
    }

    return true;
  }

  void loop() {
    if (!has_joint_ || !has_wrench_) {
      return;
    }

    const double force_age =
      (now() - last_force_time_).seconds();

    if (force_age > force_timeout_) {
      publish_zero_velocity();

      RCLCPP_WARN_THROTTLE(
        get_logger(), *get_clock(), 2000,
        "Force sensor timeout: velocity command stopped");

      return;
    }

    Eigen::Vector3d force(
      last_wrench_.wrench.force.x,
      last_wrench_.wrench.force.y,
      last_wrench_.wrench.force.z);

    Eigen::Vector3d torque(
      last_wrench_.wrench.torque.x,
      last_wrench_.wrench.torque.y,
      last_wrench_.wrench.torque.z);

    try {
      const auto tf =
        tf_buffer_->lookupTransform(
          base_frame_, tool_frame_, tf2::TimePointZero,
          tf2::durationFromSec(0.02));

      tf2::Quaternion q(
        tf.transform.rotation.x,
        tf.transform.rotation.y,
        tf.transform.rotation.z,
        tf.transform.rotation.w);

      tf2::Matrix3x3 tf_rotation(q);
      Eigen::Matrix3d R;

      for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
          R(r, c) = tf_rotation[r][c];
        }
      }

      force = R * correction_ * force;
      torque = R * correction_ * torque;

    } catch (...) {
      publish_zero_velocity();
      return;
    }

    force = apply_dead_zone(force, dead_cart_);
    torque = apply_dead_zone(torque, dead_rot_);

    Eigen::VectorXd wrench(6);
    wrench << force.x(), force.y(), force.z(),
              torque.x(), torque.y(), torque.z();

    // First-order low-pass filter.
    force_filter_ =
      (1.0 - F_alpha_) * force_filter_
      + F_alpha_ * wrench;

    const double dt = 1.0 / freq_;

    // M * a + B * v = F
    Eigen::VectorXd acceleration =
      (force_filter_
       - B_.cwiseProduct(v_))
      .cwiseQuotient(M_);

    v_ += acceleration * dt;

    // Cartesian velocity limits.
    Eigen::Vector3d v_linear = v_.head<3>();
    Eigen::Vector3d v_angular = v_.tail<3>();

    if (v_linear.norm() > max_linear_speed_) {
      v_linear *= max_linear_speed_ / v_linear.norm();
      v_.head<3>() = v_linear;
    }

    if (v_angular.norm() > max_angular_speed_) {
      v_angular *= max_angular_speed_ / v_angular.norm();
      v_.tail<3>() = v_angular;
    }

    // Correct acceleration limiter:
    // |v(k)-v(k-1)| <= a_max * dt.
    const double max_dv_linear =
      max_linear_accel_ * dt;
    const double max_dv_angular =
      max_angular_accel_ * dt;

    for (int i = 0; i < 3; ++i) {
      const double requested_delta =
        v_(i) - v_previous_(i);

      const double applied_delta =
        std::clamp(
          requested_delta,
          -max_dv_linear,
          max_dv_linear);

      v_(i) =
        v_previous_(i) + applied_delta;
    }

    for (int i = 3; i < 6; ++i) {
      const double requested_delta =
        v_(i) - v_previous_(i);

      const double applied_delta =
        std::clamp(
          requested_delta,
          -max_dv_angular,
          max_dv_angular);

      v_(i) =
        v_previous_(i) + applied_delta;
    }

    v_previous_ = v_;

    Eigen::MatrixXd J;

    if (!compute_jacobian(J)) {
      publish_zero_velocity();
      return;
    }

    // Damped least-squares pseudoinverse.
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(
      J,
      Eigen::ComputeThinU | Eigen::ComputeThinV);

    const Eigen::VectorXd singular =
      svd.singularValues();

    Eigen::VectorXd damped(singular.size());

    for (int i = 0; i < singular.size(); ++i) {
      damped(i) =
        singular(i) /
        (singular(i) * singular(i)
         + lambda_ * lambda_);
    }

    Eigen::MatrixXd J_pinv =
      svd.matrixV()
      * damped.asDiagonal()
      * svd.matrixU().transpose();

    Eigen::VectorXd qdot =
      J_pinv * v_;

    if (!qdot_filter_.size()) {
      qdot_filter_ = qdot;
    } else {
      qdot_filter_ =
        V_alpha_ * qdot
        + (1.0 - V_alpha_) * qdot_filter_;
    }

    for (int i = 0; i < qdot_filter_.size(); ++i) {
      qdot_filter_(i) =
        clamp_abs(
          qdot_filter_(i),
          max_joint_speed_);
    }

    std_msgs::msg::Float64MultiArray cmd;

    cmd.data.assign(
      qdot_filter_.data(),
      qdot_filter_.data() + qdot_filter_.size());

    velocity_pub_->publish(cmd);
  }

  std::string sensor_;
  std::string tool_frame_;
  std::string base_frame_;

  double M_trans_;
  double M_rot_;
  double B_trans_;
  double B_rot_;

  double lambda_;
  double F_alpha_;
  double V_alpha_;
  double dead_cart_;
  double dead_rot_;
  double freq_;

  double max_linear_speed_;
  double max_angular_speed_;
  double max_joint_speed_;

  double max_linear_accel_;
  double max_angular_accel_;
  double force_timeout_;

  Eigen::VectorXd M_;
  Eigen::VectorXd B_;
  Eigen::VectorXd v_;
  Eigen::VectorXd v_previous_;
  Eigen::VectorXd force_filter_;
  Eigen::VectorXd qdot_filter_;

  Eigen::Matrix3d correction_;

  bool has_wrench_{false};
  bool has_joint_{false};

  geometry_msgs::msg::WrenchStamped last_wrench_;
  sensor_msgs::msg::JointState last_joint_;
  rclcpp::Time last_force_time_;

  rclcpp::Subscription<geometry_msgs::msg::WrenchStamped>::SharedPtr
    wrench_sub_;

  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr
    joint_sub_;

  rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr
    velocity_pub_;

  rclcpp::TimerBase::SharedPtr timer_;

  std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;

  KDL::Chain chain_;
  std::unique_ptr<KDL::ChainJntToJacSolver> jacobian_solver_;

  std::vector<std::string> joint_names_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);

  try {
    rclcpp::spin(
      std::make_shared<AdmittanceControlROS2>());
  } catch (const std::exception &e) {
    RCLCPP_ERROR(
      rclcpp::get_logger("admittance"),
      "%s", e.what());
  }

  rclcpp::shutdown();
  return 0;
}
