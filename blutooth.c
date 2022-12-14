#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <pthread.h>
#include "common.h"

#define BAUD_RATE 115200
#define MAX_COMMAND_LENGTH 100
static const char *UART2_DEV = "/dev/ttyAMA1";

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

  if ((fd_serial = serialOpen(UART2_DEV, BAUD_RATE)) < 0)
  {
    printf("Unable to open serial device.\n");
  }
  printf("open serial device!!\n");

  serialWriteBytes(fd_serial, "명령어를 입력해주세요.\n");
  serialWriteBytes(fd_serial, "알람 설정 -> insert/update/delete MMDDHHmm\n");
  serialWriteBytes(fd_serial, "알람 확인 -> select\n");


  while (1)
  {
    if (serialDataAvail(fd_serial))
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
        char *curData = subString(16, 23, command);
        updateAlarm(fd_serial, atoi(preData), atoi(curData), state);

        free(preData);
        free(curData);
      }

      free(command);
      free(crud);
    }
  }
  serialClose(fd_serial);
}

void selectAlarm(int fd_serial, void *state)
{
  serialWriteBytes(fd_serial, "알람 리스트 확인!\n");
  printAlarm(fd_serial, state);
}

void printAlarm(int fd_serial, void *state)
{
  for (int i = 0; i < ALARM_SIZE; i++)
  {
    if (((State *)state)->alarm[i] == -1)
    {
      if(i == 0)
      {
        serialWriteBytes(fd_serial, "설정된 알람이 없습니다.\n");
        return;
      }
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
    if (((State *)state)->alarm[i] == data)
    {
      serialWriteBytes(fd_serial, "이미 설정된 알람입니다!\n");
      return;
    }
    
    if (((State *)state)->alarm[i] != -1)
    {
      continue;
    }
    pthread_mutex_lock(&mid);
    ((State *)state)->alarm[i] = data;
    pthread_mutex_unlock(&mid);
    serialWriteBytes(fd_serial, "알람 추가 완료!\n");
    break;
  }

  printAlarm(fd_serial, state);
}

void deleteAlarm(int fd_serial, int data, void *state)
{
  int deleteIndex = -1;
  for (int i = 0; i < ALARM_SIZE; i++)
  {
    if (((State *)state)->alarm[i] == data)
    {
      printf("find\n");
      deleteIndex = i;
      break;
    }
  }

  if(deleteIndex == -1)
  {
    serialWriteBytes(fd_serial, "해당 알람이 존재하지 않습니다.\n");
    return;
  }

  for (int i = deleteIndex; i < ALARM_SIZE - 1; i++)
  {
    pthread_mutex_lock(&mid);
    ((State *)state)->alarm[i] = ((State *)state)->alarm[i + 1];
    pthread_mutex_unlock(&mid);
  }
  pthread_mutex_lock(&mid);
  ((State *)state)->alarm[ALARM_SIZE - 1] = -1;
  pthread_mutex_unlock(&mid);

  serialWriteBytes(fd_serial, "알람 삭제 완료!\n");
  printAlarm(fd_serial, state);
}

void updateAlarm(int fd_serial, int pre, int cur, void *state)
{
  for (int i = 0; i < ALARM_SIZE; i++)
  {
    if (pre == ((State *)state)->alarm[i])
    {
      for (int i = 0; i < ALARM_SIZE; i++)
      {
        if (cur == ((State *)state)->alarm[i])
        {
          serialWriteBytes(fd_serial, "변경할 알람이 이미 존재합니다!\n");
          return;
        }
      }
      pthread_mutex_lock(&mid);
      ((State *)state)->alarm[i] = cur;
      pthread_mutex_unlock(&mid);
      serialWriteBytes(fd_serial, "알람 변경 완료!\n");
      printAlarm(fd_serial, state);
      return;
    }
  }
  serialWriteBytes(fd_serial, "해당 알람이 존재하지 않습니다.\n");
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
