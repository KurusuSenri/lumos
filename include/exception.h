#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdint.h>

void exceptions_init(void);

__attribute__((noreturn))
void exception_panic(uint64_t kind);

__attribute__((noreturn))
void exception_panic_unsupported(void);

#endif