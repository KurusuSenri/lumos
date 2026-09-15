#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>
#include "boot_info.h"

void debug_print_boot_info(const BootInfo *info);
void debug_print_sp(uintptr_t sp);
void debug_print_memory_map(const BootInfo *info);

#endif