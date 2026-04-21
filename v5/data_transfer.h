#ifndef DATA_TRANSFER_H
#define DATA_TRANSFER_H

#include <stdint.h>

/**
 * Sends a specific amount of raw bytes over a socket,
 * looping until all data is sent.
 * * Returns 0 on success, -1 on failure/disconnection.
 */
int transfer_chunks(int socket, const void *buffer, size_t length);

/**
 * Sends a message by first sending a 4-byte size header
 * followed by the data payload.
 */
void transfer_message(int socket, const char *data, uint32_t data_length);

/**
 * Receives a specific amount of raw bytes from a socket,
 * looping until the buffer is full.
 * * Returns 0 on success, -1 on failure or disconnection.
 */
int receive_chunks(int socket, void *buffer, size_t total_to_receive);

/**
 * Receives a message by reading a 4-byte size header,
 * allocating the necessary memory, and reading the payload.
 * Returns a pointer to the allocated string (must be freed by caller),
 * or NULL on failure.
 */
char* receive_message(int socket);

#endif //DATA_TRANSFER_H