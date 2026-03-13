#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

bool write_to_file(const char* filename, const char* content) {
    if (filename == NULL || content == NULL) {
        return false;
    }

    // "a+" opens for writing; if file exists, the content is appended
    // If the file does not exist, it is created and the content appended
    FILE* file = fopen(filename, "a+");
    if (file == NULL) {
        perror("Error opening file for writing");
        return false;
    }

    // Write the content string to the file
    int result = fputs(content, file);
    fprintf(file,"\n");
    
    // Close the file to flush buffers
    fclose(file);

    // fputs returns a non-negative value on success
    return (result != EOF);
}

char* read_from_file(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    // Move cursor to the end to find the size
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory (length + 1 for the null terminator)
    char* buffer = malloc(length + 1);
    if (buffer == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    // Read file into buffer
    size_t read_count = fread(buffer, 1, length, file);
    buffer[read_count] = '\0'; // Always null-terminate

    fclose(file);
    return buffer;
}

bool search_in_file(const char* filename, const char* target) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Could not open file for searching");
        return false;
    }

    char line[MAX_LINE_LENGTH];
    int line_number = 1;
    bool found = false;

    // Read the file line by line
    while (fgets(line, sizeof(line), file)) {
        // strstr returns NULL if the target is not in the current line
        if (strstr(line, target) != NULL) {
            printf("Found '%s' at line %d: %s", target, line_number, line);
            found = true;
        }
        line_number++;
    }

    fclose(file);
    return found;
}

int main(){
    // Example usage for the file interaction functions above 
    char test[100] = "Example text to populate file and enable testing of functions";
    char filename[10] = "test.txt";
    char target[20] = "file";

    if(write_to_file(filename, test)){
        printf("Successful\n");
    }

    if(search_in_file(filename, target)){
        printf("Target found in file");
    }

    char *content = read_from_file(filename);

    if(content){
        printf("File Content:\n%s\n", content);
        free(content); // NOTE: Remember to free memory once content is read from file
    }

    return 0;
}