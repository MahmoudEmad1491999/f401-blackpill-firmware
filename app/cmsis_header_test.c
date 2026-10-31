/*
 * Compile-time smoke-test: include the main device header and assert that
 * CMSIS and the device macros resolve to values expected for STM32F401CCU6.
 *
 * No object code is produced that will ever run; the file exists solely
 * so that `cmake --build` fails fast if the include paths are broken.
 */
#include "stm32f401xc.h"

/* CMSIS core must be Cortex-M4 (value 4) */
_Static_assert(__CORTEX_M == 4, "Expected Cortex-M4 core");

/* CMSIS version must be at least 5.x */
_Static_assert(__CM_CMSIS_VERSION_MAIN >= 5, "Expected CMSIS 5.x headers");

/*
 * FLASH_BASE and GPIOC_BASE are defined in stm32f401xc.h; their presence
 * at the correct addresses is a quick sanity-check that the right variant
 * header was included.
 */
_Static_assert(FLASH_BASE == 0x08000000UL, "Unexpected FLASH_BASE");
_Static_assert(GPIOC_BASE == 0x40020800UL, "Unexpected GPIOC_BASE");
