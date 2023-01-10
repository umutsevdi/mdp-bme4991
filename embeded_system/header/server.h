#ifndef UDP_SERVER
#define UDP_SERVER

#include "util.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

/**
 * configuration related information about the UDP
 * listener server
 */
typedef struct SV_SERVER_CONFIG {
  int port;
  int max_line;
  logger *log;
} sv_conf;

/**
 * sv_listen - creates a UDP server
 * @args sv_conf pointer arguments of server in server configuration
 */
void *sv_listen(void *args);

/**
 * sv_handle - handles incoming request and sends received sv_motion to
 * write_pipe
 * @sockfd socket to handle
 * @cliaddr client address
 * @sv_args arguments of the sv_listen thread.
 *
 */
void sv_handle(int sockfd, struct sockaddr_in cliaddr, const sv_conf *sv_args);
#endif
