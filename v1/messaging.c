#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "messaging.h"
#include "../file_io.h"

#define MESSAGES_FILE "messages.txt"

// Function to send a message or a reply
void display_messages(const char* group_name) {
    FILE* file = fopen(MESSAGES_FILE, "r");
    if (!file) return;

    char line[512];
    printf("\n--- Message History for %s ---\n", group_name);
    
    while (fgets(line, sizeof(line), file)) {
        // Schema: MessageID|GroupID|Sender|Content|Timestamp|ParentID
        char *m_id = strtok(line, "|");
        char *g_id = strtok(NULL, "|");
        char *sender = strtok(NULL, "|");
        char *content = strtok(NULL, "|");
        char *time_str = strtok(NULL, "|");
        char *p_id = strtok(NULL, "|");

        // Only show messages belonging to the current group
        if (g_id && strcmp(g_id, group_name) == 0) {
            if (p_id && atoi(p_id) != 0) {
                printf("[ID: %s] (Reply to %s) %s: %s\n", m_id, p_id, sender, content);
            } else {
                printf("[ID: %s] %s: %s\n", m_id, sender, content);
            }
        }
    }
    fclose(file);
    printf("-------------------------------\n");
}

// Function to send a message
void send_message(const char* username, const char* group_id, const char* content, int parent_id){
    char message_record[512];
    long timestamp = (long)time(NULL);
    int message_id = rand() % 1000000; // Generate a random message ID

    // Schema: MessageID|GRoupID|Content|Timestamp|ParentMsgID
    snprintf(message_record, sizeof(message_record), "/%d|%s|%s|%s|%ld|%d",message_id, group_id, username, content, timestamp, parent_id);
    write_to_file(MESSAGES_FILE, message_record);
    printf("Message sent successfully.\n");
}

// Function to display chat room
void enter_chat_room(const char* username, const char* group_name){
    while(1){
        system("clear");  // Clear screen
        printf("--- CHAT ROOM: %s ---\n", group_name);
        printf("Type 'EXIT' to leave, 'REPLY [ID] [MSG]' to reply, or just type your message to send.\n\n");

        display_messages(group_name);
        printf("\n%s: ", username);
        char input[256];
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        if(strcmp(input, "EXIT")==0) break;
        if (strncmp(input, "REPLY", 5) == 0){
            int p_id;
            char msg[200];
            sscanf(input, "REPLY %d %[^\n]", &p_id, msg);
            send_message(username, group_name, msg, p_id);

        }else {
            send_message(username, group_name, input, 0);
        }

    }

}


