#ifndef MM_PFA_H
#define MM_PFA_H

void *MmAllocatePhysicalPage();
void MmFreePhysicalPage(void *pAddr);
void MmReservePhysicalPage(void *pAddr);
uint32_t MmGetFreePhysicalPages();

#endif