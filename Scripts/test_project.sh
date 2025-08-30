#!/usr/bin/env bash

echo "Running unit tests..."

# this is where we can make our project-specific modifications
echo ${PWD}

make -C  ./unit_test realclean || exit 1

echo "Testing PowerMonitor" 
# Clean project and test power monitor
make -C ./unit_test gcov TEST_POWER_MONITOR=true || exit 1

echo "Testing BusVoltage"
# Clean project and test ads1115 module
make -C ./unit_test gcov TEST_BUS_VOLTAGE=true || exit 1

# echo "Testing ADS1115"
# # Clean project and test ads1115 module
# make -C  ./unit_test clean || exit 1
# make -C ./unit_test all TEST_ADS1115=true || exit 1

echo "Running gcovr to generate coverage report..." 

./unit_test/coverage.sh || exit 1

