#include "server.h"
#include "util.h"
#include <stdio.h>
#include <strings.h>

void sv_handle(int sockfd, struct sockaddr_in cliaddr, const sv_conf *sv_args) {
  char buffer[sv_args->max_line];
  bzero(buffer, sv_args->max_line);
  int len = sizeof(cliaddr);
  printf("Listening\n");
  int n = recvfrom(sockfd, (char *)buffer, sv_args->max_line, MSG_WAITALL,
                   (struct sockaddr *)&cliaddr, &len);
  if (n > 0) {
    printf("%d:%d\tSent: %s\n", cliaddr.sin_addr.s_addr, cliaddr.sin_port,
           buffer);
    sv_motion data;
    memcpy(&data.direction, buffer, sizeof(n));
    data.timestamp = util_ts();
    util_set(data.direction);
    //  write(write_pipe, &data, sizeof(sv_motion));
  }
}

void *sv_listen(void *args) {
  const sv_conf *sv_args = (sv_conf *)args;
  long long ts = util_ts();
  int sockfd;
  struct sockaddr_in servaddr, cliaddr;
  // Creating socket file descriptor
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Error: Socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  memset(&cliaddr, 0, sizeof(cliaddr));

  // Filling server information
  servaddr.sin_family = AF_INET; // IPv4
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(sv_args->port);

  // Bind the socket with the server address
  if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror("Error: Bind failed");
    exit(EXIT_FAILURE);
  }

  int len = sizeof(cliaddr);
  int n;
  while (1) {
    printf("Listening\n");
    sv_handle(sockfd, cliaddr, sv_args);
  }
}
