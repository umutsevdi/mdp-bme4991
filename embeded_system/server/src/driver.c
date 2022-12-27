#include "driver.h"
#include "util.h"
#include <stdio.h>
#include <unistd.h>
#define FIRST_MOTOR_PIN_1 27
#define FIRST_MOTOR_PIN_2 22
#define SECOND_MOTOR_PIN_1 23
#define SECOND_MOTOR_PIN_2 24

void dv_drive(const dv_conf *args, int read_pipe) {
  wiringPiSetupGpio();
  // setting pins
  pinMode(FIRST_MOTOR_PIN_1, OUTPUT);
  pinMode(FIRST_MOTOR_PIN_2, OUTPUT);
  pinMode(SECOND_MOTOR_PIN_1, OUTPUT);
  pinMode(SECOND_MOTOR_PIN_2, OUTPUT);

  while (1) {
    printf("%i\tWaiting for the child\n", args->pid);

    char readbuffer[args->max_line];
    bzero(readbuffer, args->max_line);
    int n = read(read_pipe, readbuffer, args->max_line);

    sv_motion data;
    memcpy(&data, readbuffer, sizeof(sv_motion));

    printf("%i\tMessage from child: %s\n", args->pid, readbuffer);
    printf("%i\tdata{%d %lu}\n", args->pid, data.direction, data.timestamp);

    printf("%i\tdirection: ", args->pid);
    switch (data.direction) {
    case DIRECTION_NONE:
      printf("NONE\n");
      dv_stop();
      break;
    case DIRECTION_LEFT:
      printf("LEFT\n");
      dv_left();
      break;
    case DIRECTION_DOWN:
      printf("DOWN\n");
      dv_back();
      break;
    case DIRECTION_UP:
      printf("UP\n");
      dv_forward();
      break;
    case DIRECTION_RIGHT:
      printf("RIGHT\n");
      dv_right();
      break;
    }
    // wait for movement to occur
    delayMicroseconds(50);
  }
}

void dv_forward() {
  digitalWrite(FIRST_MOTOR_PIN_1, HIGH);
  digitalWrite(FIRST_MOTOR_PIN_2, LOW);
  digitalWrite(SECOND_MOTOR_PIN_1, LOW);
  digitalWrite(SECOND_MOTOR_PIN_2, HIGH);
}
void dv_left() {
  digitalWrite(FIRST_MOTOR_PIN_1, HIGH);
  digitalWrite(FIRST_MOTOR_PIN_2, LOW);
  digitalWrite(SECOND_MOTOR_PIN_1, HIGH);
  digitalWrite(SECOND_MOTOR_PIN_2, LOW);
}
void dv_right() {
  digitalWrite(FIRST_MOTOR_PIN_1, LOW);
  digitalWrite(FIRST_MOTOR_PIN_2, HIGH);
  digitalWrite(SECOND_MOTOR_PIN_1, LOW);
  digitalWrite(SECOND_MOTOR_PIN_2, HIGH);
}
void dv_back() {
  digitalWrite(FIRST_MOTOR_PIN_1, LOW);
  digitalWrite(FIRST_MOTOR_PIN_2, HIGH);
  digitalWrite(SECOND_MOTOR_PIN_1, HIGH);
  digitalWrite(SECOND_MOTOR_PIN_2, LOW);
}
void dv_stop() {
  digitalWrite(FIRST_MOTOR_PIN_1, LOW);
  digitalWrite(FIRST_MOTOR_PIN_2, LOW);
  digitalWrite(SECOND_MOTOR_PIN_1, LOW);
  digitalWrite(SECOND_MOTOR_PIN_2, LOW);
}
