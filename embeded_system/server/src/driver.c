#include "driver.h"
#include "util.h"

void dv_drive(const dv_conf *args, int read_pipe) {
  while (1) {
    //      sleep(1);
    printf("%i\tWaiting for the child\n", args->pid);

    char readbuffer[args->max_line];
    bzero(readbuffer, args->max_line);
    int n = read(read_pipe, readbuffer, args->max_line);

    sv_motion data;
    memcpy(&data, readbuffer, sizeof(sv_motion));

    printf("%i\tMessage from child: %s\n", args->pid, readbuffer);
    printf("%i::data{%d %lu %d}\n",args->pid, data.dir,data.timestamp,data.c_bit);
  }
}
