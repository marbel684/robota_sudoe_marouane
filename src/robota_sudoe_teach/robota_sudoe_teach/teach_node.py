#!/usr/bin/env python3

import json
import time
from pathlib import Path

import rclpy
from rclpy.node import Node
from geometry_msgs.msg import PoseStamped, WrenchStamped
from sensor_msgs.msg import JointState
from std_srvs.srv import Trigger


class TeachNode(Node):
    """Acquisition synchronisée des données d'une démonstration."""

    def __init__(self):
        super().__init__("teach_node")

        self.declare_parameter("task_directory", "~/robota_tasks")
        self.declare_parameter("sample_period", 0.02)
        self.declare_parameter(
            "pose_topic",
            "/cartesian_compliance_controller/current_pose",
        )
        self.declare_parameter(
            "wrench_topic",
            "/force_sensor_eth",
        )

        self.task_directory = Path(
            self.get_parameter("task_directory").value
        ).expanduser()
        self.task_directory.mkdir(
            parents=True,
            exist_ok=True,
        )

        self.sample_period = float(
            self.get_parameter("sample_period").value
        )

        self.pose = None
        self.wrench = None
        self.joints = None

        self.recording = False
        self.samples = []
        self.record_start = None
        self.task_name = "demo"

        self.create_subscription(
            PoseStamped,
            self.get_parameter("pose_topic").value,
            self.pose_cb,
            20,
        )

        self.create_subscription(
            WrenchStamped,
            self.get_parameter("wrench_topic").value,
            self.wrench_cb,
            20,
        )

        self.create_subscription(
            JointState,
            "/joint_states",
            self.joint_cb,
            20,
        )

        self.create_timer(
            self.sample_period,
            self.record_cb,
        )

        self.create_service(
            Trigger,
            "/teach/start",
            self.start_cb,
        )
        self.create_service(
            Trigger,
            "/teach/stop",
            self.stop_cb,
        )
        self.create_service(
            Trigger,
            "/teach/save",
            self.save_cb,
        )

        self.get_logger().info(
            "Teach recorder ready."
        )

    def pose_cb(self, msg):
        self.pose = msg

    def wrench_cb(self, msg):
        self.wrench = msg

    def joint_cb(self, msg):
        self.joints = msg

    def record_cb(self):
        if not self.recording:
            return

        if self.pose is None or self.joints is None:
            return

        now = time.monotonic()

        sample = {
            "t": now - self.record_start,
            "pose": {
                "frame_id": self.pose.header.frame_id,
                "position": {
                    "x": self.pose.pose.position.x,
                    "y": self.pose.pose.position.y,
                    "z": self.pose.pose.position.z,
                },
                "orientation": {
                    "x": self.pose.pose.orientation.x,
                    "y": self.pose.pose.orientation.y,
                    "z": self.pose.pose.orientation.z,
                    "w": self.pose.pose.orientation.w,
                },
            },
            "joints": {
                name: position
                for name, position in zip(
                    self.joints.name,
                    self.joints.position,
                )
            },
        }

        if self.wrench is not None:
            sample["wrench"] = {
                "frame_id": self.wrench.header.frame_id,
                "force": {
                    "x": self.wrench.wrench.force.x,
                    "y": self.wrench.wrench.force.y,
                    "z": self.wrench.wrench.force.z,
                },
                "torque": {
                    "x": self.wrench.wrench.torque.x,
                    "y": self.wrench.wrench.torque.y,
                    "z": self.wrench.wrench.torque.z,
                },
            }

        self.samples.append(sample)

    def start_cb(self, request, response):
        self.samples = []
        self.record_start = time.monotonic()
        self.recording = True

        response.success = True
        response.message = "Recording started."

        self.get_logger().info(
            "Teach recording started."
        )
        return response

    def stop_cb(self, request, response):
        self.recording = False

        response.success = bool(self.samples)
        response.message = (
            f"Recording stopped: "
            f"{len(self.samples)} samples."
        )

        self.get_logger().info(
            response.message
        )
        return response

    def save_cb(self, request, response):
        if not self.samples:
            response.success = False
            response.message = (
                "No demonstration to save."
            )
            return response

        path = (
            self.task_directory
            / f"{self.task_name}.json"
        )

        data = {
            "format":
                "robota_sudoe_demonstration_v2",
            "sample_period":
                self.sample_period,
            "sample_count":
                len(self.samples),
            "samples":
                self.samples,
        }

        path.write_text(
            json.dumps(
                data,
                indent=2,
            )
        )

        response.success = True
        response.message = str(path)

        self.get_logger().info(
            f"Demonstration saved: {path}"
        )

        return response


def main():
    rclpy.init()
    node = TeachNode()

    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
