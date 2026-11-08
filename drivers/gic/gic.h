#ifndef GIC_H
#define GIC_H

#include <stdint.h>

// SPI interrupt ID for the PL011 UART
#define UART_IRQ_ID 33U

void gic_init(void);
uint32_t gic_acknowledge(void);
void gic_end_interrupt(uint32_t acknowledge);

#endif