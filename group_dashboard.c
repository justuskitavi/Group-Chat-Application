#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_transfer.h"
#include "group_dashboard.h"
#include "group_interaction.h"
#include "messaging.h"
#include "registration.h"

void handle_group_action(int sock, struct sockaddr_in *client_addr, const char* username, int action_id, const char* payload) {
    char *payload_copy = NULL;
    char *parsed_payload = NULL;
    if (payload) {
        payload_copy = strdup(payload);
        parsed_payload = trim(payload_copy);
    }

    switch (action_id) {
        case 3: // Create Group
            if (group_registration(username, parsed_payload)) {
                send_udp_message(sock, client_addr, "1|Group created successfully.\n");
            } else {
                send_udp_message(sock, client_addr, "0|Failed to create group.\n");
            }
            break;
            
        case 4: // Join Group
            if (join_group(username, parsed_payload)) {
                send_udp_message(sock, client_addr, "1|Joined group successfully.\n");
            } else {
                send_udp_message(sock, client_addr, "0|Failed to join group.\n");
            }
            break;
            
        case 5: // View Joined Groups
            show_user_groups(sock, client_addr, username);
            break;
            
        case 6: // View Group Members
            show_group_members(sock, client_addr, username, parsed_payload);
            break;

        case 7: // Delete Group
            if (delete_group(username, parsed_payload)) {
                send_udp_message(sock, client_addr, "1|Group deleted.\n");
            } else {
                send_udp_message(sock, client_addr, "0|Failed or unauthorized.\n");
            }
            break;
            
        case 8: // Leave Group
            if (leave_group(username, parsed_payload)) {
                send_udp_message(sock, client_addr, "1|Left group successfully.\n");
            } else {
                send_udp_message(sock, client_addr, "0|Could not leave group.\n");
            }
            break;

        case 9: // Get Chat History
            display_messages(sock, client_addr, parsed_payload);
            break;
            
        case 10: // Send a Chat Message (Payload format: GroupName|ParentId|MessageContent)
            if (parsed_payload) {
                char *grp = strtok(parsed_payload, "|");
                char *parent_id = strtok(NULL, "|");
                char *msg = strtok(NULL, "");
                if (grp && parent_id && msg) {
                    save_message(username, trim(grp), trim(msg), atoi(trim(parent_id)));
                    send_udp_message(sock, client_addr, "1|Message saved.\n");
                } else {
                    send_udp_message(sock, client_addr, "0|Invalid message payload.\n");
                }
            }
            break;

        default:
            send_udp_message(sock, client_addr, "ERR: Unknown dashboard command.\n");
            break;
    }

    if (payload_copy) free(payload_copy);
}
