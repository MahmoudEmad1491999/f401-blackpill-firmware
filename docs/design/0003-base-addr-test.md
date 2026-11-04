# 0003 — Host test for peripheral base addresses (DS9716 §2.3)

**Date:** 2026-11-05  
**Status:** Implemented

## Context

CLAUDE.md requires a host test that cross-checks every peripheral base address
against DS9716 Rev 7 §2.3 Table 2.  Without this, a one-hex-digit typo in a
`#define GPIOA ((gpio_regs_t *)0x4002000UL)` compiles silently and produces
subtle hardware failures at runtime.

## Decision

1. Added `_BASE` integer-constant macros to `regs/scb.h`, `regs/rcc.h`, and
   `regs/gpio.h` (e.g. `#define RCC_BASE 0x40023800UL`).  The existing pointer
   macros now use them (`#define RCC ((rcc_regs_t *)RCC_BASE)`), making them DRY.

2. Created `tests/host/test_base_addrs.c` with:
   - A complete `DS9716_xxx_BASE` reference table for all STM32F401 peripherals,
     documenting the expected value even for headers not yet written.
   - One `_Static_assert` per defined `_BASE` macro comparing it against the
     reference entry.  Future drivers add one line here when their header is written
     (guided by the "no header yet" comment block at the bottom).
   - Empty `main()` — all checks are compile-time, consistent with existing tests.

## Alternatives considered

**Runtime check via `(uintptr_t)(void *)RCC`** — avoids header modifications but
yields only runtime errors; not a constant expression so cannot use `_Static_assert`.
Rejected in favour of compile-time detection.

**Central `regs/base_addrs.h`** — a dependency hub that every peripheral header
must include.  Adds coupling with no benefit over per-header `_BASE` macros that
match the existing self-contained header style.

## Consequences

- Each existing pointer macro (`RCC`, `GPIOA`, …) reads through its `_BASE` alias;
  no functional change to firmware.
- When a new `regs/xxx.h` is written: add `#define xxx_BASE 0x…UL` there and add
  a matching `_Static_assert` row in `tests/host/test_base_addrs.c`.
