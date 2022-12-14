#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h> /* atoi( ) */
#include "common.h"

#define GPIO 5

void* switchThread(void* data){
  pinMode(GPIO, INPUT);
  while(1){
    if(digitalRead(GPIO) == 0){
      pthread_mutex_lock(&mid);
      ((State*)data)->switchOn = 1;
      pthread_mutex_unlock(&mid);
    }
  }
  return 0;
}
