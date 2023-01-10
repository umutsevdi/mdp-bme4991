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

enum SEVERITY { SEVERITY_LOW, SEVERITY_HIGH };

typedef struct LOGGER {
  enum SEVERITY sev;
} logger;

/**
 * logger_new - generates a new logger
 * @sev minimum severity to display
 *
 */
logger *logger_new(enum SEVERITY sev);

/**
 * log_f - printf like logging function with SEVERITY control. If the severity
 * is set to SEVERITY_LOW all log messages will be displayed. Else only
 * SEVERITY_HIGH messages will be displayed
 * @sev Severity of the log message
 * @fmt print format
 * @arguments... list of arguments to replace on fmt
 *
 */
void log_f(struct LOGGER *this, enum SEVERITY sev, const char *fmt, ...);

/**
 * gets the direction value
 */
enum DIRECTION util_get();
/**
 * sets the direction value
 */
void util_set(enum DIRECTION direction);

#endif
