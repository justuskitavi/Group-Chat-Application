#ifndef GROUP_DASHBOARD_H
#define GROUP_DASHBOARD_H

#define INPUT_READY "<<INPUT_READY>>"

/**
 * Main loop for managing group-related actions for a specific user.
 * * This function sends menu options to the client, waits for a choice,
 * and routes the logic to various group functions (create, join, chat, etc.)
 * * - Client_socket - The file descriptor for the connected client.
 * - Username - The string containing the authenticated user's name.
 */
void group_dashboard(int client_socket, char* username);

#endif //GROUP_DASHBOARD_H