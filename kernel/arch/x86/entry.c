#include <stdint.h>
#include <kernel.h>

#include <mm/pfa.h>
#include <rtl/debug.h>

#include "mem.h"

#include "multiboot.h"

extern void *KERNEL_END;

void ArStart(uint32_t mbMagic, multiboot_info_t *mbInfo)
{
    multiboot_info_t *vinfo = (multiboot_info_t *)P2V((pa_t)mbInfo);

    RtlDebugPrint("Multiboot magic: %x, cmdline: %s, bootloader: %s\n", mbMagic,
                  P2V(vinfo->cmdline), P2V(vinfo->boot_loader_name));

    multiboot_memory_map_t *mmapArray = (multiboot_memory_map_t *)P2V(vinfo->mmap_addr);

    for(int i = 0; i < vinfo->mmap_length/sizeof(multiboot_memory_map_t); i++)
    {
        RtlDebugPrint("Memory region: addr_high %x addr_low %x, len_high %x len_low %x, Size %x, Type %x\n", 
                      mmapArray[i].addr_high, mmapArray[i].addr_low, mmapArray[i].len_high, mmapArray[i].len_low, mmapArray[i].size, mmapArray[i].type);

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

    RtlDebugPrint("MmGetFreePhysicalPages: %x\n", MmGetFreePhysicalPages());

    pa_t phys = MmAllocatePhysicalPage();
    RtlDebugPrint("First avaliable physical page %x\n", (void*)phys);

    RtlDebugPrint("MmGetFreePhysicalPages: %x\n", MmGetFreePhysicalPages());

    MmInitVirtualMemoryManager();
    pa_t physs = MmAllocatePhysicalPage();
    RtlDebugPrint("Next avaliable physical page %x\n", (void*)physs);
    MmFreePhysicalPage(phys);
    MmFreePhysicalPage(physs);

    asm volatile (
        "movl %%cr3, %%eax\n"
        "movl %%eax, %%cr3\n"
        ::: "%eax");

    MmMapKernelPage(V2P((int)(&ArStart) & ~(PAGE_SIZE - 1)), 0, PAGE_FLAGS_NONE);

    RtlDebugPrint("0x0 == 0x%x\n", MmWalkPageTable(0x0));
    RtlDebugPrint("0xc0000000 == 0x%x\n", MmWalkPageTable(0xc0000000));


    while(1){ }

    RtlDebugAssert(0, "Kernel ended!");
}
