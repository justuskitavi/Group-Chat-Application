#ifndef GROUP_INTERACTION_H
#define GROUP_INTERACTION_H

#include <stdbool.h>
#include <netinet/in.h>

#define GROUPS_FILE "groups.txt"
#define MEMBERSHIPS_FILE "memberships.txt"

void show_all_groups(int sock, struct sockaddr_in *client_addr);
bool join_group(const char* username, const char *group_name);
void show_user_groups(int sock, struct sockaddr_in *client_addr, const char* target_user);
void show_group_members(int sock, struct sockaddr_in *client_addr, const char* username, const char* group_name);
bool is_group_admin(const char* username, const char* group_name);
bool leave_group(const char* username, const char* group_name);

#endif // GROUP_INTERACTION_H
