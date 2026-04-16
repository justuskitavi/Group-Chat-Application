#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <arpa/inet.h>

#include "server.h"
#include "data_transfer.h"

volatile sig_atomic_t server_running = 1;
int s_socket;

void handle_shutdown(int signal) {
    server_running = 0;
    close(s_socket);
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    signal(SIGINT, handle_shutdown);

    const ServerConfig server = server_init(9000);
    s_socket = server.server_socket;

    printf("Server running... Press Ctrl+C to stop.\n");
    printf("Mode: iterative, connectionless (UDP).\n");

    while(server_running) {
        struct sockaddr_in client_addr;

        // Iterative connectionless server algorithm:
        // 1. Read request
        char *msg = receive_udp_message(s_socket, &client_addr);

        if (msg) {
            printf("[READ] %s:%d -> %s\n",
                   inet_ntoa(client_addr.sin_addr),
                   ntohs(client_addr.sin_port),
                   msg);

            // 2. Process request
            // 3. Formulate a response
            // 4. Send reply
            printf("[PROCESS] Handling one datagram request.\n");
            route_packet(s_socket, &client_addr, msg);
            printf("[REPLY] Response sent to %s:%d\n",
                   inet_ntoa(client_addr.sin_addr),
                   ntohs(client_addr.sin_port));
            free(msg);
        }
    }

    printf("Closing server socket...\n");
    printf("Server shutdown complete.\n");
    return 0;
}
