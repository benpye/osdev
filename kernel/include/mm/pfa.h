#ifndef MM_PFA_H
#define MM_PFA_H

pa_t MmAllocatePhysicalPage();
void MmFreePhysicalPage(pa_t pAddr);
void MmReservePhysicalPage(pa_t pAddr);
unsigned int MmGetFreePhysicalPages();

#endif
