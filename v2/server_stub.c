#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

#include "server_stub.h"

volatile sig_atomic_t server_running = 1;
int s_socket;

void handle_shutdown(int signal) {
    server_running = 0;
    close(s_socket);
}

int main(){
    signal(SIGINT, handle_shutdown);

    const ServerConfig server = server_init(9000);
    s_socket = server.server_socket;
    printf("Server running... Press Ctrl+C to stop.\n");

    while(server_running) {
        const int client_socket = get_client(&server);

        if (client_socket >= 0) {
            pthread_t thread_id;

            int* client_ptr = malloc(sizeof(int));
            *client_ptr = client_socket;

            if (pthread_create(&thread_id, NULL, client_handler, client_ptr) != 0) {
                perror("Thread creation failed");
                close(client_socket);
                free(client_ptr);
                continue;
            }

            pthread_detach(thread_id);
        }
    }
    printf("Closing server socket...\n");

    printf("Server shutdown complete.\n");
    return 0;
}

