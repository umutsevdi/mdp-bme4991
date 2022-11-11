#include "driver.h"
#include "server.h"
#include "util.h"

#define PORT 8080
#define MAX_LINE 1024

int main(int argc, char *argv[]) {
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
    };
    printf("%i\tStarting UDP server{port: %d, buffer_size: %d}\n", conf.pid,
           conf.port, conf.max_line);
    sv_listen(&conf, sv_pipe[1]);
  }

  return 0;
}
