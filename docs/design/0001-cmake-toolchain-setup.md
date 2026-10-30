# 0001 — CMake project + arm-none-eabi toolchain setup

## Context
Phase 0 of the firmware project needs a reproducible cross-build system before
any source code is written.  The CI script (ci/check.sh) already expects:
- `cmake -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi.cmake` to configure
- `cmake --build` for both Debug and Release configurations

## Decision
**Toolchain file** (`cmake/arm-none-eabi.cmake`):
- Uses `CMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY` so CMake's compiler
  probe works without a linker script.  Without this the configure step would
  fail because bare-metal newlib cannot link a test executable at configure time.
- Hard-codes `-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard` in
  `*_FLAGS_INIT` variables.  These are initial defaults; per-target flags can
  still override them.
- Sets `nosys.specs` in linker flags to stub syscalls (avoids link errors before
  custom `_write`/`_sbrk` are implemented in Phase 2).

**Root CMakeLists.txt** defines three `INTERFACE` libraries:
- `firmware_warnings` — `-Wall -Wextra -Werror` + extra diagnostics
- `firmware_opts` — build-type-specific `-Og/-g3` vs `-Os`
- `firmware_defs` — `STM32F401xC`, `HSE_VALUE`

All three are aggregated by `firmware_config`.  Every future executable target
links `firmware_config` to get the full compile environment.

No actual source targets are created in this task; `cmake --build` succeeds
immediately (no targets = no work).  Subdirectories are listed as comments and
will be uncommented by subsequent tasks.

## Alternatives considered
- **Single monolithic `set(CMAKE_C_FLAGS ...)`** — harder to override per-target
  and harder to test in isolation.  Interface libraries compose better.
- **`add_compile_options()` globally** — would affect host tests if they ever
  share the same build tree.  Interface targets are scoped.

## Consequences
- Subsequent tasks must call `target_link_libraries(<target> PRIVATE firmware_config)`.
- The `CMAKE_TRY_COMPILE_TARGET_TYPE` setting means CMake cannot verify that the
  linker works at configure time.  A broken linker script will only surface at
  build time — acceptable for this project.
