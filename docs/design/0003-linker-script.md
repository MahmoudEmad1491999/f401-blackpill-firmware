# 0003 — Hand-written linker script for STM32F401CCU6

## Context
Phase-0 requires a linker script before any real firmware can be linked.
`cmake/arm-none-eabi.cmake` uses `CMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY`
so CMake never needs a linker script itself; the script is only needed when
`add_executable` is used.

## Decision
`startup/stm32f401cc.ld` is written from scratch against DS9716 Table 3
(memory map) and PM0214 §2.3.4 / §4.4.4 (MSP init, VTOR).

Key choices:
- `_estack = ORIGIN(SRAM) + LENGTH(SRAM)` (0x2001_0000): placed before SECTIONS
  as a fixed absolute symbol — the conventional ST Micro approach.  The value
  is the initial MSP, loaded from vector-table word 0 by hardware before
  Reset_Handler runs.
- `._stack (NOLOAD)` allocates STACK_SIZE bytes in SRAM so the linker's own
  region-overflow detection fires if .data + .bss + stack exceed 64 K.  The
  explicit `ASSERT` adds a human-readable message on top.
- `_sstack` (bottom of reserved stack) is exported for future MPU/canary use.
- `/DISCARD/` drops `.ARM.attributes`, `.comment`, `.eh_frame`, and
  `.note.gnu.build-id` — none are useful in a freestanding binary.
- A minimal `startup_stub.c` (2-entry vector table + bare Reset_Handler) is
  added alongside to make the linker script testable; the next task replaces
  it with the full 98-IRQ startup.

## Alternatives considered
- **ST-Cube-generated linker script** — rejected per CLAUDE.md (no vendor files).
- **`_estack` inside ._stack section** — would place _estack at `_sstack +
  STACK_SIZE` (somewhere mid-SRAM), not at the true top.  Hardware needs the
  actual top of SRAM; defining it as a constant outside SECTIONS is simpler
  and unambiguous.

## Consequences
- `ci/check.sh` now verifies all startup symbols and that `_estack == 0x20010000`.
- The next task (full startup) replaces `startup_stub.c` in-place; the linker
  script itself does not change.
- `app/blackpill_f401.elf` is now a real linked ELF: 20 B FLASH, 2 KB SRAM (stack).
