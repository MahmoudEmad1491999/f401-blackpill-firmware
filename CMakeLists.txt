cmake_minimum_required(VERSION 3.25)

# The toolchain must be set before project() is called.  Pass it on the
# command line:  -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi.cmake
# (ci/check.sh already does this.)

project(blackpill_f401
    VERSION     0.1.0
    DESCRIPTION "STM32F401CCU6 bare-metal firmware"
    LANGUAGES   C ASM
)

# ---------------------------------------------------------------------------
# Language standard
# ---------------------------------------------------------------------------
set(CMAKE_C_STANDARD   11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# ---------------------------------------------------------------------------
# firmware_warnings — compiler diagnostics applied to every firmware target
# ---------------------------------------------------------------------------
add_library(firmware_warnings INTERFACE)
target_compile_options(firmware_warnings INTERFACE
    -Wall
    -Wextra
    -Werror
    -Wdouble-promotion  # float->double silent widening is expensive on hard-fp
    -Wshadow
    -Wundef
    -fno-common
)

# ---------------------------------------------------------------------------
# firmware_opts — per-build-type optimisation flags
# ---------------------------------------------------------------------------
add_library(firmware_opts INTERFACE)
target_compile_options(firmware_opts INTERFACE
    $<$<CONFIG:Debug>:   -Og -g3>
    $<$<CONFIG:Release>: -Os>
)
target_compile_definitions(firmware_opts INTERFACE
    $<$<CONFIG:Debug>:   DEBUG=1>
    $<$<CONFIG:Release>: NDEBUG>
)

# ---------------------------------------------------------------------------
# firmware_defs — project-wide compile definitions
# ---------------------------------------------------------------------------
add_library(firmware_defs INTERFACE)
target_compile_definitions(firmware_defs INTERFACE
    STM32F401xC
    HSE_VALUE=25000000UL
)

# ---------------------------------------------------------------------------
# firmware_config — umbrella target every firmware target links against
# ---------------------------------------------------------------------------
add_library(firmware_config INTERFACE)
target_link_libraries(firmware_config INTERFACE
    firmware_warnings
    firmware_opts
    firmware_defs
)

# ---------------------------------------------------------------------------
# Subdirectories — uncommented as each Phase-0 task lands:
#
#   add_subdirectory(third_party)  # task 2: CMSIS submodules
#   add_subdirectory(startup)      # task 3: linker script + Reset_Handler
#   add_subdirectory(drivers)      # Phase 1+
#   add_subdirectory(lib)          # Phase 2+
#   add_subdirectory(app)          # Phase 1+
# ---------------------------------------------------------------------------
