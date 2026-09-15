#include "../include/memory_map.h"

int memory_map_validate(const BootInfo *info)
{
    return info != 0 &&     // Check null pointer
           info->memory_map != 0 && // Check null pointer
           info->descriptor_version == 1 && // Check descriptor version
           info->descriptor_size >= sizeof(MemoryDescriptor) && // Check descriptor size
           (uintptr_t)info->memory_map %
               _Alignof(MemoryDescriptor) == 0 &&   // Check alignment of memory map
           info->descriptor_size % _Alignof(MemoryDescriptor) == 0 &&   // Check alignment of descriptor size
           info->memory_map_size % info->descriptor_size == 0;  // Check that memory map size is a multiple of descriptor size
}

uint64_t memory_map_count(const BootInfo *info)
{
    return info->memory_map_size / info->descriptor_size;
}

const MemoryDescriptor *memory_map_get(const BootInfo *info,
                                       uint64_t index)
{
    const uint8_t *map = (const uint8_t *)info->memory_map;

    return (const MemoryDescriptor *)
        (map + index * info->descriptor_size);
}
