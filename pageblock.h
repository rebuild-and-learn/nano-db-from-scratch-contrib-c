// prevent include multiple times
#ifndef PAGEBLOCK_H
#define PAGEBLOCK_H

#include <stdio.h>

// The O_DIRECT flag is not part of the C standard, and its availability may depend on the operating system and filesystem
#ifndef O_DIRECT
    #define O_DIRECT 040000
#endif

#define PAGE_SIZE 1024 // 1kb
#define MAX_TUPLE_SIZE 100 // 100bytes
#define PAGE_HEADER_SIZE 8 // for uint64

typedef struct Pager {
    char* blockpath;
    int fd;
} pager;

int main();
pager newPager(char* blockPath);
unsigned char* readFD(int fd, off_t, size_t limit);
void rewrite(pager *pager, off_t offset, unsigned char *data);
int getLastPgaeStartOffset(pager *pager);
int initFile(char* filename, char *data);


#endif

