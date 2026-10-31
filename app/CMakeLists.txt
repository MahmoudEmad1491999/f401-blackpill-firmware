# Compile-test object: verify toolchain flags; no linker script required.
add_library(app_compile_test OBJECT main.c)
target_link_libraries(app_compile_test PRIVATE firmware_config)

# ---------------------------------------------------------------------------
# blackpill_f401.elf — full linked firmware image.
# Uses stm32f401cc.ld (linker script) + startup_stub (vector table + Reset_Handler).
# The stub is replaced by the full startup in the next Phase-0 task.
# ---------------------------------------------------------------------------
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/startup/stm32f401cc.ld)

add_executable(blackpill_f401 main.c)
target_link_libraries(blackpill_f401 PRIVATE
    firmware_config
    startup_stub
)
set_target_properties(blackpill_f401 PROPERTIES
    SUFFIX       ".elf"
    LINK_DEPENDS ${LINKER_SCRIPT}
)
target_link_options(blackpill_f401 PRIVATE
    -T ${LINKER_SCRIPT}
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/blackpill_f401.map
    -Wl,--print-memory-usage
)
