#include "driver.h"
#include "server.h"
#include "util.h"
#include <pthread.h>

#define PORT 8080
#define MAX_LINE 64

int main(int argc, char *argv[]) {

  if (argc < 2) {
    printf("Usage: %s [low|high]\n", argv[0]);
    return 1;
  }
  enum SEVERITY severity;
  if (strncasecmp(argv[1], "LOW", 3) == 0) {
    severity = SEVERITY_LOW;
  } else if (strncasecmp(argv[1], "HIGH", 3) == 0) {
    severity = SEVERITY_HIGH;
  } else {
    printf("Invalid argument: %s\n, falling back to SEVERITY_LOW", argv[1]);
    severity = SEVERITY_LOW;
  }
  pthread_t sv_thread;
  pthread_t dv_thread;

  logger *log = logger_new(severity);
  const sv_conf conf = {
      .max_line = MAX_LINE,
      .port = PORT,
      .log = log,
  };
  const dv_conf d_conf = {
      .log = log,
  };

  if (pthread_create(&sv_thread, NULL, sv_listen, (void *)&conf)) {
    printf("Error: Can not create UDP listener thread\n");
    exit(EXIT_FAILURE);
  }

  if (pthread_create(&dv_thread, NULL, dv_drive, (void *)&d_conf)) {
    printf("Error: Can not create driver thread\n");
    exit(EXIT_FAILURE);
  }

  // Wait for both threads to finish
  if (pthread_join(sv_thread, NULL)) {
    printf("Error: Could not join server thread\n");
  }

  if (pthread_join(dv_thread, NULL)) {
    printf("Error: Could not join driver thread\n");
  }
  free(log);
  return 0;
}
