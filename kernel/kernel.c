#include <stdint.h>
#include "../include/boot_info.h"
#include "../include/serial.h"
#include "../include/debug.h"

__attribute__((noreturn))   // Will never return to the caller
void kernel_main(const BootInfo *boot_info)
{
    /*
     * We do not yet have our own exception vectors
     * Mask debug, SError, IRQ and FIQ exceptions
     */
    __asm__ volatile("msr daifset, #0xf" ::: "memory");

    serial_puts("Hello World from kernel!\r\n");

    debug_print_boot_info(boot_info);

    debug_print_sp((uintptr_t)__builtin_frame_address(0));

    debug_print_memory_map(boot_info);

    for (;;) {
        serial_puts("Halting CPU...\r\n");
        __asm__ volatile("wfi" ::: "memory");
    }
}