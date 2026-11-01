/* core/cpu_compile_test.c
 *
 * Compiled by arm-none-eabi-gcc (via the core_compile_test OBJECT library)
 * to verify that every inline-asm template in core/cpu.h is accepted by the
 * ARMv7-M assembler.  This file is NOT linked into the firmware ELF.
 *
 * Safe reads restore the original value before writing, so the function is a
 * functional no-op if accidentally called (it never is).
 * WFI/WFE are placed in a never-executed branch so the assembler validates
 * them but they cannot cause the CPU to suspend.
 */

#include "core/cpu.h"

__attribute__((used, noinline))
void _cpu_compile_test(void)
{
    /* Barriers */
    __DSB();
    __ISB();
    __DMB();

    /* IRQ global control */
    __enable_irq();
    __disable_irq();

    /* Special-register round-trips: read then write-back (net no-op) */
    uint32_t pm   = __get_PRIMASK();  __set_PRIMASK(pm);
    uint32_t bp   = __get_BASEPRI();  __set_BASEPRI(bp);
    uint32_t ctrl = __get_CONTROL();  __set_CONTROL(ctrl);
    uint32_t psp  = __get_PSP();      __set_PSP(psp);
    uint32_t msp  = __get_MSP();      __set_MSP(msp);

    /* WFI / WFE: verified by the assembler but never reached at runtime */
    if (pm == 0xDEADBEEFU) {
        __WFI();
        __WFE();
    }
}
