# 0005 — core/cpu.h: Cortex-M4 inline-asm intrinsics

## Context
Phase 0 requires CPU intrinsics before any driver or kernel code can be written.
No CMSIS is allowed, so every wrapper must be hand-written from the manuals.

## Decision
All intrinsics are `static inline` with `__asm volatile` in `core/cpu.h`:

- **IRQ enable/disable** (`cpsie i` / `cpsid i`, PM0214 §3.5.1) with a `"memory"`
  clobber to prevent the compiler from hoisting or sinking memory accesses across
  the IRQ state boundary.
- **Barriers** (`dsb 0xF`, `isb 0xF`, `dmb 0xF`, PM0214 §3.4.5); `0xF` (SY) is the
  only valid option-field for Cortex-M4.
- **WFI / WFE** with a `"memory"` clobber to ensure stores complete before the CPU
  suspends.
- **Special-register accessors** via `MRS`/`MSR` for PRIMASK, BASEPRI, CONTROL, PSP,
  and MSP (PM0214 §2.5).  Write accessors carry a `"memory"` clobber because changing
  interrupt masks or the active stack pointer affects the compiler's view of
  subsequent memory operations.
- **`__set_CONTROL` includes a mandatory ISB** (PM0214 §4.1.4) to guarantee the new
  SP selection / privilege level is in effect before the next instruction.

## Testing strategy
- **ARM compile test** (`core/cpu_compile_test.c`, OBJECT library): every asm template
  is assembled by `arm-none-eabi-gcc`.  WFI/WFE are guarded by a dead-code branch so
  they are assembled but never executed.
- **Host compile test** (`tests/host/test_cpu.c`): a native-compiler parse check that
  includes the header twice (include-guard check) and verifies getter return types via
  `_Static_assert(sizeof(...))` at file scope.  ARM asm strings are never instantiated
  on the host compiler.
- **Behavioral / Renode gap**: correct interrupt-enable/disable, barrier, and
  sleep-until-interrupt behaviour will be verified by Renode robot tests once the
  simulation platform is set up (ROADMAP Phase 0).

## Alternatives considered
- Using GCC built-ins (`__builtin_arm_dsb` etc.): less explicit, harder to trace to
  the manual, and some are GCC-version-dependent.  Rejected in favour of named asm.
- A C file with non-inline functions: would add call overhead and prevent the compiler
  from scheduling barriers optimally.  Rejected.

## Consequences
- `firmware_config` propagates the project-root include path so all firmware targets
  can `#include "core/cpu.h"` without extra configuration.
- Renode-vs-silicon gap: none known for this component.
