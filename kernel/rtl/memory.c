#include <stdint.h>
#include <stddef.h>
#include <kernel.h>

#include <rtl/memory.h>

void RtlZeroMemory(void *addr, size_t len)
{
    uint8_t *buf = (uint8_t *)addr;
    for(unsigned int i = 0; i < len; i++)
    {
        buf[i] = 0;
    }
}