/* tests/host/test_scb.c — compile-time checks for regs/scb.h
 *
 * What this tests:
 *   - The header parses cleanly under a native C11 compiler.
 *   - Include-guard works (double inclusion is silent).
 *   - The struct total size is 0x8C bytes (CPACR at 0x088 + 4).
 *   - Every _Static_assert(offsetof(…)) in the header passes (they are
 *     transitively compiled when this file is built).
 *   - Key bit-field constants encode the values mandated by PM0214.
 *
 * What this does NOT test:
 *   Hardware register access — that is only possible on a Cortex-M4 and
 *   will be covered by Renode Robot tests.
 */

#include <stdint.h>
#include "regs/scb.h"
#include "regs/scb.h"  /* double-include guard: second inclusion must be silent */

/* Struct total size: CPACR sits at offset 0x088, sizeof(uint32_t) = 4,
 * so the struct ends at 0x08C = 140 bytes.  PM0214 §4.6.1. */
_Static_assert(sizeof(scb_regs_t) == 0x08CU, "scb_regs_t total size must be 0x8C");

/* CPACR FPU-enable constants — PM0214 §4.6.1 */
_Static_assert(SCB_CPACR_CP10_FULL == (3U << 20U), "CP10 full-access = 0b11 at bits [21:20]");
_Static_assert(SCB_CPACR_CP11_FULL == (3U << 22U), "CP11 full-access = 0b11 at bits [23:22]");
_Static_assert(SCB_CPACR_FPU_FULL  == ((3U << 20U) | (3U << 22U)), "FPU full-access mask");

/* AIRCR write key — PM0214 §4.3.5 */
_Static_assert(SCB_AIRCR_VECTKEY == (0x5FAU << 16U), "AIRCR write key 0x5FA at bits [31:16]");

/* PendSV bit used by kernel context-switch — PM0214 §4.3.2 */
_Static_assert(SCB_ICSR_PENDSVSET == (1U << 28U), "ICSR PENDSVSET at bit 28");

int main(void)
{
    return 0;
}
