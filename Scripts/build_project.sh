#!/usr/bin/env bash

echo "Running ESP8266-RTOS-SDK export script..."

# this is where we can make our project-specific modifications
echo ${PWD}

# build the project
make -C  ./solar_meter all || exit 1

# clean the project
make -C  ./solar_meter clean || exit 1

