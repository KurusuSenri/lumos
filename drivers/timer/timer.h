#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

// Number of timer interrupts per second. 100 Hz means one tick every 10 ms
#define TIMER_HZ 100U

void timer_init(void);
void timer_handle_irq(void);
uint64_t timer_ticks(void);

#endif