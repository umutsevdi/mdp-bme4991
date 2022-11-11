#include "util.h"

long long util_ts() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (((long long)tv.tv_sec) * 1000) + (tv.tv_usec / 1000);
}

int util_mkbit(int dir, long long ts) { return ts ^ dir; }
