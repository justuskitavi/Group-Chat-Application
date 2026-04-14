#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

typedef struct {
    int server_socket;
    struct sockaddr_in server_address;
    int server_port;
} ServerConfig;

// Takes in a port number and a pointer to a sockaddr_in struct
// Adds address information to the address provided e.g. domain, family, port
void svr_addr(struct sockaddr_in *addr, int port);

// Initializes a socket and binds it to a given port
// The port number to be used is provided as input
// Returns a file descriptor for the socket
ServerConfig server_init(int port);

int get_client(const ServerConfig* server);

/**
 * Thread routine to handle a single client session.
 * Client - A void pointer to the allocated client_socket integer.
 * Returns -void* Always returns NULL upon client disconnection.
 */
void* client_handler(void* client);

#endif //SERVER_H