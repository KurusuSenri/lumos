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

all: kernel.elf

boot.o: boot.S
	$(CC) --target=aarch64-none-elf -c $< -o $@

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel.elf: boot.o kernel.o linker.ld
	$(LD) $(LDFLAGS) boot.o kernel.o -o $@

# - QEMU virt machine with GICv2
# - Cortex-A57 CPU, 512MB RAM, 1 core
# - No graphical output, use terminal for I/O
# - Specify kernel.elf as the boot image
run: kernel.elf
	qemu-system-aarch64 \
		-M virt,gic-version=2 \
		-cpu cortex-a57 \
		-m 512M \
		-smp 1 \
		-nographic \
		-kernel kernel.elf

clean:
	rm -f *.o *.elf

.PHONY: all run clean