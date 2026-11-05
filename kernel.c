#include <stdint.h>

// UART base address for QEMU virt
#define UART_BASE 0x09000000UL
// Data Register offset for writing characters
#define UART_DR (*(volatile uint32_t *)UART_BASE)

static void uart_putc(char c)
{
    UART_DR = (uint32_t)c;
}

static void uart_puts(const char *s)
{
    while (*s) {
        if (*s == '\n') {
            uart_putc('\r');
        }
        uart_putc(*s++);
    }
}

void kernel_main(void)
{
    uart_puts("Hello from my kernel!\n");
}