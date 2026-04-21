#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

#include "server.h"

volatile sig_atomic_t server_running = 1;
int s_socket;

void handle_shutdown(int signal) {
    server_running = 0;
    close(s_socket);
}

int main(){
    setvbuf(stdout, NULL, _IONBF, 0);
    signal(SIGINT, handle_shutdown);

    const ServerConfig server = server_init(9000);
    s_socket = server.server_socket;
    printf("Server running... Press Ctrl+C to stop.\n");

    while(server_running) {
        const int client_socket = get_client(&server);

        int p_identifier = fork();

        if(p_identifier == 0){
            int* client_ptr = malloc(sizeof(int));
            *client_ptr = client_socket;

            if (client_socket >= 0) {
                client_handler(client_ptr);
            }
        }else if(p_identifier < 0){
            perror("Error forking process.");
        }
    }
    printf("Closing server socket...\n");

    printf("Server shutdown complete.\n");
    return 0;
}

