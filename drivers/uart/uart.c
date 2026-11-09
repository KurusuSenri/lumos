#include "uart.h"

// QEMU virt: PL011 UART 
#define UART_BASE 0x09000000UL

// Data Register: read received characters or write characters to transmit
#define UART_DR (*(volatile uint32_t *)(UART_BASE + 0x00))

// Flag Register: read the current UART status
#define UART_FR (*(volatile uint32_t *)(UART_BASE + 0x18))
// Transmit FIFO full, or transmit holding register full when FIFO is disabled
#define UART_FR_TXFF (1U << 5)
// Bit indicating that the UART is busy transmitting data
#define UART_FR_BUSY (1U << 3)
// Receive FIFO empty, or receive holding register empty when FIFO is disabled
#define UART_FR_RXFE (1U << 4)

// Line Control Register: configure word length, parity, stop bits and FIFO mode
#define UART_LCR_H (*(volatile uint32_t *)(UART_BASE + 0x2c))
// Word Length 8: set bits [6:5] to 11 to select 8 data bits per character
#define UART_LCR_H_WLEN_8 (0b11U << 5)

// Control Register: enable or disable the UART, transmitter and receiver
#define UART_CR (*(volatile uint32_t *)(UART_BASE + 0x30))
// Bit enabling the UART when set
#define UART_CR_UARTEN (1U << 0)
// Bit enabling the transmitter when set
#define UART_CR_TXE (1U << 8)
// Bit enabling the receiver when set
#define UART_CR_RXE (1U << 9)

// Interrupt Mask Set/Clear Register: select which UART interrupt sources are enabled
#define UART_IMSC (*(volatile uint32_t *)(UART_BASE + 0x38))
// Receive Interrupt Mask: write 1 to allow receive interrupts
#define UART_IMSC_RXIM (1U << 4)

// Interrupt Clear Register: write 1 to clear the corresponding interrupt status
#define UART_ICR (*(volatile uint32_t *)(UART_BASE + 0x44))
// Receive Interrupt Clear: write 1 to clear the receive interrupt status
#define UART_ICR_RXIC (1U << 4)

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

void uart_print_hex(uint64_t value)
{
    static const char digits[] = "0123456789abcdef";

    uart_print("0x");

    for (int shift = 60; shift >= 0; shift -= 4) {
        unsigned int digit = (value >> shift) & 0xf;
        uart_putc(digits[digit]);
    }
}

void uart_print_u64(uint64_t value)
{
    // UINT64_MAX has 20 decimal digits.
    char buffer[20];
    unsigned int count = 0;

    do {
        buffer[count++] = '0' + (value % 10);
        value /= 10;
    } while (value != 0);

    while (count != 0) {
        uart_putc(buffer[--count]);
    }
}

void uart_enable_rx_irq(void)
{
    // Disable all UART interrupts before configuration
    UART_IMSC = 0;

    // Wait for earlier output to finish before reconfiguration
    while (UART_FR & UART_FR_BUSY) {
    }

    // Disable UART receiving and transmitting
    UART_CR = 0;

    // Clear all UART interrupt status bits
    UART_ICR = 0x7ff;

    // 8 data bits; FIFO ENABLE remains zero (single-character mode)
    UART_LCR_H = UART_LCR_H_WLEN_8;

    // Enable UART, transmitter and receiver
    UART_CR = UART_CR_UARTEN | UART_CR_TXE | UART_CR_RXE;

    // Enable receive interrupt
    UART_IMSC = UART_IMSC_RXIM;
}

void uart_handle_irq(void)
{
    // Clear the old RX indication before draining received data
    UART_ICR = UART_ICR_RXIC;

    while (!(UART_FR & UART_FR_RXFE)) {
        uint32_t hex = UART_DR & 0xff;
        uart_print_hex(hex);
        uart_print(" ");

        char c = (char)(UART_DR & 0xff);

        if (c == '\r' || c == '\n') {
            uart_print("\n");
        } else {
            uart_putc(c);
        }
    }
}
