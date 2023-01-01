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
  /*

  int sv_pipe[2];
  pipe(sv_pipe);

  pid_t childpid = fork();

  if (!childpid) {
    const dv_conf conf = {
        .pid = getpid(),
        .max_line = MAX_LINE,
    };
    printf("%i\tStarting up embedded driver{buffer_size: %d}}\n", conf.pid,
           conf.max_line);
    close(sv_pipe[1]);
    dv_drive(&conf, sv_pipe[0]);
    // parent
  } else {
    close(sv_pipe[0]);
    const sv_conf conf = {
        .pid = getpid(),
        .port = PORT,
        .max_line = MAX_LINE,
        .write_pipe = sv_pipe[1],
    };
    printf("%i\tStarting UDP server{port: %d, buffer_size: %d}\n", conf.pid,
           conf.port, conf.max_line);
    sv_listen(&conf);
  }

  */

  return 0;
}
