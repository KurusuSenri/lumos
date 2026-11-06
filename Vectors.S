.section .text.vectors, "ax"
.balign 2048
.global exception_vectors

// 128 bytes per vector
.macro vector target
    b \target
    .balign 128
.endm

exception_vectors:
    // Current EL using SP_EL0
    vector unsupported_entry
    vector unsupported_entry
    vector unsupported_entry
    vector unsupported_entry

    // Current EL using SP_EL1 (EL2 and EL3 are not supported)
    vector sync_entry
    vector irq_entry
    vector fiq_entry
    vector serror_entry

    // Lower EL AArch64 will be implemented in the future
    vector unsupported_entry
    vector unsupported_entry
    vector unsupported_entry
    vector unsupported_entry

    // Lower EL AArch32 not supported
    vector unsupported_entry
    vector unsupported_entry
    vector unsupported_entry
    vector unsupported_entry

unsupported_entry:
    msr daifset, #0xf
1:
    wfe
    b 1b

// Macro to define exception entry points for different exception types
.macro exception_entry name, kind
\name:
    sub sp, sp, #288

    stp x0,  x1,  [sp, #0]
    stp x2,  x3,  [sp, #16]
    stp x4,  x5,  [sp, #32]
    stp x6,  x7,  [sp, #48]
    stp x8,  x9,  [sp, #64]
    stp x10, x11, [sp, #80]
    stp x12, x13, [sp, #96]
    stp x14, x15, [sp, #112]
    stp x16, x17, [sp, #128]
    stp x18, x19, [sp, #144]
    stp x20, x21, [sp, #160]
    stp x22, x23, [sp, #176]
    stp x24, x25, [sp, #192]
    stp x26, x27, [sp, #208]
    stp x28, x29, [sp, #224]
    str x30,     [sp, #240]

    // The structure is defined in exception_frame in exception.h
    mrs x0, elr_el1
    mrs x1, spsr_el1
    stp x0, x1, [sp, #248]

    mrs x0, esr_el1
    mrs x1, far_el1
    stp x0, x1, [sp, #264]

    mov x0, #\kind
    str x0, [sp, #280]

    mov x0, sp
    bl exception_handler

    b exception_return
.endm

exception_entry sync_entry,   0
exception_entry irq_entry,    1
exception_entry fiq_entry,    2
exception_entry serror_entry, 3

exception_return:
    ldp x0, x1, [sp, #248]
    msr elr_el1, x0
    msr spsr_el1, x1
    // ESR_EL1 and FAR_EL1 are not restored because they only provide diagnostic information

    ldp x0,  x1,  [sp, #0]
    ldp x2,  x3,  [sp, #16]
    ldp x4,  x5,  [sp, #32]
    ldp x6,  x7,  [sp, #48]
    ldp x8,  x9,  [sp, #64]
    ldp x10, x11, [sp, #80]
    ldp x12, x13, [sp, #96]
    ldp x14, x15, [sp, #112]
    ldp x16, x17, [sp, #128]
    ldp x18, x19, [sp, #144]
    ldp x20, x21, [sp, #160]
    ldp x22, x23, [sp, #176]
    ldp x24, x25, [sp, #192]
    ldp x26, x27, [sp, #208]
    ldp x28, x29, [sp, #224]
    ldr x30,     [sp, #240]

    add sp, sp, #288
    eret