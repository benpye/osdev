#include <kernel.h>
#include <stdint.h>

#include <mm/pfa.h>
#include <rtl/debug.h>

uint8_t MmPfaBitmap[(PHYSICAL_MEMORY_SIZE/PAGE_SIZE)/8] = { 0 };
unsigned int MmPfaFreePages = 0;

pa_t MmAllocatePhysicalPage()
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
                    pa_t pAddr = ((i * 8) + j) * PAGE_SIZE;
                    MmReservePhysicalPage(pAddr);
                    return pAddr;
                }
            }
        }
    }

    return 0;
}

void MmFreePhysicalPage(pa_t pAddr)
{
    RtlDebugAssert((pAddr % PAGE_SIZE) == 0, "Tried to free non aligned page");
    RtlDebugAssert(pAddr < PHYSICAL_MEMORY_SIZE, "Tried to free page outside physical memory");
    int page = pAddr / PAGE_SIZE;
    int entry = page / 8;
    int bit = page % 8;
    RtlDebugAssert((MmPfaBitmap[entry] & (1 << bit)) == 0, "Tried to free free physical page at 0x%x", pAddr);
    MmPfaBitmap[entry] |= (1 << bit);
    MmPfaFreePages++;
}

void MmReservePhysicalPage(pa_t pAddr)
{
    RtlDebugAssert((pAddr % PAGE_SIZE) == 0, "Tried to reserve non aligned page");
    RtlDebugAssert(pAddr < PHYSICAL_MEMORY_SIZE, "Tried to reserve page outside physical memory");
    int page = pAddr / PAGE_SIZE;
    int entry = page / 8;
    int bit = page % 8;
    RtlDebugAssert((MmPfaBitmap[entry] & (1 << bit)) != 0, "Tried to reserve reserved physical page at 0x%x", pAddr);
    MmPfaBitmap[entry] &= ~(1 << bit);
    MmPfaFreePages--;
}

unsigned int MmGetFreePhysicalPages()
{
    return MmPfaFreePages;
}
