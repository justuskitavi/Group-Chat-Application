#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "authentication.h"
#include "data_transfer.h"
#include "group_dashboard.h"
#include "registration.h"
#include "server_stub.h"

#define INPUT_READY "<<INPUT_READY>>"

void svr_addr(struct sockaddr_in *addr, const int port) {
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = inet_addr("127.0.0.1");
    addr->sin_port = htons(port);
}

ServerConfig server_init(const int port) {
    ServerConfig server;
    server.server_port = port;
    // Initialize Server Socket
    server.server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Check for successful creation
    if (server.server_socket < 0) {
        perror(("Could not initialize socket"));
        exit(1);
    }

    // Defining server address
    svr_addr(&server.server_address, port);

    // Binding socket to localhost:{port}
    const int svr_bind = bind(server.server_socket, (struct sockaddr*)&server.server_address, sizeof(server.server_address));
    if (svr_bind < 0) {
        perror("Could not bind to address");
        exit(1);
    }
    return server;
}

int get_client(const ServerConfig* server) {
    struct sockaddr_in client_address;
    socklen_t address_length = sizeof(client_address);

    if (listen(server->server_socket, 5) < 0) {
        perror("Could not listen for clients.\n");
        return -1;
    }

    const int client_socket = accept(server->server_socket, (struct sockaddr *) &client_address, &address_length);

    if (client_socket < 0) {

        if (errno == EINTR) {
            return -1;
        }

        perror("Could not connect to client.\n");
        return -1;
    }

    return client_socket;
}

void* client_handler(void* client) {
    const int client_socket = *(int*)client;
    free(client);

    while (1) {
        const char *welcome_message = "\n--- WELCOME TO GROUP CHAT ---\n"
                            "1. Login\n2. Register\n3. Exit\nSelection: ";
        transfer_message(client_socket, welcome_message, strlen(welcome_message));
        transfer_message(client_socket, INPUT_READY, strlen(INPUT_READY));

        char *response = receive_message(client_socket);
        if (response == NULL) {
            printf("Client disconnected.\n");
            close(client_socket);
            return NULL;
        }

        if (strcmp(response, "1") == 0) {
            free(response);

            char current_user[256] = {0};
            if (authenticate_user(client_socket, current_user, sizeof(current_user))) {
                group_dashboard(client_socket, current_user);
            } else {
                const char *message = "\nAuthentication failed.\n";
                transfer_message(client_socket, message, strlen(message));
            }

        } else if (strcmp(response, "2") == 0) {
            free(response);
            user_registration(client_socket);
        } else if (strcmp(response, "3") == 0) {
            free(response);
            const char *message = "Goodbye.\n";
            transfer_message(client_socket, message, strlen(message));
            break;
        } else {
            free(response);
        }
    }

    close(client_socket);
    return NULL;
}