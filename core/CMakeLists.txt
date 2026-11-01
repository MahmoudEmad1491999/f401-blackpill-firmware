# core: CPU intrinsics and other header-only core utilities.
#
# 'core' is an INTERFACE library; consumers link it to gain
#   #include "core/cpu.h"   (and future core/ headers)
# via the project-root include path.
add_library(core INTERFACE)
target_include_directories(core INTERFACE ${CMAKE_SOURCE_DIR})

# Compile test: arm-none-eabi-gcc validates every inline-asm template.
# Built automatically by cmake --build; never linked into the firmware ELF.
add_library(core_compile_test OBJECT cpu_compile_test.c)
target_link_libraries(core_compile_test PRIVATE firmware_config core)
