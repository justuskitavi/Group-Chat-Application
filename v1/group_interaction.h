#ifndef GROUP_INTERACTION_H
#define GROUP_INTERACTION_H

#include <stdbool.h>

// File Path Constants
#define GROUPS_FILE "groups.txt"
#define MEMBERSHIPS_FILE "memberships.txt"

/**
 * Displays all existing groups from GROUPS_FILE.
 */
void show_all_groups();

/**
 * Prompts user for a group name, verifies its existence, 
 * and adds the provided username to MEMBERSHIPS_FILE if not already present.
 * Returns true on success.
 */
bool join_group(const char* username);

/**
 * Searches MEMBERSHIPS_FILE and prints all records 
 * associated with a specific user.
 */
void show_user_groups(const char* target_user);

/**
 * Searches MEMBERSHIPS_FILE and prints all users 
 * belonging to a specific group.
 */
void show_group_members(const char* group_name);

/**
 * Removes the membership record for a specific user and group.
 * Returns true if the operation was successful.
 */
bool leave_group(const char* username, char* group_name);

#endif // GROUP_INTERACTION_H