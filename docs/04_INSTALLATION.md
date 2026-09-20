# Installation and Setup

## Software baseline

- Ubuntu 24.04 LTS
- ROS 2 Jazzy
- Python 3
- C++
- Universal Robots ROS 2 driver
- MoveIt / RViz / Gazebo as required by the project

## Clone

```bash
git clone https://github.com/marbel684/robota_sudoe_marouane.git
cd robota_sudoe_marouane
```

## Workspace

```bash
mkdir -p ~/robota_ws/src
cp -r src/* ~/robota_ws/src/
source /opt/ros/jazzy/setup.bash
cd ~/robota_ws
colcon build --symlink-install
source install/setup.bash
```

For subsequent terminals:

```bash
source /opt/ros/jazzy/setup.bash
source ~/robota_ws/install/setup.bash
```

The repository also provides `scripts/build.sh`.

## Physical startup order

1. Start KAIROS / UR5e.
2. Source ROS 2 and the workspace.
3. Start the UR5e ROS 2 driver.
4. Start External Control on the teach pendant.
5. Verify `/joint_states` and controller state.
6. Start the gripper if required.
7. Start the HEX-E path if required.
8. Verify force/torque measurements.
9. Perform a simple motion.
10. Test Pick and Place / demolding.
11. Test admittance only after the previous checks.

## UR5e driver

Use the local robot address:

```bash
ros2 launch ur_robot_driver ur_control.launch.py   ur_type:=ur5e   robot_ip:=ROBOT_IP   launch_rviz:=true
```

Replace `ROBOT_IP` locally. Do not commit laboratory network information.

## Basic verification

```bash
ros2 topic echo /joint_states --once
ros2 topic echo /force_sensor_eth --once
ros2 topic echo /gripper/position --once
```

Before physical motion, verify TCP, payload, center of gravity, operating mode, reference frame and workspace.
