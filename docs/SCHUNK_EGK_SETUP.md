# Schunk EGK50 — setup notes

The Schunk EGK50 was the main gripper used for the physical manipulation tests.

## Mechanical mounting

An adapter plate was designed to connect the UR5e flange to the gripper. The custom fingertips were designed for the doll geometry and produced by 3D printing.

The dimensions of the first prototype required a small manual adjustment before the tool could be used on the physical cell.

## Communication

The gripper was tested through the robot tool interface. The communication settings in PolyScope must match the gripper configuration.

The repository does not contain the laboratory serial number or other device-specific identifiers. These values should be entered locally when required.

## Physical checks

Before a manipulation test, check:

- mechanical fixation of the adapter;
- fingertip clearance;
- cable routing;
- tool center point;
- gripper opening and closing direction;
- maximum stroke and gripping force;
- possible collision with the mold.

The printed fingertips were sufficient for the basic pick-and-place and demolding tests, but their robustness was limited when higher gripping forces were required.
