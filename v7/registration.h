#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <stdbool.h>

#define GROUPS_FILE "groups.txt"
#define USERS_FILE "users.txt"
#define MEMBERSHIPS_FILE "memberships.txt"

unsigned long hash_password(const char *password);
bool group_registration(const char* username, const char *group_name);
bool delete_group(const char* username, const char *group_name);
bool user_registration(char* user_record);

#endif // REGISTRATION_H
