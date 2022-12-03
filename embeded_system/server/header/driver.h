#ifndef DRIVER
#define DRIVER

#include "wiringPi.h"
#include <unistd.h>

typedef struct DV_DRIVE_CONFIG {
  pid_t pid;
  int max_line;

} dv_conf;

void dv_drive(const dv_conf *args, int read_pipe);

void go();
void go_left();
void go_right();
void go_back();

#endif
