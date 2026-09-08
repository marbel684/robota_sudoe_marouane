# Force-Guided Robotic Manipulation with ROS 2

This repository contains the software developed during my internship at
CiTIUS, University of Santiago de Compostela.

The project focuses on a force-guided manipulation system for a UR5e robot
installed in the KAIROS workcell. The work includes the ROS 2 migration,
robot and gripper control, mechanical integration, force/torque sensing and
admittance control.

## Hardware

The main hardware used in the project is:

- Universal Robots UR5e
- Schunk EGK50 gripper
- OnRobot HEX-E force/torque sensor
- KAIROS workcell

## Software

The project was developed using:

- Ubuntu 24.04 LTS
- ROS 2 Jazzy
- Python
- C++
- MoveIt
- RViz
- Gazebo Harmonic

## Main work

The main tasks carried out during the project were:

- migration of the existing environment to ROS 2 Jazzy;
- checking and adapting the URDF/Xacro/SRDF configuration;
- setup of MoveIt, RViz and Gazebo;
- communication with the Schunk EGK50 through TCP;
- CAD design of the mechanical adapters and demolding fingers;
- 3D printing and physical integration on the KAIROS workcell;
- development of the Pick and Place and demolding sequence;
- integration and calibration of the OnRobot HEX-E sensor;
- implementation of an admittance controller;
- numerical validation of the controller and Jacobian calculation;
- preparation of teaching and demonstration recording.

## Current status

The Pick and Place and demolding sequence was tested on the physical robot.

The custom 3D-printed fingers can grasp, demold and place the object correctly.
However, the PLA fingers can deform or break when higher gripping forces are
required. A more resistant version is therefore being considered.

The HEX-E sensor was calibrated using known loads. A force of approximately
5 N was measured, and the communication and force measurement were validated.

The admittance controller was implemented in C++ and numerically validated.
The current development uses a 250 Hz control loop.

The complete physical validation of the admittance loop has not been finished.
The next step is to connect the calibrated HEX-E measurements to the physical
controller and test the resulting compliant motion progressively on the robot.

## Repository structure

```text
robota_sudoe_marouane/
├── src/
│   ├── admittance_control_ros2/
│   ├── robota_sudoe_bringup/
│   ├── robota_sudoe_imitation/
│   └── robota_sudoe_teach/
├── config/
├── docs/
├── validation/
├── legacy/
├── scripts/
├── MIGRATION.md
├── VALIDATION_REPORT.md
└── README.md
