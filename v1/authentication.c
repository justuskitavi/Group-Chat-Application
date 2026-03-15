#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../file_io.h"
#include "authentication.h"
#include "registration.h"

#define USERS_FILE "users.txt"

bool authenticate_user(char* username, size_t username_capacity){
    char password[20];
    char stored_hash[20];
    char input_hash_str[20];

    if (username == NULL || username_capacity == 0) {
        return false;
    }

    printf("Enter username: ");
    fgets(username, username_capacity, stdin);
    username[strcspn(username, "\n")] = '\0';

    printf("Enter password: ");
    scanf("%19s", password);

    char* user_record = search_string(USERS_FILE, username);
    
    if (user_record == NULL) {
        perror("Unregistered user");
        return false;
    }

    char* last_space = strrchr(user_record, ' ');
    if (last_space) {
        sscanf(last_space + 1, "%s", stored_hash);
    }

    free(user_record);
    unsigned long input_hash = hash_password(password);

    sprintf(input_hash_str, "%lx", input_hash);

    bool is_authenticated = (strcmp(stored_hash, input_hash_str) == 0);

    return is_authenticated;
}

// int main(){
//     if(authenticate_user()){
//         printf("\nUser authenticated successfully");
//     }
//     return 0;
// }