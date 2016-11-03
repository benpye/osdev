#ifndef ARCH_H
#define ARCH_H

// Comments reference the Intel Manual, taken from Volume 3A, Section 2.5

// Protection Enable
#define CR0_PE (1 << 0)
// Monitor Coprocessor
#define CR0_MP (1 << 1)
// Emulation
#define CR0_EM (1 << 2)
// Task Switched
#define CR0_TS (1 << 3)
// Extension Type
#define CR0_ET (1 << 4)
// Numeric Error
#define CR0_NE (1 << 5)
// Write Protect
#define CR0_WP (1 << 16)
// Alignment Mask
#define CR0_AM (1 << 18)
// Not Write-Through
#define CR0_NW (1 << 29)
// Cache Disable
#define CR0_CD (1 << 30)
// Paging
#define CR0_PG (1 << 31)

// Virtual-8086 Mode Extensions
#define CR4_VME        (1 << 0)
// Protected-Mode Virtual Interrupts
#define CR4_PVI        (1 << 1)
// Time Stamp Disable
#define CR4_TSD        (1 << 2)
// Debugging Extensions
#define CR4_DE         (1 << 3)
// Page Size Extensions
#define CR4_PSE        (1 << 4)
// Physical Address Extension
#define CR4_PAE        (1 << 5)
// Machine-Check Enable
#define CR4_MCE        (1 << 6)
// Page Global Enable
#define CR4_PGE        (1 << 7)
// Performance-Monitoring Counter Enable
#define CR4_PCE        (1 << 8)
// Operating System Support for FXSAVE and FXRSTOR instructions
#define CR4_OSFXSR     (1 << 9)
// Operating System Support for Unmasked SIMD Floating-Point Exceptions
#define CR4_OSXMMEXCPT (1 << 10)
// User-Mode Instruction Prevention
#define CR4_UMIP       (1 << 11)
// VMX-Enable Bit
#define CR4_VMXE       (1 << 13)
// SMX-Enable Bit
#define CR4_SMXE       (1 << 14)
// FSGSBASE-Enable Bit
#define CR4_FSGSBASE   (1 << 16)
// PCID-Enable Bit
#define CR4_PCIDE      (1 << 17)
// XSAVE and Processor Extended States-Enable Bit
#define CR4_OSXSAVE    (1 << 18)
// SMEP-Enable Bit
#define CR4_SMEP       (1 << 20)
// SMAP-Enable Bit
#define CR4_SMAP       (1 << 21)
// Protection-Key-Enable Bit
#define CR4_PKE        (1 << 22)

// Comments reference the Intel Manual, taken from Volume 3A, Section 4.3

// CR3 for 32-bit paging
// Page-level write-through; indirectly determines the memory type used to access the page directory during linear-address translation (see Section 4.9)
#define CR3_PWT (1 << 3)
// Page-level cache disable; indirectly determines the memory type used to access the page directory during linear-address translation (see Section 4.9)
#define CR3_PCD (1 << 4)

// Page directory entry for 4-MByte pages/page tables
// Present; must be 1 to map a 4-MByte page/page table
#define PD_P   (1 << 0)
// Read/write; if 0, writes may not be allowed to the 4-MByte region controlled by this entry (see Section 4.6)
#define PD_RW  (1 << 1)
// User/supervisor; if 0, user-mode accesses are not allowed to the 4-MByte region controlled by this entry (see Section 4.6)
#define PD_US  (1 << 2)
// Page-level write-through; indirectly determines the memory type used to access the page table referenced by this entry (see Section 4.9)
#define PD_PWT (1 << 3)
// Page-level cache disable; indirectly determines the memory type used to access the page table referenced by this entry (see Section 4.9)
#define PD_PCD (1 << 4)
// Accessed; indicates whether this entry has been used for linear-address translation (see Section 4.8)
#define PD_A   (1 << 5)
// 4MB ONLY: Dirty; indicates whether software has written to the 4-MByte page referenced by this entry (see Section 4.8)
#define PD_D   (1 << 6)
// Page size; must be 1 to map a 4-MByte page, otherwise maps a page table if CR4.PSE = 1
#define PD_PS  (1 << 7)
// 4MB ONLY: Global; if CR4.PGE = 1, determines whether the translation is global (see Section 4.10); ignored otherwise
#define PD_G   (1 << 8)
// 4MB ONLY: If the PAT is supported, indirectly determines the memory type used to access the 4-MByte page referenced by this entry (see Section 4.9.2); otherwise, reserved (must be 0)
#define PD_PAT (1 << 12)

// Page table entry for 4-KByte pages
// Present; must be 1 to map a 4-KByte page
#define PT_P   (1 << 0)
// Read/write; if 0, writes may not be allowed to the 4-KByte page referenced by this entry (see Section 4.6)
#define PT_RW  (1 << 1)
// User/supervisor; if 0, user-mode accesses are not allowed to the 4-KByte page referenced by this entry (see Section 4.6)
#define PT_US  (1 << 2)
// Page-level write-through; indirectly determines the memory type used to access the 4-KByte page referenced by this entry (see Section 4.9)
#define PT_PWT (1 << 3)
// Page-level cache disable; indirectly determines the memory type used to access the 4-KByte page referenced by this entry (see Section 4.9)
#define PT_PCD (1 << 4)
// Accessed; indicates whether software has accessed the 4-KByte page referenced by this entry (see Section 4.8)
#define PT_A   (1 << 5)
// Dirty; indicates whether software has written to the 4-KByte page referenced by this entry (see Section 4.8)
#define PT_D   (1 << 6)
// If the PAT is supported, indirectly determines the memory type used to access the 4-KByte page referenced by this entry (see Section 4.9.2); otherwise, reserved (must be 0)
#define PT_PAT (1 << 7)
// Global; if CR4.PGE = 1, determines whether the translation is global (see Section 4.10); ignored otherwise
#define PT_G   (1 << 8)

#endif