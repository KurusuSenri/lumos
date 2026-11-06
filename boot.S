.section .text.boot
.global _start

_start:
    msr daifset, #0xf   // Mask debug exceptions, SError, IRQ and FIQ during initialisation
    msr spsel, #1       // Switch to stack pointer for EL1

    ldr x0, =stack_top  // Defined by linker script
    mov sp, x0

    ldr x0, =__bss_start
    ldr x1, =__bss_end

clear_bss:
    cmp x0, x1
    b.hs bss_done

    str xzr, [x0], #8
    b clear_bss

bss_done:
    bl kernel_main

halt:
    wfe
    b halt