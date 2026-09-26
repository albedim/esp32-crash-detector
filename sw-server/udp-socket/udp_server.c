#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "udp_server.h"

int prepare_udp_server(int port)
{
  int sockfd;
  struct sockaddr_in server_addr;

  // create the socket
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("[UDP Server] Socket creation failed");
    return -1;
  }

  // configure address
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);

  if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
  {
    perror("[UDP sever] Bind failed");
    close(sockfd);
    return -1;
  }

  printf("[UDP sever] UDP Server prepared and listening on port %d...\n", port);
  return sockfd;
}

int receive_single_message(int sockfd, char *buffer, int max_len)
{
  struct sockaddr_in client_addr;
  socklen_t client_len = sizeof(client_addr);

  memset(&client_addr, 0, sizeof(client_addr));

  // recvfrom blocks until ONE message arrives
  int received_bytes = recvfrom(sockfd, buffer, max_len - 1, MSG_WAITALL,
    (struct sockaddr *)&client_addr, &client_len);

  if (received_bytes < 0)
  {
    perror("[UDP sever] receive error");
    return -1;
  }

  buffer[received_bytes] = '\0'; // null-terminate safely

  printf("Received from %s:%d -> %s\n",
    inet_ntoa(client_addr.sin_addr),
    ntohs(client_addr.sin_port),
    buffer
  );

  return received_bytes; // return the number of bytes received
}