# 0002 — CMSIS submodules: added then removed

## Context
An earlier attempt added `CMSIS_5` and `cmsis-device-f4` as git submodules to
`third_party/` and used them in `app/cmsis_header_test.c`.  This was superseded
when the project policy was formalised as 100% bare metal (see CLAUDE.md).

## Decision
Both submodules were removed.  `third_party/CMakeLists.txt` is now a stub.
All register definitions are hand-written in `regs/` from RM0368/PM0214/DS9716.

## Why CMSIS was not kept
- CI's bare-metal guard rejects `#include <stm32...>` / `#include <core_cm...>`
  in firmware directories.
- CMSIS headers drag in `system_stm32f4xx.c` clock init and `_syscalls.c`
  newlib stubs — both are written from scratch in this project.
- Submodules add `git submodule update --init` overhead and a network dependency
  that conflicts with the "everything hand-written" CV goal.

## Consequences
- No vendor headers anywhere in the tree.
- `third_party/` is kept as an empty tracked directory for possible future use
  (e.g., a vendored test double or a Python script); it will never hold ARM/ST
  licensed code.
