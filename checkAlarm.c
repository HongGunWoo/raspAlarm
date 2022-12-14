#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "common.h"

void *checkAlarmThread(void *data)
{
  int prevTime = 0;
  while (1)
  {
    int curTime = ((State *)data)->curTime;
    for (int i = 0; i < ALARM_SIZE; i++)
    {
      if (((State *)data)->alarm[i] == -1)
      {
        break;
      }
      pthread_mutex_lock(&mid);
      if (((State *)data)->alarm[i] == curTime)
      {
        printf("check alaram");
        prevTime = curTime;
        ((State *)data)->alarmOn = 1;

        for (int j = i; j < ALARM_SIZE - 1; j++)
        {
          ((State *)data)->alarm[j] = ((State *)data)->alarm[j + 1];
        }
        ((State *)data)->alarm[ALARM_SIZE - 1] = -1;
      }
      pthread_mutex_unlock(&mid);
    }
  }
}