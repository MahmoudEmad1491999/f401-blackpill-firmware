#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
export PATH="$HOME/.local/bin:$HOME/.local/opt/renode:$PATH"
[ -f "$HOME/.local/opt/renode-venv/bin/activate" ] && source "$HOME/.local/opt/renode-venv/bin/activate"

echo "== bare-metal guard (no CMSIS / HAL / libc) =="
FW_DIRS=$(ls -d regs core startup drivers kernel lib app 2>/dev/null || true)
if [ -n "$FW_DIRS" ]; then
  # shellcheck disable=SC2086
  if grep -rnE '#\s*include\s*[<"](stm32|core_cm|cmsis|system_stm32|arm_math|FreeRTOS|stdio\.h|stdlib\.h|string\.h|math\.h)' $FW_DIRS; then
    echo "forbidden include found (firmware must be 100% bare metal)"; exit 1
  fi
fi
if [ -f .gitmodules ] && grep -qiE 'cmsis|stm32|hal|freertos' .gitmodules; then
  echo "vendor submodule present in .gitmodules"; exit 1
fi

if [ ! -f CMakeLists.txt ]; then echo "no build system yet — nothing else to check"; exit 0; fi

for cfg in Debug Release; do
  echo "== firmware $cfg =="
  cmake -S . -B "build/$cfg" -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi.cmake -DCMAKE_BUILD_TYPE=$cfg >/dev/null
  cmake --build "build/$cfg" -j "$(nproc)"
done

shopt -s nullglob
elfs=(build/Release/*.elf)
for e in "${elfs[@]}"; do
  arm-none-eabi-size "$e"
  if arm-none-eabi-nm "$e" | grep -wE '_sbrk|malloc|free|printf|puts|_write|_impure_ptr|__libc_init_array'; then
    echo "libc symbols linked into $e"; exit 1
  fi
done

if [ -f tests/host/CMakeLists.txt ]; then
  echo "== host unit tests =="
  cmake -S tests/host -B build/host -G Ninja -DCMAKE_BUILD_TYPE=Debug >/dev/null
  cmake --build build/host -j "$(nproc)"
  ctest --test-dir build/host --output-on-failure
fi

robots=(renode/*.robot)
if [ ${#robots[@]} -gt 0 ]; then
  echo "== Renode integration tests =="
  renode-test --show-log "${robots[@]}"
fi
echo "ALL CHECKS PASSED"
