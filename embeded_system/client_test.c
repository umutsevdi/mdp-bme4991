// Client side implementation of UDP client-server model
#include "server/header/util.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define PORT 8081

#define HOSTNAME "pi.local"
#define MAXLINE 256

/**
 * Gets the IP address of the given host name
 * @param String representation of the local name ex: pi.local, fedora
 * @return IP address as string ex: 192.168.1.1
 *
 */
char *get_ip_by_hostname(char *hostname) {
  struct hostent *host_entry = gethostbyname(hostname);
  if (host_entry == NULL) {
    return NULL;
  }
  return inet_ntoa(*((struct in_addr *)host_entry->h_addr_list[0]));
}

int util_ts() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (((long long)tv.tv_sec) * 1000) + (tv.tv_usec / 1000) % 100000;
}

int util_mkbit(int dir, int ts) { return (ts % 100000) ^ dir; }

// Driver code
int main() {
  int sockfd;
  struct sockaddr_in servaddr;

  char *hname = malloc(sizeof(HOSTNAME));
  strcpy(hname, HOSTNAME);

  char *sv_ip_address = get_ip_by_hostname(hname);
  if (sv_ip_address == NULL) {
    perror("Error while finding given hostname " HOSTNAME ", no such entry\n");
  }
  printf("server{hostname:" HOSTNAME ", ip:%s}\n", sv_ip_address);

  // Creating socket file descriptor
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&servaddr, 0, sizeof(servaddr));

  // Filling server information
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);
  servaddr.sin_addr.s_addr = inet_addr(sv_ip_address);

  sv_motion data;
  data.timestamp = util_ts();

  char direction[10];
  scanf("%s", direction);

  if (!strncmp(direction, "NONE", 4)) {
    data.direction = DIRECTION_NONE;
  } else if (!strncmp(direction, "LEFT", 4)) {
    data.direction = DIRECTION_LEFT;
  } else if (!strncmp(direction, "DOWN", 4)) {
    data.direction = DIRECTION_DOWN;
  } else if (!strncmp(direction, "UP", 4)) {
    data.direction = DIRECTION_UP;
  } else if (!strncmp(direction, "RIGHT", 4)) {
    data.direction = DIRECTION_RIGHT;
  } else {
    perror("Unrecognized direction");
    exit(1);
  }
  printf("data{%d %lu %d}\n", data.direction, data.timestamp, data.c_bit);

  int msg_status =
      sendto(sockfd, (const char *)&data, sizeof(data), MSG_CONFIRM,
             (const struct sockaddr *)&servaddr, sizeof(servaddr));
  if (msg_status == -1) {
    perror("Error while sending the data\n");
  }
  printf("Message sent to server\n");
  close(sockfd);
  return 0;
}
