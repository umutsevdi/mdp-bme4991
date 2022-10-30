#include "server.h"
#include "util.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
void sv_listen(const sv_conf *args, int write_pipe) {
  const pid_t pid = getpid();
  long long ts = UTIL_timestamp();
  int sockfd;
  struct sockaddr_in servaddr, cliaddr;

  // Creating socket file descriptor
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  memset(&cliaddr, 0, sizeof(cliaddr));

  // Filling server information
  servaddr.sin_family = AF_INET; // IPv4
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(args->port);

  // Bind the socket with the server address
  if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  int len, n;

  len = sizeof(cliaddr); // len is value/result
  while (1) {
    printf("%i\tChild is listening\n", pid);
    // @TODO add threading here maybe
    char buffer[args->max_line];
    n = recvfrom(sockfd, (char *)buffer, args->max_line, MSG_WAITALL,
                 (struct sockaddr *)&cliaddr, &len);
    // @TODO Handle this text processing in a separate thread
    if (n > 0) {
      // Set buffer as `null terminated string`
      buffer[n] = '\0';

      printf("%i\treceived{%d:%d}: %s\n", pid, cliaddr.sin_addr.s_addr,
             cliaddr.sin_port, buffer);

      if (args->should_respond) {
        char *response = "OK";
        sendto(sockfd, response, strlen(response), MSG_CONFIRM,
               (const struct sockaddr *)&cliaddr, len);
        printf("%i\tResponse sent to the client\n", pid);
      }
      // Writing to the pipe
      ts = UTIL_timestamp();
      printf("%i\tTransferring to parent\n", pid);
      write(write_pipe, buffer, strlen(buffer));
    }
  }
}

sv_motion *parse(char *buffer) { return NULL; }
