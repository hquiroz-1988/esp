#!/usr/bin/env bash

echo "Running ESP8266-RTOS-SDK test script..."

# this is where we can make our project-specific modifications
echo ${PWD}

make -C  ./unit_test gcov || exit 1

echo "Running gcovr to generate coverage report..." 

./unit_test/coverage.sh || exit 1

