#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8080
#define MAX_LINE 1024

void my_atexit(void) { printf("Parent said bye\n"); }

int main(int argc, char *argv[]) {

  int sv_pipe[2];
  pipe(sv_pipe);

  pid_t childpid = fork();
  if (!childpid) {
    pid_t pid = getpid();
    printf("%i\tStarting up embeded device\n", pid);
    close(sv_pipe[1]);
    // embedded_program_run(pipe[0]); <-- read pipe
    // TODO insert the code of the car here
    int i = atexit(my_atexit);
    // parent
    while (1) {
      //      sleep(1);
      printf("%i\tWaiting for the child\n", pid);

      char readbuffer[MAX_LINE];
      bzero(readbuffer, MAX_LINE);
      int n = read(sv_pipe[0], readbuffer, MAX_LINE);
      readbuffer[n] = '\n';
      printf("%i\tMessage from child: %s\n", pid, readbuffer);
    }
  } else {
    printf("%i\tStarting UDP server{port: %d, buffer_size: %d}\n", getpid(),
           PORT, MAX_LINE);
    close(sv_pipe[0]);
    const sv_conf server_config = {
        .port = PORT, .max_line = MAX_LINE, .should_respond = 0
    };
    sv_listen(&server_config, sv_pipe[1]);
  }

  return 0;
}
