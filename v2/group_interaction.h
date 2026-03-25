#ifndef GROUP_INTERACTION_H
#define GROUP_INTERACTION_H

#include <stdbool.h>

#define GROUPS_FILE "groups.txt"
#define MEMBERSHIPS_FILE "memberships.txt"
#define INPUT_READY "<<INPUT_READY>>"

/**
 * Displays all existing groups from GROUPS_FILE.
 */
void show_all_groups(int client_socket);

/**
 * Prompts user for a group name, verifies its existence, 
 * and adds the provided username to MEMBERSHIPS_FILE if not already present.
 * Returns true on success.
 */
bool join_group(int client_socket, const char* username);

/**
 * Searches MEMBERSHIPS_FILE and prints all records 
 * associated with a specific user.
 */
void show_user_groups(int client_socket, const char* target_user);

/**
 * Searches MEMBERSHIPS_FILE and prints all users 
 * belonging to a specific group.
 */
void show_group_members(int client_socket, const char* username);

/**
 * Searches through the GROUPS_FILE to determine whether the current
 * user is the admin if the group. Used in conjunction with other functions
 * e.g. group deletion
 */
bool is_group_admin(const char* username, const char* group_name);

/**
 * Removes the membership record for a specific user and group.
 * Returns true if the operation was successful.
 */
bool leave_group(int client_socket, const char* username);

#endif // GROUP_INTERACTION_H