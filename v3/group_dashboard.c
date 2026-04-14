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
        char *response = receive_message(client_socket);
        if (response == NULL) {
            printf("Client disconnected from dashboard.\n");
            return;
        }

        const int choice = atoi(response);
        free(response);
        char *group_name;

        switch (choice) {
            case 1:
                group_name = receive_message(client_socket);
                group_name = trim(group_name);
                if(group_registration(client_socket, username, group_name)) {
                    char status = '1';
                    transfer_message(client_socket, &status, 1);
                }else{
                    char status = '0';
                    transfer_message(client_socket, &status, 1);
                }
                free(group_name);
                break;
            case 2:
                group_name = receive_message(client_socket);
                group_name = trim(group_name);
                if(join_group(client_socket, username, group_name)) {
                    char status = '1';
                    transfer_message(client_socket, &status, 1);
                }else{
                    char status = '0';
                    transfer_message(client_socket, &status, 1);
                }
                free(group_name);
                break;
            case 3:
                show_user_groups(client_socket, username);
                break;
            case 4:
                group_name = receive_message(client_socket);
                group_name = trim(group_name);
                show_group_members(client_socket, username, group_name);
                free(group_name);
                break;
            case 5:
                group_name = receive_message(client_socket);
                group_name = trim(group_name);

                if (delete_group(client_socket, username, group_name)) {
                    char status = '1';
                    transfer_message(client_socket, &status, 1);
                }else{
                    char status = '0';
                    transfer_message(client_socket, &status, 1);
                }
                free(group_name);
                break;
            case 6:
                group_name = receive_message(client_socket);
                group_name = trim(group_name);

                if (leave_group(client_socket, username, group_name)) {
                    char status = '1';
                    transfer_message(client_socket, &status, 1);
                }else{
                    char status = '0';
                    transfer_message(client_socket, &status, 1);
                }
                free(group_name);
                break;
            case 7: {
                group_name = receive_message(client_socket);
                group_name = trim(group_name);
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