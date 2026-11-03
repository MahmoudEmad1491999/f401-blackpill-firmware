/* regs/gpio.h — STM32F401 General Purpose I/O (GPIO) register map
 *
 * Sources:
 *   RM0368 Rev 5 §8.4   GPIO registers, Table 26 (offsets 0x00–0x24)
 *   DS9716  Rev 7 §2.3  Memory map — GPIO base addresses
 *   DS9716  Rev 7 §4    Pinout and pin description (alternate function table)
 *
 * Every register offset is verified by a _Static_assert(offsetof(…)) below.
 * One gpio_regs_t struct covers all ports (A–E, H on F401); each port is a
 * separate peripheral instance at its own base address.
 * Only freestanding C11 headers (<stdint.h>, <stddef.h>) are used.
 * No CMSIS, no HAL.
 */

#ifndef REGS_GPIO_H
#define REGS_GPIO_H

#include <stddef.h>
#include <stdint.h>

/* ---------------------------------------------------------------------------
 * GPIO register layout — RM0368 §8.4 Table 26
 * --------------------------------------------------------------------------- */
typedef struct {
    volatile uint32_t MODER;    /* 0x00  Port mode register                     */
    volatile uint32_t OTYPER;   /* 0x04  Port output type register              */
    volatile uint32_t OSPEEDR;  /* 0x08  Port output speed register             */
    volatile uint32_t PUPDR;    /* 0x0C  Port pull-up/pull-down register        */
    volatile uint32_t IDR;      /* 0x10  Port input data register      (RO)     */
    volatile uint32_t ODR;      /* 0x14  Port output data register              */
    volatile uint32_t BSRR;     /* 0x18  Port bit set/reset register   (WO)     */
    volatile uint32_t LCKR;     /* 0x1C  Port configuration lock register       */
    volatile uint32_t AFR[2];   /* 0x20  AFR[0]=AFRL pins 0-7, AFR[1]=AFRH 8-15*/
} gpio_regs_t;

/* Offset assertions — RM0368 §8.4 Table 26 */
_Static_assert(offsetof(gpio_regs_t, MODER)   == 0x00U, "GPIO MODER offset");
_Static_assert(offsetof(gpio_regs_t, OTYPER)  == 0x04U, "GPIO OTYPER offset");
_Static_assert(offsetof(gpio_regs_t, OSPEEDR) == 0x08U, "GPIO OSPEEDR offset");
_Static_assert(offsetof(gpio_regs_t, PUPDR)   == 0x0CU, "GPIO PUPDR offset");
_Static_assert(offsetof(gpio_regs_t, IDR)     == 0x10U, "GPIO IDR offset");
_Static_assert(offsetof(gpio_regs_t, ODR)     == 0x14U, "GPIO ODR offset");
_Static_assert(offsetof(gpio_regs_t, BSRR)   == 0x18U, "GPIO BSRR offset");
_Static_assert(offsetof(gpio_regs_t, LCKR)   == 0x1CU, "GPIO LCKR offset");
_Static_assert(offsetof(gpio_regs_t, AFR)    == 0x20U, "GPIO AFR[0] offset");
_Static_assert(sizeof(gpio_regs_t)            == 0x28U, "gpio_regs_t total size");

/* GPIO port base addresses — DS9716 Table 2, RM0368 §2.3
 * F401 has ports A, B, C, D, E, H (no F or G in this device family). */
#define GPIOA  ((gpio_regs_t *)0x40020000UL)
#define GPIOB  ((gpio_regs_t *)0x40020400UL)
#define GPIOC  ((gpio_regs_t *)0x40020800UL)
#define GPIOD  ((gpio_regs_t *)0x40020C00UL)
#define GPIOE  ((gpio_regs_t *)0x40021000UL)
#define GPIOH  ((gpio_regs_t *)0x40021C00UL)

/* ---------------------------------------------------------------------------
 * MODER — Port mode register  (RM0368 §8.4.1)
 *
 * Two bits per pin [2n+1:2n].  Reset state: GPIOA=0xA800 0000, others=0.
 * --------------------------------------------------------------------------- */
#define GPIO_MODER_INPUT    0U  /* 00: Input (reset state for most pins)  */
#define GPIO_MODER_OUTPUT   1U  /* 01: General purpose output             */
#define GPIO_MODER_AF       2U  /* 10: Alternate function                 */
#define GPIO_MODER_ANALOG   3U  /* 11: Analog                             */

/* Build a MODER mask or value for a specific pin */
#define GPIO_MODER_PIN_Pos(n)   ((n) * 2U)
#define GPIO_MODER_PIN_Msk(n)   (3U << GPIO_MODER_PIN_Pos(n))
#define GPIO_MODER_PIN_VAL(n,v) ((uint32_t)(v) << GPIO_MODER_PIN_Pos(n))

/* ---------------------------------------------------------------------------
 * OTYPER — Output type register  (RM0368 §8.4.2)
 *
 * One bit per pin [n].  Ignored when pin is in input or analog mode.
 * --------------------------------------------------------------------------- */
#define GPIO_OTYPER_PP  0U  /* 0: Output push-pull  (reset state)        */
#define GPIO_OTYPER_OD  1U  /* 1: Output open-drain                       */

#define GPIO_OTYPER_PIN_Pos(n)   (n)
#define GPIO_OTYPER_PIN_Msk(n)   (1U << (n))
#define GPIO_OTYPER_PIN_VAL(n,v) ((uint32_t)(v) << (n))

/* ---------------------------------------------------------------------------
 * OSPEEDR — Output speed register  (RM0368 §8.4.3)
 *
 * Two bits per pin [2n+1:2n].
 * --------------------------------------------------------------------------- */
#define GPIO_OSPEEDR_LOW    0U  /* 00: Low speed (~2 MHz)                 */
#define GPIO_OSPEEDR_MED    1U  /* 01: Medium speed (~12.5–50 MHz)        */
#define GPIO_OSPEEDR_HIGH   2U  /* 10: High speed (~25–100 MHz)           */
#define GPIO_OSPEEDR_VHIGH  3U  /* 11: Very high speed (~50–200 MHz)      */

#define GPIO_OSPEEDR_PIN_Pos(n)   ((n) * 2U)
#define GPIO_OSPEEDR_PIN_Msk(n)   (3U << GPIO_OSPEEDR_PIN_Pos(n))
#define GPIO_OSPEEDR_PIN_VAL(n,v) ((uint32_t)(v) << GPIO_OSPEEDR_PIN_Pos(n))

/* ---------------------------------------------------------------------------
 * PUPDR — Pull-up/pull-down register  (RM0368 §8.4.4)
 *
 * Two bits per pin [2n+1:2n].
 * --------------------------------------------------------------------------- */
#define GPIO_PUPDR_NONE  0U  /* 00: No pull-up or pull-down (reset state) */
#define GPIO_PUPDR_UP    1U  /* 01: Pull-up                                */
#define GPIO_PUPDR_DOWN  2U  /* 10: Pull-down                              */

#define GPIO_PUPDR_PIN_Pos(n)   ((n) * 2U)
#define GPIO_PUPDR_PIN_Msk(n)   (3U << GPIO_PUPDR_PIN_Pos(n))
#define GPIO_PUPDR_PIN_VAL(n,v) ((uint32_t)(v) << GPIO_PUPDR_PIN_Pos(n))

/* ---------------------------------------------------------------------------
 * IDR / ODR — Input/Output data registers  (RM0368 §8.4.5, §8.4.6)
 *
 * One bit per pin [n].  IDR is read-only; ODR is read/write.
 * --------------------------------------------------------------------------- */
#define GPIO_PIN(n)  (1U << (n))

/* ---------------------------------------------------------------------------
 * BSRR — Bit set/reset register  (RM0368 §8.4.7)
 *
 * Write-only atomic set/reset: upper 16 bits reset (BR[n] = bit [n+16]),
 * lower 16 bits set (BS[n] = bit [n]).  Simultaneous BS+BR: BS wins.
 * --------------------------------------------------------------------------- */
#define GPIO_BSRR_BS(n)  (1U << (n))         /* Set pin n                   */
#define GPIO_BSRR_BR(n)  (1U << ((n) + 16U)) /* Reset pin n                 */

/* ---------------------------------------------------------------------------
 * LCKR — Configuration lock register  (RM0368 §8.4.8)
 *
 * LCKK (bit 16) is the lock key; pins are locked with the lock sequence.
 * --------------------------------------------------------------------------- */
#define GPIO_LCKR_LCKK  (1U << 16)  /* Lock key bit                        */
#define GPIO_LCKR_LCK(n) (1U << (n)) /* Lock pin n configuration           */

/* ---------------------------------------------------------------------------
 * AFR[0]/AFR[1] — Alternate function registers  (RM0368 §8.4.9, §8.4.10)
 *
 * AFR[0] (AFRL) covers pins 0–7, 4 bits each [4n+3:4n].
 * AFR[1] (AFRH) covers pins 8–15, 4 bits each [4(n-8)+3:4(n-8)].
 * AF values 0–15 select the alternate function per DS9716 Table 9.
 * --------------------------------------------------------------------------- */
#define GPIO_AF0   0U   /* System (SWD, MCO, …)                            */
#define GPIO_AF1   1U   /* TIM1/TIM2                                        */
#define GPIO_AF2   2U   /* TIM3/TIM4/TIM5                                   */
#define GPIO_AF3   3U   /* TIM9/TIM10/TIM11                                 */
#define GPIO_AF4   4U   /* I2C1/I2C2/I2C3                                   */
#define GPIO_AF5   5U   /* SPI1/SPI2/SPI3/SPI4                              */
#define GPIO_AF6   6U   /* SPI3                                              */
#define GPIO_AF7   7U   /* USART1/USART2                                     */
#define GPIO_AF8   8U   /* USART6                                            */
#define GPIO_AF9   9U   /* I2C2/I2C3                                         */
#define GPIO_AF10  10U  /* OTG_FS                                            */
#define GPIO_AF12  12U  /* SDIO                                              */
#define GPIO_AF15  15U  /* EVENTOUT                                          */

/* Helper: build the 4-bit field for pin n (0–7) inside AFRL (AFR[0]) */
#define GPIO_AFRL_PIN_Pos(n)   ((n) * 4U)
#define GPIO_AFRL_PIN_Msk(n)   (0xFU << GPIO_AFRL_PIN_Pos(n))
#define GPIO_AFRL_PIN_VAL(n,f) ((uint32_t)(f) << GPIO_AFRL_PIN_Pos(n))

/* Helper: build the 4-bit field for pin n (8–15) inside AFRH (AFR[1]) */
#define GPIO_AFRH_PIN_Pos(n)   (((n) - 8U) * 4U)
#define GPIO_AFRH_PIN_Msk(n)   (0xFU << GPIO_AFRH_PIN_Pos(n))
#define GPIO_AFRH_PIN_VAL(n,f) ((uint32_t)(f) << GPIO_AFRH_PIN_Pos(n))

#endif /* REGS_GPIO_H */
