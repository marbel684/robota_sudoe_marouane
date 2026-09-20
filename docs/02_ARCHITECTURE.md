# System Architecture

## High-level architecture

The system is organized around four functional layers:

1. **Robot control** — UR5e ROS 2 driver, joint states and velocity controller.
2. **Interaction hardware** — SCHUNK EGK50 gripper and OnRobot HEX-E sensor.
3. **Control logic** — admittance controller, kinematics/Jacobian and safety limits.
4. **Experiment layer** — Pick and Place, demolding, teaching and offline validation.

```text
                  +----------------------+
                  |      UR5e / KAIROS   |
                  +----------+-----------+
                             |
                    ROS 2 / UR Driver
                             |
             +---------------+---------------+
             |                               |
       /joint_states              velocity controller
             |                               |
             +---------------+---------------+
                             |
                  Admittance Controller
                    |                |
              HEX-E wrench       Jacobian / DLS
                    |                |
                    +-------+--------+
                            |
                       joint velocity

      SCHUNK EGK50 <---- ROS 2 gripper interface
      Teaching node ----> demonstration JSON
```

## Repository responsibilities

- `src/admittance_control_ros2/`: real-time-oriented admittance computation.
- `src/robota_sudoe_teach/`: demonstration acquisition.
- `src/robota_sudoe_imitation/`: demonstration processing.
- `src/robota_sudoe_bringup/`: project package structure for future bringup.
- `config/`: controller and teaching parameters.
- `validation/`: offline numerical tests.
- `legacy/`: previous experiments retained for traceability.
- `docs/`: operational and engineering documentation.

## Control data flow

The admittance node reads joint states, TF and a wrench source. The wrench is transformed into the configured robot frame, filtered and passed through dead zones. The Cartesian velocity is computed from the admittance model, constrained by velocity and acceleration limits, and mapped to joint velocity using a damped-least-squares Jacobian pseudoinverse.

If required inputs are unavailable, the controller publishes zero velocity rather than continuing with stale data.
