# Teaching and Imitation

## Teaching node

The teaching implementation is in:

```text
src/robota_sudoe_teach/
```

The node records:

- relative time;
- joint positions;
- TCP pose;
- force/torque measurements.

The default task directory is `~/robota_tasks`.

The node provides:

```text
/teach/start
/teach/stop
/teach/save
```

Demonstrations are stored as JSON data.

## Imitation processing

The processing code is in:

```text
src/robota_sudoe_imitation/
```

The current trajectory processor can create a uniform time grid. Pose interpolation is not yet implemented.

## Project maturity

Teaching and imitation were prepared and explored during the internship, but this part is less mature than the robot control, manipulation and offline validation components. The physical admittance validation remains a separate experimental step.
