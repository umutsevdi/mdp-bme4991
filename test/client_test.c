// Client side implementation of UDP client-server model
#include "server/header/util.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define PORT 8080

#define HOSTNAME "pi.local"
#define MAXLINE 64

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

bool send_message(int sockfd, struct sockaddr_in servaddr, char *buffer) {
  int len = strnlen(buffer, MAXLINE);
  enum DIRECTION data;
  fgets(buffer, MAXLINE, stdin);
  if (!strncmp(buffer, "NONE", 4)) {
    data = DIRECTION_NONE;
  } else if (!strncmp(buffer, "LEFT", 4)) {
    data = DIRECTION_LEFT;
  } else if (!strncmp(buffer, "DOWN", 4)) {
    data = DIRECTION_DOWN;
  } else if (!strncmp(buffer, "UP", 2)) {
    data = DIRECTION_UP;
  } else if (!strncmp(buffer, "RIGHT", 4)) {
    data = DIRECTION_RIGHT;
  } else if (!strncmp(buffer, "EXIT", 4)) {
    data = DIRECTION_END;
    int msg_status =
        sendto(sockfd, (const char *)&data, sizeof(data), MSG_CONFIRM,
               (const struct sockaddr *)&servaddr, sizeof(servaddr));
    close(sockfd);
    printf("Exiting client\n");
    exit(0);
  } else {
    perror("Unrecognized direction\n");
  }
  printf("data{%d}\n", data);
  int msg_status =
      sendto(sockfd, (const char *)&data, sizeof(data), MSG_CONFIRM,
             (const struct sockaddr *)&servaddr, sizeof(servaddr));
  return msg_status >= 0;
}

// Driver code
int main() {
  int sockfd;
  struct sockaddr_in servaddr;

  char *hname = malloc(sizeof(HOSTNAME));
  strcpy(hname, HOSTNAME);
  char *sv_ip_address = get_ip_by_hostname(hname);
  // "192.168.1.203";
  if (sv_ip_address == NULL) {
    perror("Error: Hostanme " HOSTNAME " was not found, no such entry\n");
    exit(EXIT_FAILURE);
  }
  printf("Server found {hostname:" HOSTNAME ", ip:%s}\n", sv_ip_address);

  // Creating socket file descriptor
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Error: Socket creation failed\n");
    exit(EXIT_FAILURE);
  }

  memset(&servaddr, 0, sizeof(servaddr));

  // Filling server information
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);
  servaddr.sin_addr.s_addr = inet_addr(sv_ip_address);

  sv_motion data;

  printf("Connected\n");
  char buffer[MAXLINE];
  bool sending = true;
  while (sending) {
    sending = send_message(sockfd, servaddr, buffer);
  }

  close(sockfd);
  return 0;
}
