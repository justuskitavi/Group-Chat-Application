#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "server.h"
#include "data_transfer.h"
#include "authentication.h"
#include "registration.h"
#include "group_dashboard.h"

ServerConfig server_init(const int port) {
    ServerConfig server;
    server.server_port = port;
    
    // Create UDP Datagram Socket (Connectionless)
    server.server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server.server_socket < 0) {
        perror("Could not initialize socket");
        exit(1);
    }

    // Increase the OS Receive Buffer to 8MB to prevent dropping queued UDP messages
    int queue_size = 8 * 1024 * 1024; 
    setsockopt(server.server_socket, SOL_SOCKET, SO_RCVBUF, &queue_size, sizeof(queue_size));

    server.server_address.sin_family = AF_INET;
    server.server_address.sin_addr.s_addr = INADDR_ANY;
    server.server_address.sin_port = htons(port);

    if (bind(server.server_socket, (struct sockaddr *)&server.server_address, sizeof(server.server_address)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    return server;
}

void route_packet(int sock, struct sockaddr_in *client_addr, char *message) {
    if (!message) return;

    // Parse the command ID from the datagram
    char *operation = strtok(message, "|");
    char *payload = strtok(NULL, "");

    if (operation == NULL) return;

    int op_code = atoi(operation);

    switch (op_code) {
        case 1: // Registration
            if (payload && user_registration(payload)) {
                send_udp_message(sock, client_addr, "1|Registration successful.\n");
            } else {
                send_udp_message(sock, client_addr, "0|Registration failed or user exists.\n");
            }
            break;

        case 2: // Login
            if (payload) {
                char current_user[100] = {0};
                if (authenticate_user(sock, client_addr, payload, current_user)) {
                    send_udp_message(sock, client_addr, "1|Login successful.\n");
                }
            }
            break;

        // Stateless group/dashboard/chat actions
        // Expected payload format: Username|ActionPayload
        case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10:
            if (payload) {
                char *username = strtok(payload, "|");
                char *action_payload = strtok(NULL, "");

                handle_group_action(sock, client_addr, username, op_code, action_payload);
            }
            break;

        case 11: // Disconnect / Leave
            send_udp_message(sock, client_addr, "1|Goodbye.\n");
            break;

        default:
            send_udp_message(sock, client_addr, "ERR: Unknown server operation.\n");
            break;
    }
}
