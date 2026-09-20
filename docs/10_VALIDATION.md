# Validation

## Offline validation

The numerical validation is in:

```text
validation/validate_six_axis.py
```

It does not command motion to the physical robot.

Run:

```bash
cd validation
python3 validate_six_axis.py
```

The validation covers:

- translational admittance;
- rotational admittance;
- velocity saturation;
- acceleration limits;
- Jacobian computation;
- damped-least-squares inversion;
- behaviour near singular configurations.

## Example

For a 5 N applied force and a 1.5 N dead zone:

```text
effective force = 5 - 1.5 = 3.5 N
```

With `B_trans = 8.31 N.s/m`, the unconstrained steady-state speed is approximately:

```text
3.5 / 8.31 = 0.421 m/s
```

The controller then applies the configured 0.20 m/s speed limit.

## Physical validation status

Offline numerical validation is completed. The complete physical admittance loop has not yet been validated on the robot.

The next experimental stage is wrench calibration and verification of:

- sensor zero;
- tool gravity compensation;
- sensor orientation;
- force/torque axes;
- sign conventions;
- response at low force;
- safe velocity and acceleration limits.

See `VALIDATION_REPORT.md` for the detailed project validation record.
