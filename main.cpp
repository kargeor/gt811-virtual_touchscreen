#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <wiringPiI2C.h>

#include "GT811.h"

uint8_t buf[1024];
int fd;
const int READ_SIZE = 34;
FILE *cmdOut = stdout;

GT811::GT811(uint16_t _RST, uint16_t _INT) {
  touchPressure[0] = 0;
  touchPressure[1] = 0;
  touchPressure[2] = 0;
  touchPressure[3] = 0;
  touchPressure[4] = 0;
}

void GT811::begin() {
  buf[0] = GT811_REGISTERS_CONFIGURATION >> 8;
  buf[1] = GT811_REGISTERS_CONFIGURATION & 0xFF;
  int sz = sizeof(gt811_config) / sizeof(gt811_config[0]);
  for(int i = 0; i < sz; i++) buf[2+i] = gt811_config[i];
  write(fd, buf, sz + 2);
}

uint16_t GT811::poll(void) {
  // Maybe check INT pin?

  buf[0] = GT811_REGISTERS_READ >> 8;
  buf[1] = GT811_REGISTERS_READ & 0xFF;
  write(fd, buf, 2);
  int count = read(fd, buf, READ_SIZE);
  if (count != READ_SIZE) {
    fprintf(stderr, "read() error\n");
    return 0;
  }

  // this is the reserved area
  for (int i = 18; i < 28; i++)
    buf[i] = buf[i + 6];

  int tpFlag = buf[0];

  for (int i = 0; i < 5; i++) {
    if ((tpFlag >> i) & 1) {
      // Swap X-Y here if needed
      touchY[i] = (buf[(i * 5) + 2] << 8) | (buf[(i * 5) + 3] & 0xFF);
      touchX[i] = (buf[(i * 5) + 4] << 8) | (buf[(i * 5) + 5] & 0xFF);
      // touchPressure[i] = buf[(i * 5) + 6];
      // Use touchPressure to save state instead
      touchPressure[i] = 1; // pressed

      // Update for orientation here
      touchY[i] = 480 - touchY[i];

      // printf("Pos[%d]: (%d, %d) Pres %d\n", i, touchX[i], touchY[i], touchPressure[i]);
      // print using "virtual_touchscreen" format
      fprintf(cmdOut, "s %d\nT %d\nX %d\nY %d\nS 0\n", i, i+10, touchX[i], touchY[i]);
    } else if (touchPressure[i]) {
      fprintf(cmdOut, "s %d\nT -1\nS 0\n", i);
      touchPressure[i] = 0; // not pressed
    }
  }

  // this is important!!!
  fflush(cmdOut);

  return 0;
}

///// BEGIN OF TS POINT
TS_Point GT811::getPoint(uint8_t n) {
  if ((touches == 0) || (n > 5) || (n < 1)) {
    return TS_Point(0, 0, 0);
  } else {
    return TS_Point(touchX[n-1], touchY[n-1], touchPressure[n-1]);
  }
}

TS_Point::TS_Point(void) {
  x = y = pressure = 0;
}

TS_Point::TS_Point(int16_t _x, int16_t _y, int16_t _pressure) {
  x = _x;
  y = _y;
  pressure = _pressure;
}


bool TS_Point::operator==(TS_Point p1) {
  return  ((p1.x == x) && (p1.y == y) && (p1.pressure == pressure));
}


bool TS_Point::operator!=(TS_Point p1) {
  return  ((p1.x != x) || (p1.y != y) || (p1.pressure != pressure));
}
///// END OF TS POINT


int main(int argc, char **argv) {
  int c;
  while ((c = getopt (argc, argv, "o:")) != -1) {
    switch (c) {
      case 'o':
        cmdOut = fopen(optarg, "wb");
        if (!cmdOut) {
          fprintf(stderr, "Cannot open output device\n");
          return 1;
        }
        break;
      case '?':
      default:
        fprintf(stderr, "Bad argument\n");
        return 1;
    }
  }

  // result of "i2cdetect -y 1"
  fd = wiringPiI2CSetup(0x5d);
  if (fd == -1) {
    fprintf(stderr, "Cannot open i2c device\n");
    return 1;
  }

  GT811 gt811(0, 0);
  gt811.begin();

  while (1) {
    gt811.poll();
  }

  close(fd);
  return 0;
}

