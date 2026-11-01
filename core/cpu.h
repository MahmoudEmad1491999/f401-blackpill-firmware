/* core/cpu.h — Cortex-M4 CPU intrinsics for STM32F401CCU6
 *
 * Inline-asm wrappers covering:
 *   IRQ global enable/disable          PM0214 §3.5.1   (CPS instruction)
 *   Memory barriers DSB / ISB / DMB    PM0214 §3.4.5
 *   Wait-for-Interrupt / Event         PM0214 §2.7.1, §2.7.2
 *   Special-register read/write        PM0214 §2.5:
 *       PRIMASK, BASEPRI, CONTROL, PSP, MSP
 *
 * All functions are static inline.  A "memory" clobber is used wherever the
 * operation may change the compiler's view of memory ordering (barriers,
 * IRQ state changes, special-register writes).
 *
 * Only freestanding C11 headers (<stdint.h>).  No CMSIS, no HAL.
 */

#ifndef CORE_CPU_H
#define CORE_CPU_H

#include <stdint.h>

/* ---------------------------------------------------------------------------
 * IRQ global enable / disable
 * CPS (Change Processor State) — PM0214 §3.5.1
 * "memory" prevents the compiler from hoisting or sinking memory accesses
 * across the enable/disable boundary.
 * --------------------------------------------------------------------------- */

static inline void __enable_irq(void)
{
    __asm volatile("cpsie i" ::: "memory");
}

static inline void __disable_irq(void)
{
    __asm volatile("cpsid i" ::: "memory");
}

/* ---------------------------------------------------------------------------
 * Memory barriers — PM0214 §3.4.5
 * The option-field SY (0xF) is the only variant valid on Cortex-M4.
 * --------------------------------------------------------------------------- */

/* DSB — Data Synchronisation Barrier: all explicit memory accesses complete
 * before the next instruction executes. */
static inline void __DSB(void)
{
    __asm volatile("dsb 0xF" ::: "memory");
}

/* ISB — Instruction Synchronisation Barrier: flushes the pipeline and
 * prefetch buffer; required after VTOR, CONTROL, or SCB writes take effect. */
static inline void __ISB(void)
{
    __asm volatile("isb 0xF" ::: "memory");
}

/* DMB — Data Memory Barrier: memory accesses before DMB are observed before
 * those after it.  Lighter than DSB (no instruction-stream effect). */
static inline void __DMB(void)
{
    __asm volatile("dmb 0xF" ::: "memory");
}

/* ---------------------------------------------------------------------------
 * Wait-for-Interrupt / Wait-for-Event — PM0214 §2.7.1, §2.7.2
 * "memory" ensures all pending stores complete before the CPU suspends.
 * --------------------------------------------------------------------------- */

static inline void __WFI(void)
{
    __asm volatile("wfi" ::: "memory");
}

static inline void __WFE(void)
{
    __asm volatile("wfe" ::: "memory");
}

/* ---------------------------------------------------------------------------
 * Special-register access via MRS / MSR — PM0214 §2.5
 *
 * MRS (Move from Special register): "=r" output operand.
 * MSR (Move to Special register):   "r"  input operand +
 *     "memory" clobber because writing these registers changes which
 *     interrupts are masked or which stack is active, affecting all
 *     subsequent memory operations from the compiler's perspective.
 * --------------------------------------------------------------------------- */

/* PRIMASK — PM0214 §2.5 / §4.1.1
 * Bit 0 PM: when set, prevents activation of all configurable-priority
 * exceptions (effective priority = -1).  Set/cleared also by CPSIE/CPSID. */
static inline uint32_t __get_PRIMASK(void)
{
    uint32_t result;
    __asm volatile("mrs %0, PRIMASK" : "=r"(result));
    return result;
}

static inline void __set_PRIMASK(uint32_t val)
{
    __asm volatile("msr PRIMASK, %0" :: "r"(val) : "memory");
}

/* BASEPRI — PM0214 §2.5 / §4.1.3
 * Bits [7:0]: when non-zero, masks all exceptions at the same or lower
 * priority.  Writing 0 disables BASEPRI masking entirely. */
static inline uint32_t __get_BASEPRI(void)
{
    uint32_t result;
    __asm volatile("mrs %0, BASEPRI" : "=r"(result));
    return result;
}

static inline void __set_BASEPRI(uint32_t val)
{
    __asm volatile("msr BASEPRI, %0" :: "r"(val) : "memory");
}

/* CONTROL — PM0214 §2.5 / §4.1.4
 * Bit 0 nPRIV: 0 = privileged Thread mode, 1 = unprivileged Thread mode.
 * Bit 1 SPSEL: 0 = MSP in Thread mode,     1 = PSP in Thread mode.
 * Bit 2 FPCA:  1 = FP context active (auto-set on first FP instruction).
 *
 * An ISB is mandatory after writing CONTROL so the new SP selection or
 * privilege level takes effect before the next instruction — PM0214 §4.1.4. */
static inline uint32_t __get_CONTROL(void)
{
    uint32_t result;
    __asm volatile("mrs %0, CONTROL" : "=r"(result));
    return result;
}

static inline void __set_CONTROL(uint32_t val)
{
    __asm volatile("msr CONTROL, %0" :: "r"(val) : "memory");
    __asm volatile("isb 0xF" ::: "memory"); /* PM0214 §4.1.4 requirement */
}

/* PSP — Process Stack Pointer — PM0214 §2.2
 * R13 banked copy used in Thread mode when CONTROL.SPSEL == 1. */
static inline uint32_t __get_PSP(void)
{
    uint32_t result;
    __asm volatile("mrs %0, PSP" : "=r"(result));
    return result;
}

static inline void __set_PSP(uint32_t val)
{
    __asm volatile("msr PSP, %0" :: "r"(val) : "memory");
}

/* MSP — Main Stack Pointer — PM0214 §2.2
 * R13 banked copy used in Handler mode and in Thread mode when
 * CONTROL.SPSEL == 0 (the reset default). */
static inline uint32_t __get_MSP(void)
{
    uint32_t result;
    __asm volatile("mrs %0, MSP" : "=r"(result));
    return result;
}

static inline void __set_MSP(uint32_t val)
{
    __asm volatile("msr MSP, %0" :: "r"(val) : "memory");
}

#endif /* CORE_CPU_H */
