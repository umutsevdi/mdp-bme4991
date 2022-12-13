#include "server/header/wiringPi.h"
#define FIRST_MOTOR_PIN_1 27
#define FIRST_MOTOR_PIN_2 22
#define SECOND_MOTOR_PIN_1 23
#define SECOND_MOTOR_PIN_2 24

int main(int argc, char *argv[]) {
  wiringPiSetup();

  pinMode(FIRST_MOTOR_PIN_1, OUTPUT);
  pinMode(FIRST_MOTOR_PIN_2, OUTPUT);
  pinMode(SECOND_MOTOR_PIN_1, OUTPUT);
  pinMode(SECOND_MOTOR_PIN_2, OUTPUT);

  while (1) {
    digitalWrite(FIRST_MOTOR_PIN_1, HIGH);
    digitalWrite(FIRST_MOTOR_PIN_2, LOW);
    digitalWrite(SECOND_MOTOR_PIN_1, LOW);
    digitalWrite(SECOND_MOTOR_PIN_2, HIGH);
    delay(1000);
  }

  return 0;
}
