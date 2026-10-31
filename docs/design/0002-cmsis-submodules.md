# 0002 — CMSIS submodules: version pinning and CMake exposure

## Context
The firmware needs CMSIS core headers (core_cm4.h, cmsis_gcc.h, …) and STM32F4
device headers (stm32f401xc.h, system_stm32f4xx.h).  Both are distributed as git
repositories with tagged releases.

## Decision
Two git submodules in `third_party/`, each pinned to an explicit tag:

| Submodule             | Tag     | Provides                              |
|-----------------------|---------|---------------------------------------|
| ARM-software/CMSIS_5  | 5.9.0   | CMSIS/Core/Include (core_cm4.h, …)    |
| STM/cmsis-device-f4   | v2.6.11 | Include/stm32f401xc.h, system header  |

`third_party/CMakeLists.txt` exposes them as `cmsis_core` and `cmsis_device_f4`
INTERFACE targets.  Both use `SYSTEM` include directories so vendor headers are
treated as system headers and do not trigger our `-Wall/-Wextra/-Werror` policy.

## Alternatives considered
- **CMSIS-Pack / ST Cube** — heavy tooling, not build-system-neutral.
- **Copy headers into the repo** — loses upstream patches and provenance.
- **FetchContent** — harder to pin reproducibly; slower first build on CI.

## Consequences
- `git submodule update --init --recursive` is required before building (already
  done automatically in `ci/check.sh`).
- `system_stm32f4xx.c` (startup clock init) lives in
  `cmsis-device-f4/Source/Templates/`; it will be compiled in Phase 0 Task 3
  alongside the linker script and Reset_Handler.
- CMSIS-DSP and CMSIS-NN (also in CMSIS_5) are not exposed yet; add them if
  needed in Phase 3/9.

## Renode vs silicon gap
None for this task — submodule addition is purely build-system work.
