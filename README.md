# f401-blackpill-firmware

**100 % bare-metal** firmware for the STM32F401CCU6 "Black Pill" (Cortex-M4F @ 84 MHz):
no CMSIS, no vendor HAL, no libc, no RTOS library. Register maps, startup code, linker script, drivers,
a preemptive kernel and a signed-update bootloader are written from the reference manuals
(RM0368, PM0214, DS9716), and every feature is tested in CI on the [Renode](https://renode.io) simulator.

> 🚧 Built incrementally — see [ROADMAP.md](ROADMAP.md) and the [development journal](docs/journal).

## Build
```sh
cmake -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi.cmake
cmake --build build
./ci/check.sh        # firmware + host unit tests + Renode integration tests + no-vendor-code guard
```

## Run in simulation
```sh
renode renode/blackpill.resc
```
