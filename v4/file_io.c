#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "file_io.h"
#define MAX_LINE_LENGTH 1024

int tag(FILE *file){
    int character;
    int tag = 1;
    int last_character = '\n';

    while ((character = fgetc(file)) != EOF) {
        if (character == '\n') {
            tag++;
        }
        last_character = character;
    }
    
    if (last_character != '\n') {
        tag++;
    }

    return tag;
}

bool write_to_file(const char* filename, const char* content) {
    if (filename == NULL || content == NULL) {
        return false;
    }

    FILE* file = fopen(filename, "a+");
    if (file == NULL) {
        perror("Error opening file for writing");
        return false;
    }

    int id = tag(file);
    int buffer_size = snprintf(NULL, 0, "%d | %s", id, content);
    
    char *buffer = malloc(buffer_size + 1);
    if (!buffer) return false;

    snprintf(buffer, buffer_size + 1, "%d | %s", id, content);

    int result = fputs(buffer, file);
    fprintf(file,"\n");
    
    free(buffer);
    fclose(file);

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
            found = true;
        }
        line_number++;
    }

    fclose(file);
    return found;
}

char* search_string(const char* filename, const char* target) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Could not open file for searching");
        return false;
    }

    char line[MAX_LINE_LENGTH];
    char* result = NULL;

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, target) != NULL) {
            result = malloc(strlen(line) + 1);
            if (result != NULL) {
                strcpy(result, line);
            }
            break; 
        }
    }

    fclose(file);
    return result;
}

void delete_all_lines_containing(const char *filename, const char *target) {
    FILE *file, *temp;
    char line[256];

    file = fopen(filename, "r");
    temp = fopen("temp.txt", "w");

    if (file == NULL || temp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, target) == NULL) {  
            fputs(line, temp);
        }
    }

    fclose(file);
    fclose(temp);

    remove(filename);
    rename("temp.txt", filename);
}

// int main(){
//     // Example usage for the file interaction functions above 
//     char test[100] = "Example text to populate file and enable testing of functions";
//     char filename[10] = "test.txt";
//     char target[50] = "jpuj08ljoh0";

//     if(write_to_file(filename, test)){
//         printf("Successful\n");
//     }

//     if(search_in_file(filename, target)){
//         printf("Target found in file");
//     }

//     char* result = search_string(filename, target);

//     if(result){
//         printf("%s", result);
//         free(result); // Remember to free memory once content is read from file
//     }


//     char* content = read_from_file(filename);

//     if(content){
//         printf("File Content:\n%s\n", content);
//         free(content); // Remember to free memory once content is read from file
//     }

//     delete_all_lines_containing(filename, target);

//     return 0;
// }
