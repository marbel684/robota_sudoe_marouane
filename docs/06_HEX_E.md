# OnRobot HEX-E Force/Torque Sensor

The OnRobot HEX-E provides six-axis force and torque measurements for interaction and admittance control.

## ROS 2 path

The EtherCAT path uses `rokubimini_ethercat` and the project publisher retained under `legacy/`.

Main topic:

```text
/force_sensor_eth
```

Verification:

```bash
ros2 topic echo /force_sensor_eth --once
ros2 topic hz /force_sensor_eth
```

## Calibration checks

Before admittance control, verify:

- communication;
- zero offset;
- mounting orientation;
- sensor frame;
- force and torque signs;
- noise at rest;
- calibration with known loads.

During the project, a known-load calibration was performed and a force of approximately 5 N was obtained in the corresponding test.

## Sensor selection

The admittance controller supports the external HEX-E path and a zeroed UR internal sensor path. The external path is selected with:

```text
sensor:=ethercat
```

The mechanical sensor mounting rotation used by the controller must still be experimentally validated for the final physical configuration.
