/* regs/scb.h — Cortex-M4 System Control Block (SCB) register map
 *
 * Source:
 *   PM0214 Rev 5 §4.3   SCB registers, Table 4-17 (CPUID … AFSR)
 *   PM0214 Rev 5 §4.6.1 Coprocessor Access Control Register (CPACR)
 *   DS9716  Table 2      SCS address range 0xE000E000–0xE000EFFF
 *
 * Every register offset is verified by a _Static_assert(offsetof(…)) below.
 * SCB base address: 0xE000ED00 (fixed across all Cortex-M4 devices).
 *
 * Only freestanding C11 headers are used (<stdint.h>, <stddef.h>).
 * No CMSIS, no HAL.
 */

#ifndef REGS_SCB_H
#define REGS_SCB_H

#include <stddef.h>
#include <stdint.h>

/* ---------------------------------------------------------------------------
 * SCB register layout — PM0214 §4.3 Table 4-17
 *
 * The reserved hole [0x040 … 0x084] (18 words) sits between the fault-status
 * registers and CPACR; it is padded explicitly so offsetof assertions hold.
 * --------------------------------------------------------------------------- */
typedef struct {
    volatile uint32_t CPUID;     /* 0x000  CPUID Base Register                  (RO) */
    volatile uint32_t ICSR;      /* 0x004  Interrupt Control and State Register      */
    volatile uint32_t VTOR;      /* 0x008  Vector Table Offset Register              */
    volatile uint32_t AIRCR;     /* 0x00C  Application Interrupt and Reset Control   */
    volatile uint32_t SCR;       /* 0x010  System Control Register                   */
    volatile uint32_t CCR;       /* 0x014  Configuration and Control Register        */
    volatile uint32_t SHPR1;     /* 0x018  System Handler Priority Register 1        */
    volatile uint32_t SHPR2;     /* 0x01C  System Handler Priority Register 2        */
    volatile uint32_t SHPR3;     /* 0x020  System Handler Priority Register 3        */
    volatile uint32_t SHCSR;     /* 0x024  System Handler Control and State Register */
    volatile uint32_t CFSR;      /* 0x028  Configurable Fault Status Registers       */
    volatile uint32_t HFSR;      /* 0x02C  HardFault Status Register                 */
    volatile uint32_t DFSR;      /* 0x030  Debug Fault Status Register               */
    volatile uint32_t MMFAR;     /* 0x034  MemManage Fault Address Register          */
    volatile uint32_t BFAR;      /* 0x038  BusFault Address Register                 */
    volatile uint32_t AFSR;      /* 0x03C  Auxiliary Fault Status Register            */
    uint32_t          _res0[18]; /* 0x040–0x087  Reserved                           */
    volatile uint32_t CPACR;     /* 0x088  Coprocessor Access Control Register      */
} scb_regs_t;

/* Offset assertions — PM0214 §4.3 Table 4-17, §4.6.1 */
_Static_assert(offsetof(scb_regs_t, CPUID)  == 0x000U, "SCB CPUID offset");
_Static_assert(offsetof(scb_regs_t, ICSR)   == 0x004U, "SCB ICSR offset");
_Static_assert(offsetof(scb_regs_t, VTOR)   == 0x008U, "SCB VTOR offset");
_Static_assert(offsetof(scb_regs_t, AIRCR)  == 0x00CU, "SCB AIRCR offset");
_Static_assert(offsetof(scb_regs_t, SCR)    == 0x010U, "SCB SCR offset");
_Static_assert(offsetof(scb_regs_t, CCR)    == 0x014U, "SCB CCR offset");
_Static_assert(offsetof(scb_regs_t, SHPR1)  == 0x018U, "SCB SHPR1 offset");
_Static_assert(offsetof(scb_regs_t, SHPR2)  == 0x01CU, "SCB SHPR2 offset");
_Static_assert(offsetof(scb_regs_t, SHPR3)  == 0x020U, "SCB SHPR3 offset");
_Static_assert(offsetof(scb_regs_t, SHCSR)  == 0x024U, "SCB SHCSR offset");
_Static_assert(offsetof(scb_regs_t, CFSR)   == 0x028U, "SCB CFSR offset");
_Static_assert(offsetof(scb_regs_t, HFSR)   == 0x02CU, "SCB HFSR offset");
_Static_assert(offsetof(scb_regs_t, DFSR)   == 0x030U, "SCB DFSR offset");
_Static_assert(offsetof(scb_regs_t, MMFAR)  == 0x034U, "SCB MMFAR offset");
_Static_assert(offsetof(scb_regs_t, BFAR)   == 0x038U, "SCB BFAR offset");
_Static_assert(offsetof(scb_regs_t, AFSR)   == 0x03CU, "SCB AFSR offset");
_Static_assert(offsetof(scb_regs_t, CPACR)  == 0x088U, "SCB CPACR offset");
_Static_assert(sizeof(scb_regs_t)           == 0x08CU, "scb_regs_t total size");

/* SCB base address — PM0214 Table 2-1, DS9716 Table 2 */
#define SCB  ((scb_regs_t *)0xE000ED00UL)

/* ---------------------------------------------------------------------------
 * ICSR — Interrupt Control and State Register  (PM0214 §4.3.2)
 * --------------------------------------------------------------------------- */
#define SCB_ICSR_NMIPENDSET    (1U << 31)  /* NMI set-pending                */
#define SCB_ICSR_PENDSVSET     (1U << 28)  /* PendSV set-pending             */
#define SCB_ICSR_PENDSVCLR     (1U << 27)  /* PendSV clear-pending           */
#define SCB_ICSR_PENDSTSET     (1U << 26)  /* SysTick set-pending            */
#define SCB_ICSR_PENDSTCLR     (1U << 25)  /* SysTick clear-pending          */

/* ---------------------------------------------------------------------------
 * AIRCR — Application Interrupt and Reset Control  (PM0214 §4.3.5)
 *
 * Write requires VECTKEY = 0x5FA in bits [31:16]; reads return 0xFA05.
 * --------------------------------------------------------------------------- */
#define SCB_AIRCR_VECTKEY      (0x5FAU << 16) /* Write key (bits [31:16])    */
#define SCB_AIRCR_SYSRESETREQ  (1U << 2)      /* Assert system reset         */
#define SCB_AIRCR_PRIGROUP_Pos  8U
#define SCB_AIRCR_PRIGROUP_Msk (7U << SCB_AIRCR_PRIGROUP_Pos)

/* ---------------------------------------------------------------------------
 * SCR — System Control Register  (PM0214 §4.3.7)
 * --------------------------------------------------------------------------- */
#define SCB_SCR_SEVEONPEND     (1U << 4)   /* Wake on pending exception      */
#define SCB_SCR_SLEEPDEEP      (1U << 2)   /* Use deep-sleep mode            */
#define SCB_SCR_SLEEPONEXIT    (1U << 1)   /* Sleep when returning to Thread */

/* ---------------------------------------------------------------------------
 * CCR — Configuration and Control Register  (PM0214 §4.3.8)
 * --------------------------------------------------------------------------- */
#define SCB_CCR_STKALIGN       (1U << 9)   /* 8-byte stack align on exception */
#define SCB_CCR_DIV_0_TRP      (1U << 4)   /* Trap divide-by-zero            */
#define SCB_CCR_UNALIGN_TRP    (1U << 3)   /* Trap unaligned access          */

/* ---------------------------------------------------------------------------
 * SHCSR — System Handler Control and State  (PM0214 §4.3.9)
 * --------------------------------------------------------------------------- */
#define SCB_SHCSR_USGFAULTENA  (1U << 18)  /* Enable UsageFault handler      */
#define SCB_SHCSR_BUSFAULTENA  (1U << 17)  /* Enable BusFault handler        */
#define SCB_SHCSR_MEMFAULTENA  (1U << 16)  /* Enable MemManage handler       */

/* ---------------------------------------------------------------------------
 * CFSR — Configurable Fault Status  (PM0214 §4.3.10)
 *
 * Composite: UFSR [31:16], BFSR [15:8], MMFSR [7:0].
 * --------------------------------------------------------------------------- */
/* UsageFault bits [31:16] */
#define SCB_CFSR_DIVBYZERO     (1U << 25)  /* Divide-by-zero UsageFault      */
#define SCB_CFSR_UNALIGNED     (1U << 24)  /* Unaligned access UsageFault    */
#define SCB_CFSR_NOCP          (1U << 19)  /* No coprocessor access fault    */
#define SCB_CFSR_INVSTATE      (1U << 18)  /* Invalid state UsageFault       */
#define SCB_CFSR_INVPC         (1U << 17)  /* Invalid PC load UsageFault     */
#define SCB_CFSR_UNDEFINSTR    (1U << 16)  /* Undefined instruction          */
/* BusFault bits [15:8] */
#define SCB_CFSR_BFARVALID     (1U << 15)  /* BFAR contains valid address    */
#define SCB_CFSR_LSPERR        (1U << 13)  /* BusFault during FP lazy save   */
#define SCB_CFSR_STKERR        (1U << 12)  /* BusFault on exception stacking */
#define SCB_CFSR_UNSTKERR      (1U << 11)  /* BusFault on unstacking         */
#define SCB_CFSR_IMPRECISERR   (1U << 10)  /* Imprecise data bus error       */
#define SCB_CFSR_PRECISERR     (1U <<  9)  /* Precise data bus error         */
#define SCB_CFSR_IBUSERR       (1U <<  8)  /* Instruction bus error          */
/* MemManage bits [7:0] */
#define SCB_CFSR_MMARVALID     (1U <<  7)  /* MMFAR contains valid address   */
#define SCB_CFSR_MLSPERR       (1U <<  5)  /* MemManage during FP lazy save  */
#define SCB_CFSR_MSTKERR       (1U <<  4)  /* MemManage on stacking          */
#define SCB_CFSR_MUNSTKERR     (1U <<  3)  /* MemManage on unstacking        */
#define SCB_CFSR_DACCVIOL      (1U <<  1)  /* Data access violation          */
#define SCB_CFSR_IACCVIOL      (1U <<  0)  /* Instruction access violation   */

/* ---------------------------------------------------------------------------
 * HFSR — HardFault Status Register  (PM0214 §4.3.11)
 * --------------------------------------------------------------------------- */
#define SCB_HFSR_DEBUGEVT      (1U << 31)  /* Debug event escalated to HF    */
#define SCB_HFSR_FORCED        (1U << 30)  /* Configurable fault escalated   */
#define SCB_HFSR_VECTTBL       (1U <<  1)  /* Vector table read fault        */

/* ---------------------------------------------------------------------------
 * CPACR — Coprocessor Access Control Register  (PM0214 §4.6.1)
 *
 * CP10 [21:20] and CP11 [23:22] control FPU access; both must be identical.
 *   0b00 = access denied  (UsageFault on any FP instruction)
 *   0b01 = privileged only
 *   0b10 = reserved
 *   0b11 = full access (privileged + unprivileged)
 * --------------------------------------------------------------------------- */
#define SCB_CPACR_CP10_Pos      20U
#define SCB_CPACR_CP11_Pos      22U
#define SCB_CPACR_CP10_FULL    (3U << SCB_CPACR_CP10_Pos)  /* CP10 full access */
#define SCB_CPACR_CP11_FULL    (3U << SCB_CPACR_CP11_Pos)  /* CP11 full access */
/* Combined mask: set both CP10 and CP11 to full access to enable FPU */
#define SCB_CPACR_FPU_FULL     (SCB_CPACR_CP10_FULL | SCB_CPACR_CP11_FULL)

#endif /* REGS_SCB_H */
