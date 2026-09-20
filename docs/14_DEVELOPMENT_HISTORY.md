# Development History

## Phase 1 — ROS 2 migration

The project was migrated to Ubuntu 24.04 / ROS 2 Jazzy and the UR5e integration was established, including URDF/Xacro/SRDF, MoveIt, RViz and Gazebo work.

## Phase 2 — Robot and gripper integration

The UR5e control path was stabilized and the SCHUNK EGK50 communication was implemented and tested.

Mechanical integration included adapter plates, custom fingertips, assembly and TCP verification.

## Phase 3 — Manipulation

The Pick and Place sequence was tested on the physical robot and then used as the basis for the demolding task.

## Phase 4 — Force sensing

The OnRobot HEX-E EtherCAT path was integrated and calibrated with known loads.

The project also retains a zeroed UR internal force-sensor path for experimentation.

## Phase 5 — Admittance controller V3

The current controller was reworked around the direct SI-unit damping model:

```text
M a + B v = F
```

V3 introduced or consolidated:

- acceleration limiting;
- force timeout;
- UR internal force path;
- relative monotonic teaching time;
- demonstration recording of TCP pose, joints and wrench;
- six-axis offline validation;
- Jacobian and DLS validation.

## Phase 6 — Teaching and imitation

A teaching node was prepared to record demonstrations and an imitation-processing package was introduced. The processing pipeline remains exploratory.

## Current state

Completed:

- ROS 2 Jazzy migration;
- UR5e integration;
- robot model and visualization setup;
- SCHUNK communication;
- mechanical integration;
- Pick and Place;
- demolding sequence;
- HEX-E calibration;
- admittance implementation;
- numerical validation;
- Jacobian/DLS validation.

Remaining:

- physical validation of the complete admittance loop;
- further teaching/imitation development;
- experimental wrench compensation and final controller tuning.
