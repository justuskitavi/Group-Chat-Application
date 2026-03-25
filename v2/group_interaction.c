#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_io.h"
#include "group_interaction.h"
#include "data_transfer.h"

#define GROUPS_FILE "groups.txt"
#define MEMBERSHIPS_FILE "memberships.txt"
#define INPUT_READY "<<INPUT_READY>>"

void show_all_groups(const int client_socket) {
    char *group_names = read_from_file(GROUPS_FILE);
    if (group_names == NULL) {
        const char *message = "No groups found.\n";
        transfer_message(client_socket, message, strlen(message));
        return;
    }

    char response[1024];
    snprintf(response, sizeof(response), "  GROUPS  \n%s", group_names);
    transfer_message(client_socket, response, strlen(response));
    free(group_names);
}

bool join_group(const int client_socket, const char *username) {
    const char *prompt = "Enter name of group to join: ";
    transfer_message(client_socket, prompt, strlen(prompt));
    transfer_message(client_socket, INPUT_READY, strlen(INPUT_READY));

    char *group_name = receive_message(client_socket);
    if (group_name == NULL) return false;

    bool result = false;

    if (search_in_file(GROUPS_FILE, group_name)) {
        char member_record[150];
        snprintf(member_record, sizeof(member_record), "%s %s", username, group_name);

        if (search_in_file(MEMBERSHIPS_FILE, member_record)) {
            const char *message = "You are already a member of this group.\n";
            transfer_message(client_socket, message, strlen(message));
        } else {
            write_to_file(MEMBERSHIPS_FILE, member_record);
            const char *message = "Successfully joined group.\n";
            transfer_message(client_socket, message, strlen(message));
            result = true;
        }
    } else {
        const char *message = "Group does not exist.\n";
        transfer_message(client_socket, message, strlen(message));
    }

    free(group_name);
    return result;
}

void show_user_groups(const int client_socket, const char *target_user) {
    FILE *file = fopen(MEMBERSHIPS_FILE, "r");
    if (file == NULL) {
        const char *message = "No group memberships found.\n";
        transfer_message(client_socket, message, strlen(message));
        return;
    }

    char line[256];
    char output[4096] = {0};
    int found = 0;

    snprintf(output, sizeof(output), "\n--- Groups for %s ---\n", target_user);

    while (fgets(line, sizeof(line), file) != NULL) {
        // Each line is "username groupname", check if it starts with target_user
        if (strncmp(line, target_user, strlen(target_user)) == 0
            && line[strlen(target_user)] == ' ') {
            // Append just the group name portion
            strncat(output, line + strlen(target_user) + 1,
                    sizeof(output) - strlen(output) - 1);
            found++;
        }
    }
    fclose(file);

    if (!found) {
        strncat(output, "You haven't joined any groups yet.\n",
                sizeof(output) - strlen(output) - 1);
    }
    strncat(output, "---------------------------\n",
            sizeof(output) - strlen(output) - 1);

    transfer_message(client_socket, output, strlen(output));
}

void show_group_members(const int client_socket, const char *username) {
    const char *prompt = "Enter name of group: ";
    transfer_message(client_socket, prompt, strlen(prompt));
    transfer_message(client_socket, INPUT_READY, strlen(INPUT_READY));

    char *group_name = receive_message(client_socket);
    if (group_name == NULL) return;

    if (!is_group_admin(username, group_name)) {
        const char *message = "Only admins can view group members.\n";
        transfer_message(client_socket, message, strlen(message));
        free(group_name);
        return;
    }

    FILE *file = fopen(MEMBERSHIPS_FILE, "r");
    if (file == NULL) {
        const char *message = "No group memberships found.\n";
        transfer_message(client_socket, message, strlen(message));
        free(group_name);
        return;
    }

    char line[256];
    char output[4096] = {0};
    int found = 0;

    snprintf(output, sizeof(output), "\n--- Members of %s ---\n", group_name);

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, group_name) != NULL) {
            strncat(output, line, sizeof(output) - strlen(output) - 1);
            found++;
        }
    }
    fclose(file);

    if (!found) {
        strncat(output, "No members found.\n", sizeof(output) - strlen(output) - 1);
    }
    strncat(output, "---------------------------\n", sizeof(output) - strlen(output) - 1);

    transfer_message(client_socket, output, strlen(output));
    free(group_name);
}

bool is_group_admin(const char *username, const char *group_name) {
    char admin_record[200];
    snprintf(admin_record, sizeof(admin_record), "%s %s", username, group_name);
    return search_in_file(GROUPS_FILE, admin_record);
}

bool leave_group(const int client_socket, const char *username) {
    const char *prompt = "Enter name of group to leave: ";
    transfer_message(client_socket, prompt, strlen(prompt));
    transfer_message(client_socket, INPUT_READY, strlen(INPUT_READY));

    char *group_name = receive_message(client_socket);
    if (group_name == NULL) return false;

    bool result = false;

    if (search_in_file(GROUPS_FILE, group_name)) {
        char member_record[150];
        snprintf(member_record, sizeof(member_record), "%s %s", username, group_name);
        delete_all_lines_containing(MEMBERSHIPS_FILE, member_record);

        const char *message = "Successfully left group.\n";
        transfer_message(client_socket, message, strlen(message));
        result = true;
    } else {
        const char *message = "Group does not exist.\n";
        transfer_message(client_socket, message, strlen(message));
    }

    free(group_name);
    return result;
}