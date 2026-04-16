#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <stdbool.h>
#include <netinet/in.h>

#define USERS_FILE "users.txt"

/**
 * Authenticates users using a UDP stateless request.
 * Returns true if matched and false if not.
 */
bool authenticate_user(int server_socket, struct sockaddr_in *client_addr, char* user_record, char* user_ref);

#endif // AUTHENTICATION_H