#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <unistd.h>

void my_atexit(void) { printf("Parent said bye\n"); }

int main(int argc, char *argv[]) {

  int sv_pipe[2];
  pipe(sv_pipe);

  pid_t childpid = fork();
  if (!childpid) {
    close(sv_pipe[1]);
    // embedded_program_run(pipe[0]); <-- read pipe
    // TODO insert the code of the car here
    int i = atexit(my_atexit);
    // parent
    while (1) {
      //      sleep(1);
      printf("%i Parent is waiting for child's message:\n", getpid());

      char readbuffer[1024];
      bzero(readbuffer, 1024);
      sv_motion motion_data;
      int n = read(sv_pipe[0], (char *)&motion_data, sizeof(motion_data));
      readbuffer[n] = '\n';
      printf("Parent received message from the child: {%d, %d, %lu}\n",
             motion_data.dir_x, motion_data.dir_y, motion_data.timestamp);
    }
  } else {
    close(sv_pipe[0]);
    printf("Starting UDP server, %d\n", getpid());
    const sv_conf server_config = {.port = 8080, .max_line = 1024};
    sv_listen(&server_config, sv_pipe[1]);
    printf("end of process\n");
  }

  return 0;
}
