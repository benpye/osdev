#include <kernel.h>
#include <stdint.h>
#include <stddef.h>

#include <rtl/memory.h>
#include <rtl/debug.h>
#include <mm/pfa.h>

#include "mem.h"

__attribute__((aligned(PAGE_SIZE))) PageTable MmPageDirectory[1024];
int MmVmmInit = 0;

extern void *KERNEL_END;
extern void *MmBootPageDirectory;

void MmInitVirtualMemoryManager()
{
    RtlZeroMemory(MmPageDirectory, sizeof(MmPageDirectory));

    for(unsigned int addr = 0; addr < VIRT_TO_PHYS((unsigned int)&KERNEL_END); addr += PAGE_SIZE)
    {
        MmMapKernelPage((void *)addr, (void *)(addr + KERNEL_VIRTUAL_BASE), PAGE_FLAGS_NONE);
    }

    MmPageDirectory[NUM_PDE - 1].Present = 1;
    MmPageDirectory[NUM_PDE - 1].Writable = 1;
    MmPageDirectory[NUM_PDE - 1].User = 1;
    MmPageDirectory[NUM_PDE - 1].Address = (int)VA_TO_PA(&MmPageDirectory) >> 12;

    MmSetPageDirectory(VA_TO_PA(MmPageDirectory));

    MmFreePhysicalPage(VA_TO_PA(MmBootPageDirectory));

    MmVmmInit = 1;
}

void MmSetPageDirectory(PageTable *pageDirectory)
{
    asm volatile ("movl %0, %%cr3\n" :: "r"(pageDirectory));
}

PageTable *MmGetPageTable(unsigned int directory)
{
    if(MmVmmInit)
        return (PageTable *)((NUM_PDE - 1) << 22);
    else
    {
        PageTable *pde = &MmPageDirectory[directory];
        RtlDebugAssert(pde->Present && !pde->PageSize, "Can only get page table for present non table PDEs");
        return (PageTable *)PA_TO_VA(pde->Address << 12);
    }
}

void MmMapKernelPage(void *pAddr, void *vAddr, PageFlags flags)
{
    RtlDebugAssert(((int)pAddr % PAGE_SIZE) == 0, "Physical address not page aligned");
    RtlDebugAssert(((int)vAddr % PAGE_SIZE) == 0, "Virtual address not page aligned");

    uint32_t phys = (uint32_t)pAddr;
    uint32_t virt = (uint32_t)vAddr;

    // Directory is top 10 bits, table is next 10
    int directory = virt >> 22;
    int table = (virt >> 12) & 0x3FF;

    RtlDebugAssert(directory < (NUM_PDE - 1), "The top entry in the directory cannot be mapped");

    PageTable *pde = &MmPageDirectory[directory];

    int alloc = 0;

    if(!pde->Present)
    {
        int ptPhys = (int)MmAllocatePhysicalPage();
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
    pte->Address = phys >> 12;
}

void MmUnmapKernelPage(void *vAddr)
{
    RtlDebugAssert(((int)vAddr % PAGE_SIZE) == 0, "Virtual address not page aligned");

    uint32_t virt = (uint32_t)vAddr;

    // Directory is top 10 bits, table is next 10
    int directory = virt >> 22;
    int table = (virt >> 12) & 0x3FF;

    RtlDebugAssert(directory < (NUM_PDE - 1), "The top entry in the directory cannot have any table unmapped");

    PageTable *pde = &MmPageDirectory[directory];

    RtlDebugAssert(pde->Present, "PDE containing PT must be present");

    PageTable *pt = MmGetPageTable(directory);
    PageTable *pte = &pt[table];

    RtlDebugAssert(pte->Present, "PTE to unmap must be present");

    pte->Present = 0;
}

void *MmWalkPageTable(void *vAddr)
{
    uint32_t virt = (uint32_t)vAddr;

    // Directory is top 10 bits, table is next 10
    int directory = virt >> 22;
    int table = (virt >> 12) & 0x3FF;

    PageTable *pde = &MmPageDirectory[directory];

    RtlDebugAssert(pde->Present, "PDE containing PT must be present");

    PageTable *pt = MmGetPageTable(directory);
    PageTable *pte = &pt[table];

    RtlDebugAssert(pte->Present, "PTE to walk must be present");

    return (void *)((pte->Address << 12) | (virt & (PAGE_SIZE - 1)));
}
