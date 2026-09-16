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
  # Disable floating-point registers
  -mgeneral-regs-only
  # Enable optimizations
  -O2
  # All warnings
  -Wall
  -Wextra
)

shopt -s nullglob

for src in boot/*.c kernel/*.c kernel/*.S drivers/*.c lib/*.c tests/*.c; do
  obj="build/${src%.*}.obj"

  mkdir -p "$(dirname "$obj")"

  "$CLANG" "${common_args[@]}" \
    -I edk2/MdePkg/Include \
    -I edk2/MdePkg/Include/AArch64 \
    -c "$src" \
    -o "$obj"
done
