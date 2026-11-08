#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdint.h>

struct exception_frame {
    // x0–x30 general registers. Floating-point registers are not used
    uint64_t x[31];  
    // Exception Link Register for the exception return address
    uint64_t elr;    
    // Saved Program Status Register for saved processor state before the exception
    uint64_t spsr;   
    // Exception Syndrome Register for the exception cause
    uint64_t esr;    
    // Fault Address Register for the address that caused the exception, valid only for applicable exceptions
    uint64_t far;    
    // Exception category set by the assembly entry code
    uint64_t kind;   
};

enum exception_kind {
    EXCEPTION_SYNC = 0,
    EXCEPTION_IRQ = 1,
    EXCEPTION_FIQ = 2,
    EXCEPTION_SERROR = 3,
};

void exception_init(void);
void exception_handler(struct exception_frame *frame);

#endif