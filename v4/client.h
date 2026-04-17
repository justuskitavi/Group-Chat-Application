#ifndef CLIENT_H
#define CLIENT_H

#include <netinet/in.h>

/**
 * Initializes the connectionless client socket (UDP).
 * Populates the server_address struct.
 */
int client_init(struct sockaddr_in *server_address, char* server_ip);

void run_dashboard(int sock, struct sockaddr_in *server_addr, const char *username);
void handle_register(int sock, struct sockaddr_in *server_addr);
void handle_login(int sock, struct sockaddr_in *server_addr);

#endif /* CLIENT_H */
