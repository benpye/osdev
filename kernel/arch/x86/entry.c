#include <stdint.h>
#include <kernel.h>

#include <mm/pfa.h>
#include <rtl/debug.h>

#include "mem.h"

#include "multiboot.h"

extern void *KERNEL_END;

void ArStart(uint32_t mbMagic, multiboot_info_t *mbInfo)
{
    multiboot_info_t *vinfo = PA_TO_VA(mbInfo);

    RtlDebugPrint("Multiboot magic: %x, cmdline: %s, bootloader: %s\n", mbMagic,
                  PA_TO_VA(vinfo->cmdline), PA_TO_VA(vinfo->boot_loader_name));

    multiboot_memory_map_t *mmapArray = PA_TO_VA(vinfo->mmap_addr);

    for(int i = 0; i < vinfo->mmap_length/sizeof(multiboot_memory_map_t); i++)
    {
        RtlDebugPrint("Memory region: addr_high %x addr_low %x, len_high %x len_low %x, Size %x, Type %x\n", 
                      mmapArray[i].addr_high, mmapArray[i].addr_low, mmapArray[i].len_high, mmapArray[i].len_low, mmapArray[i].size, mmapArray[i].type);

        if(mmapArray[i].type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            uint64_t addr = ((uint64_t)mmapArray[i].addr_high) << 32 | mmapArray[i].addr_low;
            uint64_t end = addr + (((uint64_t)mmapArray[i].len_high) << 32 | mmapArray[i].len_low);

            addr &= ~(PAGE_SIZE - 1);

            for(; addr < end; addr += PAGE_SIZE)
            {
                if(addr > (uint64_t)VA_TO_PA(&KERNEL_END))
                    MmFreePhysicalPage((void *)addr);
            }
        }
    }

    RtlDebugPrint("MmGetFreePhysicalPages: %x\n", MmGetFreePhysicalPages());

    void *phys = MmAllocatePhysicalPage();
    RtlDebugPrint("First avaliable physical page %x\n", phys);

    RtlDebugPrint("MmGetFreePhysicalPages: %x\n", MmGetFreePhysicalPages());

    MmInitVirtualMemoryManager();

    asm volatile (
        "movl %%cr3, %%eax\n"
        "movl %%eax, %%cr3\n"
        ::: "%eax");

    MmMapKernelPage(VA_TO_PA((void*)((int)(&ArStart) & ~(PAGE_SIZE - 1))), (void*)0, PAGE_FLAGS_NONE);
    while(1){ }

    RtlDebugAssert(0, "Kernel ended!");
}