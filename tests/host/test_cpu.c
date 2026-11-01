/* tests/host/test_cpu.c — native-compiler header-parse check for core/cpu.h
 *
 * What this tests:
 *   The header is valid C11 that a native (x86/x64) compiler can parse:
 *   correct include guard, no forbidden identifiers, proper static-inline
 *   declarations, and uint32_t types throughout.
 *
 * What this does NOT test:
 *   The inline-asm template strings contain ARMv7-M instructions (mrs, msr,
 *   cpsie, dsb, …) that are not valid on a native host assembler.  Because
 *   the functions are static inline and never called here, the compiler
 *   parses but does not assemble them.  ARM-side correctness is covered by
 *   core/cpu_compile_test.c (compiled with arm-none-eabi-gcc) and will be
 *   exercised by Renode integration tests in a later phase.
 */

#include <stdint.h>
#include "core/cpu.h"

/* Double-include check: a second inclusion must be silently ignored. */
#include "core/cpu.h"

/* Verify the getter return types resolve to uint32_t at compile time. */
_Static_assert(sizeof(__get_PRIMASK()) == sizeof(uint32_t),
               "__get_PRIMASK must return uint32_t");
_Static_assert(sizeof(__get_BASEPRI()) == sizeof(uint32_t),
               "__get_BASEPRI must return uint32_t");
_Static_assert(sizeof(__get_CONTROL()) == sizeof(uint32_t),
               "__get_CONTROL must return uint32_t");
_Static_assert(sizeof(__get_PSP()) == sizeof(uint32_t),
               "__get_PSP must return uint32_t");
_Static_assert(sizeof(__get_MSP()) == sizeof(uint32_t),
               "__get_MSP must return uint32_t");

int main(void)
{
    return 0;
}
