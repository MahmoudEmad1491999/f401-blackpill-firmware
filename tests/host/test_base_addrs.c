/* tests/host/test_base_addrs.c — compile-time check: peripheral base addresses
 * vs. DS9716 Rev 7 §2.3 Table 2 (STM32F401xC/D/E memory map).
 *
 * Every peripheral present in DS9716 Table 2 is listed here as a DS9716_xxx_BASE
 * constant (the authoritative reference).  For peripherals that already have a
 * register header the constant is cross-checked against the header's _BASE macro
 * with _Static_assert.  Rows marked "no header yet" are documentation-only;
 * add an assert here when the matching regs/ header is written.
 *
 * Sources:
 *   DS9716 Rev 7 §2.3 Table 2  — STM32F401xC/D/E bus matrix / memory map
 *   PM0214 Rev 5 §2.1 Table 2-1 — Cortex-M4 Private Peripheral Bus (PPB)
 */

#include <stdint.h>
#include "regs/scb.h"
#include "regs/rcc.h"
#include "regs/gpio.h"

/* -------------------------------------------------------------------------
 * DS9716 Rev 7 §2.3 Table 2 — reference base addresses, full STM32F401 map.
 * ---------------------------------------------------------------------- */

/* Cortex-M4 Private Peripheral Bus (PM0214 §2.1 Table 2-1) */
#define DS9716_ITM_BASE        0xE0000000UL  /* Instrumentation Trace Macrocell */
#define DS9716_DWT_BASE        0xE0001000UL  /* Data Watchpoint and Trace       */
#define DS9716_SYSTICK_BASE    0xE000E010UL  /* SysTick timer                   */
#define DS9716_NVIC_BASE       0xE000E100UL  /* Nested Vectored Interrupt Ctrl  */
#define DS9716_SCB_BASE        0xE000ED00UL  /* System Control Block            */

/* AHB2 bus */
#define DS9716_USB_OTG_FS_BASE 0x50000000UL  /* USB OTG FS                     */

/* AHB1 bus */
#define DS9716_DMA2_BASE       0x40026400UL  /* DMA2                            */
#define DS9716_DMA1_BASE       0x40026000UL  /* DMA1                            */
#define DS9716_FLASH_BASE      0x40023C00UL  /* Flash interface registers       */
#define DS9716_RCC_BASE        0x40023800UL  /* Reset and Clock Control         */
#define DS9716_CRC_BASE        0x40023000UL  /* CRC calculation unit            */
#define DS9716_GPIOH_BASE      0x40021C00UL  /* GPIO port H                    */
#define DS9716_GPIOE_BASE      0x40021000UL  /* GPIO port E                    */
#define DS9716_GPIOD_BASE      0x40020C00UL  /* GPIO port D                    */
#define DS9716_GPIOC_BASE      0x40020800UL  /* GPIO port C                    */
#define DS9716_GPIOB_BASE      0x40020400UL  /* GPIO port B                    */
#define DS9716_GPIOA_BASE      0x40020000UL  /* GPIO port A                    */

/* APB2 bus */
#define DS9716_TIM11_BASE      0x40014800UL  /* TIM11 general-purpose timer    */
#define DS9716_TIM10_BASE      0x40014400UL  /* TIM10 general-purpose timer    */
#define DS9716_TIM9_BASE       0x40014000UL  /* TIM9 general-purpose timer     */
#define DS9716_EXTI_BASE       0x40013C00UL  /* External interrupt controller  */
#define DS9716_SYSCFG_BASE     0x40013800UL  /* System configuration           */
#define DS9716_SPI4_BASE       0x40013400UL  /* SPI4                           */
#define DS9716_SPI1_BASE       0x40013000UL  /* SPI1 / I2S1                    */
#define DS9716_SDIO_BASE       0x40012C00UL  /* SDIO                           */
#define DS9716_ADC1_BASE       0x40012000UL  /* ADC1                           */
#define DS9716_USART6_BASE     0x40011400UL  /* USART6                         */
#define DS9716_USART1_BASE     0x40011000UL  /* USART1                         */
#define DS9716_TIM1_BASE       0x40010000UL  /* TIM1 advanced-control timer    */

/* APB1 bus */
#define DS9716_PWR_BASE        0x40007000UL  /* Power control                  */
#define DS9716_I2C3_BASE       0x40005C00UL  /* I2C3                           */
#define DS9716_I2C2_BASE       0x40005800UL  /* I2C2                           */
#define DS9716_I2C1_BASE       0x40005400UL  /* I2C1                           */
#define DS9716_USART2_BASE     0x40004400UL  /* USART2                         */
#define DS9716_I2S3EXT_BASE    0x40004000UL  /* I2S3ext                        */
#define DS9716_SPI3_BASE       0x40003C00UL  /* SPI3 / I2S3                    */
#define DS9716_SPI2_BASE       0x40003800UL  /* SPI2 / I2S2                    */
#define DS9716_I2S2EXT_BASE    0x40003400UL  /* I2S2ext                        */
#define DS9716_IWDG_BASE       0x40003000UL  /* Independent watchdog           */
#define DS9716_WWDG_BASE       0x40002C00UL  /* Window watchdog                */
#define DS9716_RTC_BASE        0x40002800UL  /* RTC and backup registers       */
#define DS9716_TIM5_BASE       0x40000C00UL  /* TIM5 general-purpose timer     */
#define DS9716_TIM4_BASE       0x40000800UL  /* TIM4 general-purpose timer     */
#define DS9716_TIM3_BASE       0x40000400UL  /* TIM3 general-purpose timer     */
#define DS9716_TIM2_BASE       0x40000000UL  /* TIM2 general-purpose timer     */

/* -------------------------------------------------------------------------
 * Cross-checks: header _BASE macros vs. DS9716 reference values above.
 * Add a new _Static_assert here when the matching regs/ header is written.
 * ---------------------------------------------------------------------- */

/* regs/scb.h (PM0214 §2.1 Table 2-1) */
_Static_assert(SCB_BASE == DS9716_SCB_BASE,
               "SCB_BASE must match DS9716 §2.3 / PM0214 §2.1");

/* regs/rcc.h */
_Static_assert(RCC_BASE == DS9716_RCC_BASE,
               "RCC_BASE must match DS9716 §2.3 Table 2");

/* regs/gpio.h */
_Static_assert(GPIOA_BASE == DS9716_GPIOA_BASE,
               "GPIOA_BASE must match DS9716 §2.3 Table 2");
_Static_assert(GPIOB_BASE == DS9716_GPIOB_BASE,
               "GPIOB_BASE must match DS9716 §2.3 Table 2");
_Static_assert(GPIOC_BASE == DS9716_GPIOC_BASE,
               "GPIOC_BASE must match DS9716 §2.3 Table 2");
_Static_assert(GPIOD_BASE == DS9716_GPIOD_BASE,
               "GPIOD_BASE must match DS9716 §2.3 Table 2");
_Static_assert(GPIOE_BASE == DS9716_GPIOE_BASE,
               "GPIOE_BASE must match DS9716 §2.3 Table 2");
_Static_assert(GPIOH_BASE == DS9716_GPIOH_BASE,
               "GPIOH_BASE must match DS9716 §2.3 Table 2");

/* Peripherals with no header yet — add a _Static_assert when written:
 *   ITM, DWT, SysTick, NVIC (core/cpu.h or regs/systick.h, regs/nvic.h, regs/dwt.h)
 *   USB OTG FS (regs/usb_otg.h)
 *   DMA1, DMA2 (regs/dma.h)
 *   Flash interface (regs/flash.h)
 *   CRC (regs/crc.h)
 *   TIM1..TIM11 (regs/tim.h)
 *   USART1, USART2, USART6 (regs/usart.h)
 *   SPI1..SPI4 (regs/spi.h)
 *   I2C1..I2C3 (regs/i2c.h)
 *   ADC1 (regs/adc.h)
 *   EXTI (regs/exti.h), SYSCFG (regs/syscfg.h)
 *   RTC (regs/rtc.h), IWDG (regs/iwdg.h), WWDG, PWR (regs/pwr.h)
 *   SDIO
 */

int main(void)
{
    return 0;
}
