/* tests/host/test_rcc.c — compile-time checks for regs/rcc.h
 *
 * What this tests:
 *   - The header parses cleanly under a native C11 compiler.
 *   - Include-guard works (double inclusion is silent).
 *   - Every _Static_assert(offsetof(…)) in the header passes.
 *   - Struct total size is 0x90 bytes (DCKCFGR at 0x08C + 4).
 *   - Key bit-field constants encode the values mandated by RM0368.
 *
 * What this does NOT test:
 *   Hardware register access — that requires a Cortex-M4 and will be covered
 *   by Renode Robot tests.
 */

#include <stdint.h>
#include "regs/rcc.h"
#include "regs/rcc.h"  /* double-include guard: second inclusion must be silent */

/* Struct total size: DCKCFGR at offset 0x08C, sizeof(uint32_t)=4 → 0x90 */
_Static_assert(sizeof(rcc_regs_t) == 0x90U, "rcc_regs_t total size must be 0x90");

/* CR bit positions — RM0368 §6.3.1 */
_Static_assert(RCC_CR_HSION    == (1U <<  0), "CR HSION at bit 0");
_Static_assert(RCC_CR_HSIRDY   == (1U <<  1), "CR HSIRDY at bit 1");
_Static_assert(RCC_CR_HSEON    == (1U << 16), "CR HSEON at bit 16");
_Static_assert(RCC_CR_HSERDY   == (1U << 17), "CR HSERDY at bit 17");
_Static_assert(RCC_CR_PLLON    == (1U << 24), "CR PLLON at bit 24");
_Static_assert(RCC_CR_PLLRDY   == (1U << 25), "CR PLLRDY at bit 25");

/* PLLCFGR — RM0368 §6.3.2 */
_Static_assert(RCC_PLLCFGR_PLLSRC_HSE == (1U << 22), "PLLCFGR PLLSRC_HSE at bit 22");
_Static_assert(RCC_PLLCFGR_PLLSRC_HSI == 0U,          "PLLCFGR PLLSRC_HSI = 0");
_Static_assert(RCC_PLLCFGR_PLLP_DIV2  == (0U << 16),  "PLLCFGR PLLP_DIV2 encoding");
_Static_assert(RCC_PLLCFGR_PLLP_DIV4  == (1U << 16),  "PLLCFGR PLLP_DIV4 encoding");
_Static_assert(RCC_PLLCFGR_PLLM_Msk   == 0x3FU,       "PLLCFGR PLLM mask [5:0]");
_Static_assert(RCC_PLLCFGR_PLLN_Msk   == (0x1FFU << 6), "PLLCFGR PLLN mask [14:6]");

/* CFGR SW/SWS — RM0368 §6.3.3 */
_Static_assert(RCC_CFGR_SW_HSI  == 0U, "CFGR SW_HSI = 0b00");
_Static_assert(RCC_CFGR_SW_HSE  == 1U, "CFGR SW_HSE = 0b01");
_Static_assert(RCC_CFGR_SW_PLL  == 2U, "CFGR SW_PLL = 0b10");
_Static_assert(RCC_CFGR_SWS_PLL == (2U << 2), "CFGR SWS_PLL at bits [3:2]");

/* CFGR prescaler encodings for 84 MHz use-case (AHB/1, APB1/2, APB2/1) */
_Static_assert(RCC_CFGR_HPRE_DIV1  == (0U  << 4), "CFGR HPRE_DIV1");
_Static_assert(RCC_CFGR_PPRE1_DIV2 == (4U  << 10), "CFGR PPRE1_DIV2");
_Static_assert(RCC_CFGR_PPRE2_DIV1 == (0U  << 13), "CFGR PPRE2_DIV1");

/* AHB1ENR GPIO enable bits — RM0368 §6.3.9 */
_Static_assert(RCC_AHB1ENR_GPIOAEN == (1U <<  0), "AHB1ENR GPIOAEN at bit 0");
_Static_assert(RCC_AHB1ENR_GPIOBEN == (1U <<  1), "AHB1ENR GPIOBEN at bit 1");
_Static_assert(RCC_AHB1ENR_GPIOCEN == (1U <<  2), "AHB1ENR GPIOCEN at bit 2");
_Static_assert(RCC_AHB1ENR_GPIODEN == (1U <<  3), "AHB1ENR GPIODEN at bit 3");
_Static_assert(RCC_AHB1ENR_GPIOEEN == (1U <<  4), "AHB1ENR GPIOEEN at bit 4");
_Static_assert(RCC_AHB1ENR_GPIOHEN == (1U <<  7), "AHB1ENR GPIOHEN at bit 7");
_Static_assert(RCC_AHB1ENR_DMA1EN  == (1U << 21), "AHB1ENR DMA1EN at bit 21");
_Static_assert(RCC_AHB1ENR_DMA2EN  == (1U << 22), "AHB1ENR DMA2EN at bit 22");

/* APB1ENR — RM0368 §6.3.11 */
_Static_assert(RCC_APB1ENR_TIM2EN   == (1U <<  0), "APB1ENR TIM2EN at bit 0");
_Static_assert(RCC_APB1ENR_USART2EN == (1U << 17), "APB1ENR USART2EN at bit 17");
_Static_assert(RCC_APB1ENR_PWREN    == (1U << 28), "APB1ENR PWREN at bit 28");

/* APB2ENR — RM0368 §6.3.12 */
_Static_assert(RCC_APB2ENR_USART1EN == (1U <<  4), "APB2ENR USART1EN at bit 4");
_Static_assert(RCC_APB2ENR_ADC1EN   == (1U <<  8), "APB2ENR ADC1EN at bit 8");
_Static_assert(RCC_APB2ENR_SYSCFGEN == (1U << 14), "APB2ENR SYSCFGEN at bit 14");
_Static_assert(RCC_APB2ENR_TIM1EN   == (1U <<  0), "APB2ENR TIM1EN at bit 0");

/* CSR reset-cause flags — RM0368 §6.3.18 */
_Static_assert(RCC_CSR_RMVF    == (1U << 24), "CSR RMVF at bit 24");
_Static_assert(RCC_CSR_PINRSTF == (1U << 26), "CSR PINRSTF at bit 26");
_Static_assert(RCC_CSR_PORRSTF == (1U << 27), "CSR PORRSTF at bit 27");
_Static_assert(RCC_CSR_SFTRSTF == (1U << 28), "CSR SFTRSTF at bit 28");
_Static_assert(RCC_CSR_IWDGRSTF== (1U << 29), "CSR IWDGRSTF at bit 29");

int main(void)
{
    return 0;
}
