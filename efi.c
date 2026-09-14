#include <Uefi.h>

EFI_STATUS  // return type
EFIAPI      // calling convention macro
efi_main(
    EFI_HANDLE ImageHandle,         // current efi program being called
    EFI_SYSTEM_TABLE *SystemTable   // uefi system table
)
{
    (void)ImageHandle;              // specify we do not use this parameter

    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *console = SystemTable->ConOut;

    CHAR16 Message1[] = L"Hello, World!\r\n";
    console->OutputString(console, Message1);

    CHAR16 Message2[] = L"From AArch64 UEFI.\r\n";
    console->OutputString(console, Message2);

    return EFI_SUCCESS;
}