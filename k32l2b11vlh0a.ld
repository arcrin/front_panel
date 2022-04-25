ENTRY(Reset_Handler)

MEMORY
{
    FLASH(rx):ORIGIN =0x00000000,LENGTH =64K
    SRAM(rwx):ORIGIN =0x1FFFE000,LENGTH =32K
}

SECTIONS
{
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } > FLASH
    .text:
    {
        *(.text)
        *(.text.)
        *(.init)
        *(.fini)
        *(.rodata)
        *(.rodata.*)
        . = ALIGN(4)
        _etext = .;
    } > FLASH

}