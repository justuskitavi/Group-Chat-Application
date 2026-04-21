#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_io.h"
#include "group_interaction.h"
#include "data_transfer.h"
#include "messaging.h"

void show_all_groups(int sock, struct sockaddr_in *client_addr) {
    char *group_names = read_from_file(GROUPS_FILE);
    if (group_names == NULL) {
        send_udp_message(sock, client_addr, "0|No groups found.\n");
        return;
    }

    char response[2048];
    snprintf(response, sizeof(response), "1|\n--- ALL GROUPS ---\n%s", group_names);
    send_udp_message(sock, client_addr, response);
    free(group_names);
}

bool join_group(const char *username, const char *group_name) {
    if (group_name == NULL) return false;

    FILE *file = fopen(MEMBERSHIPS_FILE, "r");
    char line[256];
    bool already_member = false;

    if (file != NULL) {
        while (fgets(line, sizeof(line), file)) {
            line[strcspn(line, "\n")] = '\0';

            char *id = strtok(line, "|");
            char *name = strtok(NULL, "|");
            char *group = strtok(NULL, "|");

            if (id && name && group) {
                if (strcmp(trim(name), username) == 0 &&
                    strcmp(trim(group), group_name) == 0) {
                    already_member = true;
                    break;
                }
            }
        }
        fclose(file);
    }

    if (already_member) return false;

    if (search_in_file(GROUPS_FILE, group_name)) {
        char member_record[200];
        snprintf(member_record, sizeof(member_record), "%s | %s", username, group_name);
        write_to_file(MEMBERSHIPS_FILE, member_record);
        return true;
    }

    return false;
}

void show_user_groups(int sock, struct sockaddr_in *client_addr, const char *target_user) {
    FILE *file = fopen(MEMBERSHIPS_FILE, "r");

    if (!file) {
        send_udp_message(sock, client_addr, "0|No group memberships found.\n");
        return;
    }

    char line[256];
    char output[4096] = {0};
    int found = 0;

    snprintf(output, sizeof(output), "1|\n--- Groups for %s ---\n", target_user);

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';

        char *id = strtok(line, "|");
        char *name = strtok(NULL, "|");
        char *group = strtok(NULL, "|");

        if (id && name && group) {
            name = trim(name);
            group = trim(group);

            if (strcmp(name, target_user) == 0) {
                strncat(output, group, sizeof(output) - strlen(output) - 1);
                strncat(output, "\n", sizeof(output) - strlen(output) - 1);
                found++;
            }
        }
    }

    fclose(file);

    if (!found) {
        strncat(output, "You haven't joined any groups yet.\n", sizeof(output) - strlen(output) - 1);
    }

    strncat(output, "---------------------------\n", sizeof(output) - strlen(output) - 1);
    send_udp_message(sock, client_addr, output);
}

void show_group_members(int sock, struct sockaddr_in *client_addr, const char *username, const char *group_name) {
    if (group_name == NULL) {
        send_udp_message(sock, client_addr, "0|Group name is required.\n");
        return;
    }

    if (!is_group_admin(username, group_name)) {
        send_udp_message(sock, client_addr, "0|Only admins can view group members.\n");
        return;
    }

    FILE *file = fopen(MEMBERSHIPS_FILE, "r");
    if (file == NULL) {
        send_udp_message(sock, client_addr, "0|No group memberships found.\n");
        return;
    }

    char line[256];
    char output[4096] = {0};
    int found = 0;

    snprintf(output, sizeof(output), "1|\n--- Members of %s ---\n", group_name);

    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0';

        char *id = strtok(line, "|");
        char *name = strtok(NULL, "|");
        char *group = strtok(NULL, "|");

        if (id && name && group) {
            name = trim(name);
            group = trim(group);

            if (strcmp(group, group_name) == 0) {
                strncat(output, name, sizeof(output) - strlen(output) - 1);
                strncat(output, "\n", sizeof(output) - strlen(output) - 1);
                found++;
            }
        }
    }

    fclose(file);

    if (!found) {
        strncat(output, "No members found.\n", sizeof(output) - strlen(output) - 1);
    }

    strncat(output, "---------------------------\n", sizeof(output) - strlen(output) - 1);
    send_udp_message(sock, client_addr, output);
}

bool is_group_admin(const char *username, const char *group_name) {
    char admin_record[200];
    snprintf(admin_record, sizeof(admin_record), "%s | %s", username, group_name);
    return search_in_file(GROUPS_FILE, admin_record);
}

bool leave_group(const char *username, const char *group_name) {
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

    return removed;
}
