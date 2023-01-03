#include "driver.h"
#include "server.h"
#include "util.h"
#include <pthread.h>

#define PORT 8080
#define MAX_LINE 64

int main(int argc, char *argv[]) {

  pthread_t sv_thread;
  pthread_t dv_thread;

  const sv_conf conf = {
      .max_line = MAX_LINE,
      .port = PORT,
  };
  if (pthread_create(&sv_thread, NULL, sv_listen, (void *)&conf)) {
    printf("Error: Can not create UDP listener thread\n");
    exit(EXIT_FAILURE);
  }

  if (pthread_create(&dv_thread, NULL, dv_drive, (void *)NULL)) {
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
  return 0;
}
