#include <stdio.h>
#include <string.h>
#include "../file_io.h"
#include "registration.h"

#define GROUPS_FILE "groups.txt"
#define USERS_FILE "users.txt"
#define MEMBERSHIPS_FILE "memberships.txt"

unsigned long hash_password(char *str){
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

bool group_registration(const char* username){
    char group_name[100];
    char group_record[256];

    printf("Enter group name: ");
    fgets(group_name, sizeof(group_name), stdin);
    group_name[strcspn(group_name, "\n")] = '\0';

    snprintf(group_record, sizeof(group_record), "%s %s", username, group_name);

    if(write_to_file(GROUPS_FILE, group_record)){
        return true;
    }

    perror("Error registering group");
    return false;
}

bool delete_group(const char* username, char* group_name){
    if(search_in_file(GROUPS_FILE,group_name)){
        char member_record[150];
        snprintf(member_record, sizeof(member_record), "%s %s", username, group_name);

        printf("%s", member_record);

        delete_all_lines_containing(GROUPS_FILE, member_record);
        delete_all_lines_containing(MEMBERSHIPS_FILE, group_name);
        return true;
    }

    perror("Group does not exist");
    return false;
}

bool user_registration(){
    char username[100];
    char password[20];
    char user_data[125];

    printf("Enter your name: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    printf("Enter your password: ");
    scanf("%19s", password);

    unsigned long hash = hash_password(password);

    sprintf(user_data, "%s %lx", username, hash);

    printf("%s", user_data);

    if(write_to_file(USERS_FILE, user_data)){
        return true;
    }

    perror("Error registering user");
    return false;
}

// int main(){
//     // if(group_registration(username)){
//     //     printf("\nGroup registred successfully.");
//     // }
//     // if(user_registration()){
//     //     printf("\nUser registred successfully.");
//     // }

//     if(delete_group(username, group_name)){
//         printf("Group deleted successfully");
//     }
//     return 0;
// }