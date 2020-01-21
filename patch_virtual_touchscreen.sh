#!/bin/bash

sed -i.bak 's/ABS_X_MAX\s*1024/ABS_X_MAX 800/; s/ABS_Y_MAX\s*1024/ABS_Y_MAX 480/' ./virtual_touchscreen/virtual_touchscreen.c

