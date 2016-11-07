#include <kernel.h>
#include <stdint.h>
#include <stddef.h>

#include <mm/pfa.h>
#include <rtl/debug.h>

uint8_t MmPfaBitmap[(PHYSICAL_MEMORY_SIZE/PAGE_SIZE)/8] = { 0 };
uint32_t MmPfaFreePages = 0;

void *MmAllocatePhysicalPage()
{
    RtlDebugAssert(MmPfaFreePages > 0, "No free physical page");
    for(int i = 0; i < (PHYSICAL_MEMORY_SIZE/PAGE_SIZE)/8; i++)
    {
        if(MmPfaBitmap[i] != 0)
        {
            for(int j = 0; j < 8; j++)
            {
                if((MmPfaBitmap[i] & (1 << j)) != 0)
                {
                    void *pAddr = (void *)(((i * 8) + j) * PAGE_SIZE);
                    MmReservePhysicalPage(pAddr);
                    return pAddr;
                }
            }
        }
    }

    return NULL;
}

void MmFreePhysicalPage(void *pAddr)
{
    RtlDebugAssert(((int)pAddr % PAGE_SIZE) == 0, "Tried to free non aligned page");
    int page = (int)pAddr / PAGE_SIZE;
    int entry = page / 8;
    int bit = page % 8;
    RtlDebugAssert((MmPfaBitmap[entry] & (1 << bit)) == 0, "Tried to free free physical page at 0x%x", pAddr);
    MmPfaBitmap[entry] |= (1 << bit);
    MmPfaFreePages++;
}

void MmReservePhysicalPage(void *pAddr)
{
    RtlDebugAssert(((int)pAddr % PAGE_SIZE) == 0, "Tried to reserve non aligned page");
    int page = (int)pAddr / PAGE_SIZE;
    int entry = page / 8;
    int bit = page % 8;
    RtlDebugAssert((MmPfaBitmap[entry] & (1 << bit)) != 0, "Tried to reserve reserved physical page at 0x%x", pAddr);
    MmPfaBitmap[entry] &= ~(1 << bit);
    MmPfaFreePages--;
}

uint32_t MmGetFreePhysicalPages()
{
    return MmPfaFreePages;
}