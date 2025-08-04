#!/usr/bin/env bash

echo "Running ESP8266-RTOS-SDK export script..."

# this is where we can make our project-specific modifications
echo ${PWD}

make -C  ./solar_meter all || exit 1

