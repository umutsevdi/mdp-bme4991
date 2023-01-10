#include "driver.h"
#include "util.h"
#include <stdio.h>
#define FIRST_MOTOR_PIN_1 27
#define FIRST_MOTOR_PIN_2 22
#define SECOND_MOTOR_PIN_1 23
#define SECOND_MOTOR_PIN_2 24

/* handles DIRECTION_NONE*/
void dv_stop();
/* handles DIRECTION_UP*/
void dv_forward();
/* handles DIRECTION_LEFT*/
void dv_left();
/* handles DIRECTION_RIGHT*/
void dv_right();
/* handles DIRECTION_RIGHT*/
void dv_back();

void *dv_drive(void *args) {
  const dv_conf *dv_args = (dv_conf *)args;
  wiringPiSetupGpio();
  // setting pins
  pinMode(FIRST_MOTOR_PIN_1, OUTPUT);
  pinMode(FIRST_MOTOR_PIN_2, OUTPUT);
  pinMode(SECOND_MOTOR_PIN_1, OUTPUT);
  pinMode(SECOND_MOTOR_PIN_2, OUTPUT);
  enum DIRECTION dir_old;
  log_f(dv_args->log, SEVERITY_HIGH, "Driver is ready\n");

  while (1) {
    enum DIRECTION direction = util_get();
    if (direction != dir_old) {
      dir_old = direction;
      log_f(dv_args->log, SEVERITY_HIGH, "New Command: %d\n", direction);
    }
    switch (direction) {
    case DIRECTION_NONE:
      dv_stop();
      break;
    case DIRECTION_LEFT:
      dv_left();
      break;
    case DIRECTION_DOWN:
      dv_back();
      break;
    case DIRECTION_UP:
      dv_forward();
      break;
    case DIRECTION_RIGHT:
      dv_right();
      break;
    case DIRECTION_END:
      printf("Driver closed\n");
      dv_stop();
      exit(0);
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
