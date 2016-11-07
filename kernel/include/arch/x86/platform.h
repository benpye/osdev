#ifndef ARCH_PLATFORM_H
#define ARCH_PLATFORM_H

#define KERNEL_STACK_SIZE 0x4000

#define KERNEL_VIRTUAL_BASE 0xC0000000
#define KERNEL_PAGE (KERNEL_VIRTUAL_BASE >> 22)

#define PAGE_SIZE 4096
#define PHYSICAL_MEMORY_SIZE 0x100000000

#ifndef ASM_FILE
static inline void ArDebugHalt()
{
     asm volatile ( "cli\nhlt" : : );
}
#endif

#endif