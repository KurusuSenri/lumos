#include <stdint.h>
#include "exception.h"
#include "drivers/gic/gic.h"
#include "drivers/uart/uart.h"
#include "drivers/timer/timer.h"

static void print_el(void);

void kernel_main(void)
{
    // Print the current exception level (EL) to the UART
    print_el();

    // Vector table first, so a fault during the rest of init has somewhere to go
    exception_init();
    // Enable the interrupt sources in the GIC: UART (SPI 33) and timer (PPI 30)
    gic_init();

    // Program and start the timer
    // IRQs are still masked here, so the first tick is only delivered after daifclr below
    timer_init();

    // Enable the UART RX interrupt, so that the character typed is echoed back
    uart_enable_rx_irq();

    uart_print("UART and Timer IRQ ready\n");

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

static void print_el(){
    uint64_t current_el;
    __asm__ volatile("mrs %0, CurrentEL" : "=r"(current_el));
    uart_print("Current EL: ");
    uart_putc('0' + ((current_el >> 2) & 3));
    uart_print("\n");
}