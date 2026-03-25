#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_transfer.h"
#include "group_dashboard.h"
#include "group_interaction.h"
#include "messaging.h"
#include "registration.h"

#define INPUT_READY "<<INPUT_READY>>"

void group_dashboard(const int client_socket, char *username) {
    while (1) {
        char menu[512];
        snprintf(menu, sizeof(menu),
           "\nWelcome, %s!\n"
                 "\n--- GROUP DASHBOARD ---\n"
                 "1. Create Group\n"
                 "2. Join Group\n"
                 "3. Show Groups Joined\n"
                 "4. Show Group Members\n"
                 "5. Delete Group\n"
                 "6. Leave Group\n"
                 "7. Enter Chat\n"
                 "8. Logout\n"
                 "Enter your choice: ", username);
        transfer_message(client_socket, menu, strlen(menu));
        transfer_message(client_socket, INPUT_READY, strlen(INPUT_READY));

        char *response = receive_message(client_socket);
        if (response == NULL) {
            printf("Client disconnected from dashboard.\n");
            return;
        }

        const int choice = atoi(response);
        free(response);

        switch (choice) {
            case 1:
                if (group_registration(client_socket, username)) {
                    const char *message = "Group created successfully.\n";
                    transfer_message(client_socket, message, strlen(message));
                }
                break;
            case 2:
                join_group(client_socket, username);
                break;
            case 3:
                show_user_groups(client_socket, username);
                break;
            case 4:
                show_group_members(client_socket, username);
                break;
            case 5:
                if (delete_group(client_socket, username)) {
                    const char *message = "Group deleted successfully.\n";
                    transfer_message(client_socket, message, strlen(message));
                }
                break;
            case 6:
                if (leave_group(client_socket, username)) {
                    const char *message = "Left group successfully.\n";
                    transfer_message(client_socket, message, strlen(message));
                }
                break;
            case 7: {
                const char *prompt = "Enter group name to chat in: ";
                transfer_message(client_socket, prompt, strlen(prompt));
                transfer_message(client_socket, INPUT_READY, strlen(INPUT_READY));

                char *group_name = receive_message(client_socket);
                if (group_name == NULL) return;

                start_chat_session(client_socket, username, group_name);
                free(group_name);
                break;
            }
            case 8:
                return;
            default: {
                const char *message = "Invalid choice. Please try again.\n";
                transfer_message(client_socket, message, strlen(message));
                break;
            }
        }
    }
}