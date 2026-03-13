#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Appends a string to the end of a file. 
 * Creates the file if it doesn't exist.
 * Returns true on success, false on failure.
 */
bool write_to_file(const char* filename, const char* content);

/**
 * Reads the entire contents of a file into a dynamically allocated buffer.
 * Note: The caller is responsible for calling free() on the returned pointer.
 * Returns a pointer to the string, or NULL on failure.
 */
char* read_from_file(const char* filename);

/**
 * Searches for a target substring within a file line by line.
 * Prints matches to stdout and returns true if at least one match is found.
 */
bool search_in_file(const char* filename, const char* target);

#endif