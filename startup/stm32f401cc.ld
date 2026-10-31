/* stm32f401cc.ld — Hand-written linker script for STM32F401CCU6
 *
 * Flash: 256 K at 0x0800_0000   DS9716 Table 3
 * SRAM:   64 K at 0x2000_0000   DS9716 Table 3
 *
 * Stack model: full-descending.  The Cortex-M4 loads MSP from the first
 * word of the vector table before branching to Reset_Handler (PM0214 §2.3.4).
 * Reset_Handler copies .data from FLASH→SRAM and zeroes .bss, then calls main().
 */

ENTRY(Reset_Handler)

/* Minimum stack reserved at top of SRAM.
 * Override at link time: -Wl,--defsym,STACK_SIZE=<bytes>            */
STACK_SIZE = DEFINED(STACK_SIZE) ? STACK_SIZE : 2048;

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 256K   /* DS9716 Table 3 */
    SRAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 64K    /* DS9716 Table 3 */
}

/* _estack — initial MSP value; first word of the vector table.
 * Cortex-M4 loads this into MSP before jumping to Reset_Handler (PM0214 §2.3.4).
 * Value = 0x2001_0000 = top of 64 K SRAM.                            */
_estack = ORIGIN(SRAM) + LENGTH(SRAM);

SECTIONS
{
    /* -------------------------------------------------------------------
     * .isr_vector — Cortex-M4 vector table.
     * Must begin at 0x0800_0000 (reset value of VTOR, PM0214 §4.4.4).
     * KEEP prevents --gc-sections from discarding the table.
     * ------------------------------------------------------------------- */
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } >FLASH

    /* -------------------------------------------------------------------
     * .text — executable code + ARM/Thumb interworking stubs.
     * ------------------------------------------------------------------- */
    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text.*)
        *(.glue_7)          /* ARM→Thumb interworking veneers */
        *(.glue_7t)         /* Thumb→ARM interworking veneers */
        . = ALIGN(4);
        _etext = .;         /* end of code region in FLASH */
    } >FLASH

    /* -------------------------------------------------------------------
     * .rodata — read-only constants and string literals.
     * ------------------------------------------------------------------- */
    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)
        *(.rodata.*)
        . = ALIGN(4);
    } >FLASH

    /* -------------------------------------------------------------------
     * .ARM.exidx — exception-unwind index (C++ ABI, PM0214 §B.3).
     * Included so --gc-sections can drop it without a linker warning.
     * __exidx_start/__exidx_end are consumed by libgcc's unwind support.
     * ------------------------------------------------------------------- */
    .ARM.exidx :
    {
        __exidx_start = .;
        *(.ARM.exidx*)
        *(.gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } >FLASH

    /* -------------------------------------------------------------------
     * .data — initialised variables.
     * Load address (LMA) = _sidata in FLASH (stored in the binary image).
     * Virtual address (VMA) = _sdata.._edata in SRAM (where the CPU sees it).
     * Reset_Handler copies [_sidata .. _sidata+(_edata-_sdata)) → _sdata.
     * ------------------------------------------------------------------- */
    _sidata = LOADADDR(.data);   /* LMA in FLASH — source for startup copy */

    .data :
    {
        . = ALIGN(4);
        _sdata = .;              /* VMA in SRAM — start of destination */
        *(.data)
        *(.data.*)
        . = ALIGN(4);
        _edata = .;              /* VMA in SRAM — end of destination */
    } >SRAM AT> FLASH

    /* -------------------------------------------------------------------
     * .bss — zero-initialised variables (SHT_NOBITS: no FLASH storage).
     * Reset_Handler zeroes [_sbss .. _ebss) at startup.
     * ------------------------------------------------------------------- */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } >SRAM

    /* -------------------------------------------------------------------
     * ._stack — stack reservation (NOLOAD: SRAM space, no FLASH payload).
     * The ARM Cortex-M4 stack is full-descending: execution starts at
     * _estack (top of SRAM) and grows toward lower addresses.
     * Allocating STACK_SIZE bytes here causes the linker to error with
     * "region SRAM overflowed" if .data + .bss + stack exceed 64 K.
     * _sstack is the lowest valid stack address (used later for canaries).
     * ------------------------------------------------------------------- */
    ._stack (NOLOAD) :
    {
        . = ALIGN(8);
        _sstack = .;            /* bottom of stack reservation */
        . += STACK_SIZE;
        . = ALIGN(8);
    } >SRAM

    /* -------------------------------------------------------------------
     * /DISCARD/ — sections not needed in a bare-metal freestanding binary.
     * ------------------------------------------------------------------- */
    /DISCARD/ :
    {
        *(.ARM.attributes)
        *(.comment)
        *(.eh_frame)
        *(.note.gnu.build-id)
    }
}

/* Link-time overflow checks (belt-and-suspenders alongside region overflow) */
ASSERT(LOADADDR(.data) + SIZEOF(.data) <= ORIGIN(FLASH) + LENGTH(FLASH),
    "FLASH overflow: reduce code / rodata / initialised data")
ASSERT(_sstack + STACK_SIZE <= _estack,
    "SRAM overflow: reduce .data / .bss or increase STACK_SIZE")
