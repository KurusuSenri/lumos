#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

void serial_putc(char ch);
void serial_puts(const char *text);
void serial_put_hex64(uint64_t value);
void serial_put_uint64(uint64_t value);

#endif