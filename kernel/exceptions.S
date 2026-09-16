.text
// Align the following code to 2^2 = 4 bytes, which is the size of an instruction on AArch64.
.p2align 2

// global is a directive that makes the symbol visible to the linker and other files.
.global exceptions_init
exceptions_init:
    // Keep exceptions masked during initialization.
    // msr is an instruction that writes to a system register.
    msr daifset, #0xf

    // Keep the existing stack, but select SP_EL1.
    mov x9, sp
    msr spsel, #1
    mov sp, x9

    // Install the EL1 vector table.
    // adr is a pseudo-instruction that loads the address of the label into a register.
    adr x9, exception_vectors
    // Write the address of the vector table to the VBAR_EL1 register.
    msr vbar_el1, x9
    // Instruction Synchronization Barrier
    // Ensure that the vector table is visible to the processor.
    isb
    // Return to the caller.
    ret


// Each vector slot occupies exactly 128 bytes.
// target is the label of the exception handler for that vector.
.macro VECTOR target
    // branch to the exception handler.
    b \target
    // b \target is 4 bytes, each entery is 128 bytes
    // 128 - 4 = 124, so we need 124 more bytes of NOPs.
    .space 124
.endm


// The vector table must be aligned to 2^11 = 2048 bytes.
.p2align 11
// The vector table has four groups in the following order:
// 1. Current EL using SP_EL0.
// 2. Current EL using SP_EL1.
// 3. Lower EL executing in AArch64 state.
// 4. Lower EL executing in AArch32 state.
//
// Each group contains four entries in this order:
// Synchronous, IRQ, FIQ, SError.
exception_vectors:

    // Current EL using SP_EL0.
    VECTOR sync_entry
    VECTOR irq_entry
    VECTOR fiq_entry
    VECTOR serror_entry

    // Current EL using SP_EL1.
    VECTOR sync_entry
    VECTOR irq_entry
    VECTOR fiq_entry
    VECTOR serror_entry

    // Lower EL: unsupported for now.
    VECTOR unsupported_entry
    VECTOR unsupported_entry
    VECTOR unsupported_entry
    VECTOR unsupported_entry

    VECTOR unsupported_entry
    VECTOR unsupported_entry
    VECTOR unsupported_entry
    VECTOR unsupported_entry

sync_entry:
    // x0 is the first argument to the exception_panic function, which indicates the type of exception.
    mov x0, #0
    b exception_panic

irq_entry:
    mov x0, #1
    b exception_panic

fiq_entry:
    mov x0, #2
    b exception_panic

serror_entry:
    mov x0, #3
    b exception_panic

unsupported_entry:
    b exception_panic_unsupported