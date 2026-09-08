#!/usr/bin/env bash
set -e
source /opt/ros/jazzy/setup.bash
colcon build --symlink-install
source install/setup.bash
echo "Build terminé."
