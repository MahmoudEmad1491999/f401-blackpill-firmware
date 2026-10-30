#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
export PATH="$HOME/.local/bin:$HOME/.local/opt/renode:$PATH"
[ -f "$HOME/.local/opt/renode-venv/bin/activate" ] && source "$HOME/.local/opt/renode-venv/bin/activate"

if [ ! -f CMakeLists.txt ]; then echo "no build system yet — nothing to check"; exit 0; fi
git submodule update --init --recursive -q 2>/dev/null || true

for cfg in Debug Release; do
  echo "== firmware $cfg =="
  cmake -S . -B "build/$cfg" -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi.cmake -DCMAKE_BUILD_TYPE=$cfg >/dev/null
  cmake --build "build/$cfg" -j "$(nproc)"
done
arm-none-eabi-size build/Release/*.elf 2>/dev/null || true

if [ -d tests/host ] && [ -f tests/host/CMakeLists.txt ]; then
  echo "== host unit tests =="
  cmake -S tests/host -B build/host -G Ninja -DCMAKE_BUILD_TYPE=Debug >/dev/null
  cmake --build build/host -j "$(nproc)"
  ctest --test-dir build/host --output-on-failure
fi

shopt -s nullglob
robots=(renode/*.robot)
if [ ${#robots[@]} -gt 0 ]; then
  echo "== Renode integration tests =="
  renode-test --show-log "${robots[@]}" || { echo "renode tests failed"; exit 1; }
fi
echo "ALL CHECKS PASSED"
