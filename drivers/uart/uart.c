#include <stdint.h>
#include "uart.h"

// QEMU virt: PL011 UART 
#define UART_BASE 0x09000000UL

// dara register offset for writing characters
#define UART_DR (*(volatile uint32_t *)(UART_BASE + 0x00))
// data register offset for reading current status
#define UART_FR (*(volatile uint32_t *)(UART_BASE + 0x18))

// bit indicating that the transmit FIFO is full
#define UART_FR_TXFF (1U << 5)
// bit indicating that the UART is busy transmitting data
#define UART_FR_BUSY (1U << 3)

void uart_putc(char c)
{
    while (UART_FR & UART_FR_TXFF) {
    }

    UART_DR = (uint32_t)(unsigned char)c;
}

void uart_print(const char *s)
{
    while (*s) {
        if (*s == '\n') {
            uart_putc('\r');
        }

        uart_putc(*s++);
    }
}