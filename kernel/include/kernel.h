#ifndef KERNEL_H
#define KERNEL_H

#include <platform.h>

// Utility macros to convert between physical and virtual addresses
// Have the downside of losing the type of pointer
#define PHYS_TO_VIRT(x) (x + VIRTUAL_BASE)
#define VIRT_TO_PHYS(x) (x - VIRTUAL_BASE)

#ifndef ASM_FILE
#define PA_TO_VA(addr) ((void *) (PHYS_TO_VIRT((int)addr)))
#define VA_TO_PA(addr) ((void *) (VIRT_TO_PHYS((int)addr)))

typedef enum
{
    Success,
    Fail
} Status;
#endif

#endif