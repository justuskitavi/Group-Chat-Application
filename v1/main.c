#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Application modules
#include "messaging.h"
#include "authentication.h"
#include "registration.h"
#include "group_interaction.h"

// User interaction menu for user to use group functions
void group_dashboard(char* username){
    int choice;
    while(1){
        printf("\n--- GROUP DASHBOARD ---\n");
        printf("1. Create Group\n");
        printf("2. Join Group\n");
        printf("3. Show Groups Joined\n");
        printf("4. Show Group Members\n");
        printf("5. Delete Group\n");
        printf("6. Leave Group\n");
        printf("7. Enter Chat\n");
        printf("8. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch(choice){
            case 1:
                if(group_registration(username)){
                    printf("Group created successfully.\n");
                }
                break;
            case 2:
                join_group(username);
                break;
            case 3:
                show_user_groups(username);
                break;
            case 4:
                show_group_members(username);
                break;
            case 5:
                if(delete_group(username)){
                    printf("Group deleted successfully.");
                }
                break;
            case 6:
                if(leave_group(username)){
                    printf("Left group Successfully");
                }
                break;
            case 7:
                printf("Enter group name to chat in: ");
                char g_name[100];
                fgets(g_name, sizeof(g_name), stdin);
                g_name[strcspn(g_name, "\n")] = '\0';
                start_chat_session(username, g_name);
                break;
            case 8:
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