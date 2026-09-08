#!/usr/bin/env bash
set -e

echo "=== Interfaces réseau ==="
ip -br addr

echo
echo "=== Interface enp3s0 ==="
ip addr show enp3s0 || true

echo
echo "=== Link ==="
sudo ethtool enp3s0 2>/dev/null | grep -E "Link detected|Speed|Duplex" || true

echo
echo "=== ROS topic HEX ==="
source /opt/ros/jazzy/setup.bash
source ~/kairos_ws/install/setup.bash
ros2 topic hz /force_sensor_eth --window 5 || true
