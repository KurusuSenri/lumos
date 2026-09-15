#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include "boot_info.h"

/* Return 1 if a usable region was found, otherwise 0. */
int find_largest_memory_region(const BootInfo *info,
                               uint64_t *best_start,
                               uint64_t *best_pages);

void page_allocator_init(uint64_t start, uint64_t pages);
int alloc_page(uint64_t *physical_address);

#endif