#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")"

CLANG="$(brew --prefix llvm)/bin/clang"

mkdir -p build

common_args=(
  # PE32+ format
  --target=aarch64-pc-windows-msvc
  # Use freestanding environment
  -ffreestanding
  # No stack protector
  -fno-stack-protector
  # Use short wchar_t
  -fshort-wchar
  # Enable optimizations
  -O2
  # All warnings
  -Wall
  -Wextra
)

"$CLANG" "${common_args[@]}" \
  -I edk2/MdePkg/Include \
  -I edk2/MdePkg/Include/AArch64 \
  -c boot/efi.c \
  -o build/efi.obj

"$CLANG" "${common_args[@]}" \
  -c kernel/kernel.c \
  -o build/kernel.obj

"$CLANG" "${common_args[@]}" \
  -c drivers/serial.c \
  -o build/serial.obj

"$CLANG" "${common_args[@]}" \
  -c kernel/debug.c \
  -o build/debug.obj