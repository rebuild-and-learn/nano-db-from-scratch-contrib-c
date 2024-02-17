#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "pageblock.h"


int main(){
    pager pager = newPager("hello");
    printf("pager: %s\n", pager.blockpath);

    char input[100];
    
    printf("nano-db >> ");

    for (;;) {
        fgets(input, sizeof(input), stdin);
        
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "INIT") == 0) {
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
        } else {
            printf("~\n");
            break;
        }
        
        printf("\nnano-db >> ");
   
    }
    
    return 0;
}
