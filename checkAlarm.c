#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <string.h>
#include "common.h"

void *checkAlarmThread(void *data){
  int prevTime = 0;
  while(1) {
    int curTime = ((State *)data)->curTime;
    for (int i = 0; i < ALARM_SIZE; i++)
    {
      if (((State *)data)->alarm[i] == -1)
      {
        break;
      }
      if(((State *)data)->alarm[i] == curTime && prevTime != curTime)
      {
        prevTime = curTime;
        ((State *)data)->alarmOn = 1;
      }
      delay(1000);
    }
  }
}