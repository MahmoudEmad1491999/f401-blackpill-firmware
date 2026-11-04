/* regs/rcc.h — STM32F401 Reset and Clock Control (RCC) register map
 *
 * Sources:
 *   RM0368 Rev 5 §6.3   RCC registers, Table 11 (offsets 0x00–0x8C)
 *   DS9716  Rev 7 §2.3  Memory map — RCC base 0x40023800
 *
 * Every register offset is verified by a _Static_assert(offsetof(…)) below.
 * Only freestanding C11 headers (<stdint.h>, <stddef.h>) are used.
 * No CMSIS, no HAL.
 */

#ifndef REGS_RCC_H
#define REGS_RCC_H

#include <stddef.h>
#include <stdint.h>

/* ---------------------------------------------------------------------------
 * RCC register layout — RM0368 §6.3 Table 11
 *
 * Gaps between register groups (2 words each at 0x18, 0x28, 0x38, 0x48, 0x58,
 * 0x68, 0x78) and one word at 0x88 are padded explicitly so offsetof holds.
 * --------------------------------------------------------------------------- */
typedef struct {
    volatile uint32_t CR;          /* 0x00  Clock control register                      */
    volatile uint32_t PLLCFGR;    /* 0x04  PLL configuration register                  */
    volatile uint32_t CFGR;        /* 0x08  Clock configuration register                */
    volatile uint32_t CIR;         /* 0x0C  Clock interrupt register                    */
    volatile uint32_t AHB1RSTR;   /* 0x10  AHB1 peripheral reset register              */
    volatile uint32_t AHB2RSTR;   /* 0x14  AHB2 peripheral reset register              */
    uint32_t          _res0[2];    /* 0x18–0x1C  Reserved                               */
    volatile uint32_t APB1RSTR;   /* 0x20  APB1 peripheral reset register              */
    volatile uint32_t APB2RSTR;   /* 0x24  APB2 peripheral reset register              */
    uint32_t          _res1[2];    /* 0x28–0x2C  Reserved                               */
    volatile uint32_t AHB1ENR;    /* 0x30  AHB1 peripheral clock enable register       */
    volatile uint32_t AHB2ENR;    /* 0x34  AHB2 peripheral clock enable register       */
    uint32_t          _res2[2];    /* 0x38–0x3C  Reserved                               */
    volatile uint32_t APB1ENR;    /* 0x40  APB1 peripheral clock enable register       */
    volatile uint32_t APB2ENR;    /* 0x44  APB2 peripheral clock enable register       */
    uint32_t          _res3[2];    /* 0x48–0x4C  Reserved                               */
    volatile uint32_t AHB1LPENR;  /* 0x50  AHB1 clock enable in low-power mode         */
    volatile uint32_t AHB2LPENR;  /* 0x54  AHB2 clock enable in low-power mode         */
    uint32_t          _res4[2];    /* 0x58–0x5C  Reserved                               */
    volatile uint32_t APB1LPENR;  /* 0x60  APB1 clock enable in low-power mode         */
    volatile uint32_t APB2LPENR;  /* 0x64  APB2 clock enable in low-power mode         */
    uint32_t          _res5[2];    /* 0x68–0x6C  Reserved                               */
    volatile uint32_t BDCR;        /* 0x70  Backup domain control register              */
    volatile uint32_t CSR;         /* 0x74  Clock control & status register             */
    uint32_t          _res6[2];    /* 0x78–0x7C  Reserved                               */
    volatile uint32_t SSCGR;       /* 0x80  Spread spectrum clock generation register  */
    volatile uint32_t PLLI2SCFGR; /* 0x84  PLLI2S configuration register              */
    uint32_t          _res7[1];    /* 0x88  Reserved                                    */
    volatile uint32_t DCKCFGR;    /* 0x8C  Dedicated clocks configuration register    */
} rcc_regs_t;

/* Offset assertions — RM0368 §6.3 Table 11 */
_Static_assert(offsetof(rcc_regs_t, CR)         == 0x00U, "RCC CR offset");
_Static_assert(offsetof(rcc_regs_t, PLLCFGR)   == 0x04U, "RCC PLLCFGR offset");
_Static_assert(offsetof(rcc_regs_t, CFGR)       == 0x08U, "RCC CFGR offset");
_Static_assert(offsetof(rcc_regs_t, CIR)        == 0x0CU, "RCC CIR offset");
_Static_assert(offsetof(rcc_regs_t, AHB1RSTR)  == 0x10U, "RCC AHB1RSTR offset");
_Static_assert(offsetof(rcc_regs_t, AHB2RSTR)  == 0x14U, "RCC AHB2RSTR offset");
_Static_assert(offsetof(rcc_regs_t, APB1RSTR)  == 0x20U, "RCC APB1RSTR offset");
_Static_assert(offsetof(rcc_regs_t, APB2RSTR)  == 0x24U, "RCC APB2RSTR offset");
_Static_assert(offsetof(rcc_regs_t, AHB1ENR)   == 0x30U, "RCC AHB1ENR offset");
_Static_assert(offsetof(rcc_regs_t, AHB2ENR)   == 0x34U, "RCC AHB2ENR offset");
_Static_assert(offsetof(rcc_regs_t, APB1ENR)   == 0x40U, "RCC APB1ENR offset");
_Static_assert(offsetof(rcc_regs_t, APB2ENR)   == 0x44U, "RCC APB2ENR offset");
_Static_assert(offsetof(rcc_regs_t, AHB1LPENR) == 0x50U, "RCC AHB1LPENR offset");
_Static_assert(offsetof(rcc_regs_t, AHB2LPENR) == 0x54U, "RCC AHB2LPENR offset");
_Static_assert(offsetof(rcc_regs_t, APB1LPENR) == 0x60U, "RCC APB1LPENR offset");
_Static_assert(offsetof(rcc_regs_t, APB2LPENR) == 0x64U, "RCC APB2LPENR offset");
_Static_assert(offsetof(rcc_regs_t, BDCR)       == 0x70U, "RCC BDCR offset");
_Static_assert(offsetof(rcc_regs_t, CSR)        == 0x74U, "RCC CSR offset");
_Static_assert(offsetof(rcc_regs_t, SSCGR)      == 0x80U, "RCC SSCGR offset");
_Static_assert(offsetof(rcc_regs_t, PLLI2SCFGR)== 0x84U, "RCC PLLI2SCFGR offset");
_Static_assert(offsetof(rcc_regs_t, DCKCFGR)   == 0x8CU, "RCC DCKCFGR offset");
_Static_assert(sizeof(rcc_regs_t)               == 0x90U, "rcc_regs_t total size");

/* RCC base address — DS9716 §2.3 Table 2, RM0368 §2.3 */
#define RCC_BASE  0x40023800UL
#define RCC       ((rcc_regs_t *)RCC_BASE)

/* ---------------------------------------------------------------------------
 * CR — Clock control register  (RM0368 §6.3.1)
 * --------------------------------------------------------------------------- */
#define RCC_CR_PLLI2SRDY    (1U << 27)  /* PLLI2S clock ready flag (RO)       */
#define RCC_CR_PLLI2SON     (1U << 26)  /* PLLI2S enable                       */
#define RCC_CR_PLLRDY       (1U << 25)  /* Main PLL clock ready flag (RO)     */
#define RCC_CR_PLLON        (1U << 24)  /* Main PLL enable                     */
#define RCC_CR_CSSON        (1U << 19)  /* Clock security system enable        */
#define RCC_CR_HSEBYP       (1U << 18)  /* HSE clock bypass                    */
#define RCC_CR_HSERDY       (1U << 17)  /* HSE clock ready flag (RO)           */
#define RCC_CR_HSEON        (1U << 16)  /* HSE clock enable                    */
#define RCC_CR_HSICAL_Pos    8U
#define RCC_CR_HSICAL_Msk   (0xFFU << RCC_CR_HSICAL_Pos)  /* HSI calibration (RO) */
#define RCC_CR_HSITRIM_Pos   3U
#define RCC_CR_HSITRIM_Msk  (0x1FU << RCC_CR_HSITRIM_Pos) /* HSI trimming        */
#define RCC_CR_HSIRDY       (1U <<  1)  /* HSI clock ready flag (RO)           */
#define RCC_CR_HSION        (1U <<  0)  /* HSI clock enable                    */

/* ---------------------------------------------------------------------------
 * PLLCFGR — PLL configuration register  (RM0368 §6.3.2)
 *
 * All PLL fields are write-protected when PLL is ON; program before enabling.
 *   PLLSRC = 0 → HSI/2; PLLSRC = 1 → HSE
 *   fVCO  = fIN × (PLLN / PLLM)          ; PLLM ∈ [2,63], PLLN ∈ [50,432]
 *   fSYS  = fVCO / PLLP                  ; PLLP ∈ {2,4,6,8}
 *   fUSB  = fVCO / PLLQ                  ; PLLQ ∈ [2,15]
 * --------------------------------------------------------------------------- */
#define RCC_PLLCFGR_PLLQ_Pos    24U
#define RCC_PLLCFGR_PLLQ_Msk   (0xFU << RCC_PLLCFGR_PLLQ_Pos)
#define RCC_PLLCFGR_PLLSRC      (1U << 22)  /* 0 = HSI, 1 = HSE              */
#define RCC_PLLCFGR_PLLSRC_HSE  (1U << 22)
#define RCC_PLLCFGR_PLLSRC_HSI  (0U << 22)
#define RCC_PLLCFGR_PLLP_Pos    16U
#define RCC_PLLCFGR_PLLP_Msk   (3U << RCC_PLLCFGR_PLLP_Pos)
#define RCC_PLLCFGR_PLLP_DIV2  (0U << RCC_PLLCFGR_PLLP_Pos)  /* PLLP = /2  */
#define RCC_PLLCFGR_PLLP_DIV4  (1U << RCC_PLLCFGR_PLLP_Pos)  /* PLLP = /4  */
#define RCC_PLLCFGR_PLLP_DIV6  (2U << RCC_PLLCFGR_PLLP_Pos)  /* PLLP = /6  */
#define RCC_PLLCFGR_PLLP_DIV8  (3U << RCC_PLLCFGR_PLLP_Pos)  /* PLLP = /8  */
#define RCC_PLLCFGR_PLLN_Pos     6U
#define RCC_PLLCFGR_PLLN_Msk   (0x1FFU << RCC_PLLCFGR_PLLN_Pos)
#define RCC_PLLCFGR_PLLM_Pos     0U
#define RCC_PLLCFGR_PLLM_Msk   (0x3FU << RCC_PLLCFGR_PLLM_Pos)

/* ---------------------------------------------------------------------------
 * CFGR — Clock configuration register  (RM0368 §6.3.3)
 * --------------------------------------------------------------------------- */
#define RCC_CFGR_MCO2_Pos       30U
#define RCC_CFGR_MCO2_Msk      (3U << RCC_CFGR_MCO2_Pos)
#define RCC_CFGR_MCO2PRE_Pos    27U
#define RCC_CFGR_MCO2PRE_Msk   (7U << RCC_CFGR_MCO2PRE_Pos)
#define RCC_CFGR_MCO1PRE_Pos    24U
#define RCC_CFGR_MCO1PRE_Msk   (7U << RCC_CFGR_MCO1PRE_Pos)
#define RCC_CFGR_MCO1_Pos       21U
#define RCC_CFGR_MCO1_Msk      (3U << RCC_CFGR_MCO1_Pos)
#define RCC_CFGR_PPRE2_Pos      13U
#define RCC_CFGR_PPRE2_Msk     (7U << RCC_CFGR_PPRE2_Pos)
#define RCC_CFGR_PPRE2_DIV1    (0U << RCC_CFGR_PPRE2_Pos)  /* APB2 = HCLK/1   */
#define RCC_CFGR_PPRE2_DIV2    (4U << RCC_CFGR_PPRE2_Pos)  /* APB2 = HCLK/2   */
#define RCC_CFGR_PPRE2_DIV4    (5U << RCC_CFGR_PPRE2_Pos)  /* APB2 = HCLK/4   */
#define RCC_CFGR_PPRE2_DIV8    (6U << RCC_CFGR_PPRE2_Pos)  /* APB2 = HCLK/8   */
#define RCC_CFGR_PPRE2_DIV16   (7U << RCC_CFGR_PPRE2_Pos)  /* APB2 = HCLK/16  */
#define RCC_CFGR_PPRE1_Pos      10U
#define RCC_CFGR_PPRE1_Msk     (7U << RCC_CFGR_PPRE1_Pos)
#define RCC_CFGR_PPRE1_DIV1    (0U << RCC_CFGR_PPRE1_Pos)  /* APB1 = HCLK/1   */
#define RCC_CFGR_PPRE1_DIV2    (4U << RCC_CFGR_PPRE1_Pos)  /* APB1 = HCLK/2   */
#define RCC_CFGR_PPRE1_DIV4    (5U << RCC_CFGR_PPRE1_Pos)  /* APB1 = HCLK/4   */
#define RCC_CFGR_PPRE1_DIV8    (6U << RCC_CFGR_PPRE1_Pos)  /* APB1 = HCLK/8   */
#define RCC_CFGR_PPRE1_DIV16   (7U << RCC_CFGR_PPRE1_Pos)  /* APB1 = HCLK/16  */
#define RCC_CFGR_HPRE_Pos        4U
#define RCC_CFGR_HPRE_Msk      (0xFU << RCC_CFGR_HPRE_Pos)
#define RCC_CFGR_HPRE_DIV1     (0U  << RCC_CFGR_HPRE_Pos)  /* HCLK = SYSCLK/1  */
#define RCC_CFGR_HPRE_DIV2     (8U  << RCC_CFGR_HPRE_Pos)  /* HCLK = SYSCLK/2  */
#define RCC_CFGR_HPRE_DIV4     (9U  << RCC_CFGR_HPRE_Pos)  /* HCLK = SYSCLK/4  */
#define RCC_CFGR_HPRE_DIV8     (10U << RCC_CFGR_HPRE_Pos)  /* HCLK = SYSCLK/8  */
#define RCC_CFGR_HPRE_DIV16    (11U << RCC_CFGR_HPRE_Pos)  /* HCLK = SYSCLK/16 */
#define RCC_CFGR_HPRE_DIV64    (12U << RCC_CFGR_HPRE_Pos)  /* HCLK = SYSCLK/64 */
#define RCC_CFGR_HPRE_DIV128   (13U << RCC_CFGR_HPRE_Pos)  /* HCLK = SYSCLK/128*/
#define RCC_CFGR_HPRE_DIV256   (14U << RCC_CFGR_HPRE_Pos)  /* HCLK = SYSCLK/256*/
#define RCC_CFGR_HPRE_DIV512   (15U << RCC_CFGR_HPRE_Pos)  /* HCLK = SYSCLK/512*/
#define RCC_CFGR_SWS_Pos         2U
#define RCC_CFGR_SWS_Msk       (3U << RCC_CFGR_SWS_Pos)    /* System clock status (RO) */
#define RCC_CFGR_SWS_HSI        (0U << RCC_CFGR_SWS_Pos)
#define RCC_CFGR_SWS_HSE        (1U << RCC_CFGR_SWS_Pos)
#define RCC_CFGR_SWS_PLL        (2U << RCC_CFGR_SWS_Pos)
#define RCC_CFGR_SW_Pos          0U
#define RCC_CFGR_SW_Msk         (3U << RCC_CFGR_SW_Pos)     /* System clock switch */
#define RCC_CFGR_SW_HSI         (0U << RCC_CFGR_SW_Pos)
#define RCC_CFGR_SW_HSE         (1U << RCC_CFGR_SW_Pos)
#define RCC_CFGR_SW_PLL         (2U << RCC_CFGR_SW_Pos)

/* ---------------------------------------------------------------------------
 * AHB1ENR — AHB1 peripheral clock enable  (RM0368 §6.3.9)
 * --------------------------------------------------------------------------- */
#define RCC_AHB1ENR_DMA2EN      (1U <<  22)  /* DMA2 clock enable             */
#define RCC_AHB1ENR_DMA1EN      (1U <<  21)  /* DMA1 clock enable             */
#define RCC_AHB1ENR_CRCEN       (1U <<  12)  /* CRC clock enable              */
#define RCC_AHB1ENR_GPIOHEN     (1U <<   7)  /* GPIOH clock enable            */
#define RCC_AHB1ENR_GPIOEEN     (1U <<   4)  /* GPIOE clock enable            */
#define RCC_AHB1ENR_GPIODEN     (1U <<   3)  /* GPIOD clock enable            */
#define RCC_AHB1ENR_GPIOCEN     (1U <<   2)  /* GPIOC clock enable            */
#define RCC_AHB1ENR_GPIOBEN     (1U <<   1)  /* GPIOB clock enable            */
#define RCC_AHB1ENR_GPIOAEN     (1U <<   0)  /* GPIOA clock enable            */

/* ---------------------------------------------------------------------------
 * AHB2ENR — AHB2 peripheral clock enable  (RM0368 §6.3.10)
 * --------------------------------------------------------------------------- */
#define RCC_AHB2ENR_OTGFSEN     (1U <<   7)  /* USB OTG FS clock enable       */

/* ---------------------------------------------------------------------------
 * APB1ENR — APB1 peripheral clock enable  (RM0368 §6.3.11)
 * --------------------------------------------------------------------------- */
#define RCC_APB1ENR_PWREN       (1U <<  28)  /* Power interface clock enable  */
#define RCC_APB1ENR_I2C3EN      (1U <<  23)  /* I2C3 clock enable             */
#define RCC_APB1ENR_I2C2EN      (1U <<  22)  /* I2C2 clock enable             */
#define RCC_APB1ENR_I2C1EN      (1U <<  21)  /* I2C1 clock enable             */
#define RCC_APB1ENR_USART2EN    (1U <<  17)  /* USART2 clock enable           */
#define RCC_APB1ENR_SPI3EN      (1U <<  15)  /* SPI3 clock enable             */
#define RCC_APB1ENR_SPI2EN      (1U <<  14)  /* SPI2 clock enable             */
#define RCC_APB1ENR_WWDGEN      (1U <<  11)  /* Window watchdog clock enable  */
#define RCC_APB1ENR_TIM5EN      (1U <<   3)  /* TIM5 clock enable             */
#define RCC_APB1ENR_TIM4EN      (1U <<   2)  /* TIM4 clock enable             */
#define RCC_APB1ENR_TIM3EN      (1U <<   1)  /* TIM3 clock enable             */
#define RCC_APB1ENR_TIM2EN      (1U <<   0)  /* TIM2 clock enable             */

/* ---------------------------------------------------------------------------
 * APB2ENR — APB2 peripheral clock enable  (RM0368 §6.3.12)
 * --------------------------------------------------------------------------- */
#define RCC_APB2ENR_TIM11EN     (1U <<  18)  /* TIM11 clock enable            */
#define RCC_APB2ENR_TIM10EN     (1U <<  17)  /* TIM10 clock enable            */
#define RCC_APB2ENR_TIM9EN      (1U <<  16)  /* TIM9 clock enable             */
#define RCC_APB2ENR_SYSCFGEN    (1U <<  14)  /* SYSCFG clock enable           */
#define RCC_APB2ENR_SPI4EN      (1U <<  13)  /* SPI4 clock enable             */
#define RCC_APB2ENR_SPI1EN      (1U <<  12)  /* SPI1 clock enable             */
#define RCC_APB2ENR_ADC1EN      (1U <<   8)  /* ADC1 clock enable             */
#define RCC_APB2ENR_USART6EN    (1U <<   5)  /* USART6 clock enable           */
#define RCC_APB2ENR_USART1EN    (1U <<   4)  /* USART1 clock enable           */
#define RCC_APB2ENR_TIM1EN      (1U <<   0)  /* TIM1 clock enable             */

/* ---------------------------------------------------------------------------
 * BDCR — Backup domain control register  (RM0368 §6.3.17)
 * --------------------------------------------------------------------------- */
#define RCC_BDCR_BDRST          (1U <<  16)  /* Backup domain software reset  */
#define RCC_BDCR_RTCEN          (1U <<  15)  /* RTC clock enable              */
#define RCC_BDCR_RTCSEL_Pos      8U
#define RCC_BDCR_RTCSEL_Msk    (3U << RCC_BDCR_RTCSEL_Pos)
#define RCC_BDCR_RTCSEL_LSE    (1U << RCC_BDCR_RTCSEL_Pos)  /* RTC = LSE     */
#define RCC_BDCR_RTCSEL_LSI    (2U << RCC_BDCR_RTCSEL_Pos)  /* RTC = LSI     */
#define RCC_BDCR_RTCSEL_HSE    (3U << RCC_BDCR_RTCSEL_Pos)  /* RTC = HSE/div */
#define RCC_BDCR_LSEBYP         (1U <<   2)  /* External low-speed bypass     */
#define RCC_BDCR_LSERDY         (1U <<   1)  /* LSE clock ready flag (RO)     */
#define RCC_BDCR_LSEON          (1U <<   0)  /* LSE oscillator enable         */

/* ---------------------------------------------------------------------------
 * CSR — Clock control & status register  (RM0368 §6.3.18)
 * --------------------------------------------------------------------------- */
#define RCC_CSR_LPWRRSTF        (1U <<  31)  /* Low-power reset flag          */
#define RCC_CSR_WWDGRSTF        (1U <<  30)  /* Window watchdog reset flag    */
#define RCC_CSR_IWDGRSTF        (1U <<  29)  /* Independent watchdog reset    */
#define RCC_CSR_SFTRSTF         (1U <<  28)  /* Software reset flag           */
#define RCC_CSR_PORRSTF         (1U <<  27)  /* POR/PDR reset flag            */
#define RCC_CSR_PINRSTF         (1U <<  26)  /* PIN reset flag                */
#define RCC_CSR_BORRSTF         (1U <<  25)  /* BOR reset flag                */
#define RCC_CSR_RMVF            (1U <<  24)  /* Remove reset flags            */
#define RCC_CSR_LSIRDY          (1U <<   1)  /* LSI clock ready flag (RO)     */
#define RCC_CSR_LSION           (1U <<   0)  /* LSI oscillator enable         */

#endif /* REGS_RCC_H */
