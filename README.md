# f401-blackpill-firmware

Register-level drivers, a FreeRTOS application layer and a field-updatable bootloader for the
**STM32F401CCU6 "Black Pill"** (Cortex-M4F @ 84 MHz). No vendor HAL: everything is written against
CMSIS and the reference manual (RM0368), and every feature is tested in CI on the
[Renode](https://renode.io) simulator.

> 🚧 Built incrementally — see [ROADMAP.md](ROADMAP.md) and the [development journal](docs/journal).

## Build
```sh
git submodule update --init --recursive
cmake -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi.cmake
cmake --build build
./ci/check.sh        # firmware + host unit tests + Renode integration tests
```

## Run in simulation
```sh
renode renode/blackpill.resc
```

## Flash real hardware
_Added once the bootloader lands (DFU / ST-Link)._
