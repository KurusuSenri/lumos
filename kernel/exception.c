#include "../include/exception.h"
#include "../include/serial.h"

__attribute__((noreturn))
void exception_panic(uint64_t kind)
{
    // Temporarily mask all exceptions to prevent further exceptions from occurring while we are handling this one.
    __asm__ volatile("msr daifset, #0xf" ::: "memory");

    uint64_t esr;   // Exception Syndrome Register
    uint64_t elr;   // Exception Link Register

    // mrs instruction reads the value of a system register into a general-purpose register.
    __asm__ volatile("mrs %0, esr_el1" : "=r"(esr));
    __asm__ volatile("mrs %0, elr_el1" : "=r"(elr));

    serial_puts("\r\n=== EL1 exception ===\r\nType: ");

    switch (kind) {
    case 0:
        serial_puts("Synchronous");
        break;
    case 1:
        serial_puts("IRQ");
        break;
    case 2:
        serial_puts("FIQ");
        break;
    case 3:
        serial_puts("SError");
        break;
    default:
        serial_puts("Unknown");
        break;
    }

    serial_puts("\r\nELR_EL1: ");
    serial_put_hex64(elr);

    // IRQ and FIQ do not provide an ESR syndrome.
    if (kind == 0 || kind == 3) {
        serial_puts("\r\nESR_EL1: ");
        serial_put_hex64(esr);
    }

    serial_puts("\r\nKernel stopped.\r\n");

    for (;;) {
        __asm__ volatile("wfi" ::: "memory");
    }
}

__attribute__((noreturn))
void exception_panic_unsupported()
{
    // Mask all exceptions to prevent further exceptions from occurring while we are handling this one.
    __asm__ volatile("msr daifset, #0xf" ::: "memory");
    serial_puts("\r\n=== Unsupported Exception ===\r\n");
    serial_puts("This exception might be level 2 or 3.\r\n");
    serial_puts("\r\nKernel stopped.\r\n");

    for (;;) {
        __asm__ volatile("wfi" ::: "memory");
    }
}