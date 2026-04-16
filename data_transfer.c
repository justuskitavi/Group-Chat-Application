#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "data_transfer.h"

void send_udp_message(int sock, struct sockaddr_in *dest, const char *data) {
    if (data == NULL) return;
    sendto(sock, data, strlen(data), 0, (struct sockaddr *)dest, sizeof(*dest));
}

char* receive_udp_message(int sock, struct sockaddr_in *src) {
    char buffer[2048];
    socklen_t src_len = sizeof(*src);
    
    ssize_t received = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)src, &src_len);
    if (received <= 0) return NULL;
    
    buffer[received] = '\0';
    return strdup(buffer); // Caller must free this memory
}