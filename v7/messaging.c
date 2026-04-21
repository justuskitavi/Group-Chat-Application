#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "data_transfer.h"
#include "file_io.h"
#include "messaging.h"

char* trim(char *str) {
    while (*str == ' ') str++;
    if (*str == 0) return str;

    char *end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\n' || *end == '\r')) {
        *end = '\0';
        end--;
    }

    return str;
}

void save_message(const char *username, const char *group_id, const char *content, const int parent_id) {
    char message_record[512];
    const long timestamp = (long)time(NULL);
    const int message_id = rand() % 10000;

    snprintf(message_record, sizeof(message_record), "%d|%s|%s|%s|%ld|%d",
             message_id, group_id, username, content, timestamp, parent_id);

    FILE* file = fopen(MESSAGES_FILE, "a+");
    if (file == NULL) {
        perror("Error opening file for writing");
        return;
    }

    fputs(message_record, file);
    fprintf(file, "\n");
    fclose(file);
}

void display_messages(int sock, struct sockaddr_in *client_addr, const char *group_name) {
    FILE *file = fopen(MESSAGES_FILE, "r");

    char buffer[8192] = {0};
    char header[256];
    snprintf(header, sizeof(header),
             "1|\n========================================\n"
             "    CHATROOM: %s\n"
             "==========================================\n", group_name);
    strncat(buffer, header, sizeof(buffer) - strlen(buffer) - 1);

    if (!file) {
        strncat(buffer, "No messages yet. Be the first to start the conversation!\n",
                sizeof(buffer) - strlen(buffer) - 1);
    } else {
        char line[512];
        while (fgets(line, sizeof(line), file)) {
            char line_copy[512];
            strncpy(line_copy, line, sizeof(line_copy) - 1);
            line_copy[sizeof(line_copy) - 1] = '\0';

            char *message_id = strtok(line_copy, "|");
            char *group_id = strtok(NULL, "|");
            char *sender = strtok(NULL, "|");
            char *content = strtok(NULL, "|");
            strtok(NULL, "|");
            char *parent_id = strtok(NULL, "|");

            if (group_id && strcmp(group_id, group_name) == 0) {
                char entry[600];
                if (parent_id && atoi(parent_id) != 0) {
                    snprintf(entry, sizeof(entry),
                             "    |--[ID: %s] (Reply to %s) %s: %s\n",
                             message_id, trim(parent_id), sender, content);
                } else {
                    snprintf(entry, sizeof(entry),
                             "[ID: %s] %s: %s\n", message_id, sender, content);
                }
                strncat(buffer, entry, sizeof(buffer) - strlen(buffer) - 1);
            }
        }
        fclose(file);
    }

    strncat(buffer, "-------------------------------\n",
            sizeof(buffer) - strlen(buffer) - 1);
    send_udp_message(sock, client_addr, buffer);
}
