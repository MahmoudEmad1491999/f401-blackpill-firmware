# CMSIS core — Cortex-M4 architecture headers (ARM-software/CMSIS_5 @ 5.9.0)
# Marked SYSTEM so the compiler treats them as system headers and suppresses
# any vendor warnings under our -Wall/-Wextra/-Werror policy.
add_library(cmsis_core INTERFACE)
target_include_directories(cmsis_core SYSTEM INTERFACE
    CMSIS_5/CMSIS/Core/Include
)

# STM32F4 device headers (STMicroelectronics/cmsis-device-f4 @ v2.6.11).
# Consumers get both the device Include/ and the CMSIS core transitively.
add_library(cmsis_device_f4 INTERFACE)
target_include_directories(cmsis_device_f4 SYSTEM INTERFACE
    cmsis-device-f4/Include
)
target_link_libraries(cmsis_device_f4 INTERFACE cmsis_core)
