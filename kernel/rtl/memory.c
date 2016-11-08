#include <stdint.h>
#include <stddef.h>
#include <kernel.h>

#include <rtl/memory.h>

void *memcpy(void *destination, const void *source, size_t num)
{
    const uint8_t *sourceBuf = (const uint8_t *)source;
    uint8_t *buf = destination;

    for(size_t i = 0; i < num; i++)
    {
        buf[i] = sourceBuf[i];
    }

    return destination;
}

void *memmove(void *destination, const void *source, size_t num)
{
    const uint8_t *sourceBuf = (const uint8_t *)source;
    uint8_t *buf = destination;

    if((uintptr_t)destination < (uintptr_t)source)
    {
        for(size_t i = 0; i < num; i++)
        {
            buf[i] = sourceBuf[i];
        }
    }
    else
    {
        for(size_t i = num - 1; i > -1; i--)
        {
            buf[i] = sourceBuf[i];
        }
    }

    return destination;
}

void *memset(void *ptr, int value, size_t num)
{
    uint8_t *buf = ptr;

    for(size_t i = 0; i < num; i++)
    {
        buf[i] = (uint8_t)value;
    }
    
    return ptr;
}

int memcmp(const void *ptr1, const void *ptr2, size_t num)
{
    const uint8_t *buf1 = (const uint8_t *)ptr1;
    const uint8_t *buf2 = (const uint8_t *)ptr2;

    for(size_t i = 0; i < num; i++)
    {
        if(buf1[i] != buf2[i]) return buf1[i] - buf2[i];
    }

    return 0;
}

void RtlZeroMemory(void *ptr, size_t len)
{
    uint8_t *buf = (uint8_t *)ptr;
    for(size_t i = 0; i < len; i++)
    {
        buf[i] = 0;
    }
}
