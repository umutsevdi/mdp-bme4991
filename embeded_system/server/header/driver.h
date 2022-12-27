#ifndef DRIVER
#define DRIVER

#include "wiringPi.h"
#include <unistd.h>

typedef struct DV_DRIVE_CONFIG {
  pid_t pid;
  int max_line;

} dv_conf;

void dv_drive(const dv_conf *args, int read_pipe);

void dv_stop();
void dv_forward();
void dv_left();
void dv_right();
void dv_back();

#endif
