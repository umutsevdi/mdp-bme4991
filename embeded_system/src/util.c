#include "util.h"
#include <limits.h>
#include <stdlib.h>

enum DIRECTION dir;

int util_ts() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (((long long)tv.tv_sec) * 1000) + (tv.tv_usec / 1000) % 100000;
}

enum DIRECTION util_get() { return dir; }

void util_set(enum DIRECTION direction) { dir = direction; }
