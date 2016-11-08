#ifndef KERNEL_H
#define KERNEL_H

#include <platform.h>

// Utility macros to convert between physical and virtual addresses
#define P2V(x) ((x) + KERNEL_VIRTUAL_BASE)
#define V2P(x) ((x) - KERNEL_VIRTUAL_BASE)

#ifndef ASM_FILE
typedef enum
{
    Fail,
    Success
} Status;

void ArDebugPrint(char c);
#endif

#endif
