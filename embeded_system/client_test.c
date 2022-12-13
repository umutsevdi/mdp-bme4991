// Client side implementation of UDP client-server model
#include "server/header/util.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>

#define PORT 8080

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

// Driver code
int main() {
  int sockfd;
  char buffer[MAXLINE];
  char msg[MAXLINE];
  sv_motion data;

  scanf("%d", &data.direction);
  data.timestamp = util_ts();
  data.c_bit = util_mkbit(data.direction, data.timestamp);
  printf("data{%d %lu %d}\n", data.direction, data.timestamp, data.c_bit);

  struct sockaddr_in servaddr;

  char *sv_ip_address = get_ip_by_hostname(HOSTNAME);
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
