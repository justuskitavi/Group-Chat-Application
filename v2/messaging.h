#ifndef MESSAGING_H
#define MESSAGING_H

#define MESSAGES_FILE "messages.txt"
#define INPUT_READY "<<INPUT_READY>>"

/**
 * Records a new message to the persistent storage and notifies the client.
 * * - Client_socket - Socket to send the confirmation message to.
 * - Username - The sender's name.
 * - Group_id - The ID or name of the group the message belongs to.
 * - Content - The actual text of the message.
 * - Parent_id - The ID of the message being replied to (0 if a new thread).
 */
void send_message(int client_socket, const char *username,
                  const char *group_id, const char *content, int parent_id);

/**
 * Reads the message file, filters out messages for a specific group,
 * and sends the formatted history to the client.
 * * Client_socket - Socket to send the chat history to.
 * Group_name - The group for which to retrieve messages.
 */
void display_messages(int client_socket, const char *group_name);

/**
 * Enters a blocking loop that acts as the chat interface for a client.
 * * This function repeatedly sends chat history, prompts for input, and
 * processes "leave", "reply", or standard messages until the user exits.
 * * Client_socket - The active connection.
 * Username - The authenticated user.
 * Group_name - The group context for the chat.
 */
void start_chat_session(int client_socket, const char *username, const char *group_name);

#endif // MESSAGING_H
