#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include "common.h"

#define ALARM_SIZE 100
pthread_mutex_t mid;

int main(){
  if (wiringPiSetupGpio() < 0)
  {
    printf("wiringPiSetup() is failed\n");
    return 1;
  }
  State state;
  state.switchOn = 0;
  state.alarmOn = 0;
  memset(state.alarm, -1, sizeof(int) * ALARM_SIZE);

  pthread_t speakerT, switchT, bluetoothT, displayT, lightT, checkAlarmT;
  
  //시간을 공유해야하는 것들 speaker, bluetooth, display
  //밝기값 공유 밝기 센서, display

  pthread_mutex_init(&mid, NULL);
  pthread_create(&speakerT, NULL, speakerThread, (void*) &state);
  pthread_create(&switchT, NULL, switchThread, (void*) &state);
  pthread_create(&bluetoothT, NULL, bluetoothThread, (void*) &state);
  pthread_create(&displayT, NULL, displayThread, (void*) &state);
  pthread_create(&lightT, NULL, lightSensorThread, (void*) &state);
  pthread_create(&checkAlarmT, NULL, checkAlarmThread, (void*) &state);

  pthread_join(switchT, NULL);
  pthread_join(speakerT, NULL);
  pthread_join(bluetoothT, NULL);
  pthread_join(displayT, NULL);
  pthread_join(lightT, NULL);
  pthread_join(checkAlarmT, NULL);
  pthread_mutex_destroy(&mid);
  return 0;
}

