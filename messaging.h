#ifndef MESSAGING_H
#define MESSAGING_H

#include <netinet/in.h>

#define MESSAGES_FILE "messages.txt"

char *trim(char *s);
void save_message(const char *username, const char *group_id, const char *content, int parent_id);
void display_messages(int sock, struct sockaddr_in *client_addr, const char *group_name);

#endif // MESSAGING_H