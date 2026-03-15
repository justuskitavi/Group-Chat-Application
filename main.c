#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "v1/messaging.h"
#include "v1/authentication.h"
#include "v1/registration.h"
#include "v1/group_interaction.h"

void group_dashboard(char* username){
    int choice;
    while(1){
        printf("\n--- GROUP DASHBOARD ---\n");
        printf("1. Create Group\n");
        printf("2. Join Group\n");
        printf("3. Leave Group\n");
        printf("4. Enter Chat\n");
        printf("5. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline

        switch(choice){
            case 1:
                if(group_registration(username)){
                    printf("Group created successfully.\n");
                }
                break;
            case 2:
                join_group(username);
                break;
            case 4:
                printf("Enter group name to chat in: ");
                char g_name[100];
                fgets(g_name, sizeof(g_name), stdin);
                g_name[strcspn(g_name, "\n")] = '\0';
                enter_chat_room(username, g_name);
                break;
            case 5:
                return; // Logout
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

int main() {
    int choice;
    char current_user[100];

    while (1) {
        printf("\n--- WELCOME TO GROUP CHAT ---\n");
        printf("1. Login\n2. Register\n3. Exit\nSelection: ");
        scanf("%d", &choice);
        getchar(); 

        if (choice == 1) {
            if (authenticate_user(current_user, sizeof(current_user))) {
                printf("\nWelcome, %s!", current_user);
                group_dashboard(current_user);
            } else {
                printf("\nAuthentication failed.");
            }
        } else if (choice == 2) {
            user_registration();
        } else if (choice == 3) {
            break;
        }
    }
    return 0;
}