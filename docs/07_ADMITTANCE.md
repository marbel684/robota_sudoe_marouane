# Admittance Control

## Controller

Source:

```text
src/admittance_control_ros2/src/admittance_control_ros2.cpp
```

Executable:

```text
admittance_control_node
```

The controller implements:

```text
M a + B v = F
```

The measured wrench produces a Cartesian velocity. The Cartesian command is converted to joint velocity using the robot Jacobian and a damped-least-squares pseudoinverse.

## Development parameters

| Parameter | Value |
|---|---:|
| Frequency | 250 Hz |
| Period | 0.004 s |
| Translational mass | 12.0 kg |
| Rotational mass | 0.10 |
| Translational damping | 8.31 N.s/m |
| Rotational damping | 0.76 |
| Force dead zone | 1.5 N |
| Torque dead zone | 0.08 Nm |
| Max linear speed | 0.20 m/s |
| Max angular speed | 0.35 rad/s |
| Max linear acceleration | 0.50 m/s² |
| Max angular acceleration | 0.80 rad/s² |
| Max joint speed | 0.60 rad/s |
| DLS damping | 0.02 |

These are development/validation values, not final experimentally tuned values.

## Safety and robustness mechanisms

The implementation includes:

- force dead zones;
- wrench filtering;
- Cartesian velocity saturation;
- Cartesian acceleration limiting;
- joint velocity limiting;
- damped-least-squares inversion;
- force timeout;
- zero-command fallback when TF/Jacobian or required inputs are unavailable.

At 250 Hz and 0.50 m/s², the maximum velocity change per cycle is:

```text
Delta v = a_max * dt = 0.50 * 0.004 = 0.002 m/s
```

## Running

After the robot, joint states and sensor are verified:

```bash
cd ~/robota_ws
colcon build --packages-select admittance_control_ros2 --symlink-install
source install/setup.bash
ros2 run admittance_control_ros2 admittance_control_node --ros-args -p sensor:=ethercat
```

Physical testing must be preceded by verification of TCP, payload, active controller, force signal and workspace.
