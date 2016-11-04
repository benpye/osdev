#ifndef ARCH_PLATFORM_H
#define ARCH_PLATFORM_H

#define STACK_SIZE 0x4000

#define VIRTUAL_BASE 0xC0000000
#define KERNEL_PAGE (VIRTUAL_BASE >> 22)

#define PAGE_SIZE 4096

#ifndef ASM_FILE
void ArDebugPrint(char c);
#endif

#endif