/* startup_stub.c — Minimal Reset_Handler and 2-entry vector table.
 *
 * This stub exists solely to satisfy the linker for the Phase-0
 * linker-script task.  It is replaced by the full startup (next task)
 * which adds all 98 RM0368 IRQ vectors, .data copy, and .bss zeroing.
 *
 * The stub deliberately omits .data copy and .bss zeroing so that the
 * linker-script symbols can be verified in isolation; main() therefore
 * only runs the _Static_assert checks in app/main.c which require no
 * initialised global data.
 */

#include <stdint.h>

/* Linker-defined symbols (stm32f401cc.ld) */
extern uint32_t _estack;   /* address = top of SRAM = initial MSP value */

/* Forward declaration — defined below */
void Reset_Handler(void);

int main(void);

/* Two-entry vector table: [0] initial MSP, [1] Reset_Handler address.
 * Placed in .isr_vector so the linker script puts it first in FLASH.
 * Full 98-entry table is added in the next Phase-0 task.             */
__attribute__((section(".isr_vector"), used))
static const uintptr_t g_vectors[2] = {
    (uintptr_t)&_estack,        /* word 0: initial stack pointer (PM0214 §2.3.4) */
    (uintptr_t)Reset_Handler,   /* word 1: reset vector (Thumb addr, bit0 = 1)   */
};

/* Reset_Handler: bare minimum — no .data/.bss init (added in next task). */
__attribute__((noreturn))
void Reset_Handler(void)
{
    main();
    for (;;)
        ;
}
