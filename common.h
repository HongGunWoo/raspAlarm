#ifndef __COMMON_H__
#define __COMMON_H__
#include <pthread.h>
#define ALARM_SIZE 100

typedef struct _state
{
  int switchOn;
  int alarmOn;
  int curTime;
  int alarm[ALARM_SIZE];
  int light;
} State;

// extern pthread_mutex_t mid;

extern pthread_mutex_t mid;
extern void *bluetoothThread(void *data);
extern void *speakerThread(void *data);
extern void *switchThread(void *data);
extern void *displayThread(void *data);
extern void *lightSensorThread(void *data);
extern void *checkAlarmThread(void *data);

#endif /* __COMMON_H__ */