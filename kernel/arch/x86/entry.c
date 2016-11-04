#include <stdint.h>
#include <kernel.h>

#include <rtl/debug.h>

#include "multiboot.h"

void ArStart(uint32_t mbMagic, struct multiboot_info *mbInfo)
{
    struct multiboot_info *vinfo = PA_TO_VA(mbInfo);

    RtlDebugPrint("Multiboot magic: %x, cmdline: %s, bootloader: %s\n", mbMagic,
                  PA_TO_VA(vinfo->cmdline), PA_TO_VA(vinfo->boot_loader_name));
    while(1){ }
}