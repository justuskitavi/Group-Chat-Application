#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "messaging.h"
#include "file_io.h"

#define MESSAGES_FILE "messages.txt"

// 1. Function to format and save a message/reply to the txt file
void send_message(const char* username, const char* group_id, const char* content, int parent_id){
    char message_record[512];
    long timestamp = (long)time(NULL);
    int message_id = rand() % 10000; // Generate a random message ID

    // Schema: MessageID|GroupID|Sender|Content|Timestamp|ParentMsgID
    snprintf(message_record, sizeof(message_record), "%d|%s|%s|%s|%ld|%d", 
             message_id, group_id, username, content, timestamp, parent_id);

    write_to_file(MESSAGES_FILE, message_record);
    printf("Message sent successfully.\n");
}

// 2. Read from file and display the chat history for a specific group
void display_messages(const char* group_name) {
    FILE* file = fopen(MESSAGES_FILE, "r");
    if (!file) {
        printf("\nNo messages yet. Be the first to start the conversation!\n");
        return;
    }

    char line[512];
    printf("\n========================================\n");
    printf("    CHATROOM: %s\n", group_name);
    printf("==========================================\n");
    
    while (fgets(line, sizeof(line), file)) {
        // Schema: MessageID|GroupID|Sender|Content|Timestamp|ParentID
        char *m_id = strtok(line, "|");
        char *g_id = strtok(NULL, "|");
        char *sender = strtok(NULL, "|");
        char *content = strtok(NULL, "|");
        strtok(NULL, "|"); // skip timestamp
        char *p_id = strtok(NULL, "|");

        // Only show messages belonging to the current group
        if (g_id && strcmp(g_id, group_name) == 0) {
            if (p_id && atoi(p_id) != 0) {
                printf("    |--[ID: %s] (Reply to %s) %s: %s\n", m_id, p_id, sender, content);
            } else {
                printf("[ID: %s] %s: %s\n", m_id, sender, content);
            }
        }
    }
    fclose(file);
    printf("-------------------------------\n");
}

// 3. Loop that refreshes the terminal screen
void start_chat_session(const char* username, const char* group_name){
    char input[256];
    while(1){
        system("clear");
        display_messages(group_name);

        printf("\nCommands: 'exit' to leave, 'reply <id> <msg>' to reply, or just type your message to send.\nPress 'ENTER' to refresh chat.\n");
        printf("%s : ", username);

        if (fgets(input, sizeof(input), stdin) == NULL) break;
        input[strcspn(input, "\n")] = '\0';

        if(strcmp(input, "exit") == 0){
            break;
        } 

        if (strncmp(input, "reply", 6) == 0){
            int parent_id;
            char msg[200];

            if(sscanf(input, "reply %d %[^\n]", &parent_id, msg) == 2){
                send_message(username, group_name, msg, parent_id);
            } else { 
                printf("Invalid reply format. Use: reply <message_id> <your_message>\n");
                printf("Press Enter to try again.");
                getchar();
            }
        }else if (strlen(input) > 0){
            //Standard message
            send_message(username, group_name, input, 0);
        }
    }

}







