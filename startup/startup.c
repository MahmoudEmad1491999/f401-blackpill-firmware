/* startup.c — STM32F401CCU6 full startup
 *
 * Provides the 98-entry Cortex-M4 vector table (RM0368 Table 38:
 * 16 core exceptions + 82 external IRQs), the Reset_Handler that
 * copies .data and zeroes .bss, and weak default handlers for every
 * exception and peripheral interrupt.
 *
 * References:
 *   RM0368 §10.1.2 Table 38  interrupt vector table (F401xB/C/D/E)
 *   PM0214 §2.3.4             reset sequence, initial MSP load
 *   PM0214 §2.3.2             weak default handler convention
 */

#include <stdint.h>

#include "core/cpu.h"
#include "regs/scb.h"

/* Linker-script symbols (stm32f401cc.ld) */
extern uint32_t _estack;   /* initial MSP: top of SRAM             (PM0214 §2.3.4) */
extern uint32_t _sidata;   /* LMA of .data image in FLASH */
extern uint32_t _sdata;    /* VMA start of .data in SRAM */
extern uint32_t _edata;    /* VMA end   of .data in SRAM */
extern uint32_t _sbss;     /* start of .bss in SRAM */
extern uint32_t _ebss;     /* end   of .bss in SRAM */

int main(void);

void Reset_Handler(void);
void Default_Handler(void);

/* ---------------------------------------------------------------------------
 * Weak default handlers — core exceptions (PM0214 §2.3.4 Table B1-5)
 * Override any of these by providing a non-weak definition elsewhere.
 * --------------------------------------------------------------------------- */
void NMI_Handler(void)           __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void)     __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void)     __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void)      __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void)    __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void)           __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void)      __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)       __attribute__((weak, alias("Default_Handler")));

/* ---------------------------------------------------------------------------
 * Weak default handlers — external IRQs (RM0368 Table 38, F401xB/C/D/E)
 * --------------------------------------------------------------------------- */
void WWDG_IRQHandler(void)                  __attribute__((weak, alias("Default_Handler")));
void PVD_IRQHandler(void)                   __attribute__((weak, alias("Default_Handler")));
void TAMP_STAMP_IRQHandler(void)            __attribute__((weak, alias("Default_Handler")));
void RTC_WKUP_IRQHandler(void)              __attribute__((weak, alias("Default_Handler")));
void FLASH_IRQHandler(void)                 __attribute__((weak, alias("Default_Handler")));
void RCC_IRQHandler(void)                   __attribute__((weak, alias("Default_Handler")));
void EXTI0_IRQHandler(void)                 __attribute__((weak, alias("Default_Handler")));
void EXTI1_IRQHandler(void)                 __attribute__((weak, alias("Default_Handler")));
void EXTI2_IRQHandler(void)                 __attribute__((weak, alias("Default_Handler")));
void EXTI3_IRQHandler(void)                 __attribute__((weak, alias("Default_Handler")));
void EXTI4_IRQHandler(void)                 __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream0_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream1_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream2_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream3_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream4_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream5_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream6_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void ADC_IRQHandler(void)                   __attribute__((weak, alias("Default_Handler")));
void EXTI9_5_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void TIM1_BRK_TIM9_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
void TIM1_UP_TIM10_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
void TIM1_TRG_COM_TIM11_IRQHandler(void)    __attribute__((weak, alias("Default_Handler")));
void TIM1_CC_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void TIM2_IRQHandler(void)                  __attribute__((weak, alias("Default_Handler")));
void TIM3_IRQHandler(void)                  __attribute__((weak, alias("Default_Handler")));
void TIM4_IRQHandler(void)                  __attribute__((weak, alias("Default_Handler")));
void I2C1_EV_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void I2C1_ER_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void I2C2_EV_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void I2C2_ER_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void SPI1_IRQHandler(void)                  __attribute__((weak, alias("Default_Handler")));
void SPI2_IRQHandler(void)                  __attribute__((weak, alias("Default_Handler")));
void USART1_IRQHandler(void)                __attribute__((weak, alias("Default_Handler")));
void USART2_IRQHandler(void)                __attribute__((weak, alias("Default_Handler")));
void EXTI15_10_IRQHandler(void)             __attribute__((weak, alias("Default_Handler")));
void RTC_Alarm_IRQHandler(void)             __attribute__((weak, alias("Default_Handler")));
void OTG_FS_WKUP_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream7_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void SDIO_IRQHandler(void)                  __attribute__((weak, alias("Default_Handler")));
void TIM5_IRQHandler(void)                  __attribute__((weak, alias("Default_Handler")));
void SPI3_IRQHandler(void)                  __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream0_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream1_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream2_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream3_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream4_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void OTG_FS_IRQHandler(void)                __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream5_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream6_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream7_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void USART6_IRQHandler(void)                __attribute__((weak, alias("Default_Handler")));
void I2C3_EV_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void I2C3_ER_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void FPU_IRQHandler(void)                   __attribute__((weak, alias("Default_Handler")));

/* ---------------------------------------------------------------------------
 * Vector table — 98 entries: 16 core exceptions + 82 external IRQs.
 *
 * Array size is inferred from the initialiser list; the _Static_assert below
 * catches any miscounts at compile time.
 *
 * Reserved slots are zero (null function pointer) per ARM convention.
 * RM0368 Table 38, PM0214 §2.3.4.
 * --------------------------------------------------------------------------- */
typedef void (*handler_t)(void);

__attribute__((section(".isr_vector"), used))
static const handler_t g_vectors[] = {
    /* ---- Core exceptions (PM0214 §2.3.4 Table B1-5) ---- */
    (handler_t)&_estack,            /*  0  initial MSP value */
    Reset_Handler,                  /*  1  Reset */
    NMI_Handler,                    /*  2  NMI */
    HardFault_Handler,              /*  3  HardFault */
    MemManage_Handler,              /*  4  MemManage (MPU fault) */
    BusFault_Handler,               /*  5  BusFault */
    UsageFault_Handler,             /*  6  UsageFault */
    0,                              /*  7  Reserved */
    0,                              /*  8  Reserved */
    0,                              /*  9  Reserved */
    0,                              /* 10  Reserved */
    SVC_Handler,                    /* 11  SVCall */
    DebugMon_Handler,               /* 12  Debug monitor */
    0,                              /* 13  Reserved */
    PendSV_Handler,                 /* 14  PendSV */
    SysTick_Handler,                /* 15  SysTick */

    /* ---- External IRQs — RM0368 Table 38 (F401xB/C/D/E) ---- */
    WWDG_IRQHandler,                /* 16  IRQ0  WWDG */
    PVD_IRQHandler,                 /* 17  IRQ1  PVD via EXTI line 16 */
    TAMP_STAMP_IRQHandler,          /* 18  IRQ2  Tamper/timestamp via EXTI 21 */
    RTC_WKUP_IRQHandler,            /* 19  IRQ3  RTC wakeup via EXTI 22 */
    FLASH_IRQHandler,               /* 20  IRQ4  Flash global */
    RCC_IRQHandler,                 /* 21  IRQ5  RCC global */
    EXTI0_IRQHandler,               /* 22  IRQ6  EXTI line 0 */
    EXTI1_IRQHandler,               /* 23  IRQ7  EXTI line 1 */
    EXTI2_IRQHandler,               /* 24  IRQ8  EXTI line 2 */
    EXTI3_IRQHandler,               /* 25  IRQ9  EXTI line 3 */
    EXTI4_IRQHandler,               /* 26  IRQ10 EXTI line 4 */
    DMA1_Stream0_IRQHandler,        /* 27  IRQ11 DMA1 stream 0 */
    DMA1_Stream1_IRQHandler,        /* 28  IRQ12 DMA1 stream 1 */
    DMA1_Stream2_IRQHandler,        /* 29  IRQ13 DMA1 stream 2 */
    DMA1_Stream3_IRQHandler,        /* 30  IRQ14 DMA1 stream 3 */
    DMA1_Stream4_IRQHandler,        /* 31  IRQ15 DMA1 stream 4 */
    DMA1_Stream5_IRQHandler,        /* 32  IRQ16 DMA1 stream 5 */
    DMA1_Stream6_IRQHandler,        /* 33  IRQ17 DMA1 stream 6 */
    ADC_IRQHandler,                 /* 34  IRQ18 ADC1 global */
    0,                              /* 35  IRQ19 Reserved */
    0,                              /* 36  IRQ20 Reserved */
    0,                              /* 37  IRQ21 Reserved */
    0,                              /* 38  IRQ22 Reserved */
    EXTI9_5_IRQHandler,             /* 39  IRQ23 EXTI lines 9:5 */
    TIM1_BRK_TIM9_IRQHandler,       /* 40  IRQ24 TIM1 break + TIM9 global */
    TIM1_UP_TIM10_IRQHandler,       /* 41  IRQ25 TIM1 update + TIM10 global */
    TIM1_TRG_COM_TIM11_IRQHandler,  /* 42  IRQ26 TIM1 trigger/comm + TIM11 */
    TIM1_CC_IRQHandler,             /* 43  IRQ27 TIM1 capture/compare */
    TIM2_IRQHandler,                /* 44  IRQ28 TIM2 global */
    TIM3_IRQHandler,                /* 45  IRQ29 TIM3 global */
    TIM4_IRQHandler,                /* 46  IRQ30 TIM4 global */
    I2C1_EV_IRQHandler,             /* 47  IRQ31 I2C1 event */
    I2C1_ER_IRQHandler,             /* 48  IRQ32 I2C1 error */
    I2C2_EV_IRQHandler,             /* 49  IRQ33 I2C2 event */
    I2C2_ER_IRQHandler,             /* 50  IRQ34 I2C2 error */
    SPI1_IRQHandler,                /* 51  IRQ35 SPI1 global */
    SPI2_IRQHandler,                /* 52  IRQ36 SPI2 global */
    USART1_IRQHandler,              /* 53  IRQ37 USART1 global */
    USART2_IRQHandler,              /* 54  IRQ38 USART2 global */
    0,                              /* 55  IRQ39 Reserved */
    EXTI15_10_IRQHandler,           /* 56  IRQ40 EXTI lines 15:10 */
    RTC_Alarm_IRQHandler,           /* 57  IRQ41 RTC alarm A/B via EXTI 17 */
    OTG_FS_WKUP_IRQHandler,         /* 58  IRQ42 USB OTG FS wakeup via EXTI 18 */
    0,                              /* 59  IRQ43 Reserved */
    0,                              /* 60  IRQ44 Reserved */
    0,                              /* 61  IRQ45 Reserved */
    0,                              /* 62  IRQ46 Reserved */
    DMA1_Stream7_IRQHandler,        /* 63  IRQ47 DMA1 stream 7 */
    0,                              /* 64  IRQ48 Reserved */
    SDIO_IRQHandler,                /* 65  IRQ49 SDIO global */
    TIM5_IRQHandler,                /* 66  IRQ50 TIM5 global */
    SPI3_IRQHandler,                /* 67  IRQ51 SPI3 global */
    0,                              /* 68  IRQ52 Reserved */
    0,                              /* 69  IRQ53 Reserved */
    0,                              /* 70  IRQ54 Reserved */
    0,                              /* 71  IRQ55 Reserved */
    DMA2_Stream0_IRQHandler,        /* 72  IRQ56 DMA2 stream 0 */
    DMA2_Stream1_IRQHandler,        /* 73  IRQ57 DMA2 stream 1 */
    DMA2_Stream2_IRQHandler,        /* 74  IRQ58 DMA2 stream 2 */
    DMA2_Stream3_IRQHandler,        /* 75  IRQ59 DMA2 stream 3 */
    DMA2_Stream4_IRQHandler,        /* 76  IRQ60 DMA2 stream 4 */
    0,                              /* 77  IRQ61 Reserved */
    0,                              /* 78  IRQ62 Reserved */
    0,                              /* 79  IRQ63 Reserved */
    0,                              /* 80  IRQ64 Reserved */
    0,                              /* 81  IRQ65 Reserved */
    0,                              /* 82  IRQ66 Reserved */
    OTG_FS_IRQHandler,              /* 83  IRQ67 USB OTG FS global */
    DMA2_Stream5_IRQHandler,        /* 84  IRQ68 DMA2 stream 5 */
    DMA2_Stream6_IRQHandler,        /* 85  IRQ69 DMA2 stream 6 */
    DMA2_Stream7_IRQHandler,        /* 86  IRQ70 DMA2 stream 7 */
    USART6_IRQHandler,              /* 87  IRQ71 USART6 global */
    I2C3_EV_IRQHandler,             /* 88  IRQ72 I2C3 event */
    I2C3_ER_IRQHandler,             /* 89  IRQ73 I2C3 error */
    0,                              /* 90  IRQ74 Reserved */
    0,                              /* 91  IRQ75 Reserved */
    0,                              /* 92  IRQ76 Reserved */
    0,                              /* 93  IRQ77 Reserved */
    0,                              /* 94  IRQ78 Reserved */
    0,                              /* 95  IRQ79 Reserved */
    0,                              /* 96  IRQ80 Reserved */
    FPU_IRQHandler,                 /* 97  IRQ81 FPU global */
};

/* Array size inferred from initialiser; any miscount is caught here. */
_Static_assert(sizeof(g_vectors) / sizeof(g_vectors[0]) == 98,
    "vector table must have exactly 98 entries (16 core + 82 external, RM0368 Table 38)");


/* ---------------------------------------------------------------------------
 * Default_Handler — entered by any unhandled exception or IRQ.
 * Spin forever so a debugger can identify the source via IPSR / LR.
 * --------------------------------------------------------------------------- */
void Default_Handler(void)
{
    for (;;)
        ;
}


/* ---------------------------------------------------------------------------
 * Reset_Handler — first code executed after every reset.
 *
 * 1. Enable FPU (CPACR CP10/CP11 full access) — PM0214 §4.6.1.
 * 2. Copy the .data initialisation image from FLASH (LMA _sidata) to SRAM.
 * 3. Zero the .bss segment.
 * 4. Call main().
 *
 * FPU is enabled before the .data copy so that the toolchain may freely emit
 * FP instructions for initialisers or function prologues.
 * Word-at-a-time loops rely on the linker script aligning both regions to 4
 * bytes (ALIGN(4) in stm32f401cc.ld).
 * --------------------------------------------------------------------------- */
__attribute__((noreturn))
void Reset_Handler(void)
{
    /* Enable FPU: grant full access to CP10 (FPv4-SP) and CP11 (must match).
     * DSB + ISB ensure the write is visible before any subsequent FP instruction.
     * PM0214 §4.6.1. */
    SCB->CPACR |= SCB_CPACR_FPU_FULL;
    __DSB();
    __ISB();

    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    uint32_t *bss = &_sbss;
    while (bss < &_ebss) {
        *bss++ = 0U;
    }

    main();

    for (;;)   /* main() must not return; spin to catch a rogue return */
        ;
}
