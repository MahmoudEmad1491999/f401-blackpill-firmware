# 0006 — SCB register map and FPU enable via CPACR

## Context
The STM32F401CCU6 has an FPv4-SP FPU (coprocessors CP10 and CP11).  At reset
the FPU is disabled; any FP instruction generates a UsageFault.  The firmware
is built with `-mfloat-abi=hard` so the toolchain is free to emit FP
instructions anywhere in generated code, including function prologues.

## Decision
1. Hand-wrote `regs/scb.h` covering all SCB registers (PM0214 §4.3) plus
   CPACR (§4.6.1) in a single `scb_regs_t` struct.  The 18-word reserved
   hole between AFSR (0x03C) and CPACR (0x088) is explicit padding.
   Every register offset is verified by a `_Static_assert(offsetof(…))`.

2. In `Reset_Handler`, the very first action is:
   ```c
   SCB->CPACR |= SCB_CPACR_FPU_FULL;   /* CP10 + CP11 full access */
   __DSB();
   __ISB();
   ```
   This matches PM0214 §4.6.1 (write CPACR, then DSB + ISB to guarantee the
   FPU is active before the next instruction).  Doing it before the `.data`
   copy is the safest ordering: the compiler may emit FP instructions in any
   C translation unit, so we enable the FPU before any C-generated code runs.

## Alternatives considered
- Enable FPU inside `main()` — rejected: FP instructions in startup or
  library init code (e.g., future `lib/mem` with SIMD-like patterns) could
  fire before `main()` is reached.
- Separate `core/fpu.c` helper — unnecessary; the three-line sequence is
  self-contained in startup.

## Consequences
- Renode gap: Renode's Cortex-M4 model does not trap FP instructions when
  CPACR CP10/CP11 are clear, so a missing FPU enable would go undetected in
  simulation.  The static assert on struct layout and the host compile test
  are the verification story until a dedicated Renode test exists.
- All future code may use `float`/FP intrinsics unconditionally.
