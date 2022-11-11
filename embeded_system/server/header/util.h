#ifndef UTIL
#define UTIL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

enum direction {
  DIRECTION_NONE = -1,
  DIRECTION_LEFT = 0,
  DIRECTION_DOWN = 1,
  DIRECTION_UP = 2,
  DIRECTION_RIGHT = 3,
  DIRECTION_LEFT_BACK = 4,
  DIRECTION_RIGHT_BACK = 5,
};

typedef struct SV_MOTION_DATA {
  enum direction dir;
  unsigned long timestamp;
  int c_bit;
} sv_motion;

/**
 * creates time stamp
 * @return the current time in milliseconds
 */
long long util_ts();
/**
 * generates a correction bit using xor
 */
int util_mkbit(int dir, long long ts);

#endif
