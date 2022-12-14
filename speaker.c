#include <wiringPi.h>
#include <softTone.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

#define GPIO 12

#define C 523 //262
#define E 659 //330
#define G 784 //392
#define R 0

#define DONE 500
#define DHALF 250
#define DQUATER 125
#define DDOUBLE 1000
#define DTRIPLE 1500

#define MUSIC_NOTES 65

void musicPlay(void* data);
void* speakerThread(void* data);

typedef struct _note{
  int key;
  int length;
}note;

note army[MUSIC_NOTES] = {
  {G,DONE}, {E,DONE}, {E,DHALF}, {E,DHALF}, {E,DONE},
  {E,DHALF}, {G,DHALF}, {E,DHALF}, {C,DHALF}, {E,DHALF}, {G,DQUATER}, {E,DQUATER}, {C,DHALF}, {E,DHALF},
  {G,DHALF}, {E,DHALF}, {E,DONE}, {C,DHALF}, {C,DQUATER}, {C,DQUATER}, {C,DONE},
  {E,DHALF}, {E,DHALF}, {E,DONE}, {E,DHALF}, {G,DHALF}, {E,DHALF}, {C,DHALF},
  {E,DHALF}, {G,DQUATER}, {E,DQUATER}, {C,DHALF}, {E,DHALF}, {G,DHALF}, {E,DHALF}, {E,DONE},
  {C,DHALF}, {C,DQUATER}, {C,DQUATER}, {C,DTRIPLE}, {R,DDOUBLE},
  {G,DHALF}, {G,DQUATER}, {E,DQUATER}, {C,DHALF}, {E,DHALF}, {C,DQUATER}, {E,DQUATER}, {C,DQUATER}, {E,DQUATER}, {G,DONE},
  {C,DONE}, {C,DHALF}, {E,DHALF}, {E,DHALF},
  {G,DHALF}, {G,DQUATER}, {E,DQUATER}, {C,DHALF}, {E,DHALF}, {C,DQUATER}, {E,DQUATER}, {C, DQUATER}, {E,DQUATER},
  {G,DONE} 
};

void* speakerThread(void* data)
{
  softToneCreate(GPIO);
  while(1){
    if(((State*)data)->alarmOn == 1){
      musicPlay(data);
      pthread_mutex_lock(&mid);
      ((State*)data)->alarmOn = 0;
      pthread_mutex_unlock(&mid);
    }
    // delay(100);
  }
}

void musicPlay(void* data){
  for (int i = 0; i < MUSIC_NOTES; i++)
  {
    if(((State*)data)->switchOn == 1){
      pthread_mutex_lock(&mid);
      ((State*)data)->switchOn = 0;
      pthread_mutex_unlock(&mid);
      softToneWrite(GPIO, 0);
      return ;
    }
    softToneWrite(GPIO, army[i].key);
    delay(army[i].length);
  }
  return;
}