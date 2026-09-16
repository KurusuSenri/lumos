# LUMOS: LUMOS' Ultra Minimal Operating System for AArch64

Work in progress.

## Original Project Brief

> ## Abstract
> 
> Investigate the fundamental mechanisms of operating systems and design and implement a small functioning operating-system environment.
>
> ## Description
> 
> The student will implement a limited operating system capable of **running simple user programs**. It is not intended that the student produce a fully developed Linux or Windows system, however, he/she should implement a **bootable kernal**, **interrupt handling**, **process management**, **basic memory management**, a **simple command shell**, and **other elements if time permits**. The OS should be capable of running user programs. The OS can be built on an emulator such as QEMU (A generic and open source machine emulator and virtualizer). Particularly ambituous students could attempt to run the OS directly on hardware such as the Rasberry Pi.

## Proposed Implementation

- **A bootable kernal**: A kernal that might be able to load by bootloader (e.g. UEFI) and executed.
- **Interrupt handling**: A basic interrupt handling that involves serial port input interrupt is essential. If time allows, timer interrupt and more types of interupts could be introduced. Exception should at least by caught, and if possible, for those exception that can be a recovered, recover them.
- **Basic memory management**: At a minimul, memory allocation should be implemented. Furture extensions could include memory deallocation, or use of certain data structues to track physical memroy pages.
- **Process management**: The system should be able to prepare stack for process, as well as switch to user mode and jump to entry. And on exiting, clean resources and return to kernel mode. The state of each process should also be tracked. If time interrupts are implemented, there can be a scheduler that will enable the two processes to run alternately.
- **Simple command shell**: Based on the serial input interrupt, the shell should be able to input some characters and run some commands.
- **Capability of running user programs**: A user program is a program that running in the user mode, as well as using system calls to achieve certain tasks. Basic system calls should be implemented first, such as print or memory allocation (or request for time, if timer interrupt is done).
- **Run the operating system on hardware such as Raspberry Pi**: Porting the OS to a specific Raspberry Pi model would require adapting the boot process and hardware dependent components, including serial port initialization and interrupt controller support. The kernel would also need to accommodate the target board’s memory map. Additional challenges may arise, which I will investigate and document as development progresses.
- **Other elements if time permits**: I would really like to implement a simple framebuffer based text console to display characters on screen, with support for line breaks and scrolling.

## Progress So Far

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

## NeXTSTEP:

- Interrupt handling and exception return
