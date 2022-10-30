// Client side implementation of UDP client-server model
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define PORT 8080

#define HOSTNAME "pi.local"
#define MAXLINE 1024
#define EXPECT_RESPONSE 0

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
  fgets(msg, MAXLINE, stdin);
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

  int msg_status = sendto(sockfd, (const char *)msg, strlen(msg), MSG_CONFIRM,
                          (const struct sockaddr *)&servaddr, sizeof(servaddr));
  if (msg_status == -1) {
    perror("Error while sending the data\n");
  }
  printf("Message sent to server\n");

  if (EXPECT_RESPONSE) {
    int n, len;
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL,
                 (struct sockaddr *)&servaddr, &len);
    buffer[n] = '\0';
    printf("Server : %s\n", buffer);
  }

  close(sockfd);
  return 0;
}
