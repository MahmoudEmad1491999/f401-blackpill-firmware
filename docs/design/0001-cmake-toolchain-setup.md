# 0001 — CMake project + arm-none-eabi toolchain (freestanding, nostdlib)

## Context
Phase 0 needs a reproducible cross-build system before any source code is written.
`ci/check.sh` expects `cmake -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi.cmake`
to configure and build cleanly for both Debug and Release, with no libc symbols
in any produced ELF.

## Decision
**`cmake/arm-none-eabi.cmake`** (toolchain file):
- `CMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY` lets CMake probe the compiler
  without a linker script.
- `-ffreestanding` in `CMAKE_C_FLAGS_INIT`: disables implicit standard library
  includes and tells the compiler it cannot assume standard function semantics.
- `-nostdlib -lgcc` in `CMAKE_EXE_LINKER_FLAGS_INIT`: no libc/crt0; libgcc is
  kept for compiler-emitted support routines (`__aeabi_*`, `__divsi3`, …).

**Root `CMakeLists.txt`** exposes three INTERFACE libraries:
- `firmware_warnings` — `-Wall -Wextra -Werror` + diagnostics
- `firmware_opts` — `-Og/-g3` (Debug) vs `-Os` (Release)
- `firmware_defs` — `STM32F401xC`, `HSE_VALUE`
All three compose into `firmware_config`; every firmware target links it.

## Alternatives considered
- **`-specs=nano.specs -specs=nosys.specs`** — original approach; links newlib-nano
  which pulls in libc and `_impure_ptr`. Rejected: CI's `arm-none-eabi-nm` check
  would catch libc symbols, and CLAUDE.md forbids newlib.
- **`-nodefaultlibs` without `-lgcc`** — would break integer division and other
  compiler-emitted calls on Cortex-M4. Rejected.

## Consequences
- All firmware targets must link `firmware_config`.
- No `printf`, `malloc`, or string.h functions are available; `lib/` will provide
  hand-written equivalents in later phases.
- The `CMAKE_TRY_COMPILE_TARGET_TYPE` setting means a broken linker script will
  only surface at build time, not configure time — acceptable for this project.
