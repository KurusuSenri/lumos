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

shopt -s nullglob

for src in boot/*.c kernel/*.c drivers/*.c lib/*.c; do
  obj="build/${src%.c}.obj"

  mkdir -p "$(dirname "$obj")"

  "$CLANG" "${common_args[@]}" \
    -I edk2/MdePkg/Include \
    -I edk2/MdePkg/Include/AArch64 \
    -c "$src" \
    -o "$obj"
done
