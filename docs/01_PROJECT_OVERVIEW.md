# Project Overview

## ROBOTA-SUDOE — UR5e ROS 2 Control and Admittance

This repository contains the ROS 2 software and engineering work developed during the internship at CiTIUS, University of Santiago de Compostela, for the ROBOTA-SUDOE project.

The work covers the migration and integration of a UR5e on the KAIROS workcell, SCHUNK EGK50 gripper communication, OnRobot HEX-E force/torque sensing, mechanical integration, Pick and Place / demolding, and development of an admittance controller.

## Main objectives

- Establish a reproducible ROS 2 Jazzy environment for the UR5e.
- Integrate robot, gripper and force/torque sensing.
- Develop and validate a force-guided admittance controller.
- Implement and test the manipulation and demolding sequence.
- Prepare teaching-by-demonstration and imitation-learning data structures.

## Hardware

| Component | Role |
|---|---|
| UR5e | 6-DOF collaborative robot |
| KAIROS | Robotic workcell |
| SCHUNK EGK50 | Electric gripper |
| OnRobot HEX-E | 6-axis force/torque sensor |
| Development/onboard PC | ROS 2 execution |

Network addresses and laboratory-specific credentials are intentionally excluded from this public repository.

## Project status

The robot integration, gripper communication, mechanical integration, manipulation sequence, HEX-E calibration, controller implementation and offline numerical validation are completed. Physical validation of the complete admittance loop remains the next experimental step.

## Documentation map

- [Architecture](02_ARCHITECTURE.md)
- [Installation](04_INSTALLATION.md)
- [SCHUNK EGK50](05_SCHUNK_EGK50.md)
- [HEX-E](06_HEX_E.md)
- [Admittance control](07_ADMITTANCE.md)
- [Teaching and imitation](08_TEACHING_IMITATION.md)
- [Pick, Place and demolding](09_PICK_PLACE_DEMOLDING.md)
- [Validation](10_VALIDATION.md)
- [Troubleshooting](13_TROUBLESHOOTING.md)
- [Development history](14_DEVELOPMENT_HISTORY.md)
