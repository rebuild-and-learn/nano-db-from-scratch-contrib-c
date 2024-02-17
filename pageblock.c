#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "pageblock.h"


pager newPager(char* blockPath) {
    int fd  = open(blockPath, O_CREAT|O_DIRECT|O_RDWR, 0664);
    pager pager;


    if (fd == -1) {
      perror("Failed open blockpath");
      close(fd);
      exit(EXIT_FAILURE);
    }

    pager.blockpath = blockPath;
    pager.fd = fd;

    return pager;
}


unsigned char* readFD(int fd, off_t offset, size_t limit){
    // fd file descriptor
    // limit is pagesize
    // offset is page using off_t for signed int to represent file sizes

    // allocate memory for the size of the limit
    unsigned char *byteData = malloc(limit);
    if (byteData == NULL) {
        perror("Failed allocating memory");
        free(byteData);
        //close(fd);
        //exit(EXIT_FAILURE);
        return NULL;
    }

    // file pointer relative to its current position
    if (lseek(fd, offset, SEEK_CUR) == -1) {
        perror("failed seeking file");
        free(byteData);
        //close(fd);
        //exit(EXIT_FAILURE);
        return NULL;
    }

    // read file
    if (read(fd, byteData, sizeof(byteData)) == -1) {
        perror("failed reading file");
        free(byteData);
       // close(fd);
        //exit(EXIT_FAILURE);
        return NULL;
    }

    //  set the file pointer to an absolute position in the file
    if (lseek(fd, offset, SEEK_SET) == -1) {
        perror("error seeking file");
        free(byteData);
        //close(fd);
        //exit(EXIT_FAILURE);
        return NULL;
    }
    return byteData;
}

void rewrite(pager *pager, off_t offset, unsigned char *data) {
    if (lseek(pager->fd, offset, SEEK_CUR) == -1) {
      perror("failed seeking file");
      exit(EXIT_FAILURE);
    }

    if (write(pager->fd, (const char *) data, sizeof(data)) == -1) {
      perror("failed write");
      exit(EXIT_FAILURE);
    }
}

int getLastPageStartOffset(pager *pager) {
    struct stat statbuf;
    int s = stat(pager->blockpath, &statbuf);

    if (s == -1) {
        perror("failed stat");
        return -1;
    }

    return statbuf.st_size - PAGE_SIZE;
}

int initFile(char* filename, char *data) {
    int fd = open(filename, O_WRONLY, 0664);

    if (fd == -1) {
        perror("failed open file");
        return -1;
    }
    
    memset(data, 0, PAGE_SIZE);

    ssize_t bytes_written = write(fd, data, sizeof(data));
    if (bytes_written == -1) {
        perror("failed writing");
        close(fd);
        return -1; 
    }

    if (fsync(fd) == -1) {
        perror("failed sync to disk");
        close(fd);
        return -1;
    }

    close(fd);
    
    return 0;
}

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
