LLVM := $(shell brew --prefix llvm)/bin
CC := $(LLVM)/clang
LD := $(shell brew --prefix lld)/bin/ld.lld

# - Target AArch64 for bare-metal
# - Do not assume any standard library or runtime
# - Disable stack protector
# - Use only general-purpose registers
# - O2 optimisation, all warnings enabled
CFLAGS := --target=aarch64-none-elf \
          -ffreestanding \
          -fno-stack-protector \
          -mgeneral-regs-only \
          -O2 -Wall -Wextra

LDFLAGS := -T linker.ld -nostdlib

# Source files
C_SOURCES := kernel.c \
             exception.c \
             drivers/uart/uart.c

OBJECTS := boot.o vectors.o $(C_SOURCES:.c=.o)

all: kernel.elf

boot.o: boot.S
	$(CC) --target=aarch64-none-elf -c $< -o $@

vectors.o: vectors.S
	$(CC) --target=aarch64-none-elf -c $< -o $@

# Automatically generate dependency files for each source file
%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

kernel.elf: $(OBJECTS) linker.ld
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

# Automatically include dependency files if they exist
-include $(C_SOURCES:.c=.d)

clean:
	rm -f $(OBJECTS) $(C_SOURCES:.c=.d) kernel.elf

# - QEMU virt machine with GICv2, disable virtualization and secure mode to run in EL0/EL1
# - Cortex-A57 CPU
# - 512MB RAM
# - Single core
# - No graphical output, use terminal for I/O
# - Specify kernel.elf as the boot image
run: kernel.elf
	qemu-system-aarch64 \
		-M virt,gic-version=2,virtualization=off,secure=off \
		-cpu cortex-a57 \
		-m 512M \
		-smp 1 \
		-nographic \
		-kernel kernel.elf

.PHONY: all run clean