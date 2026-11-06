#ifndef UART_H
#define UART_H

void uart_putc(char c);
void uart_print(const char *s);
void uart_print_hex(uint64_t value);
void uart_print_u64(uint64_t value);

#endif