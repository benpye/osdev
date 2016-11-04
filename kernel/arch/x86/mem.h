#ifndef ARCH_MEM_H
#define ARCH_MEM_H

typedef struct
{
    unsigned int Present : 1;
    unsigned int Writable : 1;
    unsigned int User : 1;
    unsigned int WriteThrough : 1;
    unsigned int CacheDisable : 1;
    unsigned int Accessed : 1;
    unsigned int Dirty : 1;
    unsigned int PageSize : 1;
    unsigned int Global : 1;
    unsigned int Ignored : 3;
    unsigned int Address : 20;
} __attribute__((packed)) PageTable;

#endif