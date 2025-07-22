MEMORY {
    RAM : ORIGIN = 0x80000000, LENGTH = 256M
}

SECTIONS {
    .vectors : {
        *(.vectors)
    } > RAM
    
    .text : {
        *(.text)
    } > RAM
    
    .data : {
        *(.data)
    } > RAM
    
    .bss : {
        __bss_start = .;
        *(.bss)
        __bss_end = .;
    } > RAM
    
    .stack : {
        _stack_start = .;
        . += 0x1000;
        _stack_end = .;
    } > RAM
    
    /* Garante que as variáveis do linker serão exportadas */
    PROVIDE(__bss_start = __bss_start);
    PROVIDE(__bss_end = __bss_end);
}