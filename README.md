# ROBOTA-SUDOE — UR5e ROS 2 Control and Admittance

**Lab:** CiTIUS, University of Santiago de Compostela, Spain  
**Project:** ROBOTA-SUDOE  
**Robot:** Universal Robots UR5e on the KAIROS workcell  
**OS:** Ubuntu 24.04 LTS  
**ROS 2:** Jazzy Jalisco  

---

## What is this project?

This repository contains the ROS 2 software developed during my internship at
CiTIUS.

The project focuses on the control of a UR5e robot for a collaborative
manipulation task. The robot is used for Pick and Place and demolding
operations with a Schunk EGK50 gripper.

The project also includes the integration of an OnRobot HEX-E force/torque
sensor and the implementation of an admittance controller. The objective is to
make the robot react to forces applied by the user instead of following only
predefined positions.

I worked on the project progressively, starting with the ROS 2 environment and
robot control, then the gripper and mechanical integration, and finally the
force-guided control.

---

## Hardware Overview

| Component | Description |
|-----------|-------------|
| UR5e | 6-DOF collaborative robot |
| KAIROS | Robotic workcell |
| SCHUNK EGK50 | Electric 2-finger gripper |
| OnRobot HEX-E | 6-axis force/torque sensor |
| Development PC | Computer running ROS 2 Jazzy |

The exact network configuration is not included in this repository because it
depends on the local laboratory setup.

---

## Software Stack

<p align="left">
  <img src="https://img.shields.io/badge/Ubuntu-24.04-E95420?style=for-the-badge&logo=ubuntu&logoColor=white" alt="Ubuntu 24.04">
  <img src="https://img.shields.io/badge/ROS_2-Jazzy-22314E?style=for-the-badge&logo=ros&logoColor=white" alt="ROS 2 Jazzy">
  <img src="https://img.shields.io/badge/Python-3.x-3776AB?style=for-the-badge&logo=python&logoColor=white" alt="Python">
  <img src="https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++">
</p>

<p align="left">
  <img src="https://img.shields.io/badge/MoveIt-ROS_2-22314E?style=for-the-badge&logo=ros&logoColor=white" alt="MoveIt">
  <img src="https://img.shields.io/badge/RViz-ROS_2-22314E?style=for-the-badge&logo=ros&logoColor=white" alt="RViz">
  <img src="https://img.shields.io/badge/Gazebo-Harmonic-7A7A7A?style=for-the-badge" alt="Gazebo Harmonic">
  <img src="https://img.shields.io/badge/Eigen-C%2B%2B-00599C?style=for-the-badge" alt="Eigen">
</p>

<p align="left">
  <img src="https://img.shields.io/badge/KDL-Robot%20Kinematics-444444?style=for-the-badge" alt="KDL">
  <img src="https://img.shields.io/badge/Universal%20Robots-UR5e-000000?style=for-the-badge" alt="Universal Robots UR5e">
  <img src="https://img.shields.io/badge/rokubimini__ethercat-EtherCAT-444444?style=for-the-badge" alt="rokubimini_ethercat">
</p>

---

## Repository Structure

```text
robota_sudoe_marouane/
├── README.md
├── config/
│   ├── admittance_teach.yaml
│   ├── bota_hex_e.json
│   └── teach.yaml
├── docs/
│   ├── SETUP.md
│   ├── SCHUNK_EGK_SETUP.md
│   └── TROUBLESHOOTING.md
├── legacy/
│   ├── compliance_demo.py
│   ├── fastdds_no_shm.xml
│   ├── force_sensor_eth_publisher_ros2.py
│   ├── schunk_gripper_node.py
│   └── ur_force_zeroed.py
├── scripts/
│   ├── build.sh
│   └── check_bota.sh
├── src/
│   ├── admittance_control_ros2/
│   ├── robota_sudoe_bringup/
│   ├── robota_sudoe_imitation/
│   └── robota_sudoe_teach/
├── validation/
│   ├── README.md
│   ├── validate_six_axis.py
│   └── validation_output.txt
├── CHANGELOG.md
├── MIGRATION.md
└── VALIDATION_REPORT.md
```

The files in `legacy/` are previous tests and experiments kept for reference.

---

# Installation

## Requirements

The project was developed with:

```text
Ubuntu 24.04 LTS
ROS 2 Jazzy
Python 3
C++
```

For the complete physical setup, the required Universal Robots, MoveIt and
force sensor packages must also be installed.

The repository itself contains the project packages and the validation code.
The exact robot network and laboratory configuration must be adapted locally.

## Clone the repository

```bash
git clone https://github.com/marbel684/robota_sudoe_marouane.git
cd robota_sudoe_marouane
```

Create a ROS 2 workspace:

```bash
mkdir -p ~/robota_ws/src
```

Copy the project source packages into the workspace:

```bash
cp -r src/* ~/robota_ws/src/
```

Source ROS 2 Jazzy:

```bash
source /opt/ros/jazzy/setup.bash
```

Build the workspace:

```bash
cd ~/robota_ws
colcon build --symlink-install
```

Then source the workspace:

```bash
source ~/robota_ws/install/setup.bash
```

For new terminals, source both environments again:

```bash
source /opt/ros/jazzy/setup.bash
source ~/robota_ws/install/setup.bash
```

A build script is also available:

```bash
cd ~/robota_ws
bash /path/to/robota_sudoe_marouane/scripts/build.sh
```

---

# Startup Order

For the physical robot, I recommend following this order:

```text
1. Start the KAIROS and the UR5e
2. Start the ROS 2 environment
3. Start the UR5e ROS 2 driver
4. Start External Control on the teach pendant
5. Check /joint_states and the controller state
6. Start the SCHUNK gripper if it is required
7. Start the HEX-E sensor if it is required
8. Check the force/torque measurements
9. Test a simple robot motion
10. Test the predefined Pick and Place / demolding sequence
11. Only then test admittance control
```

The admittance controller should not be started directly on the real robot
without checking the robot configuration and the force measurement first.

---

# Step-by-Step Startup

## 1. Start the robot

Turn on the UR5e and wait for PolyScope to start.

Before enabling motion, check:

- robot status;
- operating mode;
- TCP;
- payload;
- center of gravity;
- workspace.

The robot configuration must match the actual mechanical setup.

---

## 2. Start ROS 2

Open a terminal and run:

```bash
source /opt/ros/jazzy/setup.bash
source ~/robota_ws/install/setup.bash
```

Check that ROS 2 is available:

```bash
ros2 node list
```

---

## 3. Start the UR5e ROS 2 driver

The UR5e is controlled with the Universal Robots ROS 2 driver.

A typical command is:

```bash
source /opt/ros/jazzy/setup.bash
ros2 launch ur_robot_driver ur_control.launch.py \
  ur_type:=ur5e \
  robot_ip:=ROBOT_IP \
  launch_rviz:=true
```

Replace `ROBOT_IP` with the address used in the local laboratory setup.

Wait for the robot hardware to become active.

Then check the joint states:

```bash
ros2 topic echo /joint_states --once
```

The message should contain the six UR5e joint states.

---

## 4. Enable External Control

Open the External Control program on the UR5e teach pendant and start it.

The ROS 2 driver should then connect to the robot and be able to receive
commands.

Before moving the robot, check again the TCP, payload and operating mode.

---

## 5. Test a simple motion

The first physical tests should be done with low speed and small motions.

Start with one pose or one short trajectory before running the complete
sequence.

During the project, some commanded trajectories were not initially reproduced
as expected. The TCP, reference frames, trajectory orientation and timing had
to be checked before continuing with the complete tests.

---

# SCHUNK EGK50 Gripper

The SCHUNK EGK50 is controlled through a ROS 2 node using TCP communication.

The main source file is:

```text
legacy/schunk_gripper_node.py
```

The related setup information is also kept in:

```text
docs/SCHUNK_EGK_SETUP.md
```

After starting the gripper node, check the topics:

```bash
ros2 topic list | grep gripper
```

The main topics are:

```text
/gripper/action
/gripper/command
/gripper/position
```

Open the gripper:

```bash
ros2 topic pub --once /gripper/action std_msgs/msg/String "data: 'open'"
```

Close the gripper:

```bash
ros2 topic pub --once /gripper/action std_msgs/msg/String "data: 'close'"
```

Read the gripper position:

```bash
ros2 topic echo /gripper/position --once
```

The OnRobot gripper was also tested during the project, but it was kept as a
secondary option after communication problems were encountered.

---

# HEX-E Force/Torque Sensor

The OnRobot HEX-E sensor is used to measure forces and torques during the
interaction with the robot.

The force sensor path uses `rokubimini_ethercat` and the project publisher
available in `legacy/`.

Before using the sensor for admittance control, check:

- communication;
- zero offset;
- sensor frame;
- force direction and sign;
- noise at rest;
- calibration.

Check the sensor topic:

```bash
ros2 topic echo /force_sensor_eth --once
```

Check the topic frequency:

```bash
ros2 topic hz /force_sensor_eth
```

The sensor was calibrated using known loads during the project. A force of
approximately 5 N was measured after calibration.

---

# Verify the System

Before running the complete manipulation sequence, check the main topics.

## Robot joint states

```bash
ros2 topic echo /joint_states --once
```

## HEX-E sensor

```bash
ros2 topic echo /force_sensor_eth --once
```

## Gripper

```bash
ros2 topic echo /gripper/position --once
```

## Velocity controller

```bash
ros2 topic echo /forward_velocity_controller/commands --once
```

The last topic is used by the admittance controller to send joint velocity
commands.

---

# Pick and Place / Demolding

The predefined sequence is based on:

```text
HOME
  ↓
PICK
  ↓
PLACE
  ↓
HOME
```

The robot trajectory and the gripper commands are synchronized from ROS 2.

This sequence was tested on the physical KAIROS workcell and was then used as
the basis for the demolding task.

The custom 3D-printed fingers can grasp, demold and place the object correctly.
The main limitation is the mechanical strength of the PLA fingers. They can
deform or break when higher forces are required during demanding demolding
operations.

---

# Admittance Control

The admittance controller is implemented in:

```text
src/admittance_control_ros2/src/admittance_control_ros2.cpp
```

The node installed by the package is:

```text
admittance_control_node
```

The controller is based on:

```text
M.a + B.v = F
```

The measured force is used to calculate a Cartesian velocity. This velocity is
then converted to joint velocities using the robot Jacobian.

A damped least squares pseudoinverse is used to reduce problems near singular
or badly conditioned configurations.

## Build only the controller package

```bash
cd ~/robota_ws
source /opt/ros/jazzy/setup.bash
colcon build --packages-select admittance_control_ros2 --symlink-install
source ~/robota_ws/install/setup.bash
```

Check that the executable is available:

```bash
ros2 pkg executables admittance_control_ros2
```

## Start the controller

The controller can be started after the robot, joint states and force sensor
are already available.

For the HEX-E path, the source code uses:

```text
sensor = ethercat
```

The executable can be started with:

```bash
ros2 run admittance_control_ros2 admittance_control_node --ros-args -p sensor:=ethercat
```

The controller uses the default parameters from the node unless other
parameters are provided.

Do not start it on the physical robot before checking the active controller,
force signal, TCP, payload and workspace.

---

## Development Parameters

The current numerical validation uses:

```text
Control frequency        : 250 Hz
Control period           : 0.004 s

M_trans                  : 12.0 kg
M_rot                    : 0.10
B_trans                  : 8.31 N.s/m
B_rot                    : 0.76

Force dead zone          : 1.5 N
Torque dead zone         : 0.08 Nm

Maximum linear speed     : 0.20 m/s
Maximum angular speed    : 0.35 rad/s

Maximum linear accel.    : 0.50 m/s^2
Maximum angular accel.   : 0.80 rad/s^2

Maximum joint speed      : 0.60 rad/s

DLS damping              : 0.02
```

These are development parameters used for the controller and numerical
validation. They are not final experimentally tuned parameters.

---

# Velocity and Acceleration Limits

The controller runs at:

```text
f = 250 Hz
dt = 0.004 s
```

The maximum Cartesian acceleration is:

```text
a_max = 0.50 m/s^2
```

Therefore, the maximum velocity change during one control cycle is:

```text
Delta v_max = a_max * dt
            = 0.50 * 0.004
            = 0.002 m/s
```

The Cartesian velocity is also limited to:

```text
v_max = 0.20 m/s
```

The acceleration limit and the velocity limit are two different constraints.

---

# Numerical Validation

The validation code is located in:

```text
validation/validate_six_axis.py
```

It does not command motion to the physical robot.

Run it with:

```bash
cd validation
python3 validate_six_axis.py
```

The validation checks:

- translational admittance;
- rotational admittance;
- velocity limits;
- acceleration limits;
- Jacobian calculation;
- damped least squares inversion;
- behaviour near singular configurations.

The current validation completes without commanding any robot movement.

For a 5 N force and a 1.5 N dead zone, the effective force used in the
translational model is 3.5 N. With `B_trans = 8.31 N.s/m`, the theoretical
steady-state speed before saturation is approximately 0.421 m/s.

The controller then limits the speed to 0.20 m/s.

---

# Teaching and Imitation

The teaching node is located in:

```text
src/robota_sudoe_teach/
```

The recorded data includes:

- time;
- joint positions;
- TCP pose;
- force/torque measurements.

The data can then be processed by:

```text
src/robota_sudoe_imitation/
```

This part was explored during the internship and is less mature than the robot
control and physical manipulation sequence.

The current priority is the physical validation of the force-guided admittance
loop.

---

# Mechanical Integration

The mechanical work included:

- CAD design of the adapter plates;
- CAD design of the custom demolding fingers;
- 3D printing;
- mechanical assembly;
- TCP verification;
- dimensional adjustments.

The printed fingers were tested on the physical workcell.

The current PLA design is sufficient for the tested manipulation sequence but
is not robust enough for all demolding forces. A stronger version is being
considered.

---

# Common Problems

## ROS 2 package not found

Source ROS 2 and the workspace again:

```bash
source /opt/ros/jazzy/setup.bash
source ~/robota_ws/install/setup.bash
```

Then rebuild if necessary:

```bash
cd ~/robota_ws
colcon build --symlink-install
```

---

## Robot does not move as expected

Check:

- TCP;
- reference frame;
- robot orientation;
- payload;
- trajectory timing;
- controller state;
- robot operating mode.

Start with simple motions before running the complete sequence.

---

## Protective stop

If the robot enters a protective stop:

1. Stop the commanded motion.
2. Check the status in PolyScope.
3. Check the TCP and payload.
4. Check the trajectory and reference frame.
5. Reduce speed and acceleration.
6. Retry with a simpler movement.

Do not continue testing without identifying the cause of the stop.

---

## HEX-E values are incorrect

Check:

- sensor mounting;
- sensor orientation;
- sensor frame;
- zero offset;
- calibration;
- communication;
- force topic.

The sensor should be calibrated with a known load before using it for
admittance control.

---

## SCHUNK gripper does not respond

Check:

- network connection;
- TCP communication;
- gripper node;
- robot connection;
- gripper topics.

Test the gripper independently before using it inside the complete sequence.

---

# Main ROS 2 Topics

| Topic | Type | Description |
|-------|------|-------------|
| `/joint_states` | `sensor_msgs/JointState` | UR5e joint states |
| `/force_sensor_eth` | `geometry_msgs/WrenchStamped` | HEX-E force/torque data |
| `/force_sensor_ur_zeroed` | `geometry_msgs/WrenchStamped` | Zeroed UR internal sensor |
| `/gripper/action` | `std_msgs/String` | Gripper command |
| `/gripper/position` | `std_msgs/Float64` | Gripper position |
| `/forward_velocity_controller/commands` | `std_msgs/Float64MultiArray` | Joint velocity commands |

---

# Safety

This repository was developed for a real collaborative robot.

Before testing code on the physical robot:

- check the robot state;
- check the TCP;
- check the payload and center of gravity;
- check the force sensor;
- check the reference frame;
- use low speed for the first tests;
- use small motions first;
- verify the active controller;
- keep the workspace clear.

The complete physical admittance loop should only be tested after the force
measurement and robot configuration have been checked.

---

# Current Project Status

| Part | Status |
|------|--------|
| ROS 2 Jazzy migration | Completed |
| UR5e ROS 2 integration | Completed |
| URDF/Xacro/SRDF work | Completed |
| MoveIt / RViz / Gazebo setup | Completed |
| SCHUNK EGK50 communication | Completed |
| Mechanical integration | Completed |
| Pick and Place | Tested on robot |
| Demolding sequence | Tested on robot |
| HEX-E calibration | Validated |
| Admittance controller | Implemented |
| Admittance numerical validation | Completed |
| Jacobian / DLS validation | Completed |
| Physical admittance loop | To be completed |
| Teaching by demonstration | Prepared / exploratory |

---

# Notes

The `legacy/` directory contains previous tests and experimental nodes kept for
reference.

The predefined manipulation sequence was tested on the physical robot.

The HEX-E communication and calibration were also validated.

The complete physical validation of the force-guided admittance controller is
still the next step.

---

# Author
Marouane Belhaddade Zanati — SIGMA Clermont (May–Sept 2026)
Mohamed Ali Jomaa Ghouil — SIGMA Clermont / ENISo (May–Sept 2026)
