# ROBOTA-SUDOE — UR5e Force-Guided Manipulation

This repository contains the ROS 2 software developed during my internship at CiTIUS (University of Santiago de Compostela).

The work was carried out on the KAIROS cell with a Universal Robots UR5e. The software covers the migration to ROS 2, admittance control, force/torque sensing, teach-by-demonstration and the first steps towards imitation learning.

## Main components

- `admittance_control_ros2`: Cartesian admittance controller and Jacobian mapping.
- `robota_sudoe_teach`: recording and replay of teaching trajectories.
- `robota_sudoe_imitation`: processing of recorded demonstrations.
- `robota_sudoe_bringup`: package used for the project bringup and configuration.
- `legacy`: older nodes and hardware tests kept for reference.
- `validation`: offline checks of the controller and Jacobian calculations.

## Control model

The translational and rotational parts use a mass-damper admittance model:

`M x_ddot + B x_dot = F`

The current development parameters are:

| Parameter | Value |
|---|---:|
| Control frequency | 250 Hz |
| Control period | 0.004 s |
| Translational mass | 12.0 kg |
| Translational damping | 8.31 N.s/m |
| Rotational inertia | 0.10 kg.m² |
| Rotational damping | 0.76 N.m.s/rad |
| Force dead-zone | 1.5 N |
| Torque dead-zone | 0.08 N.m |
| Maximum linear speed | 0.20 m/s |
| Maximum linear acceleration | 0.50 m/s² |
| Maximum angular speed | 0.35 rad/s |
| Maximum angular acceleration | 0.80 rad/s² |
| Maximum joint speed | 0.60 rad/s |
| DLS damping | 0.02 |

These values were used as starting values for development and numerical tests. They are not presented as final tuning values for the physical robot.

## Current state

The predefined robot and gripper sequence was tested on the physical KAIROS cell.

The Bota/HEX force sensor path was calibrated and tested separately. The offline admittance controller and Jacobian calculations were also checked before physical use.

The complete force-guided teaching loop still requires progressive testing on the real robot. Payload, TCP, sensor frame, signs, velocity limits and the physical setup must be checked before this stage.

## Build

The workspace is a standard ROS 2 colcon workspace.

```bash
source /opt/ros/jazzy/setup.bash
cd ~/robota_sudoe_v2
colcon build --symlink-install
source install/setup.bash
```

The exact workspace path can be changed to match the local installation.

## Validation

The six-axis validation does not connect to ROS 2 and does not command the robot.

```bash
cd validation
python3 validate_six_axis.py
```

The latest validation checks include:

- translational and rotational dynamics;
- force and torque dead-zones;
- Cartesian velocity and acceleration limits;
- Jacobian calculation;
- damped least-squares pseudoinverse;
- reconstruction error and conditioning near singular configurations.

For the 5 N test case used during development:

`F_eff = 5 - 1.5 = 3.5 N`

`v_inf = 3.5 / 8.31 = 0.421179 m/s`

The theoretical value is above the configured `0.20 m/s` velocity limit, so the velocity saturation is expected to become active.

## Hardware notes

The repository contains configuration files for the Bota/HEX force sensor and the Schunk EGK50 gripper. Device-specific network settings, serial numbers and credentials are not included in the public repository.

Before using the code on the physical cell, check:

1. robot controller state;
2. Payload and center of gravity in PolyScope;
3. TCP and mounting configuration;
4. force/torque sensor frame and zero offset;
5. controller and topic names;
6. Cartesian and joint limits;
7. workspace and collision conditions.

Start with low-speed tests and keep the emergency stop accessible.

## Documentation

- `docs/SETUP.md`: startup procedure and checks before connecting ROS 2 to the robot.
- `docs/SCHUNK_EGK_SETUP.md`: notes from the EGK50 integration tests.
- `docs/TROUBLESHOOTING.md`: communication issues found during development.
- `MIGRATION.md`: changes between the earlier controller and V3.
- `VALIDATION_REPORT.md`: numerical validation notes.
