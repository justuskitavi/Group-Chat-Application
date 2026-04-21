#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include "data_transfer.h"

int transfer_chunks(const int socket, const void *buffer, size_t length) {
    const char *holder = (const char *)buffer;
    while (length > 0) {
        const ssize_t sent = send(socket, holder, length, 0);
        if (sent <= 0) return -1;
        holder += sent;
        length -= sent;
    }
    return 0;
}

void transfer_message(const int socket, const char *data, const uint32_t data_length) {
    const uint32_t network_size = htonl(data_length);

    char *frame = malloc(sizeof(network_size) + data_length);
    if (frame == NULL) return;

    memcpy(frame, &network_size, sizeof(network_size));
    memcpy(frame + sizeof(network_size), data, data_length);

    transfer_chunks(socket, frame, sizeof(network_size) + data_length);
    free(frame);
}

int receive_chunks(const int socket, void *buffer, const size_t total_to_receive) {
    char *ptr = (char *)buffer;
    size_t remaining = total_to_receive;

    while (remaining > 0) {
        const ssize_t received = read(socket, ptr, remaining);
        if (received < 0) return -1;
        if (received == 0) return -1; /* peer disconnected mid-message */
        ptr += received;
        remaining -= received;
    }
    return 0;
}

char* receive_message(const int socket) {
    uint32_t network_size;

    if (receive_chunks(socket, &network_size, sizeof(network_size)) < 0)
        return NULL;

    const uint32_t message_size = ntohl(network_size);

    char *buffer = malloc(message_size + 1);
    if (buffer == NULL) return NULL;

    if (receive_chunks(socket, buffer, message_size) != 0) {
        free(buffer);
        return NULL;
    }

    buffer[message_size] = '\0';
    return buffer;
}