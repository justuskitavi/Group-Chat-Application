#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "data_transfer.h"

#define MAX_BUFFER_SIZE 1024
#define INPUT_READY "<<INPUT_READY>>"

int main() {
    const int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Could not create socket");
        exit(1);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(9000);

    const int connection_status = connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    if (connection_status < 0) {
        perror("Could not connect to server");
        exit(1);
    }

    while (1) {
        while (1) {
            char *server_msg = receive_message(client_socket);

            if (server_msg == NULL) {
                printf("Disconnected from server.\n");
                close(client_socket);
                return 0;
            }

            if (strcmp(server_msg, INPUT_READY) == 0) {
                free(server_msg);
                break;
            }

            printf("%s", server_msg);
            free(server_msg);
        }

        char input[1024];

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            printf("Exiting...\n");
            break;
        }

        transfer_message(client_socket, input, strlen(input));
    }

    close(client_socket);
    return 0;
}