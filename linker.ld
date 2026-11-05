ENTRY(_start)

SECTIONS
{
    . = 0x40100000;

    .text : {
        KEEP(*(.text.boot))
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    .bss (NOLOAD) : {
        . = ALIGN(8);       /* will use xzr to clear bss and xzr is 8 bytes */
        __bss_start = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(8);
        __bss_end = .;
    }

    . = ALIGN(16);

    stack_bottom = .;
    . += 0x4000;
    stack_top = .;
}