# Compile-test: verify CMSIS headers are reachable and the device macros are
# correct.  Compiled as an OBJECT library (no linker script needed yet).
add_library(cmsis_header_test OBJECT cmsis_header_test.c)
target_link_libraries(cmsis_header_test PRIVATE firmware_config cmsis_device_f4)
