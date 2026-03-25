#ifndef MESSAGING_H
#define MESSAGING_H

void send_message(const char* username, const char* group_id, const char* content, int parent_id);
void start_chat_session(const char* username, const char* group_name);

#endif // MESSAGING_H
