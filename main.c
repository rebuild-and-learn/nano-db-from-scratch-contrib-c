#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/stat.h>
#include "pageblock.h"


void toUpper(char *value) {
    for (ssize_t i = 0; value[i] != '\0'; i++) {
        value[i] = toupper(value[i]);
    }
}


int main(){
    pager pager = newPager("hello");
    printf("pager: %s\n", pager.blockpath);

    // char input[100];
    char *input = NULL;
    size_t input_size = 0;
    ssize_t input_length;
    
    printf("nano-db >> ");

    for (;;) {
        input_length = getline(&input, &input_size, stdin);
        if (input_length == -1) {
            perror("failed get input"); 
            break;
        }

        char *command = strtok(input, " ");
        toUpper(command);
        char *input_value = strtok(NULL, "\n");
         
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(command, "INIT") == 0) {
            printf("INIT");
            char *data = (char *)malloc(PAGE_SIZE);
            int initfile = initFile("hello", data);
            if (initfile == -1) {
                perror("failed init file");
                free(data);
                break;
            }
            free(data);
        } else if (strcmp(input, "INSERT") == 0) {
            printf("INSERT");
            printf("input value: %s", input_value);
        } else {
            printf("~\n");
            break;
        }
        free(input);
        printf("\nnano-db >> ");
   
    }
    
    return 0;
}
