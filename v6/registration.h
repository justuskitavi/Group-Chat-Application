#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <stdbool.h>

#define GROUPS_FILE "groups.txt"
#define USERS_FILE "users.txt"
#define MEMBERSHIPS_FILE "memberships.txt"

/**
 * Generates a djb2 hash for a given password string.
 */
unsigned long hash_password(const char *password);

/**
 * Prompts for a group name and saves a record of "username group_name".
 * Returns true on success.
 */
bool group_registration(int client_socket, const char* username, const char *group_name);

/**
 * Removes a group from GROUPS_FILE and scrubs all its memberships.
 * Returns true if the group was found and processed.
 */
bool delete_group(int client_socket, const char* username, const char *group_name);

/**
 * Handles user signup: captures name, hashes password, and saves to file.
 */
bool user_registration(int client_socket, char* user_record);

#endif // REGISTRATION_H