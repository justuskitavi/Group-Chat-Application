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
#define INPUT_READY "<<INPUT_READY>>"

void consume_newline(){
    int character;
    while ((character = getchar()) != '\n' && character != EOF);
}

int client_init(char* server_ip){
    const int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Could not create socket");
        exit(1);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(server_ip);
    server_address.sin_port = htons(9000);

    const int connection_status = connect(
        client_socket,
        (struct sockaddr *)&server_address,
        sizeof(server_address));

    if (connection_status < 0) {
        perror("Could not connect to server");
        exit(1);
    }

    return client_socket;
}

void handle_create_group(int client_socket){
    char group_name[100];
    printf("Enter group name: ");

    fgets(group_name, sizeof(group_name), stdin);
    group_name[strcspn(group_name, "\n")] = '\0';

    transfer_message(client_socket, group_name, strlen(group_name));

    char *response = receive_message(client_socket);
    if (response == NULL) {
        fprintf(stderr, "Could not receive response from server.\n");
        return;
    }

    if (atoi(response))
        printf("\nGroup created successfully.\n");
    else
        fprintf(stderr, "\nError creating group.\n");

    free(response);
}

void handle_join_group(int client_socket){
    char group_name[100];
    printf("Enter group name: ");

    fgets(group_name, sizeof(group_name), stdin); 
    group_name[strcspn(group_name, "\n")] = '\0';

    transfer_message(client_socket, group_name, strlen(group_name));

    char *response = receive_message(client_socket);
    if (response == NULL) {
        fprintf(stderr, "Could not receive response from server.\n");
        return;
    }

    if (atoi(response))
        printf("\nJoined group successfully.\n");
    else
        fprintf(stderr, "\nError joining group.\n");

    free(response);
}

void handle_show_groups(int client_socket){
    char *response = receive_message(client_socket);
    if (response == NULL) {
        fprintf(stderr, "Could not receive response from server.\n");
        return;
    }
    printf("%s\n", response);
    free(response);
}

void handle_show_members(int client_socket){
    char group_name[100];
    printf("Enter group name: ");

    fgets(group_name, sizeof(group_name), stdin); 
    group_name[strcspn(group_name, "\n")] = '\0';

    transfer_message(client_socket, group_name, strlen(group_name));

    char *response = receive_message(client_socket);
    if (response == NULL) {
        fprintf(stderr, "Could not receive response from server.\n");
        return;
    }
    printf("%s\n", response);
    free(response); 
}

void handle_delete_group(int client_socket){
    char group_name[100];
    printf("Enter group name: ");

    fgets(group_name, sizeof(group_name), stdin); 
    group_name[strcspn(group_name, "\n")] = '\0';

    transfer_message(client_socket, group_name, strlen(group_name));

    char *response = receive_message(client_socket);
    if (response == NULL) {
        fprintf(stderr, "Could not receive response from server.\n");
        return;
    }

    if (atoi(response)){
        printf("\nGroup deleted successfully.\n");
    }else{
        fprintf(stderr, "\nCould not delete group.\n");
    }

    free(response);
}

void handle_leave_group(int client_socket){
    char group_name[100];
    printf("Enter group name: ");

    fgets(group_name, sizeof(group_name), stdin); 
    group_name[strcspn(group_name, "\n")] = '\0';

    transfer_message(client_socket, group_name, strlen(group_name));

    char *response = receive_message(client_socket);
    if (response == NULL) {
        fprintf(stderr, "Could not receive response from server.\n");
        return;
    }

    if (atoi(response))
        printf("\nLeft group successfully.\n");
    else
        fprintf(stderr, "\nCould not leave group.\n");

    free(response);
}

void handle_enter_chat(int client_socket){
    char group_name[100];
    printf("Enter group name: ");
    fgets(group_name, sizeof(group_name), stdin); 
    group_name[strcspn(group_name, "\n")] = '\0';

    transfer_message(client_socket, group_name, strlen(group_name));

    while (1) {
        while (1) {
            char *server_msg = receive_message(client_socket);
            if (server_msg == NULL) {
                printf("Disconnected from server.\n");
                close(client_socket);
                exit(0);
            }
            if (strcmp(server_msg, "<<EXIT>>") == 0) {
                free(server_msg);
                return;  
            }

            if (strcmp(server_msg, INPUT_READY) == 0) {
                free(server_msg);
                break;
            }
            printf("%s", server_msg);
            free(server_msg);
        }

        char input[MAX_BUFFER_SIZE];
        if (fgets(input, sizeof(input), stdin) == NULL)
            break;

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            printf("Exiting chat...\n");
            break;
        }

        transfer_message(client_socket, input, strlen(input));
    }
}

void run_dashboard(int client_socket, const char *username){
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

        transfer_message(client_socket, &selection, 1);

        switch (selection) {
            case '1': handle_create_group(client_socket);  break;
            case '2': handle_join_group(client_socket);    break;
            case '3': handle_show_groups(client_socket);   break;
            case '4': handle_show_members(client_socket);  break;
            case '5': handle_delete_group(client_socket);  break;
            case '6': handle_leave_group(client_socket);   break; 
            case '7': handle_enter_chat(client_socket);    break;
            case '8': return; 
            default:  printf("Invalid choice. Please try again.\n"); break;
        }
    }
}

void handle_register(int client_socket){
    char username[100];
    char password[20];
    char user_record[150];

    printf("Enter username: ");
    fgets(username, sizeof(username), stdin); 
    username[strcspn(username, "\n")] = '\0';

    printf("Enter password: ");
    scanf("%19s", password);
    consume_newline();

    snprintf(user_record, sizeof(user_record), "1|%s |%s", username, password);
    transfer_message(client_socket, user_record, strlen(user_record));  
    
    char *response = receive_message(client_socket);

    if (response == NULL) {
        fprintf(stderr, "Could not receive response from server.\n");
        return;
    }

    if (atoi(response))
        printf("Registered successfully.\n");
    else
        fprintf(stderr, "Error while registering user.\n");

    free(response);
}

void handle_login(int client_socket){
    char username[100];
    char password[20];
    char user_record[150];

    printf("Enter username: ");
    fgets(username, sizeof(username), stdin); 
    username[strcspn(username, "\n")] = '\0';

    printf("Enter password: ");
    scanf("%19s", password);
    consume_newline();

    snprintf(user_record, sizeof(user_record), "2|%s |%s", username, password);
    transfer_message(client_socket, user_record, strlen(user_record));

    char *response = receive_message(client_socket);
    if (response == NULL) {
        fprintf(stderr, "Could not receive response from server.\n");
        return;
    }

    if (atoi(response)) {
        printf("Logged in successfully.\n");
        free(response);
        run_dashboard(client_socket, username);
    } else {
        fprintf(stderr, "Error authenticating user.\n");
        free(response);
    }
}