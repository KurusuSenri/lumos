#include "exception.h"
#include "drivers/uart/uart.h"

extern char exception_vectors[];
static void exception_dump(const struct exception_frame *frame);

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

    uart_print("=== Caught EL1 exception ===\n");
    exception_dump(frame);

    // BRK just return to next instruction
    if (frame->kind == EXCEPTION_SYNC && ec == 0x3c) {
        uart_print("Exception BRK\n");

        // The ELR of BRK points to the BRK instruction itself
        // So needed to skip it to continue execution
        frame->elr += 4;

        // Return to Vector.S which will branch to exception_return
        uart_print("=== Return to next instruction ===\n");
        return;
    } 

    // Unhandled exception, halt the system
    uart_print("Unhandled EL1 exception\n");
    for (;;) {
        __asm__ volatile("wfe");
    }
}

static void exception_dump(const struct exception_frame *frame)
{
    uart_print("kind: ");
    uart_print_u64(frame->kind);
    uart_print("\n");

    uart_print("ESR:  ");
    uart_print_hex(frame->esr);
    uart_print("\n");

    uart_print("EC:   ");
    uart_print_hex((frame->esr >> 26) & 0x3f);
    uart_print("\n");

    uart_print("ELR:  ");
    uart_print_hex(frame->elr);
    uart_print("\n");

    uart_print("SPSR: ");
    uart_print_hex(frame->spsr);
    uart_print("\n");

    uart_print("FAR:  ");
    uart_print_hex(frame->far);
    uart_print("\n");
}