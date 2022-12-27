#include "util.h"
#include <limits.h>

int util_ts() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (((long long)tv.tv_sec) * 1000) + (tv.tv_usec / 1000) % 100000;
}
