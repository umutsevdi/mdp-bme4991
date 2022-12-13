#include "driver.h"
#include "util.h"
#include <unistd.h>
#define FIRST_MOTOR_PIN_1 27
#define FIRST_MOTOR_PIN_2 22
#define SECOND_MOTOR_PIN_1 23
#define SECOND_MOTOR_PIN_2 24

void dv_drive(const dv_conf *args, int read_pipe) {
  wiringPiSetup();
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
    printf("%i\tdata{%d %lu %d}\n", args->pid, data.direction, data.timestamp,
           data.c_bit);

    switch (data.direction) {
    case DIRECTION_NONE:
      break;
    case DIRECTION_LEFT:
      go_left();
      break;
    case DIRECTION_DOWN:
      go_back();
      break;
    case DIRECTION_UP:
      go();
      break;
    case DIRECTION_RIGHT:
      go_right();
      break;
    }
    // wait for movement to occur
    delayMicroseconds(50);
  }
}

void go() {
  digitalWrite(FIRST_MOTOR_PIN_1, HIGH);
  digitalWrite(FIRST_MOTOR_PIN_2, LOW);
  digitalWrite(SECOND_MOTOR_PIN_1, LOW);
  digitalWrite(SECOND_MOTOR_PIN_2, HIGH);
}
void go_left() {
  digitalWrite(FIRST_MOTOR_PIN_1, HIGH);
  digitalWrite(FIRST_MOTOR_PIN_2, LOW);
  digitalWrite(SECOND_MOTOR_PIN_1, HIGH);
  digitalWrite(SECOND_MOTOR_PIN_2, LOW);
}
void go_right() {
  digitalWrite(FIRST_MOTOR_PIN_1, LOW);
  digitalWrite(FIRST_MOTOR_PIN_2, HIGH);
  digitalWrite(SECOND_MOTOR_PIN_1, LOW);
  digitalWrite(SECOND_MOTOR_PIN_2, HIGH);
}
void go_back() {
  digitalWrite(FIRST_MOTOR_PIN_1, LOW);
  digitalWrite(FIRST_MOTOR_PIN_2, HIGH);
  digitalWrite(SECOND_MOTOR_PIN_1, HIGH);
  digitalWrite(SECOND_MOTOR_PIN_2, LOW);
}
