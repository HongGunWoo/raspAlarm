#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <pthread.h>
#include "common.h"

#define BAUD_RATE 115200
// #define BAUD_RATE 9600
#define MAX_COMMAND_LENGTH 100
static const char *UART2_DEV = "/dev/ttyAMA1"; // UART2 연결을 위한 장치 파일

void *bluetoothThread(void *data);

char *serialReadBytes(const int fd);
void serialWriteBytes(const int fd, const char *s);

void printAlarm(int fd_serial, void *state);
char *subString(int s, int e, char *str);

void insertAlarm(int fd_serial, int data, void *state);
void selectAlarm(int fd_serial, void *state);
void deleteAlarm(int fd_serial, int data, void *state);
void updateAlarm(int fd_serial, int pre, int cur, void *state);

/*
<Usage>
insert MMDDHHmm
select
delete MMDDHHmm
update MMDDHHmm MMDDHHmm
*/

void *bluetoothThread(void *state)
{
  int fd_serial;

  if ((fd_serial = serialOpen(UART2_DEV, BAUD_RATE)) < 0) // UART2 포트 오픈
  {
    printf("Unable to open serial device.\n");
  }
  printf("open serial device!!\n");

  serialWriteBytes(fd_serial, "please input command\n");

  while (1)
  {
    char *command = serialReadBytes(fd_serial);
    printf("command : %s", command);

    // filtering command TODO
    char *crud = subString(0, 5, command);
    printf("crud : %s\n", crud);

    if (strcmp(crud, "insert") == 0)
    {
      char *data = subString(7, 14, command);
      insertAlarm(fd_serial, atoi(data), state);
      free(data);
    }

    else if (strcmp(crud, "select") == 0)
    {
      selectAlarm(fd_serial, state);
    }

    else if (strcmp(crud, "delete") == 0)
    {
      char *data = subString(7, 14, command);
      deleteAlarm(fd_serial, atoi(data), state);

      free(data);
    }

    else if (strcmp(crud, "update") == 0)
    {
      char *preData = subString(7, 14, command);
      printf("pre : dddd\n");
      char *curData = subString(16, 23, command);
      printf("cur : eeee\n");
      updateAlarm(fd_serial, atoi(preData), atoi(curData), state);

      free(preData);
      free(curData);
    }

    free(command);
    free(crud);
    delay(10);
  }
  serialClose(fd_serial);
}

void selectAlarm(int fd_serial, void *state)
{
  serialWriteBytes(fd_serial, "select complete!\n");
  printAlarm(fd_serial, state);
}

void printAlarm(int fd_serial, void *state)
{
  for (int i = 0; i < ALARM_SIZE; i++)
  {
    if (((State *)state)->alarm[i] == -1)
    {
      break;
    }

    printf("%d\n", ((State *)state)->alarm[i]);
    char buf[50];
    sprintf(buf, "%d : %d\n", i + 1, ((State *)state)->alarm[i]);

    serialWriteBytes(fd_serial, buf);
    printf("%s", buf);
  }
  printf("complete alarm print\n");
}

void insertAlarm(int fd_serial, int data, void *state)
{
  for (int i = 0; i < ALARM_SIZE; i++)
  {
    if (((State *)state)->alarm[i] != -1)
    {
      continue;
    }
    ((State *)state)->alarm[i] = data;
    serialWriteBytes(fd_serial, "insert complete!\n");
    break;
  }

  printAlarm(fd_serial, state);
}

void deleteAlarm(int fd_serial, int data, void *state)
{
  int deleteIndex;
  for (int i = 0; i < ALARM_SIZE; i++)
  {
    if (((State *)state)->alarm[i] == data)
    {
      printf("find\n");
      deleteIndex = i;
      break;
    }
  }

  for (int i = deleteIndex; i < ALARM_SIZE - 1; i++)
  {
    ((State *)state)->alarm[i] = ((State *)state)->alarm[i + 1];
  }
  ((State *)state)->alarm[ALARM_SIZE - 1] = -1;

  serialWriteBytes(fd_serial, "delete complete!\n");
  printAlarm(fd_serial, state);
}

void updateAlarm(int fd_serial, int pre, int cur, void *state)
{
  for (int i = 0; i < ALARM_SIZE; i++)
  {
    if (pre == ((State *)state)->alarm[i])
    {
      ((State *)state)->alarm[i] = cur;
      break;
    }
  }
  serialWriteBytes(fd_serial, "update complete!\n");
  printAlarm(fd_serial, state);
}

void serialWriteBytes(const int fd, const char *s)
{
  write(fd, s, strlen(s));
}

char *serialReadBytes(const int fd)
{
  char *command = (char *)malloc(sizeof(char) * MAX_COMMAND_LENGTH + 1);

  int index = 0;
  // memset(command,0,MAX_COMMAND_LENGTH+1);

  while (serialDataAvail(fd) > -1 && index < MAX_COMMAND_LENGTH)
  {
    command[index] = serialGetchar(fd);
    if (command[index] == '\n')
    {
      break;
    }
    index++;
  }

  return command;
}

char *subString(int s, int e, char *str)
{
  char *new = (char *)malloc(sizeof(char) * (e - s + 2));
  strncpy(new, str + s, e - s + 1);
  new[e - s + 1] = '\0';
  return new;
}
