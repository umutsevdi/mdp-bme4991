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
  pid_t pid;
  int port;
  int max_line;
} sv_conf;

/**
 * creates a UDP server, must be executed in a  separate process
 * @param arg arguments of server in server configuration
 * @param write_pipe write file descriptor for the server
 */
void sv_listen(const sv_conf *args, int write_pipe);

#endif
