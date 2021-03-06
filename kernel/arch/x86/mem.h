#ifndef ARCH_MEM_H
#define ARCH_MEM_H

#define NUM_PDE 1024
#define NUM_PTE 1024

// Number of PDEs for the kernels section in memory, the highest 1GB
#define NUM_KERNEL_PDE 256
// Number of PDEs at the top of memory reserved for recursive mappings
#define NUM_RESERVED_PDE 2

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

typedef enum
{
    PAGE_FLAGS_NONE = 0,
    PAGE_FLAGS_WRITE,
    PAGE_FLAGS_USER
} PageFlags;

typedef PageTable *AddressSpace;

void MmInitPhysicalMemoryManager(multiboot_info_t *mbInfo);
void MmInitVirtualMemoryManager();
void MmSetPageDirectory(pa_t pageDirectory);
PageTable *MmGetPageTable(unsigned int directory);
void MmMapPage(pa_t pAddr, va_t vAddr, PageFlags flags);
void MmUnmapPage(va_t vAddr);
pa_t MmWalkPageTable(va_t vAddr);
void MmInvalidatePage(va_t vAddr);
void MmInvalidateAll();

#endif
