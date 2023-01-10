#ifndef DRIVER
#define DRIVER

#include "wiringPi.h"
#include "util.h"
#include <unistd.h>

/**
 * configuration related information about the driver
 */
typedef struct DV_DRIVE_CONFIG {
  logger* log;
} dv_conf;

/**
 * dv_drive - listens the direction and controls the device
 * @args dv_drive configuration arguments in dv_conf
 *
 */
void *dv_drive(void *args);

#endif
