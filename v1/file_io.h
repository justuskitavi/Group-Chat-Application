#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdbool.h>

#define MAX_LINE_LENGTH 1024

/**
 * Appends a string to the specified file and adds a newline.
 * Returns true on success, false on failure.
 */
bool write_to_file(const char* filename, const char* content);

/**
 * Reads the entire contents of a file into a dynamically allocated buffer.
 * NOTE: The caller is responsible for calling free() on the returned pointer.
 */
char* read_from_file(const char* filename);

/**
 * Checks if a target string exists anywhere within the specified file.
 * Returns true if found, false otherwise.
 */
bool search_in_file(const char* filename, const char* target);

/**
 * Searches for a target string and returns the first line containing it.
 * NOTE: The caller is responsible for calling free() on the returned pointer.
 */
char* search_string(const char* filename, const char* target);

/**
 * Removes every line from the file that contains the specified target string.
 * Uses a temporary file to rewrite the content.
 */
void delete_all_lines_containing(char *filename, char *target);

#endif // FILE_IO_H