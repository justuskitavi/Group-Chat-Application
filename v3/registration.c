#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "data_transfer.h"
#include "file_io.h"
#include "group_interaction.h"
#include "registration.h"

#define GROUPS_FILE "groups.txt"
#define USERS_FILE "users.txt"
#define MEMBERSHIPS_FILE "memberships.txt"
#define INPUT_READY "<<INPUT_READY>>"

unsigned long hash_password(const char *password) {
    unsigned long hash = 5381;
    int c;
    while ((c = *password++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

bool group_registration(const int client_socket, const char *username, const char *group_name) {
    if (group_name == NULL) return false;

    char group_record[256];
    snprintf(group_record, sizeof(group_record), "%s | %s", username, group_name);

    if (write_to_file(GROUPS_FILE, group_record)) {
        write_to_file(MEMBERSHIPS_FILE, group_record);
        return true;
    }

    return false;
}

bool delete_group(const int client_socket, const char *username, const char *group_name) {
    if (group_name == NULL) return false;

    bool result = false;

    if (search_in_file(GROUPS_FILE, group_name) && is_group_admin(username, group_name)) {
        char group_record[150];
        snprintf(group_record, sizeof(group_record), "%s | %s", username, group_name);

        delete_all_lines_containing(GROUPS_FILE, group_record);
        delete_all_lines_containing(MEMBERSHIPS_FILE, group_name);

        result = true;
    }

    return result;
}

bool user_registration(const int client_socket, char *user_record) {
    if (user_record == NULL) return false;

    char username[100];
    char password[20];

    char *split = strrchr(user_record, '|');
    if (split == NULL) return false;

    strncpy(password, split + 1, sizeof(password) - 1);
    password[sizeof(password) - 1] = '\0';
    password[strcspn(password, "\n")] = '\0';

    *split = '\0';  
    strncpy(username, user_record, sizeof(username) - 1);
    username[sizeof(username) - 1] = '\0';

    size_t username_length = strlen(username);
    while (username_length > 0 && username[username_length - 1] == ' ') username[--username_length] = '\0';

    const unsigned long hash = hash_password(password);

    char user_data[256];
    snprintf(user_data, sizeof(user_data), "%s |%lx", username, hash);

    if (write_to_file(USERS_FILE, user_data)) return true;

    return false;
}