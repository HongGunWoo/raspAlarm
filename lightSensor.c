#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "common.h"

#define SLAVE_ADDR_01 0x48
static const char *I2C_DEV = "/dev/i2c-1";

void* lightSensorThread(void* data){
  int i2c_fd;
  int preVal = 0;
  int curVal = 0;
  int light = 100;
  int adcChannel = 0;

  if ((i2c_fd = wiringPiI2CSetupInterface(I2C_DEV, SLAVE_ADDR_01)) < 0)
  {
    printf("wiringPi2CSetup Failed: \n");
    return NULL;
  }

  while (1)
  {
    wiringPiI2CWrite(i2c_fd, 0x40 | adcChannel);

    preVal = wiringPiI2CRead(i2c_fd);
    curVal = wiringPiI2CRead(i2c_fd);
    // printf("조도 Current value= %d \n", curVal);

    if(curVal >= 220 && light != 3)
    {
      light = 3;
      ((State *)data)->light = 3;
    }
    if(curVal < 220 && light != 100)
    {
      light = 100;
      ((State *)data)->light = 100;
    }
    delay(1000);
  }
}