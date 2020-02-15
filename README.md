# gt811-virtual_touchscreen
This is a driver for GT811 touch sensor display and any RaspberryPi.

## Hardware
Connect *I2C* to **GPIO02, GPIO03** and *INT* to **GPIO04**.

## Requirements
Make, GCC, Wiring PI and kernel module build files.

## Compile Instructions
1. `git clone --recursive git@github.com:kargeor/gt811-virtual_touchscreen.git`
1. `cd gt811-virtual_touchscreen`
1. `make`
1. `./patch_virtual_touchscreen.sh`
1. `cd virtual_touchscreen`
1. `make`

## Install Instructions
