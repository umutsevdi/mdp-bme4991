#ifndef UDP_SERVER
#define UDP_SERVER
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#endif /* ifndef SYMBOL */

/**
 * configuration related information about the UDP
 * listener server
 */
typedef struct SV_SERVER_CONFIG {
  int port;
  int max_line;
} sv_conf;

/**
 * creates a UDP server, must be executed in a separate
 * thread
 * @param arg {@link server_config}
 */
void sv_listen(void *arg);
