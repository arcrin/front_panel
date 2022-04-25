ENTRY(Init_Program_Counter)

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
    }> FLASH
    .text :
    {
        *(.text)
        *(.text.*)
        *(.init)
        *(.fini)
        *(.rodata)
        *(.rodata.*)
        . = ALIGN(4);
        _etext = .;
    }> FLASH

     __exidx_start = .;
     .ARM.exidx :
     {
         *(.ARM.exidx* .gnu.linkonce.armexidx.*)
     } >FLASH
     __exidx_end = .;

    .data :
    {
        _sdata = .;
        *(.data)
        *(.data.*)
        . = ALIGN(4);
        _edata = .;
    }> SRAM AT> FLASH

    .bss :
    {
        _sbss = .;
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss.*)
        . = ALIGN(4);
        _ebss = .;
        __bss_end__ = _ebss;
        end = .;
        __end__ = end;
    }> SRAM
}