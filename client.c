#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "data_transfer.h"
#include "client.h"

#define MAX_BUFFER_SIZE 1024

void consume_newline(void){
    int character;
    while ((character = getchar()) != '\n' && character != EOF);
}

static char *send_request(int sock, struct sockaddr_in *server_addr, const char *payload) {
    send_udp_message(sock, server_addr, payload);
    return receive_udp_message(sock, server_addr);
}

static void print_server_response(char *response) {
    if (response == NULL) {
        fprintf(stderr, "No response from server.\n");
        return;
    }

    char *body = strchr(response, '|');
    if (body != NULL) {
        printf("%s", body + 1);
    } else {
        printf("%s", response);
    }
}

static void handle_create_group(int sock, struct sockaddr_in *server_addr, const char *username) {
    char group_name[100];
    char payload[256];

    printf("Enter group name: ");
    if (!fgets(group_name, sizeof(group_name), stdin)) return;
    group_name[strcspn(group_name, "\n")] = '\0';

    snprintf(payload, sizeof(payload), "3|%s|%s", username, group_name);
    char *response = send_request(sock, server_addr, payload);
    print_server_response(response);
    free(response);
}

static void handle_join_group(int sock, struct sockaddr_in *server_addr, const char *username) {
    char group_name[100];
    char payload[256];

    printf("Enter group name: ");
    if (!fgets(group_name, sizeof(group_name), stdin)) return;
    group_name[strcspn(group_name, "\n")] = '\0';

    snprintf(payload, sizeof(payload), "4|%s|%s", username, group_name);
    char *response = send_request(sock, server_addr, payload);
    print_server_response(response);
    free(response);
}

static void handle_show_groups(int sock, struct sockaddr_in *server_addr, const char *username) {
    char payload[160];
    snprintf(payload, sizeof(payload), "5|%s", username);

    char *response = send_request(sock, server_addr, payload);
    print_server_response(response);
    free(response);
}

static void handle_show_members(int sock, struct sockaddr_in *server_addr, const char *username) {
    char group_name[100];
    char payload[256];

    printf("Enter group name: ");
    if (!fgets(group_name, sizeof(group_name), stdin)) return;
    group_name[strcspn(group_name, "\n")] = '\0';

    snprintf(payload, sizeof(payload), "6|%s|%s", username, group_name);
    char *response = send_request(sock, server_addr, payload);
    print_server_response(response);
    free(response);
}

static void handle_delete_group(int sock, struct sockaddr_in *server_addr, const char *username) {
    char group_name[100];
    char payload[256];

    printf("Enter group name: ");
    if (!fgets(group_name, sizeof(group_name), stdin)) return;
    group_name[strcspn(group_name, "\n")] = '\0';

    snprintf(payload, sizeof(payload), "7|%s|%s", username, group_name);
    char *response = send_request(sock, server_addr, payload);
    print_server_response(response);
    free(response);
}

static void handle_leave_group(int sock, struct sockaddr_in *server_addr, const char *username) {
    char group_name[100];
    char payload[256];

    printf("Enter group name: ");
    if (!fgets(group_name, sizeof(group_name), stdin)) return;
    group_name[strcspn(group_name, "\n")] = '\0';

    snprintf(payload, sizeof(payload), "8|%s|%s", username, group_name);
    char *response = send_request(sock, server_addr, payload);
    print_server_response(response);
    free(response);
}

static void handle_enter_chat(int sock, struct sockaddr_in *server_addr, const char *username) {
    char group_name[100];
    char payload[512];
    char input[MAX_BUFFER_SIZE];

    printf("Enter group name: ");
    if (!fgets(group_name, sizeof(group_name), stdin)) return;
    group_name[strcspn(group_name, "\n")] = '\0';

    while (1) {
        snprintf(payload, sizeof(payload), "9|%s|%s", username, group_name);
        char *history = send_request(sock, server_addr, payload);
        print_server_response(history);
        free(history);

        printf("Commands: 'leave' | 'reply <id> <message>' | or type to send.\n%s: ", username);
        if (!fgets(input, sizeof(input), stdin)) {
            printf("Leaving chat...\n");
            return;
        }
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "leave") == 0) {
            printf("Leaving chat...\n");
            return;
        }

        if (strncmp(input, "reply ", 6) == 0) {
            int parent_id;
            char message[MAX_BUFFER_SIZE];

            if (sscanf(input, "reply %d %[^\n]", &parent_id, message) == 2) {
                snprintf(payload, sizeof(payload), "10|%s|%s|%d|%s", username, group_name, parent_id, message);
            } else {
                printf("Invalid format. Use: reply <id> <message>\n");
                continue;
            }
        } else if (strlen(input) > 0) {
            snprintf(payload, sizeof(payload), "10|%s|%s|0|%s", username, group_name, input);
        } else {
            continue;
        }

        char *response = send_request(sock, server_addr, payload);
        print_server_response(response);
        free(response);
    }
}

int client_init(struct sockaddr_in *server_address){
    const int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket < 0) {
        perror("Could not create socket");
        exit(1);
    }

    server_address->sin_family = AF_INET;
    server_address->sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address->sin_port = htons(9000);

    return client_socket;
}

void handle_register(int sock, struct sockaddr_in *server_addr){
    char username[100];
    char password[20];
    char payload[150];

    printf("Enter username: ");
    fgets(username, sizeof(username), stdin); 
    username[strcspn(username, "\n")] = '\0';

    printf("Enter password: ");
    scanf("%19s", password);
    consume_newline();

    snprintf(payload, sizeof(payload), "1|%s |%s", username, password);
    char *response = send_request(sock, server_addr, payload);
    if (response) {
        print_server_response(response);
        free(response);
    }
}

void handle_login(int sock, struct sockaddr_in *server_addr){
    char username[100];
    char password[20];
    char payload[150];

    printf("Enter username: ");
    fgets(username, sizeof(username), stdin); 
    username[strcspn(username, "\n")] = '\0';

    printf("Enter password: ");
    scanf("%19s", password);
    consume_newline();

    snprintf(payload, sizeof(payload), "2|%s |%s", username, password);
    char *response = send_request(sock, server_addr, payload);
    if (response) {
        if (response[0] == '1') {
            printf("Login successful.\n");
            free(response);
            run_dashboard(sock, server_addr, username);
            return;
        } else {
            print_server_response(response);
        }
        free(response);
    }
}

void run_dashboard(int sock, struct sockaddr_in *server_addr, const char *username) {
    while (1) {
        printf("\nWelcome, %s!\n"
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

        char selection = getchar();
        consume_newline();

        switch (selection) {
            case '1': handle_create_group(sock, server_addr, username); break;
            case '2': handle_join_group(sock, server_addr, username); break;
            case '3': handle_show_groups(sock, server_addr, username); break;
            case '4': handle_show_members(sock, server_addr, username); break;
            case '5': handle_delete_group(sock, server_addr, username); break;
            case '6': handle_leave_group(sock, server_addr, username); break;
            case '7': handle_enter_chat(sock, server_addr, username); break;
            case '8': {
                char payload[160];
                snprintf(payload, sizeof(payload), "11|%s", username);
                char *response = send_request(sock, server_addr, payload);
                if (response) {
                    print_server_response(response);
                    free(response);
                }
                return;
            }
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
}
