#define ASM_FILE

#include <kernel.h>
#include "arch.h"

#include "multiboot.h"

#define MB_MAGIC MULTIBOOT_HEADER_MAGIC

#define MB_FLAGS (MULTIBOOT_INFO_MEMORY | MULTIBOOT_INFO_CMDLINE \
                | MULTIBOOT_INFO_MEM_MAP | MULTIBOOT_INFO_BOOT_LOADER_NAME)

#define MB_CHECKSUM (-(MB_MAGIC + MB_FLAGS))

.section .bss
.align 16
stack:
.skip KERNEL_STACK_SIZE

.section .data
.align 4096
.global MmBootPageDirectory
MmBootPageDirectory:
    .long (PD_P | PD_RW | PD_PS)
    .fill (KERNEL_PAGE - 1), 4, 0
    .long (PD_P | PD_RW | PD_PS)
    .fill (1024 - KERNEL_PAGE - 1), 4, 0

.section .mb_header
.align 4
MB_HEADER:
    .long MB_MAGIC
    .long MB_FLAGS
    .long MB_CHECKSUM

.section .text
.equ MultibootEntry, V2P(_MultibootEntry)
.global MultibootEntry
.type MultibootEntry, @function
_MultibootEntry:
    mov $(V2P(MmBootPageDirectory)), %ecx
    mov %ecx, %cr3

    mov %cr4, %ecx
    orl $CR4_PSE, %ecx
    mov %ecx, %cr4

    mov %cr0, %ecx
    orl $CR0_PG, %ecx
    mov %ecx, %cr0

    lea HigherHalf, %ecx
    jmp *%ecx

HigherHalf:
    movl $0, MmBootPageDirectory
    invlpg 0

    mov $(stack + KERNEL_STACK_SIZE), %esp

    push %ebx
    push %eax

    call ArStart
