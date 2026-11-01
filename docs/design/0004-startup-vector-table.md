# 0004 — Full startup: vector table, Reset_Handler, weak handlers

## Context
Phase 0 needs a proper startup before any peripheral work can proceed.
The previous task left a 2-entry stub with no `.data`/`.bss` initialisation.

## Decision
Implemented the complete STM32F401CCU6 startup in a single C file (`startup/startup.c`):

- **98-entry vector table** (RM0368 Table 38): 16 Cortex-M4 core exceptions followed
  by 82 external IRQs (IRQ0–IRQ81, ending with FPU at IRQ81).  Array size is
  inferred from the initialiser list; a `_Static_assert` catches any miscount.
- **Reset_Handler** in C: word-at-a-time copy of `.data` from FLASH, then word-at-a-time
  zero of `.bss`, then `main()`.  The linker script already enforces 4-byte alignment
  on both regions, so no masking is required.
- **Weak default handlers**: all 9 core exceptions and all 55 named peripheral IRQs
  alias to `Default_Handler` (infinite spin).  Override any handler simply by
  providing a non-weak definition in another translation unit.
- **No assembly file needed**: a pure-C Reset_Handler is correct here because the
  MSP is already loaded by hardware from word 0 of the vector table before Reset_Handler
  runs (PM0214 §2.3.4), so no manual stack setup is needed.  The minimal-asm
  language from the ROADMAP refers to context-switch code in later tasks.
- **FPU enable deferred**: CPACR CP10/CP11 bits are set in the `regs/scb.h` task
  (ROADMAP Phase 0) to keep concerns separated.

## Alternatives considered
- Assembly startup (`.S`): unnecessary complexity — C handles `.data`/`.bss` init
  cleanly and the compiler emits correct Thumb code.
- Separate `.S` trampoline for alignment: Cortex-M4 hardware guarantees SP alignment
  from the vector table, so no trampoline is needed before calling `main`.

## Consequences
- All external IRQs silently spin in Default_Handler until real handlers are added.
- Flash footprint: 392 B (vector table) + ~72 B (Reset_Handler + Default_Handler) = 464 B.
- Renode-vs-silicon gap: none known for this component.
