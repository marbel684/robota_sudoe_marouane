# SCHUNK EGK50 Integration

The SCHUNK EGK50 is used for the manipulation and demolding sequence.

## Software

The main historical implementation is:

```text
legacy/schunk_gripper_node.py
```

Additional setup information is available in `docs/SCHUNK_EGK_SETUP.md`.

## ROS 2 interface

Main topics:

```text
/gripper/action
/gripper/command
/gripper/position
```

Example commands:

```bash
ros2 topic pub --once /gripper/action std_msgs/msg/String "data: 'open'"
ros2 topic pub --once /gripper/action std_msgs/msg/String "data: 'close'"
ros2 topic echo /gripper/position --once
```

## Mechanical integration

The gripper integration included adapter-plate design, custom 3D-printed fingertips, assembly and TCP verification.

The tested PLA fingers are sufficient for the demonstrated manipulation sequence but can deform or break when higher demolding forces are required.

## Communication note

The project also investigated alternative gripper communication paths. The public repository intentionally excludes device-specific identifiers and laboratory credentials.
