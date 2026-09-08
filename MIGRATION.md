# Migration and controller history

The current controller is the V3 implementation in:

```text
src/admittance_control_ros2/src/admittance_control_ros2.cpp
```

The earlier experiments are kept in `legacy/` because they were useful during the migration and hardware debugging stages.

## Main changes in V3

### Damping

The controller now uses the damping coefficients directly in SI units:

- `B_trans` in N.s/m;
- `B_rot` in N.m.s/rad.

The model is:

`M x_ddot + B x_dot = F`

### Acceleration limit

The discrete controller limits the change in velocity at each control cycle:

`|v(k) - v(k-1)| <= a_max * dt`

At 250 Hz:

`dt = 0.004 s`

For `a_max = 0.50 m/s²`:

`delta_v_max = 0.002 m/s` per cycle.

### Force timeout

If no valid wrench is received during the configured timeout, the controller stops producing motion commands.

### Sensor selection

The controller can use the Bota/HEX path or the zeroed UR internal force signal, depending on the selected sensor mode.

### Teaching

The teaching node records relative time, TCP pose, joint values and wrench data. The recorded data can later be processed for replay or imitation learning.

The complete physical admittance loop remains a separate experimental step and should not be considered validated only from the offline checks.
