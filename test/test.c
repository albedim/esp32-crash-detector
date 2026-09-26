#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr,
      "Usage: %s <accelerometer_x,accelerometer_y,accelerometer_z,"
      "gyroscope_x,gyroscope_y,gyroscope_z,"
      "gps_latitude,gps_longitude,speed>\n",
      argv[0]);

    return EXIT_FAILURE;
  }

  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  if (sockfd < 0)
  {
    perror("socket");
    return EXIT_FAILURE;
  }

  struct sockaddr_in server_address;

  memset(&server_address, 0, sizeof(server_address));

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(SERVER_PORT);

  if (inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) <= 0)
  {
    perror("inet_pton");
    close(sockfd);
    return EXIT_FAILURE;
  }

  const char *data = argv[1];

  ssize_t bytes_sent = sendto(
    sockfd,
    data,
    strlen(data),
    0,
    (struct sockaddr *)&server_address,
    sizeof(server_address)
  );

  if (bytes_sent < 0)
  {
    perror("sendto");
    close(sockfd);
    return EXIT_FAILURE;
  }

  printf("sent: %s\n", data);

  close(sockfd);

  return EXIT_SUCCESS;
}