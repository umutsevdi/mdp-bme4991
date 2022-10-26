#include "server.h"
#include "util.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

void sv_listen(const sv_conf *args, int write_pipe) {
  long long ts = timestamp();
  int sockfd;
  char *hello = "Hello from server";
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
    // @TODO add threading here maybe
    char buffer[args->max_line];
    n = recvfrom(sockfd, (char *)buffer, args->max_line, MSG_WAITALL,
                 (struct sockaddr *)&cliaddr, &len);
    // @TODO Handle this text processing in a separate thread
    if (n > 0) {
      // Set buffer as `null terminated string`
      buffer[n] = '\0';

      printf("Child: received from %d:%d\t: %s\n", cliaddr.sin_addr.s_addr,
             cliaddr.sin_port, buffer);
      sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM,
             (const struct sockaddr *)&cliaddr, len);
      // Writing to the pipe
      ts = timestamp();
      sv_motion motion_data = {.dir_x = 10, .dir_y = 5, .timestamp = ts};
      printf("Sending {%d, %d, %lu} to parent\n", motion_data.dir_x,
             motion_data.dir_y, motion_data.timestamp);
      write(write_pipe, (char *)&motion_data, sizeof(motion_data));
      printf("Response sent\n");
    }
  }
}

/**
 * parses incoming UDP data into a valid struct
 * @param buffer incoming message as char array
 * @return struct data
 */
sv_motion *parse(char *buffer) { return NULL; }
