LLD_LINK="$(brew --prefix lld)/bin/lld-link"

args=(
  # Mark the output file as a UEFI application
  /subsystem:efi_application  

  # Use the entry point defined in efi.c 
  /entry:efi_main             

  # Use the AArch64 architecture for linking 
  /machine:arm64         

  # Do not link against default libraries; UEFI apps don't use the standard C runtime      
  /nodefaultlib

  # Output file name for the linked UEFI application                
  /out:BOOTAA64.EFI     
  
  # Input object file generated from compiling efi.c       
  efi.obj                      
)

"$LLD_LINK" "${args[@]}"