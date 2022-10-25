#include "server.h"
#include <pthread.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  const sv_conf server_config = {.port = 8080, .max_line = 1024};
  sv_listen((void *)&server_config);
  printf("end of thread\n");

  return 0;
}
