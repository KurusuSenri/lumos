Progress so far:

- Built a minimal AArch64 UEFI bootloader
- Retrieved the UEFI memory map and exited Boot Services
- Transferred control and boot information from the bootloader to the kernel
- Implemented debug output through the PL011 UART using MMIO
- Parsed the UEFI memory map and printed memory region types, addresses, and sizes
- Separated serial output and debug utilities from the kernel entry code

NeXTSTEP:

- Manage memory