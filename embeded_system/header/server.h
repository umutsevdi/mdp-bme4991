#ifndef UDP_SERVER
#define UDP_SERVER

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
} sv_conf;

/**
 * creates a UDP server, must be executed in a  separate process
 * @args sv_conf pointer arguments of server in server configuration
 */
void *sv_listen(void *args);

/**
 * sends a {@link sv_motion} to write_pipe
 */
void sv_handle(int sockfd, struct sockaddr_in cliaddr, const sv_conf *sv_args);
#endif
