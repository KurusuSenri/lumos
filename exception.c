#include "exception.h"
#include "drivers/uart/uart.h"

extern char exception_vectors[];

void exception_init(void)
{
    __asm__ volatile(
        "msr vbar_el1, %0\n"
        "isb\n"
        :
        : "r"(exception_vectors)
        : "memory"
    );
}

void exception_handler(struct exception_frame *frame)
{
    // The exception class (EC) is stored in bits [31:26] of ESR_EL1
    uint64_t ec = (frame->esr >> 26) & 0x3f;

    uart_print("Caught EL1 exception\n");

    if (frame->kind == EXCEPTION_SYNC &&
        ec == 0x3c &&
        (frame->esr & 0xffff) == 0) {
        uart_print("Exception BRK\n");

        // The ELR of BRK points to the BRK instruction itself
        // So needed to skip it to continue execution
        frame->elr += 4;

        // Return to Vector.S which will branch to exception_return
        return;
    } else {
        uart_print("Unknown exception\n");
    }

    for (;;) {
        __asm__ volatile("wfe");
    }
}