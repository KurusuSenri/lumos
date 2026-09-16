#include <stdint.h>
#include "../include/boot_info.h"
#include "../include/serial.h"
#include "../include/debug.h"
#include "../include/memory.h"
#include "../include/memory_map.h"
#include "../include/string.h"
#include "../include/exception.h"
#include "../tests/tests.h"

__attribute__((noreturn))
void kernel_halt(void)
{
    for (;;) {
        serial_puts("Kernel halting...\r\n");
        __asm__ volatile("wfi" ::: "memory");
    }
}

__attribute__((noreturn))   // Will never return to the caller
void kernel_main(const BootInfo *boot_info)
{
    /*
     * Initialize exception handling
     */
    exceptions_init();
    serial_puts("EL1 exception vectors installed.\r\n");
    debug_print_el();

    /*
     * Print the boot information, stack pointer, and memory map
     */
    if (!memory_map_validate(boot_info)) {
        serial_puts("Invalid boot memory map. Halting.\r\n");
        kernel_halt();
    }
    debug_print_boot_info(boot_info);
    debug_print_sp((uintptr_t)__builtin_frame_address(0));
    debug_print_memory_map(boot_info);

    /*
     * Initialize the page allocator with the largest usable memory region.
     */
    uint64_t best_start = 0;
    uint64_t best_pages = 0;
    
    find_largest_memory_region(boot_info, &best_start, &best_pages);
    page_allocator_init(best_start, best_pages);

    serial_puts("Largest usable memory region: start=");
    serial_put_hex64(best_start);
    serial_puts(" pages=");
    serial_put_uint64(best_pages);
    serial_puts("\r\n");

    /*
     * Run tests
     */
    test_all();

    kernel_halt();
}