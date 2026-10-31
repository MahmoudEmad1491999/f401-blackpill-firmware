# Toolchain file for arm-none-eabi GCC targeting STM32F401CCU6 (Cortex-M4F).
#
# Usage:
#   cmake -S . -B build/Debug -G Ninja \
#         -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi.cmake \
#         -DCMAKE_BUILD_TYPE=Debug
#
# CMake re-reads this file multiple times during configuration; keep it
# side-effect-free so repeated reads are idempotent.

set(CMAKE_SYSTEM_NAME      Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m4)

# ---------------------------------------------------------------------------
# Cross tool discovery
# ---------------------------------------------------------------------------
find_program(ARM_CC   arm-none-eabi-gcc  REQUIRED)
find_program(ARM_CXX  arm-none-eabi-g++  REQUIRED)
find_program(ARM_OBJCOPY arm-none-eabi-objcopy)
find_program(ARM_OBJDUMP arm-none-eabi-objdump)
find_program(ARM_SIZE    arm-none-eabi-size)

set(CMAKE_C_COMPILER   ${ARM_CC})
set(CMAKE_CXX_COMPILER ${ARM_CXX})
set(CMAKE_ASM_COMPILER ${ARM_CC})

# ---------------------------------------------------------------------------
# Skip try_compile link step — we have no libc/linker script at configure time.
# ---------------------------------------------------------------------------
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# ---------------------------------------------------------------------------
# CPU flags — applied to C, C++, and ASM.
# -mcpu=cortex-m4         : Cortex-M4 instruction set + DSP extensions
# -mthumb                 : Thumb-2 encoding (required for CM4)
# -mfpu=fpv4-sp-d16       : single-precision FPv4 unit on F401
# -mfloat-abi=hard        : use FP registers for arguments and return values
# ---------------------------------------------------------------------------
set(_MCU_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")

# -ffreestanding: no standard startup, no implicit standard library includes,
# no assumptions about standard function semantics.
set(CMAKE_C_FLAGS_INIT          "${_MCU_FLAGS} -ffreestanding")
set(CMAKE_CXX_FLAGS_INIT        "${_MCU_FLAGS} -ffreestanding")
set(CMAKE_ASM_FLAGS_INIT        "${_MCU_FLAGS} -x assembler-with-cpp")

# ---------------------------------------------------------------------------
# Linker flags
# -nostdlib           : do not link libc, libstdc++, or crt0
# -lgcc               : keep compiler support routines (__aeabi_*, __divsi3…)
# -Wl,--gc-sections   : strip dead code/data (added per-target when ready)
# ---------------------------------------------------------------------------
set(CMAKE_EXE_LINKER_FLAGS_INIT
    "${_MCU_FLAGS} -nostdlib -lgcc")

# ---------------------------------------------------------------------------
# Search-path policy: host tools from PATH, libraries from sysroot only.
# ---------------------------------------------------------------------------
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
