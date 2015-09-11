/**
 * \file: client.c
 * \author: Naum Azeredo
 */

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//#include <sys/ioctl.h>
//#include <net/if.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MES_BEAT 0
#define MES_RESP 1
#define MES_QUIT 2

const char* beatmes = "BEAT\n";
const char* quitmes = "QUIT\n";

int main(int argc, char** argv) {
  if (argc != 3) {
    printf("Usage: %s <server_ip> <port>\n", argv[0]);
    return 1;
  }

  // temps
  char buffer[1024];

  unsigned short port = atoi(argv[2]);

  // Socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd <= 0) {
    perror("Failed to create socket");
    return 1;
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  inet_aton(argv[1], &addr.sin_addr);
  addr.sin_port = htons(port);

  if (connect(sockfd, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
    perror("Failed to connect socket");
    return 1;
  }

  int quit= 0;
  while (!quit) {
    if (read(sockfd, buffer, 1024) <= 0) {
      perror("Failed to read from connected socket");
      return 1;
    }

    int p = buffer[0] - '0';
    printf("%s", &buffer[1]);

    if (p == MES_BEAT) {
      write(sockfd, beatmes, strlen(beatmes)+1);
    }
    if (p == MES_RESP) {
      fgets(buffer, 1024, stdin);
      write(sockfd, buffer, strlen(buffer)+1);
    }
    if (p == MES_QUIT) {
      write(sockfd, quitmes, strlen(quitmes)+1);
      quit = 1;
    }
  }

  close(sockfd);

  return 0;
}
