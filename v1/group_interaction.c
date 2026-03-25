#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_io.h"
#include "group_interaction.h"

#define GROUPS_FILE "groups.txt"
#define MEMBERSHIPS_FILE "memberships.txt"

void show_all_groups(){
    char* group_names = read_from_file(GROUPS_FILE);
    printf("  GROUPS  \n%s", group_names);
    free(group_names);
}

bool join_group(const char* username){
    char group_name[100];

    printf("Enter name of group to join: ");
    fgets(group_name, sizeof(group_name), stdin);
    group_name[strcspn(group_name, "\n")] = '\0';

    if(search_in_file(GROUPS_FILE,group_name)){
        char member_record[150];
        snprintf(member_record, sizeof(member_record), "%s %s", username, group_name);

        if(search_in_file(MEMBERSHIPS_FILE, member_record)){
            perror("Already a member");
            return false;
        }

        write_to_file(MEMBERSHIPS_FILE, member_record);
        return true;
    }
    perror("Group does not exist");
    return false;
}

void show_user_groups(const char* target_user) {
    FILE* file = fopen(MEMBERSHIPS_FILE, "r");
    if (file == NULL) {
        printf("No group memberships found (file missing).\n");
        return;
    }

    char line[256];
    int count = 0;

    printf("\n--- Groups for %s ---\n", target_user);

    while (fgets(line, sizeof(line), file) != NULL) {
        count++;

        if (strstr(line, target_user) != NULL) {
            printf("%s", line + strlen(target_user) + 1);
        }
    }

    if (count == 0) {
        printf("You haven't joined any groups yet.\n");
    }
    printf("\n---------------------------\n");

    fclose(file);
}

void show_group_members(const char* username) {
    FILE* file = fopen(MEMBERSHIPS_FILE, "r");
    if (file == NULL) {
        printf("No group memberships found (file missing).\n");
        return;
    }

    char line[256];
    int count = 0;
    char group_name[100];

    printf("Enter name of group: ");
    fgets(group_name, sizeof(group_name), stdin);
    group_name[strcspn(group_name, "\n")] = '\0';

    if(is_group_admin(username, group_name)){
        printf("\n--- Members for %s ---\n", group_name);

        while (fgets(line, sizeof(line), file) != NULL) {
            count++;

            if (strstr(line, group_name) != NULL) {
                printf("%s", line);
            }
        }

        if (count == 0) {
            printf("No members joined.\n");
        }
        printf("\n---------------------------\n");
    }else{
        printf("Only admins can view group members.");
    }

    fclose(file);
}

bool is_group_admin(const char* username, char* group_name){
    char admin_record[200];
    snprintf(admin_record, sizeof(admin_record), "%s %s", username, group_name);

    if(search_in_file(GROUPS_FILE,admin_record)){
        return true;
    }

    return false;
}

bool leave_group(const char* username){
    char group_name[100];

    printf("Enter name of group to leave: ");
    fgets(group_name, sizeof(group_name), stdin);
    group_name[strcspn(group_name, "\n")] = '\0';

    if(search_in_file(GROUPS_FILE,group_name)){
        char member_record[150];
        snprintf(member_record, sizeof(member_record), "%s %s", username, group_name);

        delete_all_lines_containing(MEMBERSHIPS_FILE, member_record);
        return true;
    }

    perror("Group does not exist");
    return false;
}

// int main(){
//     leave_group(username, group_name);
//     join_group();
// }