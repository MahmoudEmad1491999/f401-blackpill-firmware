# Roadmap

One task ≈ one day. The agent takes the first unchecked item. 100 % bare metal: no CMSIS, no HAL, no libc, no RTOS library.

## Phase 0 — Toolchain & boot from nothing
- [x] CMake project + `cmake/arm-none-eabi.cmake` (freestanding, -nostdlib, libgcc only), `.gitignore`, `.clang-format`; remove any CMSIS/vendor files or submodules if present
- [ ] Hand-written linker script for STM32F401CC (256K flash / 64K RAM): sections, stack, symbols for startup
- [ ] Startup in C + minimal asm: full vector table (all F401 IRQs from RM0368 table 38), Reset_Handler, .data/.bss init, weak default handlers
- [ ] `core/cpu.h`: inline-asm intrinsics (irq enable/disable, DSB/ISB/DMB, WFI/WFE, PRIMASK/BASEPRI/CONTROL/PSP/MSP access)
- [ ] `regs/scb.h` + FPU enable via CPACR in startup (PM0214 4.6) with offset asserts
- [ ] `regs/rcc.h`, `regs/gpio.h` hand-written with `_Static_assert(offsetof)` for every register
- [ ] Host test checking all peripheral base addresses against the DS9716 memory map table
- [ ] `renode/blackpill-f401.repl` (F401 memory map, PC13 LED, USART1/2) + `blackpill.resc`, boot to `main` verified
- [ ] First Robot test: firmware boots and toggles PC13 (GPIO state assertion)
- [ ] Host unit-test harness in `tests/host/` (CMake, sanitizers, own tiny assert framework)
- [ ] Map file + size report, `--gc-sections`, stack/heap symbols, CI artifact upload

## Phase 1 — Own mini-libc & core peripherals
- [ ] `lib/mem`: memcpy/memset/memmove/memcmp (word-optimised) + host tests incl. overlap cases; satisfy compiler-emitted calls
- [ ] `lib/str`: strlen/strcmp/strncpy-safe/strtol-like parser + host tests
- [ ] `lib/fmt`: minimal printf (%d %u %x %s %c %p, width, zero-pad, fixed-point float) writing to a sink callback + host tests
- [ ] `regs/flash.h`, RCC driver: HSE 25 MHz → PLL → 84 MHz, flash wait states/prefetch/caches, APB prescalers
- [ ] GPIO driver: mode/pull/speed/AF config, atomic BSRR set/reset, pin helpers
- [ ] `regs/systick.h` + SysTick 1 ms tick, `millis()`, `delay_ms`; Robot test checks blink period
- [ ] `regs/nvic.h` + NVIC driver: enable/disable/pending/priority, priority grouping (AIRCR), critical-section macros
- [ ] `regs/exti.h`, `regs/syscfg.h` + EXTI driver; KEY button (PA0) interrupt with software debounce
- [ ] `regs/dwt.h` + cycle counter driver for µs timing and profiling macros
- [ ] Fault handlers in asm+C: HardFault/MemManage/BusFault/UsageFault stacked-frame dump + CFSR/HFSR/MMFAR/BFAR decode
- [ ] `regs/rcc.h` reset-cause reporting (RCC_CSR) + `regs/iwdg.h` independent watchdog driver

## Phase 2 — Serial & console
- [ ] `regs/usart.h` + USART2 polled driver (115200 8N1, BRR computed from APB1 clock); `fmt` bound to it; Robot test reads "boot ok"
- [ ] `lib/ringbuf` lock-free SPSC ring buffer + host tests
- [ ] USART interrupt-driven TX/RX using ringbuf
- [ ] `regs/dma.h` + DMA driver (DMA1/DMA2 streams, channel select, TC/HT/TE interrupts)
- [ ] USART RX via circular DMA + IDLE-line detection
- [ ] `lib/cli`: tokenizer + command table + help, host-tested
- [ ] Console shell over UART: `help`, `version`, `uptime`, `reboot`, `peek`/`poke` (register read/write); Robot test drives commands
- [ ] Logging module: levels, timestamps, compile-time filtering, ISR-safe deferred output
- [ ] `lib/crc`: CRC-16/CCITT and CRC-32 + host tests; `regs/crc.h` hardware CRC unit with equivalence test

## Phase 3 — Timers, PWM, ADC
- [ ] `regs/tim.h` (general-purpose + advanced TIM1) + periodic-interrupt timer driver (TIM2..TIM5)
- [ ] PWM output on TIM3 with duty-cycle API; Robot test measures toggling
- [ ] Input capture: measure frequency/duty of an input signal
- [ ] Encoder interface mode on TIM4 (quadrature counter)
- [ ] `regs/adc.h` + ADC1 single conversion, internal temperature sensor + VREFINT using factory calibration addresses
- [ ] ADC scan mode with DMA into a buffer, timer-triggered sampling
- [ ] `lib/filter`: moving average, IIR low-pass, median-of-3 — host tests with known vectors
- [ ] Shell commands: `adc`, `pwm <ch> <duty>`, `temp`

## Phase 4 — Buses & low power
- [ ] `regs/spi.h` + SPI1 master driver (polled), modes 0..3, prescaler
- [ ] SPI with DMA, full-duplex transfer API with completion callback
- [ ] SPI NOR flash driver (W25Q-style): JEDEC ID, read, page program, sector erase (Renode model)
- [ ] `regs/i2c.h` + I2C1 master driver: start/addr/read/write/stop with timeout + bus recovery (9 clock pulses)
- [ ] I2C sensor driver (BME280-compatible register map) with Renode model or stub
- [ ] I2C scanner shell command
- [ ] `lib/sensor_if`: generic sensor interface + host test with a fake
- [ ] `regs/rtc.h`, `regs/pwr.h` + RTC driver with LSE/LSI, calendar, alarm interrupt
- [ ] Low power: Sleep and Stop modes, wake on EXTI/RTC alarm; current budget notes in docs
- [ ] `regs/flash.h` flash programming driver (unlock, sector erase, word program, error flags) + Robot test

## Phase 5 — Own preemptive kernel (no RTOS library)
- [ ] Kernel design doc: task model, priorities, tick, memory layout, PSP/MSP split, what is out of scope
- [ ] Task control blocks + static task creation, initial stack frame fabrication (incl. FPU lazy-stacking flags)
- [ ] Context switch in PendSV (asm), SVC to start the first task; two tasks alternating, Robot test
- [ ] Fixed-priority preemptive scheduler with ready bitmap (CLZ), round-robin within a priority
- [ ] `sleep_ms` / delayed list driven by SysTick; idle task with WFI
- [ ] FPU context handling (lazy stacking, EXC_RETURN), test with two float-heavy tasks
- [ ] Semaphores (binary/counting) with blocking + timeouts
- [ ] Mutexes with priority inheritance + priority-inversion demo and test
- [ ] Message queues (fixed-size, copy semantics), ISR-safe send
- [ ] Event flags and task notifications
- [ ] Software timers serviced by a timer task
- [ ] Stack overflow detection: canary + MPU guard region per task (`regs/mpu.h`)
- [ ] Kernel host tests: scheduler/queue logic compiled for host with a fake port layer
- [ ] `top` shell command: per-task CPU usage via DWT, stack high-water marks
- [ ] Tickless idle using Stop mode; measure wake latency in a Robot test
- [ ] Move app to kernel tasks: blink, console, sensor, telemetry; UART driver uses notifications
- [ ] Mutex-protected I2C/SPI bus layer for concurrent tasks
- [ ] Task-liveness supervisor feeding the IWDG

## Phase 6 — Protocol & data
- [ ] `lib/cobs` framing + host tests
- [ ] Binary telemetry protocol (COBS + CRC-16) with message IDs; Python host decoder in `tools/`
- [ ] Telemetry task streams sensor data at 50 Hz; Robot test decodes frames
- [ ] Command protocol: set PWM, read ADC, query status over the binary link
- [ ] Persistent config in the last flash sector: versioned struct, CRC, wear-aware writes
- [ ] Ring log in SPI flash with power-fail-safe header
- [ ] `tools/monitor.py`: live plotting of telemetry from serial / Renode socket

## Phase 7 — Bootloader & updates
- [ ] Split layout: 32K bootloader + app slot; separate linker scripts and CMake targets
- [ ] Bootloader: validate app (magic + CRC32), relocate VTOR, set MSP, jump; fallback to recovery
- [ ] Image header format + `tools/mkimage.py` (version, size, CRC, build id)
- [ ] Firmware update over UART (own protocol or XMODEM-1K) into the app slot
- [ ] A/B slots in SPI flash with rollback if the new image doesn't confirm itself
- [ ] Image signature verification: own SHA-256 + Ed25519 (written from the RFCs, host-tested with RFC test vectors)
- [ ] Renode end-to-end test: flash v1 → update to v2 → corrupt v3 → rollback to v2
- [ ] Option bytes driver (RDP/WRP) + doc (applied only on real hardware)

## Phase 8 — USB (register level)
- [ ] `regs/usb_otg.h` + OTG FS core init, device enumeration as CDC-ACM (descriptors, EP0 control state machine)
- [ ] CDC-ACM data endpoints; console mirrored on USB
- [ ] USB DFU runtime → jump to ST system-memory bootloader
- [ ] Real-hardware bring-up doc: flashing with dfu-util/ST-Link, expected console output

## Phase 9 — Quality & polish
- [ ] Static analysis in CI: cppcheck + clang-tidy (arm target), MISRA-inspired rule subset documented
- [ ] Coverage report for lib/ and kernel host tests (gcov) in CI summary
- [ ] Stack usage analysis (`-fstack-usage` + call graph) report, worst case per task documented
- [ ] Doxygen docs for regs/drivers/kernel, GitHub Pages workflow
- [ ] Architecture diagram (Mermaid) + memory map in README
- [ ] Demo: "smart fan controller" — temp sensor → PID → PWM, telemetry, shell tuning
- [ ] `lib/pid` with anti-windup + host tests against simulated plant
- [ ] Robot test for the demo: setpoint change → PWM response within tolerance
- [ ] Release v1.0.0 with prebuilt .bin/.elf artifacts and CHANGELOG

## Phase 10 — Extensions
- [ ] Hierarchical state machine framework in lib/ + host tests; port the demo state machine to it
- [ ] Bit-banged I2C/SPI fallback drivers
- [ ] WS2812 LED driver via timer PWM + DMA, Robot test checks bit timing
- [ ] Stepper driver with trapezoidal ramp generator + host tests for exact step counts
- [ ] Multi-node Renode test: two Black Pills over UART, node A sends setpoints, node B controls the fan
- [ ] Modbus RTU slave over USART + host tests for frame parsing + Robot test with Python master
- [ ] Unprivileged tasks: run app tasks in thread-unprivileged mode, kernel calls via SVC
- [ ] Event trace buffer + `tools/trace2perfetto.py` to visualise context switches
- [ ] Persistent crash record in RTC backup registers, dumped on next boot; Robot test forces a fault
- [ ] Fuzz the bootloader image-validation logic on host with libFuzzer
- [ ] Fixed-point math (Q15/Q31) + PID variant; cycle-count benchmark vs float
- [ ] Own radix-2 FFT (Q15) + `fft` shell command printing the dominant frequency
- [ ] Vibration-monitoring demo: ADC → FFT → threshold alarm over telemetry
- [ ] Hardware-in-the-loop guide: running the Robot suite against a real Black Pill via serial
- [ ] Final pass: README with feature matrix, test counts, flash/RAM usage table
