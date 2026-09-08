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

The different parts of the system were developed and tested progressively,
starting with the ROS 2 environment and robot control, then the gripper and
mechanical integration, and finally the force-guided control.

---

## Hardware Overview

| Component | Description |
|-----------|-------------|
| UR5e | 6-DOF collaborative robot |
| KAIROS | Robotic workcell |
| SCHUNK EGK50 | Electric 2-finger gripper |
| OnRobot HEX-E | 6-axis force/torque sensor |
| Development PC | Computer running ROS 2 Jazzy |

The exact network configuration depends on the laboratory setup and is not
included in the public repository.

---

## Software Stack

The main software used in the project is:

- Ubuntu 24.04 LTS
- ROS 2 Jazzy
- Python
- C++
- Universal Robots ROS 2 driver
- MoveIt
- RViz
- Gazebo Harmonic
- KDL / Eigen
- `rokubimini_ethercat`

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
