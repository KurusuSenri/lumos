#include <stdint.h>
#include "exception.h"
#include "drivers/gic/gic.h"
#include "drivers/uart/uart.h"

void kernel_main(void)
{
    uint64_t current_el;
    __asm__ volatile("mrs %0, CurrentEL" : "=r"(current_el));
    uart_print("Current EL: ");
    uart_putc('0' + ((current_el >> 2) & 3));
    uart_print("\n");

    exception_init();
    gic_init();
    uart_enable_rx_irq();

    uart_print("UART IRQ echo ready\n");

    __asm__ volatile(
        "dsb sy\n"
        "msr daifclr, #2\n"
        "isb\n"
        :
        :
        : "memory"
    );

    for (;;) {
        __asm__ volatile("wfi");
    }
}