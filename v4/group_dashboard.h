#ifndef GROUP_DASHBOARD_H
#define GROUP_DASHBOARD_H

#include <netinet/in.h>

/**
 * Stateless dispatcher for group commands.
 * Processes a single action and returns immediately.
 */
void handle_group_action(int sock, struct sockaddr_in *client_addr, const char* username, int action_id, const char* payload);

#endif //GROUP_DASHBOARD_H
