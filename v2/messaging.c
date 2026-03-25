#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "data_transfer.h"
#include "file_io.h"
#include "messaging.h"

#define MESSAGES_FILE "messages.txt"
#define INPUT_READY "<<INPUT_READY>>"

void send_message(const int client_socket, const char *username,
                  const char *group_id, const char *content, const int parent_id) {
    char message_record[512];
    const long timestamp = (long)time(NULL);
    const int message_id = rand() % 10000;

    // Schema: MessageID|GroupID|Sender|Content|Timestamp|ParentMsgID
    snprintf(message_record, sizeof(message_record), "%d|%s|%s|%s|%ld|%d",
             message_id, group_id, username, content, timestamp, parent_id);

    write_to_file(MESSAGES_FILE, message_record);

    const char *message = "Message sent.\n";
    transfer_message(client_socket, message, strlen(message));
}

// Builds the full chat history of a given group as a string and sends it in one transfer
void display_messages(const int client_socket, const char *group_name) {
    FILE *file = fopen(MESSAGES_FILE, "r");

    char output[8192] = {0};
    char header[128];
    snprintf(header, sizeof(header),
             "\n========================================\n"
             "    CHATROOM: %s\n"
             "==========================================\n", group_name);
    strncat(output, header, sizeof(output) - strlen(output) - 1);

    if (!file) {
        strncat(output, "No messages yet. Be the first to start the conversation!\n",
                sizeof(output) - strlen(output) - 1);
    } else {
        char line[512];
        while (fgets(line, sizeof(line), file)) {
            char line_copy[512];
            strncpy(line_copy, line, sizeof(line_copy));

            char *message_id   = strtok(line_copy, "|");
            const char *group_id   = strtok(NULL, "|");
            char *sender = strtok(NULL, "|");
            char *content = strtok(NULL, "|");
            strtok(NULL, "|"); // skip timestamp
            char *parent_id   = strtok(NULL, "|");

            if (group_id && strcmp(group_id, group_name) == 0) {
                char entry[600];
                if (parent_id && atoi(parent_id) != 0) {
                    snprintf(entry, sizeof(entry),
                             "    |--[ID: %s] (Reply to %s) %s: %s\n",
                             message_id, parent_id, sender, content);
                } else {
                    snprintf(entry, sizeof(entry),
                             "[ID: %s] %s: %s\n", message_id, sender, content);
                }
                strncat(output, entry, sizeof(output) - strlen(output) - 1);
            }
        }
        fclose(file);
    }

    strncat(output, "-------------------------------\n",
            sizeof(output) - strlen(output) - 1);
    transfer_message(client_socket, output, strlen(output));
}

void start_chat_session(const int client_socket,
                        const char *username, const char *group_name) {
    while (1) {
        // Send current chat history
        display_messages(client_socket, group_name);

        char prompt[256];
        snprintf(prompt, sizeof(prompt),
                 "Commands: 'leave' | 'reply <id> <message>' | or type to send.\n%s: ",
                 username);
        transfer_message(client_socket, prompt, strlen(prompt));
        transfer_message(client_socket, INPUT_READY, strlen(INPUT_READY));

        char *input = receive_message(client_socket);
        if (input == NULL) break;

        if (strcmp(input, "leave") == 0) {
            free(input);
            break;
        }

        if (strncmp(input, "reply ", 6) == 0) {
            int parent_id;
            char message[200];
            if (sscanf(input, "reply %d %[^\n]", &parent_id, message) == 2) {
                send_message(client_socket, username, group_name, message, parent_id);
            } else {
                const char *error = "Invalid format. Use: reply <id> <message>\n";
                transfer_message(client_socket, error, strlen(error));
            }
        } else if (strlen(input) > 0) {
            send_message(client_socket, username, group_name, input, 0);
        }

        free(input);
    }
}