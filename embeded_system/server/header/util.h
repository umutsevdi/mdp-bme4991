#ifndef UTIL
#define UTIL

#include <sys/time.h>
#include <unistd.h>

#include <stdio.h>

/**
 * creates timestamp
 * @return the current time in milliseconds
 */
long long UTIL_timestamp();
#endif
