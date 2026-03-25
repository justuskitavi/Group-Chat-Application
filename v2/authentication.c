#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_io.h"
#include "authentication.h"
#include "data_transfer.h"
#include "registration.h"

#define USERS_FILE "users.txt"
#define INPUT_READY "<<INPUT_READY>>"

bool authenticate_user(const int client_socket, char *username, const size_t username_capacity) {
    if (username == NULL || username_capacity == 0) return false;

    // Get username
    const char *username_prompt = "Enter username: ";
    transfer_message(client_socket, username_prompt, strlen(username_prompt));
    transfer_message(client_socket, INPUT_READY, strlen(INPUT_READY));

    char *username_response = receive_message(client_socket);
    if (username_response == NULL) return false;

    // Get password
    const char *password_prompt = "Enter password: ";
    transfer_message(client_socket, password_prompt, strlen(password_prompt));
    transfer_message(client_socket, INPUT_READY, strlen(INPUT_READY));
    char *password_response = receive_message(client_socket);
    if (password_response == NULL) { free(username_response); return false; }

    // Look up stored record for this username
    char *user_record = search_string(USERS_FILE, username_response);
    if (user_record == NULL) {
        const char *message = "No account found with that username.\n";
        transfer_message(client_socket, message, strlen(message));
        free(username_response);
        free(password_response);
        return false;
    }

    // Extract stored hash (last space-delimited token)
    char stored_hash[65] = {0};
    char *last_space = strrchr(user_record, ' ');
    if (last_space) {
        snprintf(stored_hash, sizeof(stored_hash), "%s", last_space + 1);
        stored_hash[strcspn(stored_hash, "\n")] = '\0';
    }
    free(user_record);

    // Hash the submitted password and compare
    const unsigned long input_hash = hash_password(password_response);
    free(password_response);

    char input_hash_str[65] = {0};
    snprintf(input_hash_str, sizeof(input_hash_str), "%lx", input_hash);

    const bool is_authenticated = (strcmp(stored_hash, input_hash_str) == 0);

    if (is_authenticated) {
        // Copy the verified username into the username pointer passed by the calling function
        snprintf(username, username_capacity, "%s", username_response);
        free(username_response);

        const char *message = "Authentication successful.\n";
        transfer_message(client_socket, message, strlen(message));
        return true;
    }

    free(username_response);
    const char *message = "Incorrect password.\n";
    transfer_message(client_socket, message, strlen(message));
    return false;
}