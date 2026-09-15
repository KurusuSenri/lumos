#include <Uefi.h>
#include "../include/boot_info.h"

/* Storage for the memory map returned by UEFI. */
static UINT64 memory_map_buffer[8192];

/* Static storage survives the handoff to the kernel. */
static BootInfo boot_info;

/**
 * Halt the system
 */
__attribute__((noreturn))
static void boot_halt(void)
{
    for (;;) {}
}

EFI_STATUS
EFIAPI
efi_main(
    EFI_HANDLE ImageHandle,
    EFI_SYSTEM_TABLE *SystemTable
)
{
    /* 
    * Boot services are a set of services provided by the UEFI firmware
    * Including memory management, device I/O, etc
    */
    EFI_BOOT_SERVICES *bs = SystemTable->BootServices;

    EFI_STATUS status;
    UINTN map_size;
    UINTN map_key;
    UINTN descriptor_size;
    UINT32 descriptor_version;

    /*
     * Any firmware-based output must happen before the final
     * GetMemoryMap() / ExitBootServices() sequence.
     *
     * The L prefix indicates that the string is a wide-character string (UTF-16),
     * which is the format expected by UEFI for text output.
     */
    SystemTable->ConOut->OutputString(
        SystemTable->ConOut,
        L"Hello world from EFI!\r\n"
    );
    SystemTable->ConOut->OutputString(
        SystemTable->ConOut,
        L"EFI: entering kernel...\r\n"
    );

    /*
     * Loop until ExitBootServices() succeeds
     */
    for (;;) {
        map_size = sizeof(memory_map_buffer);

        status = bs->GetMemoryMap(
            &map_size,
            (EFI_MEMORY_DESCRIPTOR *)memory_map_buffer,
            &map_key,
            &descriptor_size,
            &descriptor_version
        );

        /* 
         * Usually this error will not happen
         * Unless the memory map buffer is too small to hold the entire memory map
         */
        if (EFI_ERROR(status)) {
            SystemTable->ConOut->OutputString(
                SystemTable->ConOut,
                L"EFI: GetMemoryMap() failed\r\n"
            );
            boot_halt();
        }

        status = bs->ExitBootServices(ImageHandle, map_key);

        if (status == EFI_SUCCESS) {
            break;
        }

        if (status != EFI_INVALID_PARAMETER) {
            SystemTable->ConOut->OutputString(
                SystemTable->ConOut,
                L"EFI: ExitBootServices() failed\r\n"
            );
            boot_halt();
        }

        /* 
         * At this point, the memory map has changed
         * So obtain a fresh map and retry
         */
    }

    /*
    * ExitBootServices() has succeeded.
    * These values describe the final successful memory map.
    */
    boot_info.memory_map = memory_map_buffer;
    boot_info.memory_map_size = (uint64_t)map_size;
    boot_info.descriptor_size = (uint64_t)descriptor_size;
    boot_info.descriptor_version = descriptor_version;

    kernel_main(&boot_info);
}