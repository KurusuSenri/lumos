#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H

#include <stdint.h>
#include "boot_info.h"

#define MEMORY_PAGE_SIZE 4096ULL
#define MEMORY_TYPE_CONVENTIONAL 7U

typedef struct {
    uint32_t type;
    uint32_t padding;
    uint64_t physical_start;
    uint64_t virtual_start;
    uint64_t pages;
    uint64_t attributes;
} MemoryDescriptor;

_Static_assert(sizeof(MemoryDescriptor) == 40,
               "Unexpected memory descriptor layout");

int memory_map_validate(const BootInfo *info);

uint64_t memory_map_count(const BootInfo *info);

/* Requires a validated map and an index within the map. */
const MemoryDescriptor *memory_map_get(const BootInfo *info,
                                       uint64_t index);

#endif