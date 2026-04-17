#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "client.h"

int main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    char server_ip[20];

    printf("Provide IP address of server to connect to: ");
    
    fgets(server_ip, sizeof(server_ip), stdin);
    server_ip[strcspn(server_ip, "\n")] = '\0';
    
    struct sockaddr_in server_addr;
    const int client_socket = client_init(&server_addr, server_ip);

    while (1) {
        printf("\n--- WELCOME TO GROUP CHAT ---\n"
               "1. Register\n"
               "2. Login\n"
               "3. Exit\n"
               "Selection: ");

        char selection[3];
        if (!fgets(selection, sizeof(selection), stdin)) break;
        selection[strcspn(selection, "\n")] = '\0';

        switch (atoi(selection)) {
            case 1:
                handle_register(client_socket, &server_addr);
                break;
            case 2:
                handle_login(client_socket, &server_addr);
                break;
            case 3:
                printf("Closing application.\n");
                close(client_socket);
                printf("Disconnected from server.\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
    return 0;
}
