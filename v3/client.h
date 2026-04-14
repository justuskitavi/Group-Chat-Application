#ifndef CLIENT_H
#define CLIENT_H

/**
 * Initializes the client socket and connects to the server at 127.0.0.1:9000.
 * Returns the socket file descriptor on success.
 * Exits the program on failure.
 */
int client_init(void);

/**
 * The main entry point for the authenticated user session.
 * Handles the menu navigation for creating/joining groups and chatting.
 * * @param sock The connected client socket.
 * @param username The username of the currently logged-in user.
 */
void run_dashboard(int sock, const char *username);

/**
 * Handles the user registration process.
 * Prompts for credentials and communicates with the server.
 * * @param sock The connected client socket.
 */
void handle_register(int sock);

/**
 * Handles the user login process.
 * If successful, automatically calls run_dashboard().
 * * @param sock The connected client socket.
 */
void handle_login(int sock);

#endif /* CLIENT_H */