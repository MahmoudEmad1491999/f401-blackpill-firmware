/* tests/host/test_gpio.c — compile-time checks for regs/gpio.h
 *
 * What this tests:
 *   - The header parses cleanly under a native C11 compiler.
 *   - Include-guard works (double inclusion is silent).
 *   - Every _Static_assert(offsetof(…)) in the header passes.
 *   - Struct total size is 0x28 bytes (AFR[1] at 0x24 + 4).
 *   - Key mode/type/speed/pull-up constants encode the values in RM0368.
 *   - BSRR set/reset helper macros are correct.
 *   - AFR helper macros produce the right bit-field positions.
 *
 * What this does NOT test:
 *   Hardware register access — that requires a Cortex-M4 and will be covered
 *   by Renode Robot tests.
 */

#include <stdint.h>
#include "regs/gpio.h"
#include "regs/gpio.h"  /* double-include guard: second inclusion must be silent */

/* Struct total size: AFR[1] at offset 0x24, sizeof(uint32_t)=4 → 0x28 */
_Static_assert(sizeof(gpio_regs_t) == 0x28U, "gpio_regs_t total size must be 0x28");

/* MODER values — RM0368 §8.4.1 */
_Static_assert(GPIO_MODER_INPUT  == 0U, "MODER INPUT = 0b00");
_Static_assert(GPIO_MODER_OUTPUT == 1U, "MODER OUTPUT = 0b01");
_Static_assert(GPIO_MODER_AF     == 2U, "MODER AF = 0b10");
_Static_assert(GPIO_MODER_ANALOG == 3U, "MODER ANALOG = 0b11");

/* MODER pin helpers: pin 13 (PC13 LED) at bits [27:26] */
_Static_assert(GPIO_MODER_PIN_Pos(13) == 26U,               "MODER pin 13 position");
_Static_assert(GPIO_MODER_PIN_Msk(13) == (3U << 26U),       "MODER pin 13 mask");
_Static_assert(GPIO_MODER_PIN_VAL(13, GPIO_MODER_OUTPUT)
               == (1U << 26U),                               "MODER pin 13 OUTPUT value");

/* OTYPER — RM0368 §8.4.2 */
_Static_assert(GPIO_OTYPER_PP == 0U, "OTYPER push-pull = 0");
_Static_assert(GPIO_OTYPER_OD == 1U, "OTYPER open-drain = 1");
_Static_assert(GPIO_OTYPER_PIN_Msk(13) == (1U << 13U), "OTYPER pin 13 mask");

/* OSPEEDR — RM0368 §8.4.3 */
_Static_assert(GPIO_OSPEEDR_LOW   == 0U, "OSPEEDR LOW = 0b00");
_Static_assert(GPIO_OSPEEDR_MED   == 1U, "OSPEEDR MED = 0b01");
_Static_assert(GPIO_OSPEEDR_HIGH  == 2U, "OSPEEDR HIGH = 0b10");
_Static_assert(GPIO_OSPEEDR_VHIGH == 3U, "OSPEEDR VHIGH = 0b11");

/* PUPDR — RM0368 §8.4.4 */
_Static_assert(GPIO_PUPDR_NONE == 0U, "PUPDR NONE = 0b00");
_Static_assert(GPIO_PUPDR_UP   == 1U, "PUPDR UP = 0b01");
_Static_assert(GPIO_PUPDR_DOWN == 2U, "PUPDR DOWN = 0b10");

/* BSRR — RM0368 §8.4.7: set in lower 16, reset in upper 16 */
_Static_assert(GPIO_BSRR_BS(0)  == (1U <<  0),  "BSRR BS0 at bit 0");
_Static_assert(GPIO_BSRR_BS(13) == (1U << 13),  "BSRR BS13 at bit 13");
_Static_assert(GPIO_BSRR_BR(0)  == (1U << 16),  "BSRR BR0 at bit 16");
_Static_assert(GPIO_BSRR_BR(13) == (1U << 29),  "BSRR BR13 at bit 29");

/* LCKR — RM0368 §8.4.8 */
_Static_assert(GPIO_LCKR_LCKK   == (1U << 16), "LCKR LCKK at bit 16");
_Static_assert(GPIO_LCKR_LCK(0) == (1U <<  0), "LCKR LCK0 at bit 0");

/* AFR helpers — RM0368 §8.4.9/10
 * Pin 5 (PA5 = SPI1_SCK, AF5) in AFRL (AFR[0]): bits [23:20] */
_Static_assert(GPIO_AFRL_PIN_Pos(5)        == 20U,         "AFRL pin 5 at bit 20");
_Static_assert(GPIO_AFRL_PIN_Msk(5)        == (0xFU << 20U), "AFRL pin 5 mask");
_Static_assert(GPIO_AFRL_PIN_VAL(5, GPIO_AF5) == (5U << 20U), "AFRL pin 5 AF5 value");

/* Pin 9 (PA9 = USART1_TX, AF7) in AFRH (AFR[1]): bits [7:4] */
_Static_assert(GPIO_AFRH_PIN_Pos(9)        == 4U,          "AFRH pin 9 at bit 4");
_Static_assert(GPIO_AFRH_PIN_Msk(9)        == (0xFU <<  4U), "AFRH pin 9 mask");
_Static_assert(GPIO_AFRH_PIN_VAL(9, GPIO_AF7) == (7U <<  4U), "AFRH pin 9 AF7 value");

/* GPIO_PIN helper */
_Static_assert(GPIO_PIN(13) == (1U << 13), "GPIO_PIN(13)");
_Static_assert(GPIO_PIN(0)  == (1U <<  0), "GPIO_PIN(0)");

int main(void)
{
    return 0;
}
