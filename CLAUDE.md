# f401-blackpill-firmware — rules for the coding agent

## Target
WeAct Black Pill v3.x — STM32F401CCU6 (Cortex-M4F, 84 MHz, 256 KB flash, 64 KB SRAM, 25 MHz HSE).
On-board: user LED on PC13 (active low), KEY button on PA0, USB-C on PA11/PA12.
There is **no physical board in the agent's loop**: everything is verified in Renode plus host-side unit tests.
Code must still be correct for real hardware (clock tree, errata, alignment) — note any Renode-vs-silicon gap in docs/design.

## Layout
```
cmake/arm-none-eabi.cmake   toolchain file
third_party/                CMSIS core + cmsis-device-f4 (git submodules), FreeRTOS kernel (later)
startup/                    vector table, Reset_Handler, linker script
drivers/<periph>.{c,h}      register-level drivers (no HAL/LL), each with a small, documented API
lib/                        hardware-independent code (ring buffer, CLI parser, protocol, crc) — host-testable
app/                        applications / RTOS tasks
renode/                     blackpill-f401.repl, .resc scripts, *.robot integration tests
tests/host/                 host unit tests for lib/ (compiled with native gcc + sanitizers)
docs/design/  docs/journal/ (human-only — never edit journal)
```

## Code rules
- C11, `-Wall -Wextra -Werror`, `-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard`.
- Register access only through CMSIS device headers (`stm32f401xc.h`) — no ST HAL.
- No dynamic allocation in bare-metal code. ISRs are short; defer work to main loop/tasks.
- Every driver has: init, a non-blocking API where sensible, and a doc comment with the reference-manual section (RM0368).
- Keep `lib/` free of hardware includes so it can be host-tested.

## Testing rules
- Logic in `lib/` → host unit tests in `tests/host/` (run under ASan/UBSan).
- Anything touching peripherals → a Renode Robot test in `renode/` asserting UART output, GPIO state, or timing.
- `./ci/check.sh` must pass: firmware build (Debug+Release, size report), host tests, Renode tests.

## Commit style
Conventional Commits, scope = driver/module: `feat(uart): add DMA RX with idle-line detection`.
