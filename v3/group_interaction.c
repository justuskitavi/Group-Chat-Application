#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_io.h"
#include "group_interaction.h"
#include "data_transfer.h"
#include "messaging.h"

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

bool join_group(const int client_socket, const char *username, const char *group_name) {
    if (group_name == NULL) return false;

    FILE *file = fopen(MEMBERSHIPS_FILE, "r");
    char line[256];

    if (file != NULL) {
        while (fgets(line, sizeof(line), file)) {
            line[strcspn(line, "\n")] = '\0';

            char *id = strtok(line, "|");
            char *name = strtok(NULL, "|");
            char *group = strtok(NULL, "|");

            if (id && name && group) {
                if (strcmp(name, username) == 0 &&
                    strcmp(group, group_name) == 0) {
                    
                    fclose(file);
                    const char *message = "You are already a member of this group.\n";
                    transfer_message(client_socket, message, strlen(message));
                    return false;
                }
            }
        }
        fclose(file);
    }

    if (!search_in_file(GROUPS_FILE, group_name)) {
        const char *message = "Group does not exist.\n";
        transfer_message(client_socket, message, strlen(message));
        return false;
    }

    char record[150];
    snprintf(record, sizeof(record), "%s | %s", username, group_name);

    write_to_file(MEMBERSHIPS_FILE, record);

    return true;
}

void show_user_groups(const int client_socket, const char *target_user) {
    FILE *file = fopen(MEMBERSHIPS_FILE, "r");

    if (!file) {
        const char *message = "No group memberships found.\n";
        transfer_message(client_socket, message, strlen(message));
        return;
    }

    char line[256];
    char output[4096] = {0};
    int found = 0;

    snprintf(output, sizeof(output),
             "\n--- Groups for %s ---\n", target_user);

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';

        char *id = strtok(line, "|");
        char *name = strtok(NULL, "|");
        char *group = strtok(NULL, "|");

        if (id && name && group) {
            name = trim(name);
            group = trim(group);

            if (strcmp(name, target_user) == 0) {
                strncat(output, group,
                        sizeof(output) - strlen(output) - 1);
                strncat(output, "\n",
                        sizeof(output) - strlen(output) - 1);
                found++;
            }
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

void show_group_members(const int client_socket, const char *username, const char *group_name) {
    if (group_name == NULL) return;

    if (!is_group_admin(username, group_name)) {
        const char *message = "Only admins can view group members.\n";
        transfer_message(client_socket, message, strlen(message));
        return;
    }

    FILE *file = fopen(MEMBERSHIPS_FILE, "r");
    if (file == NULL) {
        const char *message = "No group memberships found.\n";
        transfer_message(client_socket, message, strlen(message));
        return;
    }

    char line[256];
    char output[4096] = {0};
    int found = 0;

    snprintf(output, sizeof(output), "\n--- Members of %s ---\n", group_name);

    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0';

        char *id = strtok(line, "|");
        char *name = strtok(NULL, "|");
        char *group = strtok(NULL, "|");

        if (id && name && group) {
            while (*name == ' ') name++;
            while (*group == ' ') group++;

            char *end;

            end = name + strlen(name) - 1;
            while (end > name && *end == ' ') *end-- = '\0';

            end = group + strlen(group) - 1;
            while (end > group && *end == ' ') *end-- = '\0';

            if (strcmp(group, group_name) == 0) {
                strncat(output, name, sizeof(output) - strlen(output) - 1);
                strncat(output, "\n", sizeof(output) - strlen(output) - 1);
                found++;
            }
        }
    }

    fclose(file);

    if (!found) {
        strncat(output, "No members found.\n",
                sizeof(output) - strlen(output) - 1);
    }

    strncat(output, "---------------------------\n",
            sizeof(output) - strlen(output) - 1);

    transfer_message(client_socket, output, strlen(output));
}

bool is_group_admin(const char *username, const char *group_name) {
    char admin_record[200];
    snprintf(admin_record, sizeof(admin_record), "%s | %s", username, group_name);
    return search_in_file(GROUPS_FILE, admin_record);
}

bool leave_group(const int client_socket, const char *username, const char *group_name) {
    if (group_name == NULL) return false;

    FILE *file = fopen(MEMBERSHIPS_FILE, "r");
    if (!file) return false;

    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        fclose(file);
        return false;
    }

    char line[256];
    bool removed = false;

    while (fgets(line, sizeof(line), file)) {
        char copy[256];
        strcpy(copy, line);

        char *id = strtok(copy, "|");
        char *name = strtok(NULL, "|");
        char *group = strtok(NULL, "|");

        if (id && name && group) {
            name = trim(name);
            group = trim(group);

            if (strcmp(name, username) == 0 &&
                strcmp(group, group_name) == 0) {
                removed = true;
                continue;
            }
        }

        fputs(line, temp);
    }

    fclose(file);
    fclose(temp);

    remove(MEMBERSHIPS_FILE);
    rename("temp.txt", MEMBERSHIPS_FILE);

    if (removed) {
        return true;
    } else {
        return false;
    }
}