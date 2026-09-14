CLANG="$(brew --prefix llvm)/bin/clang"

args=(
  # PE32+ format for UEFI applications
  --target=aarch64-pc-windows-msvc  
  
 # Use freestanding environment for UEFI applications
  -ffreestanding                    
 
  # No stack protector for UEFI applications
  -fno-stack-protector              
  
  # Use short wchar_t for UEFI applications
  -fshort-wchar          

  # Enable optimizations      
  -O2                         
  # Include directories for EDK2 headers      
  -I edk2/MdePkg/Include         

  # AArch64-specific EDK2 headers
  -I edk2/MdePkg/Include/AArch64    

  # The source file to compile, not linking
  -c efi.c           
  
  # Output object file               
  -o efi.obj                        
)

"$CLANG" "${args[@]}"