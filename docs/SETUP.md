# Setup notes

This document describes the checks used before running the ROS 2 control stack on the KAIROS cell. Network addresses are intentionally left out of this public repository and should be filled in from the local laboratory configuration.

## 1. Start the hardware

Power on the KAIROS base and the UR5e arm. Wait for the controller and the onboard computer to finish starting.

## 2. Check the robot controller

Open PolyScope and verify that the robot is ready. Check the active program and make sure that no other external control application is connected to the robot.

A second RTDE client can prevent the ROS 2 driver from taking control of the arm. If an old ROS 1 process is started automatically on the onboard computer, stop it before starting the ROS 2 driver.

## 3. Check tool communication

Verify the Tool I/O settings in PolyScope when a gripper is connected directly to the robot tool flange. The baud rate and parity must match the gripper configuration.

The Schunk EGK50 was used for the main tests. The OnRobot gripper was kept as a secondary option because of communication problems encountered during the project.

## 4. Start the UR ROS 2 driver

Use the Universal Robots ROS 2 driver configuration used on the laboratory machine. The robot IP address must be supplied locally.

Example:

```bash
source /opt/ros/jazzy/setup.bash
ros2 launch ur_robot_driver ur_control.launch.py \
  ur_type:=ur5e \
  robot_ip:=ROBOT_IP \
  launch_rviz:=true
```

Wait until the hardware becomes active.

## 5. Start External Control on the pendant

Open the External Control program in PolyScope and start it from the pendant. The ROS 2 driver should then report that the robot has connected to the reverse interface.

## 6. Check ROS 2 topics and controllers

```bash
ros2 control list_controllers
ros2 topic echo /joint_states --once
```

Check that the expected trajectory or velocity controller is active before sending commands.

## 7. Check the force sensor

For the Bota/HEX path, first verify that the sensor data is available without moving the robot.

```bash
ros2 topic hz /force_sensor_eth
ros2 topic echo /force_sensor_eth --once
```

The force and torque values must be checked for zero offset, noise, frame orientation and sign convention before starting admittance control.

## 8. First motion test

Start with a known configuration and low speed. Do not begin with the complete teaching sequence. Check one motion at a time and confirm that the robot follows the intended reference frame and TCP configuration.
