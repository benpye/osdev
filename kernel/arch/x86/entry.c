#include <stdint.h>
#include <kernel.h>

#include <mm/pfa.h>
#include <rtl/debug.h>

#include "multiboot.h"
#include "mem.h"

extern void *KERNEL_END;

void ArStart(uint32_t mbMagic, multiboot_info_t *mbInfo)
{
    multiboot_info_t *vinfo = (multiboot_info_t *)P2V((pa_t)mbInfo);

    RtlDebugPrint("Multiboot magic: %x, cmdline: %s, bootloader: %s\n", mbMagic,
                  P2V(vinfo->cmdline), P2V(vinfo->boot_loader_name));

    MmInitPhysicalMemoryManager(vinfo);

    RtlDebugPrint("MmGetFreePhysicalPages: %x\n", MmGetFreePhysicalPages());

    pa_t phys = MmAllocatePhysicalPage();
    RtlDebugPrint("First avaliable physical page %x\n", (void*)phys);

    RtlDebugPrint("MmGetFreePhysicalPages: %x\n", MmGetFreePhysicalPages());

    MmInitVirtualMemoryManager();
    pa_t physs = MmAllocatePhysicalPage();
    RtlDebugPrint("Next avaliable physical page %x\n", (void*)physs);
    MmFreePhysicalPage(phys);
    MmFreePhysicalPage(physs);

    MmMapKernelPage(V2P((int)(&ArStart) & ~(PAGE_SIZE - 1)), 0, PAGE_FLAGS_NONE);
    MmInvalidatePage(0);

    RtlDebugPrint("0x0 == 0x%x\n", MmWalkPageTable(0x0));
    RtlDebugPrint("0xc0000000 == 0x%x\n", MmWalkPageTable(0xc0000000));

    while(1){ }

    RtlDebugAssert(0, "Kernel ended!");
}
