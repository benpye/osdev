#include <stdint.h>
#include <kernel.h>

#include "multiboot.h"

void ArStart(uint32_t mbMagic, struct multiboot_info *mbInfo)
{
    struct multiboot_info *vinfo = PA_TO_VA(mbInfo);
    if(mbMagic == MULTIBOOT_BOOTLOADER_MAGIC)
    {
        int i = 0;
        char *c = PA_TO_VA(vinfo->cmdline);
        while(*c != 0)
        {
            ((uint16_t*) PA_TO_VA(0xB8000))[i] = *c | (15 << 8);
            c++;
            i++;
        }
    }
    while(1){ }
}