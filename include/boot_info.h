#ifndef BOOT_INFO_H
#define BOOT_INFO_H

#include <stdint.h>

typedef struct {
    const void *memory_map;
    uint64_t memory_map_size;
    uint64_t descriptor_size;
    uint32_t descriptor_version;
} BootInfo;

__attribute__((noreturn))
void kernel_main(const BootInfo *boot_info);

#endif