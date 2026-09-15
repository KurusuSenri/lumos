#include "../include/debug.h"
#include "../include/serial.h"

/*
 * Read a 32-bit little-endian value from a byte array.
 */
static uint32_t read_le32(const uint8_t *p)
{
    uint32_t value = 0;

    for (unsigned int i = 0; i < 4; ++i) {
        value |= (uint32_t)p[i] << (i * 8);
    }

    return value;
}

/*
 * Read a 64-bit little-endian value from a byte array.
 */
static uint64_t read_le64(const uint8_t *p)
{
    uint64_t value = 0;

    for (unsigned int i = 0; i < 8; ++i) {
        value |= (uint64_t)p[i] << (i * 8);
    }

    return value;
}

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

static void debug_print_memory_map(const BootInfo *info)
{
    if (info == 0 || info->memory_map == 0) {
        serial_puts("Missing memory map.\r\n");
        return;
    }

    /*
     * This parser understands the version 1 descriptor prefix:
     * Type, PhysicalStart, VirtualStart, NumberOfPages, Attribute.
     */
    if (info->descriptor_version != 1 ||
        info->descriptor_size < 40 ||
        info->memory_map_size % info->descriptor_size != 0) {
        serial_puts("Unsupported or invalid memory map.\r\n");
        return;
    }

    const uint8_t *map = (const uint8_t *)info->memory_map;
    uint64_t count =
        info->memory_map_size / info->descriptor_size;

    serial_puts("Memory map: entries=");
    serial_put_hex64(count);
    serial_puts(" descriptor_size=");
    serial_put_hex64(info->descriptor_size);
    serial_puts("\r\n");

    for (uint64_t i = 0; i < count; ++i) {
        /*
         * Advance by the stride returned by UEFI,
         * which may be larger than the 40-byte prefix.
         */
        const uint8_t *entry =
            map + i * info->descriptor_size;

        uint32_t type = read_le32(entry + 0);
        uint64_t physical_start = read_le64(entry + 8);
        uint64_t pages = read_le64(entry + 24);
        uint64_t attributes = read_le64(entry + 32);

        serial_puts("type=");
        serial_puts(memory_type_name(type));
        serial_puts(" (");
        serial_put_uint64(type);
        serial_puts(")");

        serial_puts(" start=");
        serial_put_hex64(physical_start);

        serial_puts(" pages=");
        serial_put_uint64(pages);

        serial_puts(" attr=");
        serial_put_hex64(attributes);

        //serial_puts("\r\n");

        serial_puts(" size=");
        serial_put_uint64(pages * 4096);
        serial_puts("bytes/");
        serial_put_uint64(pages * 4096 / 1024 / 1024);
        serial_puts("MiB");
        serial_puts("\r\n");
            
    }
}