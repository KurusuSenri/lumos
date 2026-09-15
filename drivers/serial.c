#include "../include/serial.h"

/*
 * These addresses are based on the QEMU virt machine's memory map. 
 * The UART is a PL011 device, which is a common UART used in ARM systems.
 * uintptr_t is a type, capable of holding a pointer
 * uint32_t is a 32-bit unsigned integer type, used fot the UART registers
*/
#define UART_BASE ((uintptr_t)0x09000000)   // Base address of the UART
#define UART_DR ((volatile uint32_t *)(UART_BASE + 0x000))  // Data Register: read/write data to the UART
#define UART_FR ((volatile uint32_t *)(UART_BASE + 0x018UL))  // Flag Register: contains status flags for the UART
#define UART_FR_TXFF (1U << 5)          // Masking bit for the Transmit FIFO Full flag
#define UART_FR_BUSY (1U << 3)          // Masking bit for the UART Busy flag

static uint32_t uart_read(volatile uint32_t *reg)
{
    return *reg;
}
 
static void uart_write(volatile uint32_t *reg, uint32_t value)
{
    *reg = value;
}

/*
 * Write a character to the UART
 */
void serial_putc(char ch)
{
    /* 
     * Wait until the transmit FIFO has space
     * Mask the FIFO Full flag to check if the FIFO is full
     */
    while ((uart_read(UART_FR) & UART_FR_TXFF) != 0) {
    }

    uart_write(UART_DR, (uint32_t)(uint8_t)ch);
}

/*
 * Write a string to the UART
 */
void serial_puts(const char *text)
{
    while (*text != '\0') {
        serial_putc(*text++);
    }
}

/*
 * Write a 64-bit hexadecimal number to the UART
 */
void serial_put_hex64(uint64_t value)
{
    static const char digits[] = "0123456789abcdef";

    serial_puts("0x");

    /*
     * Shift the value right by 60 bits to get the most significant 4 bits
     * Mask with 0xf to get the last 4 bits
     * Use that 4 bits as an index into the digits array to get the corresponding hex character
     * Repeat for each 4 bits, shifting right by 4 bits each time
     */
    for (int shift = 60; shift >= 0; shift -= 4) {
        serial_putc(digits[(value >> shift) & 0xf]);
    }
}

void serial_put_uint64(uint64_t value)
{
    char buffer[21]; // Enough to hold the maximum value of a 64-bit unsigned integer and a null terminator
    int index = 20;  // Start from the end of the buffer
    buffer[index] = '\0'; // Null-terminate the string

    if (value == 0) {
        buffer[--index] = '0'; // Handle the case for zero
    } else {
        while (value > 0) {
            buffer[--index] = '0' + (value % 10); // Get the last digit and convert to character
            value /= 10; // Remove the last digit
        }
    }

    serial_puts(&buffer[index]); // Print the resulting string
}