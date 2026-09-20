# Troubleshooting

## ROS 2 package not found

```bash
source /opt/ros/jazzy/setup.bash
source ~/robota_ws/install/setup.bash
cd ~/robota_ws
colcon build --symlink-install
```

## Robot does not move as expected

Check:

- TCP;
- reference frame;
- orientation;
- payload and center of gravity;
- trajectory timing;
- controller state;
- robot operating mode.

Start with a simple motion before the complete sequence.

## HEX-E values are incorrect

Check:

- sensor mounting;
- orientation and frame;
- zero offset;
- calibration;
- communication;
- topic frequency;
- force/torque signs.

Do not use an unverified wrench signal for admittance experiments.

## SCHUNK does not respond

Check network/TCP communication, node status, robot connection and gripper topics. Test the gripper independently before using it inside the complete sequence.

## Protective stop

Stop the commanded motion and inspect the robot state in PolyScope. Check TCP, payload, trajectory and reference frame, then retry with reduced speed and simpler motion after identifying the cause.

## URCap / gripper communication

During the project, the Universal Robots RS485 Tool Communication Forwarder route was investigated. The TCP bridge could be reached, but it did not provide the expected Modbus response. The native PolyScope URCap path worked more reliably.

The documented workaround was a hybrid approach: use the native PolyScope path for the gripper while retaining the ROS 2 `ur_robot_driver` path for arm control.

For the detailed investigation, see `docs/TROUBLESHOOTING.md`.
