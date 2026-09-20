# ROBOTA-SUDOE — UR5e ROS 2 Control and Admittance

**CiTIUS — University of Santiago de Compostela, Spain**  
**Project:** ROBOTA-SUDOE  
**Robot:** Universal Robots UR5e mounted on a KAIROS mobile manipulator workcell  
**OS:** Ubuntu 24.04 LTS  
**ROS:** ROS 2 Jazzy

---

## Project Overview

This repository contains the ROS 2 control, integration, validation and experimentation work developed for the **ROBOTA-SUDOE** project.

The main objective is to develop a collaborative robotic manipulation system based on a **UR5e robot**, a **SCHUNK EGK50 gripper** and an **OnRobot HEX-E force/torque sensor**.

The project focuses on:

- UR5e integration with ROS 2 Jazzy
- Robot motion and controller configuration
- SCHUNK EGK50 gripper integration
- OnRobot HEX-E force/torque sensing
- Admittance control
- Pick and Place manipulation
- Demolding experiments
- Teaching by demonstration
- Trajectory recording and imitation
- Numerical validation of the admittance controller
- Mechanical integration and custom tooling

The repository contains both the software developed for the project and the documentation required to reproduce and understand the experimental setup.

---

## System

The robotic system is composed of:

- **Universal Robots UR5e**
- **Robotnik KAIROS** mobile manipulator/workcell
- **SCHUNK EGK50** electric gripper
- **OnRobot HEX-E** six-axis force/torque sensor
- Custom mechanical adapters and 3D-printed gripper fingers
- Development and control PC

The UR5e is controlled through ROS 2 using the Universal Robots ROS 2 driver.

The force/torque sensor is used as the external wrench source for the admittance controller.

---

## Hardware

| Component | Description |
|---|---|
| UR5e | 6-axis collaborative robotic arm |
| KAIROS | Mobile robotic platform / workcell |
| SCHUNK EGK50 | Electric parallel gripper |
| OnRobot HEX-E | Six-axis force/torque sensor |
| Custom tooling | Adapter plates and 3D-printed demolding fingers |
| Development PC | ROS 2 development and control environment |

---

## Software

The project is based on:

- Ubuntu 24.04 LTS
- ROS 2 Jazzy
- `ros2_control`
- Universal Robots ROS 2 Driver
- MoveIt 2
- RViz
- Gazebo
- C++
- Python
- Eigen
- Orocos KDL
- TF2
- URDF / Xacro / SRDF

---

## Repository Structure

```text
robota_sudoe_marouane/
│
├── config/
│   ├── admittance_teach.yaml
│   ├── bota_hex_e.json
│   └── teach.yaml
│
├── docs/
│   ├── SETUP.md
│   ├── SCHUNK_EGK_SETUP.md
│   ├── TROUBLESHOOTING.md
│   ├── 01_PROJECT_OVERVIEW.md
│   ├── 02_ARCHITECTURE.md
│   ├── 04_INSTALLATION.md
│   ├── 05_SCHUNK_EGK50.md
│   ├── 06_HEX_E.md
│   ├── 07_ADMITTANCE.md
│   ├── 08_TEACHING_IMITATION.md
│   ├── 09_PICK_PLACE_DEMOLDING.md
│   ├── 10_VALIDATION.md
│   ├── 13_TROUBLESHOOTING.md
│   └── 14_DEVELOPMENT_HISTORY.md
│
├── legacy/
│   ├── compliance_demo.py
│   ├── fastdds_no_shm.xml
│   ├── force_sensor_eth_publisher_ros2.py
│   ├── schunk_gripper_node.py
│   └── ur_force_zeroed.py
│
├── scripts/
│   ├── build.sh
│   └── check_bota.sh
│
├── src/
│   ├── admittance_control_ros2/
│   ├── robota_sudoe_bringup/
│   ├── robota_sudoe_imitation/
│   └── robota_sudoe_teach/
│
├── validation/
│   ├── README.md
│   ├── validate_six_axis.py
│   └── validation_output.txt
│
├── CHANGELOG.md
├── MIGRATION.md
├── README.md
└── VALIDATION_REPORT.md
```

---

## Documentation

The project documentation is divided into dedicated sections.

### Project and Architecture

- [Project Overview](docs/01_PROJECT_OVERVIEW.md)
- [System Architecture](docs/02_ARCHITECTURE.md)
- [Development History](docs/14_DEVELOPMENT_HISTORY.md)

### Installation and Setup

- [Installation](docs/04_INSTALLATION.md)
- [Initial Setup](docs/SETUP.md)
- [Troubleshooting](docs/TROUBLESHOOTING.md)
- [Extended Troubleshooting](docs/13_TROUBLESHOOTING.md)

### Hardware Integration

- [SCHUNK EGK50](docs/05_SCHUNK_EGK50.md)
- [SCHUNK Setup](docs/SCHUNK_EGK_SETUP.md)
- [HEX-E Force/Torque Sensor](docs/06_HEX_E.md)

### Control

- [Admittance Control](docs/07_ADMITTANCE.md)
- [Pick & Place / Demolding](docs/09_PICK_PLACE_DEMOLDING.md)

### Teaching and Validation

- [Teaching and Imitation](docs/08_TEACHING_IMITATION.md)
- [Numerical Validation](docs/10_VALIDATION.md)
- [Validation Report](VALIDATION_REPORT.md)

---

# Installation

## Requirements

The current development environment is based on:

```text
Ubuntu 24.04 LTS
ROS 2 Jazzy
Python 3
C++
colcon
```

The ROS 2 workspace should be configured and sourced before building the project.

For the complete installation procedure:

[Installation and Setup](docs/04_INSTALLATION.md)

---

## Build

From the ROS 2 workspace:

```bash
source /opt/ros/jazzy/setup.bash

colcon build --symlink-install

source install/setup.bash
```

A helper script is also available:

```bash
./scripts/build.sh
```

---

# Startup Procedure

The robotic system should be started progressively.

The recommended order is:

1. Start the KAIROS / UR5e system.
2. Start the ROS 2 environment.
3. Start the UR5e ROS 2 driver.
4. Activate **External Control** on the UR pendant.
5. Verify the robot state and controllers.
6. Start the gripper if required.
7. Start the HEX-E force/torque sensor if required.
8. Verify force/torque measurements.
9. Perform a simple robot motion test.
10. Run Pick and Place or demolding experiments.
11. Start the admittance controller only after the previous checks are successful.

Detailed procedures are available in:

[SETUP.md](docs/SETUP.md)

---

# UR5e ROS 2 Control

The UR5e is controlled using the Universal Robots ROS 2 driver.

A typical launch command is:

```bash
ros2 launch ur_robot_driver ur_control.launch.py \
  ur_type:=ur5e \
  robot_ip:=ROBOT_IP \
  launch_rviz:=true
```

The robot must be configured correctly on the pendant and the **External Control** program must be active before ROS 2 can command the robot.

Important topics include:

```text
/joint_states
/forward_velocity_controller/commands
```

The controller and robot state should always be verified before running higher-level applications.

---

# SCHUNK EGK50 Gripper

The project integrates a **SCHUNK EGK50** electric gripper.

The gripper is used for manipulation tasks including Pick and Place and demolding.

The repository contains:

```text
legacy/schunk_gripper_node.py
```

and the corresponding documentation:

[SCHUNK EGK50 Documentation](docs/05_SCHUNK_EGK50.md)

[SCHUNK Setup](docs/SCHUNK_EGK_SETUP.md)

Main ROS topics include:

```text
/gripper/action
/gripper/command
/gripper/position
```

The gripper can be commanded to open and close through the ROS interface used by the project.

The mechanical integration also includes:

- Custom adapter plate
- Custom 3D-printed fingers
- Mechanical mounting
- TCP verification
- Demolding tooling

---

# HEX-E Force/Torque Sensor

An **OnRobot HEX-E** six-axis force/torque sensor is used as an external wrench source.

The sensor is integrated through the EtherCAT communication stack.

The project uses:

```text
rokubimini_ethercat
```

and publishes the measured wrench through:

```text
/force_sensor_eth
```

The sensor provides:

```text
Fx
Fy
Fz
Tx
Ty
Tz
```

The sensor configuration is stored in:

```text
config/bota_hex_e.json
```

The project also contains a checking script:

```bash
./scripts/check_bota.sh
```

Documentation:

[HEX-E Documentation](docs/06_HEX_E.md)

---

# Admittance Control

The main control development of the project is the implementation of a Cartesian admittance controller.

The controller is implemented in:

```text
src/admittance_control_ros2/src/admittance_control_ros2.cpp
```

The ROS 2 node is:

```text
admittance_control_node
```

The controller follows the basic admittance relationship:

```text
M a + B v = F
```

where:

- `M` is the virtual mass
- `B` is the damping
- `a` is the Cartesian acceleration
- `v` is the Cartesian velocity
- `F` is the external force/torque

The resulting Cartesian velocity is converted into joint velocities using the robot Jacobian.

A damped least-squares pseudoinverse is used to improve numerical behavior near singular configurations.

---

## Admittance Parameters

The current development configuration contains:

```text
Control frequency       250 Hz
dt                      0.004 s

M_trans                 12.0 kg
M_rot                   0.10 kg.m²

B_trans                 8.31 N.s/m
B_rot                   0.76 N.m.s/rad

Force dead zone         1.5 N
Torque dead zone        0.08 Nm

Max linear speed        0.20 m/s
Max angular speed       0.35 rad/s

Max linear acceleration 0.50 m/s²
Max angular acceleration 0.80 rad/s²

Max joint speed         0.60 rad/s

DLS damping             0.02
```

These values are **development parameters** and are not considered final experimentally tuned values.

The physical admittance loop remains a separate experimental step requiring further calibration and testing.

Detailed explanation:

[Admittance Control Documentation](docs/07_ADMITTANCE.md)

---

# Safety and Control Limits

The controller includes several protection mechanisms.

## Velocity limits

Cartesian linear and angular velocities are limited.

## Acceleration limits

The change in velocity between two control cycles is limited according to:

```text
|v(k) - v(k-1)| <= a_max * dt
```

This prevents abrupt velocity changes.

## Joint velocity limits

The generated joint velocities are also limited.

## Force dead zones

Small force and torque values are ignored to reduce sensitivity to measurement noise.

## Force timeout

If no valid wrench is received within the configured timeout, the controller commands zero velocity.

## Jacobian protection

A damped least-squares pseudoinverse is used instead of a direct matrix inverse.

If the Jacobian or required TF transformation cannot be obtained, the controller stops the commanded motion.

---

# Sensor Selection

The admittance controller supports different wrench sources.

The external EtherCAT sensor can be selected with:

```yaml
sensor: ethercat
```

The corresponding wrench topic is:

```text
/force_sensor_eth
```

The UR internal force signal can also be used through the project-specific zeroed force path:

```text
/force_sensor_ur_zeroed
```

Sensor calibration and frame transformations must be verified experimentally before physical admittance experiments.

---

# Pick and Place / Demolding

A complete manipulation workflow has been developed around:

```text
HOME
  ↓
PICK
  ↓
PLACE
  ↓
HOME
```

The robot and gripper are coordinated through ROS 2.

The same manipulation infrastructure is used as a basis for the demolding experiments.

The mechanical setup includes custom 3D-printed fingers designed for the manipulation task.

The current implementation has been tested on the physical robot for Pick and Place and demolding sequences.

Documentation:

[Pick & Place / Demolding](docs/09_PICK_PLACE_DEMOLDING.md)

---

# Teaching by Demonstration

A teaching and demonstration framework is also included.

The ROS 2 package is:

```text
src/robota_sudoe_teach/
```

The teaching node records information such as:

- Relative time
- Robot joint positions
- TCP pose
- Force/torque measurements

The node provides services:

```text
/teach/start
/teach/stop
/teach/save
```

Demonstrations are saved as JSON files.

The default task directory is:

```text
~/robota_tasks
```

The imitation component is located in:

```text
src/robota_sudoe_imitation/
```

This part of the project is currently less mature than the robot control and manipulation components and remains exploratory.

Documentation:

[Teaching and Imitation](docs/08_TEACHING_IMITATION.md)

---

# Validation

The admittance controller includes an offline numerical validation framework.

The main validation script is:

```text
validation/validate_six_axis.py
```

The validation does **not command the physical robot**.

It checks:

- Translational admittance
- Rotational admittance
- Force and torque dead zones
- Velocity saturation
- Acceleration limits
- Jacobian calculations
- Damped least-squares behavior
- Near-singular behavior

For example, with:

```text
Applied force       = 5 N
Dead zone           = 1.5 N
Effective force     = 3.5 N
Damping             = 8.31 N.s/m
```

the theoretical unconstrained velocity is approximately:

```text
v = 3.5 / 8.31
  ≈ 0.421 m/s
```

The configured velocity limit then constrains the command to:

```text
0.20 m/s
```

The validation report is available here:

[Validation Report](VALIDATION_REPORT.md)

and:

[Validation Documentation](docs/10_VALIDATION.md)

---

# Mechanical Integration

The project also includes mechanical integration work required for the manipulation experiments.

This includes:

- CAD adapter plates
- Gripper mounting
- Custom demolding fingers
- 3D printing
- Mechanical assembly
- TCP verification
- Tool dimensions and positioning

The mechanical components were developed specifically for the experimental manipulation and demolding setup.

---

# Current Status

| Component | Status |
|---|---|
| ROS 2 Jazzy migration | Completed |
| UR5e ROS 2 integration | Completed |
| URDF / Xacro / SRDF work | Completed |
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

# Development History

The project has gone through several development stages, including:

1. ROS 2 migration and robot integration
2. UR5e description and controller configuration
3. MoveIt / RViz / Gazebo integration
4. SCHUNK EGK50 communication
5. Mechanical integration
6. Pick and Place implementation
7. Demolding experiments
8. HEX-E sensor integration and calibration
9. Admittance controller development
10. Numerical validation
11. Teaching and demonstration framework

Detailed development changes are documented in:

[Development History](docs/14_DEVELOPMENT_HISTORY.md)

Additional technical information is available in:

[MIGRATION.md](MIGRATION.md)

[CHANGELOG.md](CHANGELOG.md)

---

# Troubleshooting

Common issues related to:

- UR5e communication
- ROS 2 controllers
- External Control
- Force sensor communication
- SCHUNK gripper communication
- Tool communication
- TF and Jacobian problems
- Admittance controller behavior

are documented in:

[Troubleshooting](docs/TROUBLESHOOTING.md)

and:

[Extended Troubleshooting](docs/13_TROUBLESHOOTING.md)

---

# Important Notes

## Network Configuration

Robot and device network addresses are intentionally not included in the public documentation.

They should be configured locally according to the actual experimental setup.

## Physical Admittance

The numerical validation confirms the mathematical and numerical behavior of the controller, but it does not constitute validation of the complete physical admittance loop.

Before physical testing, the following must be experimentally verified:

- Force sensor zero
- Tool gravity compensation
- Sensor orientation
- Force/torque axis conventions
- Sign conventions
- Robot/tool frame transformations
- Controller behavior under contact

## Development Parameters

The parameters currently present in the configuration files should be considered development values.

They must not automatically be interpreted as final or universally safe values for another robot, tool or experimental configuration.

---

# Repository Purpose

This repository is intended to provide a complete technical record of the ROBOTA-SUDOE robotic development work, including:

- Software implementation
- Hardware integration
- Control development
- Mechanical integration
- Experimental procedures
- Numerical validation
- Development history
- Known limitations

The documentation is structured so that the repository can be used both as a development workspace and as a technical reference for the internship project.

---

# Authors

**Marouane Belhaddade Zanati**  
SIGMA Clermont  
Internship — May to September 2026

**Mohamed Ali Jomaa Ghouil**  
SIGMA Clermont / ENISo  
Internship — May to September 2026

---

# Project

**ROBOTA-SUDOE**  
CiTIUS — University of Santiago de Compostela  
Spain
