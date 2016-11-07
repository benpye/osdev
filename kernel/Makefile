ARCH = x86

QEMU = /mnt/c/Program\ Files/qemu/qemu-system-i386.exe
GDB = gdb
CC = clang
LD = ld

INCLUDES = -Iinclude -Iinclude/arch/$(ARCH)

CFLAGS = --target=i686-pc-none-elf -Wall -Werror -g -ffreestanding -std=gnu11 -nostdlib -nostdlibinc -nodefaultlibs $(INCLUDES)
LDFLAGS = -m elf_i386 -T arch/$(ARCH)/linker.ld -nostdlib -nostartfiles

KERNEL = boot.elf

OBJ_DIR = obj
SRC_DIR = .
BIN_DIR = bin

OBJ = $(OBJ_DIR)/arch/x86/entry.o \
      $(OBJ_DIR)/arch/x86/boot.o \
      $(OBJ_DIR)/arch/x86/mem.o \
      $(OBJ_DIR)/arch/x86/serial.o \
      $(OBJ_DIR)/rtl/debug.o \
      $(OBJ_DIR)/rtl/memory.o \
      $(OBJ_DIR)/mm/pfa.o

all: $(KERNEL)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.S
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(LD) $(LDFLAGS) $(OBJ) -o $(BIN_DIR)/$(KERNEL)

.PHONY : clean qemu gdb

qemu: $(KERNEL)
	$(QEMU) -kernel $(BIN_DIR)/$(KERNEL) -monitor stdio

gdb: $(KERNEL)
	$(QEMU) -kernel $(BIN_DIR)/$(KERNEL) -s -S &
	$(GDB) --command kernel.gdb

clean:
	-rm -rf $(BIN_DIR) $(OBJ_DIR)
