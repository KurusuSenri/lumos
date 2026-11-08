#ifndef UART_H
#define UART_H

#include <stdint.h>

void uart_putc(char c);
void uart_print(const char *s);
void uart_print_hex(uint64_t value);
void uart_print_u64(uint64_t value);
void uart_enable_rx_irq(void);
void uart_handle_irq(void);

#endif