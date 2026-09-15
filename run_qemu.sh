#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")"

mkdir -p esp/EFI/BOOT
cp build/BOOTAA64.EFI esp/EFI/BOOT/BOOTAA64.EFI

QEMU_PREFIX="$(brew --prefix qemu)"

args=(
  # Emulate a generic ARM virtual machine
  -machine virt                          

  # Use Cortex-A57 CPU model supporting aarch64 architecture
  -cpu cortex-a57  

  # Allocate 512MB of RAM                                             
  -m 512M                 

  # Use EDK2 AArch64 UEFI firmware as BIOS                                      
  -bios "$QEMU_PREFIX/share/qemu/edk2-aarch64-code.fd" 

  # Mount the "esp" directory as a read-only FAT disk image (EFI System Partition)
  -drive if=none,id=esp,format=raw,file=fat:ro:esp,readonly=on

  # Attach the ESP drive as a virtio block device            
  -device virtio-blk-pci,drive=esp     

  # Disable networking                         
  -net none    

  # Run without a graphical display, use the terminal for I/O                                                       
  -nographic
)

"$QEMU_PREFIX/bin/qemu-system-aarch64" "${args[@]}"