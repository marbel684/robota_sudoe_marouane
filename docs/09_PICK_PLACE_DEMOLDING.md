# Pick and Place / Demolding

## Sequence

The tested manipulation sequence follows:

```text
HOME
  ↓
PICK
  ↓
PLACE
  ↓
HOME
```

Robot motion and gripper commands are synchronized through ROS 2.

## Mechanical integration

The work included:

- CAD adapter plates;
- custom demolding fingers;
- 3D printing;
- mechanical assembly;
- TCP verification;
- dimensional adjustments.

The custom fingers were tested on the physical KAIROS workcell.

## Demolding limitation

The current PLA fingers can perform the tested manipulation sequence, but their mechanical strength limits the forces that can be applied during demanding demolding operations. A stronger mechanical design is a possible next iteration.

## Recommended test progression

1. Verify robot state and TCP.
2. Test gripper independently.
3. Test a simple robot motion.
4. Test PICK.
5. Test PLACE.
6. Run the complete sequence.
7. Only then introduce force-guided admittance experiments.
