# f401-blackpill-firmware — rules for the coding agent

## Target
WeAct Black Pill v3.x — STM32F401CCU6 (Cortex-M4F, 84 MHz, 256 KB flash, 64 KB SRAM, 25 MHz HSE).
On-board: user LED on PC13 (active low), KEY button on PA0, USB-C on PA11/PA12.
There is **no physical board in the agent's loop**: everything is verified in Renode plus host-side unit tests.
Code must still be correct for real hardware (clock tree, errata, alignment) — note any Renode-vs-silicon gap in docs/design.

## 100 % bare metal — hard rules
- **No CMSIS, no ST HAL/LL, no vendor headers, no third-party code, no RTOS libraries.** Do not add submodules.
- Every register definition is hand-written in `regs/` from the manuals, with the source section cited:
  - RM0368 (STM32F401xB/C/D/E reference manual) for peripherals
  - PM0214 (Cortex-M4 programming manual) for core peripherals: SCB, NVIC, SysTick, MPU, FPU, DWT/ITM
  - DS9716 (STM32F401xC datasheet) for pinout/alternate functions/memory map
- Register style: one header per peripheral, `typedef struct { volatile uint32_t CR1; ... } usart_regs_t;`
  with reserved padding, `_Static_assert(offsetof(...))` for every register offset, base addresses as
  `#define USART2 ((usart_regs_t *)0x40004400UL)`, bit fields as `#define USART_CR1_UE (1U << 13)`.
- Core intrinsics (`__disable_irq`, `__DSB`, `__ISB`, `__WFI`, `__get_PRIMASK`, ...) implemented as inline asm in `core/cpu.h`.
- Startup: vector table, Reset_Handler, `.data`/`.bss` init, FPU enable (CPACR) and linker script all written by hand.
- Only the compiler's freestanding headers (`stdint.h`, `stdbool.h`, `stddef.h`) are allowed in firmware.
  No newlib calls (`printf`, `malloc`, `memcpy` from libc) — implement minimal `mem*`/`str*` and a small
  `fmt` (printf-like) in `lib/`. Link with `-nostdlib -ffreestanding` plus libgcc only.

## Layout
```
cmake/arm-none-eabi.cmake   toolchain file
regs/                       hand-written register maps (one header per peripheral, offsets asserted)
core/                       cpu.h intrinsics, nvic, systick, scb/fault handling, mpu, dwt
startup/                    vector table, Reset_Handler, linker scripts
drivers/<periph>.{c,h}      register-level drivers, documented API, RM0368 section cited
kernel/                     own preemptive scheduler (PendSV/SysTick), sync primitives
lib/                        hardware-independent code (fmt, ringbuf, cli, crc, cobs, pid…) — host-testable
app/                        applications
renode/                     blackpill-f401.repl, .resc scripts, *.robot integration tests
tests/host/                 host unit tests for lib/ (and kernel logic where possible), native gcc + sanitizers
docs/design/  docs/journal/ (human-only — never edit journal)
```

## Code rules
- C11, `-Wall -Wextra -Werror`, `-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffreestanding -nostdlib`.
- No dynamic allocation. ISRs are short; defer work.
- Assembly only where necessary (context switch, startup bits); keep it in `.S` files with comments per instruction group.
- Keep `lib/` free of hardware includes so it can be host-tested.

## Testing rules
- Logic in `lib/` → host unit tests in `tests/host/` (run under ASan/UBSan).
- Register maps → compile-time offset asserts; a host test checks every peripheral base address against a table from DS9716.
- Anything touching peripherals → a Renode Robot test in `renode/` asserting UART output, GPIO state, or timing.
- `./ci/check.sh` must pass: firmware build (Debug+Release, size report), host tests, Renode tests, and the
  "no forbidden includes" guard (fails if CMSIS/HAL/newlib headers appear).

## Commit style
Conventional Commits, scope = driver/module: `feat(uart): add DMA RX with idle-line detection`.
