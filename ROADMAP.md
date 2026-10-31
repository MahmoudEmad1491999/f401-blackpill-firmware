# Roadmap

One task ≈ one day. The agent takes the first unchecked item.

## Phase 0 — Toolchain & boot
- [x] CMake project + `cmake/arm-none-eabi.cmake` toolchain, `.gitignore`, `.clang-format`
- [x] Add CMSIS_5 core and STMicroelectronics `cmsis-device-f4` as git submodules (pinned tags)
- [ ] Linker script for STM32F401CC (256K flash / 64K RAM), startup code in C (vector table, .data/.bss init, FPU enable)
- [ ] `renode/blackpill-f401.repl` (F401 memory map, PC13 LED, USART1/2) + `blackpill.resc`, boot to `main` verified
- [ ] First Robot test: firmware boots and toggles PC13 (GPIO state assertion)
- [ ] Host unit-test harness in `tests/host/` (CMake, sanitizers) with a placeholder test for lib/
- [ ] Map file + size report, `-ffunction-sections`/`--gc-sections`, stack/heap symbols, CI artifact upload

## Phase 1 — Clocks, GPIO, timing
- [ ] RCC driver: HSE 25 MHz → PLL → 84 MHz SYSCLK, flash wait states, APB prescalers; `SystemCoreClock`
- [ ] GPIO driver: mode/pull/speed/AF config, atomic set/reset via BSRR, port helpers
- [ ] SysTick 1 ms tick, `millis()`, blocking `delay_ms`, Robot test checks blink period
- [ ] EXTI driver + KEY button (PA0) interrupt with debounce in software
- [ ] NVIC priority helpers, priority grouping, critical-section macros (PRIMASK/BASEPRI)
- [ ] DWT cycle counter driver for microsecond timing and profiling macros
- [ ] Fault handlers: HardFault/MemManage/BusFault/UsageFault dump of stacked registers + CFSR decode

## Phase 2 — Serial & console
- [ ] USART2 polled driver (115200 8N1), `printf` retarget via `_write`, Robot test reads "boot ok"
- [ ] `lib/ringbuf` lock-free SPSC ring buffer + host tests
- [ ] USART interrupt-driven TX/RX using ringbuf
- [ ] DMA driver (DMA1/DMA2 streams, channel select, TC/HT interrupts)
- [ ] USART RX via circular DMA + IDLE-line detection
- [ ] `lib/cli`: tokenizer + command table + help, host-tested
- [ ] Console shell over UART: `help`, `version`, `uptime`, `reboot`; Robot test drives commands
- [ ] Logging module: levels, timestamps, compile-time filtering, deferred output from ISR context
- [ ] `lib/crc`: CRC-16/CCITT and CRC-32 + host tests; use F401 hardware CRC unit for CRC-32 with equivalence test

## Phase 3 — Timers, PWM, ADC
- [ ] General-purpose timer driver (TIM2..TIM5): periodic interrupt at configurable rate
- [ ] PWM output on TIM3 with duty-cycle API; Robot test measures toggling
- [ ] Input capture: measure frequency/duty of an input signal
- [ ] Encoder interface mode on TIM4 (quadrature counter)
- [ ] ADC1 single-conversion driver, internal temperature sensor + VREFINT calibration
- [ ] ADC scan mode with DMA into a buffer, timer-triggered sampling
- [ ] `lib/filter`: moving average, IIR low-pass, median-of-3 — host tests with known vectors
- [ ] Shell commands: `adc`, `pwm <ch> <duty>`, `temp`
- [ ] Independent watchdog (IWDG) + reset-cause reporting (RCC_CSR)

## Phase 4 — Buses
- [ ] SPI1 master driver (polled), mode 0..3, configurable prescaler
- [ ] SPI with DMA, full-duplex transfer API with completion callback
- [ ] Driver for a SPI NOR flash (W25Q-style): JEDEC ID, read, page program, sector erase (Renode model)
- [ ] I2C1 master driver: start/addr/read/write/stop with timeout + bus recovery
- [ ] I2C sensor driver (e.g. BME280-compatible register map) with Renode peripheral model or stub
- [ ] I2C scanner shell command
- [ ] `lib/sensor_hal`: generic sensor interface implemented by the I2C driver; host test with a fake
- [ ] RTC driver with LSE/LSI, calendar get/set, alarm interrupt
- [ ] Low-power: Sleep and Stop modes, wake on EXTI/RTC alarm, current budget notes in docs

## Phase 5 — FreeRTOS
- [ ] Add FreeRTOS-Kernel submodule (pinned), port for ARM_CM4F, `FreeRTOSConfig.h`, heap_4
- [ ] Convert app to tasks: blink task, console task, sensor task
- [ ] Inter-task queues: sensor samples → processing task → logger
- [ ] UART driver made RTOS-aware (task notifications instead of polling)
- [ ] Mutex-protected I2C/SPI bus layer for concurrent tasks
- [ ] Software timers for heartbeat and watchdog feeding (task-liveness supervisor)
- [ ] Stack overflow hook, malloc-failed hook, `top` shell command (runtime stats via DWT)
- [ ] Tickless idle with Stop mode; measure wake latency in a Robot test
- [ ] Event groups: system state machine (INIT → RUN → FAULT → SAFE)
- [ ] Priority-inversion demo + fix with priority inheritance, documented in docs/design

## Phase 6 — Protocol & data
- [ ] `lib/cobs` framing + host tests
- [ ] Binary telemetry protocol (COBS + CRC-16) with message IDs; Python host-side decoder in `tools/`
- [ ] Telemetry task streams sensor data at 50 Hz; Robot test decodes frames
- [ ] Command protocol: set PWM, read ADC, query status over the binary link
- [ ] Persistent config in the last flash sector: versioned struct, CRC, wear-aware write
- [ ] Ring log stored in SPI flash with power-fail-safe header
- [ ] `tools/monitor.py`: live plotting of telemetry from serial / Renode socket

## Phase 7 — Bootloader & updates
- [ ] Split memory layout: 32K bootloader + application slot; separate linker scripts and CMake targets
- [ ] Bootloader: validate app (magic + CRC32), set VTOR, jump; fall back to recovery
- [ ] Image header format + `tools/mkimage.py` (version, size, CRC, build id)
- [ ] Firmware update over UART (XMODEM-1K or own protocol) into the app slot
- [ ] A/B slots in SPI flash with rollback if the new image doesn't confirm itself
- [ ] Image signing: Ed25519 verification in bootloader (small vendored implementation), keys in tools/
- [ ] Renode end-to-end test: flash v1 → update to v2 → corrupt v3 → rollback to v2
- [ ] Flash read-out protection & write protection notes + option-byte helper (doc only for Renode)

## Phase 8 — USB
- [ ] USB OTG FS core init, device enumeration as CDC-ACM (descriptors, EP0 control handling)
- [ ] CDC-ACM data endpoints; console mirrored on USB
- [ ] USB DFU runtime → jump to ST system bootloader
- [ ] Document real-hardware bring-up: flashing with dfu-util/ST-Link, expected console output

## Phase 9 — Quality & polish
- [ ] Static analysis in CI: cppcheck + clang-tidy (arm target), MISRA-inspired rule subset documented
- [ ] Unit-test coverage report for lib/ (gcov) in CI summary
- [ ] Stack usage analysis (`-fstack-usage`) report, worst-case per task documented
- [ ] Doxygen docs for drivers, GitHub Pages workflow
- [ ] Architecture diagram (Mermaid) and memory map in README
- [ ] Demo application: "smart fan controller" — temp sensor → PID → PWM, telemetry, shell tuning
- [ ] PID controller in lib/ with anti-windup + host tests with simulated plant
- [ ] Robot test for the demo: setpoint change → PWM response within tolerance
- [ ] Release v1.0.0 with prebuilt .bin/.elf artifacts and CHANGELOG

## Phase 10 — Extensions
- [ ] Event-driven HSM framework (hierarchical state machine) in lib/ with host tests
- [ ] Port the demo's system state machine to the HSM framework
- [ ] Software I2C/SPI bit-bang fallback drivers for pin-constrained designs
- [ ] WS2812 LED driver via timer PWM + DMA, Robot test checks bit timing
- [ ] Stepper motor driver (step/dir with timer-generated ramps, trapezoidal profile)
- [ ] Ramp generator unit tests on host (acceleration limits, exact step counts)
- [ ] CAN-style message bus emulation over UART between two Renode machines (multi-node test)
- [ ] Multi-node Renode test: node A sends setpoints, node B controls the fan
- [ ] Modbus RTU slave over USART with register map + host tests for frame parsing
- [ ] Modbus RTU Robot test with a Python master in Renode
- [ ] MPU configuration: stack guard regions per task, null-pointer trap; fault test
- [ ] FreeRTOS MPU port evaluation (doc) and privileged/unprivileged task split for the console
- [ ] Trace recording: lightweight event trace buffer + `tools/trace2perfetto.py` converter
- [ ] Power profiling estimates per mode (datasheet numbers × residency) reported by `power` command
- [ ] Assert/panic framework with persistent crash record in backup SRAM/RTC registers
- [ ] Crash record dump on next boot + Robot test forcing a fault
- [ ] Unit-test the bootloader's image validation logic on host (fuzz with libFuzzer)
- [ ] Fixed-point math library (Q15/Q31) + PID variant using it, benchmark cycles vs float
- [ ] CMSIS-DSP integration: FFT of ADC samples, `fft` shell command printing dominant frequency
- [ ] Vibration-monitoring demo: ADC → FFT → threshold alarm over telemetry
- [ ] Rust interop experiment: one lib/ module reimplemented in no_std Rust, linked into C build (doc results)
- [ ] Hardware-in-the-loop guide: running the Robot suite against a real Black Pill via serial
- [ ] Final pass: README with feature matrix, test counts, flash/RAM usage table
