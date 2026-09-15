#include "../include/debug.h"
#include "../include/serial.h"
#include "../include/memory_map.h"

void debug_print_boot_info(const BootInfo *info)
{
    if (info == 0) {
        serial_puts("BootInfo: NULL\r\n");
        return;
    }

    serial_puts("BootInfo address: ");
    serial_put_hex64((uint64_t)(uintptr_t)info);

    serial_puts("\r\nMemory map address: ");
    serial_put_hex64((uint64_t)(uintptr_t)info->memory_map);

    serial_puts("\r\nMemory map size: ");
    serial_put_hex64(info->memory_map_size);

    serial_puts("\r\nDescriptor size: ");
    serial_put_hex64(info->descriptor_size);

    serial_puts("\r\nDescriptor version: ");
    serial_put_hex64(info->descriptor_version);

    serial_puts("\r\n");
}

void debug_print_sp(uintptr_t sp)
{
    serial_puts("Current SP: ");
    serial_put_hex64((uint64_t)sp);
    serial_puts("\r\n");
}

/*
 * Return the name of a memory type based on its value.
 */
static const char *memory_type_name(uint32_t type)
{
    switch (type) {
    case 0:  return "Reserved";
    case 1:  return "LoaderCode";
    case 2:  return "LoaderData";
    case 3:  return "BootServicesCode";
    case 4:  return "BootServicesData";
    case 5:  return "RuntimeServicesCode";
    case 6:  return "RuntimeServicesData";
    case 7:  return "Conventional";
    case 8:  return "Unusable";
    case 9:  return "ACPIReclaim";
    case 10: return "ACPINVS";
    case 11: return "MMIO";
    case 12: return "MMIOPortSpace";
    case 13: return "PalCode";
    case 14: return "Persistent";
    case 15: return "Unaccepted";
    default: return "Unknown";
    }
}

/* Requires a validated map. */
void debug_print_memory_map(const BootInfo *info)
{
    uint64_t count = memory_map_count(info);

    serial_puts("Memory map: entries=");
    serial_put_uint64(count);
    serial_puts(" descriptor_size=");
    serial_put_hex64(info->descriptor_size);
    serial_puts("\r\n");

    for (uint64_t i = 0; i < count; ++i) {
        const MemoryDescriptor *entry = memory_map_get(info, i);

        serial_puts("type=");
        serial_puts(memory_type_name(entry->type));
        serial_puts("(");
        serial_put_uint64(entry->type);
        serial_puts(")");

        serial_puts(" start=");
        serial_put_hex64(entry->physical_start);

        serial_puts(" pages=");
        serial_put_uint64(entry->pages);

        serial_puts(" attr=");
        serial_put_hex64(entry->attributes);

        serial_puts(" size=");
        serial_put_uint64(entry->pages * MEMORY_PAGE_SIZE);
        serial_puts("bytes/");
        serial_put_uint64(entry->pages * MEMORY_PAGE_SIZE / 1024 / 1024);
        serial_puts("MB\r\n");
    }
}