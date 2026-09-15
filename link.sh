#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")"

LLD_LINK="$(brew --prefix lld)/bin/lld-link"

shopt -s nullglob

objects=()

for src in boot/*.c kernel/*.c drivers/*.c lib/*.c; do
  objects+=("build/${src%.c}.obj")
done

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
  /out:build/BOOTAA64.EFI
  # Input object file generated from compiling c source files
  "${objects[@]}"
)

"$LLD_LINK" "${args[@]}"
