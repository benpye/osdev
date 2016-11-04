#include <kernel.h>

#include "mem.h"

extern PageTable *MmPageDirectory;

void MmMapKernelPage(void *pAddr, void *vAddr)
{

}

Status MmUnmapKernelPage(void *vAddr)
{
    return Fail;
}