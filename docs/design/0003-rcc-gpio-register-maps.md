# 0003 — RCC and GPIO register map headers

**Date:** 2026-11-03

## Context

Phase 0 requires hand-written peripheral headers for every register, with
compile-time offset assertions. RCC and GPIO are the first two peripherals
needed before any driver work can begin (clock gating before GPIO toggle).

## Decision

`regs/rcc.h` covers RM0368 §6.3 Table 11 (offsets 0x00–0x8C, total 0x90 bytes)
with six 2-word reserved gaps and one 1-word gap modelled as explicit padding
arrays. `regs/gpio.h` covers RM0368 §8.4 Table 26 (offsets 0x00–0x24, 0x28
bytes total); `AFR` is a `uint32_t[2]` so AFRL/AFRH share one field with a
clean offsetof assertion on the array base.

## Alternatives

- Separate AFRL/AFRH fields: simpler names but AFR[n] matches the manual's
  notation and keeps the struct contiguous without an extra pad.
- Bitfield structs for MODER/OSPEEDR/etc.: avoided — they are non-portable
  across compilers and the manual defines 2-bit fields, making mask macros
  clearer and safer.

## Consequences

All per-register offsets are asserted at compile time; struct sizes are
asserted too (0x90 for RCC, 0x28 for GPIO). Four host tests (cpu, scb, rcc,
gpio) all pass under `ci/check.sh`. No runtime code added yet — drivers come
in Phase 1.
