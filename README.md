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

### Testing
Open I2C and write coordinates to screen with debug output:
`sudo ./gt811-virtual_touchscreen -d`

## Kernel Module Install Instructions
1. ```sudo cp virtual_touchscreen.ko /lib/modules/`uname -r`/kernel/drivers/input/misc/``` -- you might need to select a different location based on your linux install
1. `sudo depmod`
1. `sudo modprobe virtual_touchscreen` -- this only stays until reboot
1. Add `virtual_touchscreen` to `/etc/modules` to auto load
1. Check: `lsmod | grep virtual_touchscreen`

### Testing
Send to kernel module:
`sudo ./gt811-virtual_touchscreen -o /dev/virtual_touchscreen`

