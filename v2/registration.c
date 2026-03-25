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

bool group_registration(const int client_socket, const char *username) {
    const char *prompt = "Enter group name: ";
    transfer_message(client_socket, prompt, strlen(prompt));
    transfer_message(client_socket, INPUT_READY, strlen(INPUT_READY));

    char *group_name = receive_message(client_socket);
    if (group_name == NULL) return false;

    char group_record[256];
    snprintf(group_record, sizeof(group_record), "%s %s", username, group_name);
    free(group_name);

    if (write_to_file(GROUPS_FILE, group_record)) {
        write_to_file(MEMBERSHIPS_FILE, group_record);
        return true;
    }

    const char *error = "Error registering group.\n";
    transfer_message(client_socket, error, strlen(error));
    return false;
}

bool delete_group(const int client_socket, const char *username) {
    const char *prompt = "Enter name of group to delete: ";
    transfer_message(client_socket, prompt, strlen(prompt));
    transfer_message(client_socket, INPUT_READY, strlen(INPUT_READY));

    char *group_name = receive_message(client_socket);
    if (group_name == NULL) return false;

    bool result = false;

    if (search_in_file(GROUPS_FILE, group_name) && is_group_admin(username, group_name)) {
        char group_record[150];
        snprintf(group_record, sizeof(group_record), "%s %s", username, group_name);

        delete_all_lines_containing(GROUPS_FILE, group_record);
        delete_all_lines_containing(MEMBERSHIPS_FILE, group_name);

        const char *message = "Group deleted successfully.\n";
        transfer_message(client_socket, message, strlen(message));
        result = true;
    } else {
        const char *message = "Could not delete group. Not found or you are not the admin.\n";
        transfer_message(client_socket, message, strlen(message));
    }

    free(group_name);
    return result;
}

bool user_registration(const int client_socket) {
    const char *username_prompt = "Enter username: ";
    transfer_message(client_socket, username_prompt, strlen(username_prompt));
    transfer_message(client_socket, INPUT_READY, strlen(INPUT_READY));

    char *username = receive_message(client_socket);
    if (username == NULL) return false;

    const char *password_prompt = "Enter password: ";
    transfer_message(client_socket, password_prompt, strlen(password_prompt));
    transfer_message(client_socket, INPUT_READY, strlen(INPUT_READY));

    char *password = receive_message(client_socket);
    if (password == NULL) { free(username); return false; }

    const unsigned long hash = hash_password(password);
    free(password);

    char user_data[256];
    snprintf(user_data, sizeof(user_data), "%s %lx", username, hash);
    free(username);

    if (write_to_file(USERS_FILE, user_data)) {
        const char *message = "Registered successfully.\n";
        transfer_message(client_socket, message, strlen(message));
        return true;
    }

    const char *error = "Error registering user.\n";
    transfer_message(client_socket, error, strlen(error));
    return false;
}