#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <stdbool.h>

#define USERS_FILE "users.txt"
#define INPUT_READY "<<INPUT_READY>>"

/**
 * Authenticates users by taking their usernames and passwords as input
 * and comparing them to the data stored in the "users.txt" file
 * Returns true if matched and false if not
 * A pointer to where the username is stored is passed to allow reusability of the username
 */
bool authenticate_user(int client_socket, char* username, char* user_ref);

#endif // AUTHENTICATION_H