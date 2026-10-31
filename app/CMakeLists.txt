# Compile-test placeholder: verify the toolchain flags are correct for
# bare-metal Cortex-M4F.  Built as an OBJECT library so no linker script
# is required yet.  Will become the real application executable in Phase 1
# once the linker script and startup code land.
add_library(app_compile_test OBJECT main.c)
target_link_libraries(app_compile_test PRIVATE firmware_config)
