#include <stdint.h>
#include "drivers/uart/uart.h"

void kernel_main(void)
{
    uint64_t current_el;
    __asm__ volatile("mrs %0, CurrentEL" : "=r"(current_el));
    uart_print("Current EL: ");
    uart_putc('0' + ((current_el >> 2) & 3));
    uart_print("\n");
}