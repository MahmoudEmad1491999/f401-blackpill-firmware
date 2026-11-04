cmake_minimum_required(VERSION 3.25)
project(blackpill_host_tests C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

enable_testing()

# Project root (two levels above tests/host/)
set(FW_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../..")

# ---------------------------------------------------------------------------
# test_cpu — compile-time check that core/cpu.h parses cleanly under a native
# C11 compiler.  The intrinsics contain ARM inline-asm and are never called;
# their behavioural correctness is verified by the ARM-toolchain OBJECT
# compile test (core/cpu_compile_test.c) and, later, by Renode robot tests.
# ---------------------------------------------------------------------------
add_executable(test_cpu test_cpu.c)
target_include_directories(test_cpu PRIVATE ${FW_ROOT})
target_compile_options(test_cpu PRIVATE -Wall -Wextra -Werror)
add_test(NAME cpu_intrinsics_header_compile COMMAND test_cpu)

# ---------------------------------------------------------------------------
# test_scb — compile-time checks for regs/scb.h: offset asserts, struct size,
# and key bit-field constant values (PM0214 §4.3, §4.6.1).
# ---------------------------------------------------------------------------
add_executable(test_scb test_scb.c)
target_include_directories(test_scb PRIVATE ${FW_ROOT})
target_compile_options(test_scb PRIVATE -Wall -Wextra -Werror)
add_test(NAME scb_register_map_compile COMMAND test_scb)

# ---------------------------------------------------------------------------
# test_rcc — compile-time checks for regs/rcc.h: offset asserts, struct size,
# and key bit-field constant values (RM0368 §6.3).
# ---------------------------------------------------------------------------
add_executable(test_rcc test_rcc.c)
target_include_directories(test_rcc PRIVATE ${FW_ROOT})
target_compile_options(test_rcc PRIVATE -Wall -Wextra -Werror)
add_test(NAME rcc_register_map_compile COMMAND test_rcc)

# ---------------------------------------------------------------------------
# test_gpio — compile-time checks for regs/gpio.h: offset asserts, struct size,
# and key bit-field constant values (RM0368 §8.4).
# ---------------------------------------------------------------------------
add_executable(test_gpio test_gpio.c)
target_include_directories(test_gpio PRIVATE ${FW_ROOT})
target_compile_options(test_gpio PRIVATE -Wall -Wextra -Werror)
add_test(NAME gpio_register_map_compile COMMAND test_gpio)

# ---------------------------------------------------------------------------
# test_base_addrs — compile-time cross-check: every _BASE macro defined in
# regs/ is compared against the DS9716 Rev 7 §2.3 Table 2 reference values.
# ---------------------------------------------------------------------------
add_executable(test_base_addrs test_base_addrs.c)
target_include_directories(test_base_addrs PRIVATE ${FW_ROOT})
target_compile_options(test_base_addrs PRIVATE -Wall -Wextra -Werror)
add_test(NAME peripheral_base_addrs_ds9716 COMMAND test_base_addrs)
