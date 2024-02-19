#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdint.h>
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


unsigned char* readFD(pager *pager, off_t offset, size_t limit){
    // fd file descriptor
    // limit is pagesize
    // offset is page using off_t for signed int to represent file sizes

    // allocate memory for the size of the limit
    unsigned char *byteData = malloc(limit);
    if (byteData == NULL) {
        perror("Failed allocating memory");
        free(byteData);
        return NULL;
    }

    // file pointer relative to its current position
    if (lseek(pager->fd, offset, SEEK_CUR) == -1) {
        perror("failed seeking file");
        free(byteData);
        return NULL;
    }

    // read file
    if (read(pager->fd, byteData, sizeof(byteData)) == -1) {
        perror("failed reading file");
        free(byteData);
        return NULL;
    }

    //  set the file pointer to an absolute position in the file
    if (lseek(pager->fd, offset, SEEK_SET) == -1) {
        perror("error seeking file");
        free(byteData);
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

    // write
    ssize_t bytes_written = write(fd, data, sizeof(data));
    if (bytes_written == -1) {
        perror("failed writing");
        close(fd);
        return -1;
    }

    // sync to disk
    if (fsync(fd) == -1) {
        perror("failed sync to disk");
        close(fd);
        return -1;
    }

    close(fd);

    return 0;
}


void readBinaryData(const uint8_t *currentPage, size_t currentPageSize, uint64_t *totalRowExist) {
    // check if current page large enough to contain PAGE_HEADER_BYTES
    if (currentPageSize < PAGE_HEADER_SIZE) {
        printf("Error: currentPage is too small");
        return;
    }

    // read the binary data (assuming little-endian encoding)
    //*totalRowExist = currentPage[0] | (currentPage[1] << 8) | (currentPage[2]) << 16) | (currentPage[3] << 24);
    *totalRowExist = 0;
    for (size_t i = 0; i < PAGE_HEADER_SIZE; i++) {
        *totalRowExist |= ((uint32_t)currentPage[i] << (i * 8));
    }

}

void insertRowData(unsigned char *rowData, pager *pager) {

    // check if the input value more than tuple size
    if (sizeof(rowData) > MAX_TUPLE_SIZE) {
        perror("MAX TUPLE SIZE");
        exit(EXIT_FAILURE);
    }

    // get page of pointer offset
    int currentPagePointer = getLastPageStartOffset(pager);

    if (currentPagePointer == -1) {
        perror("failed get last page start offset");
        exit(EXIT_FAILURE);
    }

    unsigned char *currentPage = readFD(pager, currentPagePointer, PAGE_SIZE);

    if (currentPage == NULL) {
        perror("failed readfd");
        exit(EXIT_FAILURE);
    }

    uint64_t totalRowExist;

    readBinaryData(currentPage, sizeof(currentPage), &totalRowExist);


}

