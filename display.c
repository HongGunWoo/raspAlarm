#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <pthread.h>
#include <time.h>
#include <softPwm.h>
#include "common.h"

int digits[] = {22, 27, 17, 24};
int segment[] = {11, 4, 23, 8, 7, 10, 18, 25};

short num[10] = {0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE4, 0xFE, 0xE6};

void initPin();
void displayTime();

void *displayThread(void *data)
{
  initPin();
  displayTime(data);
}

void initPin()
{
  for (int i = 0; i < 4; i++)
  {
    pinMode(digits[i], OUTPUT);
  }
  for (int i = 0; i < 8; i++)
  {
    pinMode(segment[i], OUTPUT);
    softPwmCreate(segment[i], 0, 100);
  }
}

void displayTime(void *data)
{
  int prevTime = 0;
  int light = 100;

  while (1)
  {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    
    int curTime = (tm.tm_mon + 1) * 1000000 + tm.tm_mday * 10000 + tm.tm_hour * 100 + tm.tm_min;
    int time[] = {tm.tm_hour / 10, tm.tm_hour % 10, tm.tm_min / 10, tm.tm_min % 10};
    
    if(light != ((State *)data)->light)
    {
      light = ((State *)data)->light;
      for (int i = 0; i < 8; i++)
      {
        softPwmStop(segment[i]);
        softPwmCreate(segment[i], 0, light);
      }
    }

    pthread_mutex_lock(&mid);
    ((State *)data)->curTime = curTime;
    pthread_mutex_unlock(&mid);
    // if(curTime != prevTime)
    // {
    //   pthread_mutex_lock(&mid);
    //   ((State *)data)->curTime = curTime;
    //   pthread_mutex_unlock(&mid);
    //   prevTime = curTime;
    // }

    for (int digit = 0; digit < 4; digit++)
    {
      for (int loop = 0; loop < 8; loop++)
      {
        short segmentData = (num[time[digit]] & (0x01 << loop)) >> loop;
        if (segmentData == 1)
          digitalWrite(segment[7 - loop], HIGH);
        else
          digitalWrite(segment[7 - loop], LOW);

        if (tm.tm_sec % 2 == 0 && digit == 1)
          digitalWrite(25, 1);
        else
          digitalWrite(25, 0);
      }
      digitalWrite(digits[digit], LOW);
      delay(1);
      digitalWrite(digits[digit], HIGH);
    }
  }
}