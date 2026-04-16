#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

typedef struct {
    int server_socket;
    struct sockaddr_in server_address;
    int server_port;
} ServerConfig;

// Initializes a UDP socket, increases the queue buffer, and binds it to a port
ServerConfig server_init(int port);

// Parses an incoming datagram and routes it to the correct application logic
void route_packet(int sock, struct sockaddr_in *client_addr, char *message);

#endif //SERVER_H
