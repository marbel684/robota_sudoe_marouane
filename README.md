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
