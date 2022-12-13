#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h> /* atoi( ) */
#include "common.h"

#define GPIO 5

void* switchThread(void* data){
  pinMode(GPIO, INPUT);
  while(1){
    if(digitalRead(GPIO) == 0){
      ((State*)data)->switchOn = 1;
    }
  }
  return 0;
}


//switchTest
// int main(int argc, char **argv)
// {
// 	wiringPiSetupGpio(); /* wiringPi */
// 	switchThread();
// 	return 0;
// }
