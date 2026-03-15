#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <stdbool.h>

// File Path Definitions
#define GROUPS_FILE "groups.txt"
#define USERS_FILE "users.txt"
#define MEMBERSHIPS_FILE "memberships.txt"

/**
 * Generates a djb2 hash for a given password string.
 */
unsigned long hash_password(char *str);

/**
 * Prompts for a group name and saves a record of "username group_name".
 * Returns true on success.
 */
bool group_registration(const char* username);

/**
 * Removes a group from GROUPS_FILE and scrubs all its memberships.
 * Returns true if the group was found and processed.
 */
bool delete_group(const char* username);

/**
 * Handles user signup: captures name, hashes password, and saves to file.
 */
bool user_registration();

#endif // REGISTRATION_H