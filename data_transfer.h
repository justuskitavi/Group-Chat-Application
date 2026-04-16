#ifndef DATA_TRANSFER_H
#define DATA_TRANSFER_H

#include <netinet/in.h>

/**
 * Sends a string over a UDP socket to a specific destination.
 */
void send_udp_message(int sock, struct sockaddr_in *dest, const char *data);

/**
 * Receives a string from a UDP socket and populates the source address.
 * Returns a dynamically allocated string (must be freed by caller).
 */
char* receive_udp_message(int sock, struct sockaddr_in *src);

#endif // DATA_TRANSFER_H