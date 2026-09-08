# Numerical validation

The validation script is used before physical experiments. It runs offline and does not send commands to the robot.

## Run

```bash
cd validation
python3 validate_six_axis.py
```

## Checks

The script evaluates:

- translation and rotation dynamics;
- dead-zones;
- velocity and acceleration saturation;
- UR5e Jacobian;
- damped least-squares pseudoinverse;
- Cartesian velocity reconstruction;
- behaviour near singular configurations.

The numerical model is a development model. For the physical system, the same robot description, tool configuration and sensor frame should be used when possible.

## Example used during development

For a constant force of 5 N with a 1.5 N dead-zone:

```text
effective force = 3.5 N
B_trans = 8.31 N.s/m
unconstrained steady speed = 0.421179 m/s
maximum linear speed = 0.20 m/s
maximum linear acceleration = 0.50 m/s²
```

The unconstrained speed is higher than the configured limit, so the velocity limiter is expected to become active.
