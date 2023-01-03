#ifndef UTIL
#define UTIL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

enum DIRECTION {
  DIRECTION_NONE = '0',
  DIRECTION_LEFT,
  DIRECTION_DOWN,
  DIRECTION_UP,
  DIRECTION_RIGHT,
  DIRECTION_END,
};

typedef struct SV_MOTION_DATA {
  enum DIRECTION direction;
  unsigned long timestamp;
} sv_motion;

/**
 * creates time stamp
 * @return the current time in milliseconds
 */
int util_ts();

enum DIRECTION util_get();
void util_set(enum DIRECTION direction);

#endif
