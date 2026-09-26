#ifndef UDP_SERVER_H
#define UDP_SERVER_H

// prepares the socket and binds it. Returns the socket file descriptor.
int prepare_udp_server(int port);

// listens for a single message. Designed to be called inside a loop.
int receive_single_message(int sockfd, char *buffer, int max_len);

#endif