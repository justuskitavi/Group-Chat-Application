#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_io.h"
#include "authentication.h"
#include "data_transfer.h"
#include "registration.h"

bool authenticate_user(int server_socket, struct sockaddr_in *client_addr, char *user_record, char* user_ref) {
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

    char *user_exists = search_string(USERS_FILE, username);
    if (user_exists == NULL) {
        const char *message = "ERR: No account found with that username.\n";
        send_udp_message(server_socket, client_addr, message);
        return false;
    }

    char stored_hash[65] = {0};
    char *last_pipe = strrchr(user_exists, '|');
    if (last_pipe) {
        char *trimmed_hash = last_pipe + 1;
        while (*trimmed_hash == ' ') trimmed_hash++;

        strncpy(stored_hash, trimmed_hash, sizeof(stored_hash) - 1);
        stored_hash[strcspn(stored_hash, "\n")] = '\0';
    }
    free(user_exists);

    const unsigned long input_hash = hash_password(password);
    char input_hash_str[65] = {0};
    snprintf(input_hash_str, sizeof(input_hash_str), "%lu", input_hash);

    if (strcmp(stored_hash, input_hash_str) == 0) {
        strcpy(user_ref, username);
        return true;
    } else {
        const char *message = "ERR: Incorrect password.\n";
        send_udp_message(server_socket, client_addr, message);
        return false;
    }
}
