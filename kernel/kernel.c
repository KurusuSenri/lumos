#include <stdint.h>
#include "../include/boot_info.h"
#include "../include/serial.h"
#include "../include/debug.h"
#include "../include/memory.h"
#include "../include/memory_map.h"
#include "../include/string.h"

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
     * We do not yet have our own exception vectors
     * Mask debug, SError, IRQ and FIQ exceptions
     */
    __asm__ volatile("msr daifset, #0xf" ::: "memory");

    serial_puts("Hello World from kernel!\r\n");

    if (!memory_map_validate(boot_info)) {
        serial_puts("Invalid boot memory map. Halting.\r\n");
        kernel_halt();
    }

    /*
     * Print the boot information, stack pointer, and memory map
     */
    debug_print_boot_info(boot_info);
    debug_print_sp((uintptr_t)__builtin_frame_address(0));
    debug_print_memory_map(boot_info);

    /*
     * Find the largest usable memory region
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
     * try to allocate a page
     */
    uint64_t a, b;

    if (alloc_page(&a) && alloc_page(&b)) {
        volatile uint64_t *pa = (volatile uint64_t *)(uintptr_t)a;
        volatile uint64_t *pb = (volatile uint64_t *)(uintptr_t)b;

        *pa = 123;
        *pb = 456;

        serial_puts("Page A: ");
        serial_put_hex64(a);
        serial_puts(" value: ");
        serial_put_uint64(*pa);

        serial_puts("\r\nPage B: ");
        serial_put_hex64(b);
        serial_puts(" value: ");
        serial_put_uint64(*pb);
        serial_puts("\r\n");

        memcpy((void *)(uintptr_t)b, (const void *)(uintptr_t)a, sizeof(uint64_t));
        serial_puts("After memcpy, Page B value: ");
        serial_put_uint64(*pb);
        serial_puts("\r\n");
    }

    kernel_halt();
}