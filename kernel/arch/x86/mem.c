#include <kernel.h>
#include <stdint.h>
#include <stddef.h>

#include <rtl/memory.h>
#include <rtl/debug.h>
#include <mm/pfa.h>

#include "multiboot.h"
#include "mem.h"

__attribute__((aligned(PAGE_SIZE))) PageTable MmKernelPD[NUM_PDE];
__attribute__((aligned(PAGE_SIZE))) PageTable MmKernelPTs[NUM_KERNEL_PDE - NUM_RESERVED_PDE][NUM_PTE];
int MmVmmInit = 0;

extern void *KERNEL_END;
extern void *MmBootPageDirectory;

void MmInitPhysicalMemoryManager(multiboot_info_t *mbInfo)
{
    RtlDebugAssert((mbInfo->flags & MULTIBOOT_INFO_MEM_MAP) != 0, "Requires memory map from bootloader");
    multiboot_memory_map_t *mmapArray = (multiboot_memory_map_t *)P2V(mbInfo->mmap_addr);

    for(int i = 0; i < mbInfo->mmap_length/sizeof(multiboot_memory_map_t); i++)
    {
        if(mmapArray[i].type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            pa_t addr = ((pa_t)mmapArray[i].addr_high) << 32 | mmapArray[i].addr_low;
            pa_t end = addr + (((pa_t)mmapArray[i].len_high) << 32 | mmapArray[i].len_low);

            addr &= ~(PAGE_SIZE - 1);

            for(; addr < end; addr += PAGE_SIZE)
            {
                if(addr > V2P((va_t)&KERNEL_END))
                    MmFreePhysicalPage(addr);
            }
        }
    }
}

void MmInitVirtualMemoryManager()
{
    RtlZeroMemory(MmKernelPD, sizeof(MmKernelPD));
    RtlZeroMemory(MmKernelPTs, sizeof(MmKernelPTs));

    for(int i = 0; i < NUM_KERNEL_PDE - NUM_RESERVED_PDE; i++)
    {
        MmKernelPD[NUM_PDE - NUM_KERNEL_PDE + i - 1].Present = 1;
        MmKernelPD[NUM_PDE - NUM_KERNEL_PDE + i - 1].Writable = 1;
        MmKernelPD[NUM_PDE - NUM_KERNEL_PDE + i - 1].User = 1;
        MmKernelPD[NUM_PDE - NUM_KERNEL_PDE + i - 1].Address = V2P((va_t)&MmKernelPTs[i]) >> 12;
    }

    MmKernelPD[NUM_PDE - 1].Present = 1;
    MmKernelPD[NUM_PDE - 1].Writable = 1;
    MmKernelPD[NUM_PDE - 1].User = 1;
    MmKernelPD[NUM_PDE - 1].Address = V2P((va_t)&MmKernelPD) >> 12;

    for(pa_t addr = 0; addr < V2P((va_t)&KERNEL_END); addr += PAGE_SIZE)
    {
        RtlDebugAssert(P2V(addr) < (1ULL << 32), "Cannot map to virtual address greater than 2^32 - 1");
        MmMapPage(addr, P2V(addr), PAGE_FLAGS_WRITE);
    }

    MmSetPageDirectory(V2P((va_t)&MmKernelPD));

    MmFreePhysicalPage(V2P((va_t)&MmBootPageDirectory));

    MmVmmInit = 1;
}

void MmSetPageDirectory(pa_t pageDirectory)
{
    RtlDebugAssert(pageDirectory < (1ULL << 32), "Do not support PD above 2^32 - 1");
    asm volatile ("movl %0, %%cr3" :: "r"((uint32_t)pageDirectory));
}

PageTable *MmGetPageTable(unsigned int directory)
{
    if(MmVmmInit)
        return &((PageTable *)((NUM_PDE - 1) << 22))[directory * NUM_PTE];
    else
    {
        PageTable *pde = &MmKernelPD[directory];
        RtlDebugAssert(pde->Present && !pde->PageSize, "Can only get page table for present non table PDEs");
        return (PageTable *)P2V(pde->Address << 12);
    }
}

void MmMapPage(pa_t pAddr, va_t vAddr, PageFlags flags)
{
    RtlDebugAssert((pAddr % PAGE_SIZE) == 0, "Physical address not page aligned");
    RtlDebugAssert((vAddr % PAGE_SIZE) == 0, "Virtual address not page aligned");
    // TODO: Lift restriction (PAE)
    RtlDebugAssert(pAddr < (1ULL << 32), "Physical addresses greater than 2^32 - 1 not supported")

     // Directory is top 10 bits, table is next 10
    int directory = vAddr >> 22;
    int table = (vAddr >> 12) & 0x3FF;

    RtlDebugAssert(directory < (NUM_PDE - NUM_RESERVED_PDE),
        "The top %i entries in the directory cannot be mapped", NUM_RESERVED_PDE);

    PageTable *pde = &MmKernelPD[directory];

    int alloc = 0;

    if(!pde->Present)
    {
        RtlDebugAssert(directory < NUM_PDE - NUM_KERNEL_PDE, "Kernel PDEs should always be present");
        pa_t ptPhys = MmAllocatePhysicalPage();
        alloc = 1;
        
        pde->Present = 1;
        pde->Writable = 1;
        pde->User = 0;
        pde->WriteThrough = 0;
        pde->CacheDisable = 0;
        pde->Accessed = 0;
        pde->Dirty = 0;
        pde->PageSize = 0;
        pde->Global = 0;
        pde->Ignored = 0;
        pde->Address = ptPhys >> 12;
    }

    PageTable *pt = MmGetPageTable(directory);    

    if(alloc)
        RtlZeroMemory(pt, sizeof(PageTable) * NUM_PTE);

    PageTable *pte = &pt[table];

    RtlDebugAssert(!pte->Present, "Virtual address already mapped 0x%x", vAddr);

    pte->Present = 1;
    pte->Writable = ((flags & PAGE_FLAGS_WRITE) != 0);
    pte->User = ((flags & PAGE_FLAGS_USER) != 0);
    pte->WriteThrough = 0;
    pte->CacheDisable = 0;
    pte->Accessed = 0;
    pte->Dirty = 0;
    pte->PageSize = 0;
    pte->Global = 0;
    pte->Ignored = 0;
    pte->Address = pAddr >> 12;
}

void MmUnmapPage(va_t vAddr)
{
    RtlDebugAssert((vAddr % PAGE_SIZE) == 0, "Virtual address not page aligned");

    // Directory is top 10 bits, table is next 10
    int directory = vAddr >> 22;
    int table = (vAddr >> 12) & 0x3FF;

    RtlDebugAssert(directory < (NUM_PDE - NUM_RESERVED_PDE),
        "The top %i entries in the directory cannot be unmapped", NUM_RESERVED_PDE);

    PageTable *pde = &MmKernelPD[directory];

    RtlDebugAssert(pde->Present, "PDE containing PT must be present");

    PageTable *pt = MmGetPageTable(directory);
    PageTable *pte = &pt[table];

    RtlDebugAssert(pte->Present, "PTE to unmap must be present");

    pte->Present = 0;
}

pa_t MmWalkPageTable(va_t vAddr)
{
    // Directory is top 10 bits, table is next 10
    int directory = vAddr >> 22;
    int table = (vAddr >> 12) & 0x3FF;

    PageTable *pde = &MmKernelPD[directory];

    RtlDebugAssert(pde->Present, "PDE containing PT must be present");

    PageTable *pt = MmGetPageTable(directory);
    PageTable *pte = &pt[table];

    RtlDebugAssert(pte->Present, "PTE to walk must be present");

    return (pte->Address << 12) | (vAddr & (PAGE_SIZE - 1));
}

void MmInvalidatePage(va_t vAddr)
{
    asm volatile ("invlpg (%0)" :: "b"(vAddr) : "memory");
}

void MmInvalidateAll()
{
    asm volatile (
        "movl %%cr3, %%eax\n"
        "movl %%eax, %%cr3\n"
        ::: "%eax");
}
