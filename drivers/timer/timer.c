#include "timer.h"
#include "drivers/uart/uart.h"

// Control register bit 0 enables the timer
// Bit 1 (IMASK) stays 0 so the timer is allowed to assert its interrupt line
// UL is for unsigned long, to make compiler happy and not warning me about msr writing a 64 bit register with a 32 bit value
#define TIMER_CTL_ENABLE (1UL << 0)

// One period measured in counter ticks, computed once from CNTFRQ_EL0
static uint64_t timer_interval;

// How many times the timer has fired since boot
static volatile uint64_t timer_ticks_count;

// Arm the timer for one more period
// Writing TVAL sets CVAL = CNTPCT + TVAL internally, and TVAL counts down by itself
// It also lowers the interrupt line, because the comparator is no longer expired
static void timer_start_period(void)
{
    __asm__ volatile("msr cntp_tval_el0, %0" : : "r"(timer_interval));

    // ENABLE = 1, IMASK = 0
    __asm__ volatile("msr cntp_ctl_el0, %0" : : "r"(TIMER_CTL_ENABLE));
}

void timer_init(void)
{
    uint64_t frequency;

    // CNTFRQ_EL0 reports how many counter ticks happen per second
    // It is platform dependent, so it must be read instead of assumed
    __asm__ volatile("mrs %0, cntfrq_el0" : "=r"(frequency));

    uart_print("Platform timer frequency: ");
    uart_print_u64(frequency);
    uart_print("\n");

    // One period = frequency / TIMER_HZ counter ticks
    timer_interval = frequency / TIMER_HZ;

    timer_start_period();
}

void timer_handle_irq(void)
{
    timer_ticks_count++;

    // Re-arm before the caller writes EOIR
    timer_start_period();

    if (timer_ticks_count % TIMER_HZ == 0) {
        uart_print("tick: ");
        uart_print_u64(timer_ticks_count);
        uart_print("\n");
    }
}

uint64_t timer_ticks(void)
{
    return timer_ticks_count;
}