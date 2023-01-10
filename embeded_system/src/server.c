#include "server.h"
#include "util.h"

void sv_handle(int sockfd, struct sockaddr_in cliaddr, const sv_conf *sv_args) {
  char buffer[sv_args->max_line];
  bzero(buffer, sv_args->max_line);
  int len = sizeof(cliaddr);
  int n = recvfrom(sockfd, (char *)buffer, sv_args->max_line, MSG_WAITALL,
                   (struct sockaddr *)&cliaddr, &len);
  if (n > 0) {
    log_f(sv_args->log, SEVERITY_HIGH, "%ul:%d\t Message received: \n",
          cliaddr.sin_addr.s_addr, cliaddr.sin_port);
    enum DIRECTION data;
    memcpy(&data, buffer, sizeof(n));
    printf("%d\n", data);
    util_set(data);
  }
}

void *sv_listen(void *args) {
  const sv_conf *sv_args = (sv_conf *)args;
  int sockfd;
  struct sockaddr_in servaddr, cliaddr;
  // Creating socket file descriptor
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Error: Socket creation failed\n");
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
    perror("Error: Bind failed\n");
    exit(EXIT_FAILURE);
  }
  log_f(sv_args->log, SEVERITY_HIGH, "UDP Server started at: %s:%d\n",
        servaddr.sin_addr.s_addr, sv_args->port);
  while (util_get() != DIRECTION_END) {
    log_f(sv_args->log, SEVERITY_LOW, "Server is listening\n");
    sv_handle(sockfd, cliaddr, sv_args);
  }
  log_f(sv_args->log, SEVERITY_LOW, "END received, closing server\n");
  return NULL;
}
