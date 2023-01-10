#include "util.h"
#include <stdarg.h>

enum DIRECTION dir;

void log_f(struct LOGGER *this, enum SEVERITY sev, const char *fmt, ...) {
  // Check if low severity log should be printed
  if (sev == SEVERITY_LOW && this->sev == SEVERITY_HIGH) {
    return;
  }
  // Print the log message
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
}

logger *logger_new(enum SEVERITY sev) {
  logger *l = malloc(sizeof(logger));
  l->sev = sev;
  return l;
}

enum DIRECTION util_get() { return dir; }

void util_set(enum DIRECTION direction) { dir = direction; }
