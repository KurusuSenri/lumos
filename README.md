Progress so far:

- Built a minimal AArch64 UEFI bootloader
- Retrieved the UEFI memory map and exited Boot Services
- Transferred control and boot information from the bootloader to the kernel
- Implemented debug output through the PL011 UART using MMIO
- Parsed the UEFI memory map and printed memory region types, addresses, and sizes
- Separated serial output and debug utilities from the kernel entry code
- Added basic validation of the UEFI memory map
- Selected the largest available memory region
- Implemented a minimal bump allocator for allocating 4 KiB physical pages
- Implemented a basic memcpy function
- Set up the EL1 exception vector table and a shared handler
- Print diagnostic information and halt on exceptions

NeXTSTEP:

- Interrupt handling and exception return
