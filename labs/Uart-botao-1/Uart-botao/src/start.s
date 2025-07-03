.global __bss_start
.global __bss_end
.extern main

.equ INTC_BASE, 0x48200000

/* CPSR */
.equ CPSR_I,   0x80
.equ CPSR_F,   0x40
.equ CPSR_IRQ, 0x12
.equ CPSR_SVC, 0x13

.section .vectors, "ax"
_vector_table:
    ldr pc, _reset
    ldr pc, _undef
    ldr pc, _swi
    ldr pc, _pabt
    ldr pc, _dabt
    nop
    ldr pc, _irq
    ldr pc, _fiq

_reset: .word _start
_undef: .word undef_handler
_swi:   .word swi_handler
_pabt:  .word pabt_handler
_dabt:  .word dabt_handler
_irq:   .word irq_handler
_fiq:   .word fiq_handler

.section .text
_start:
    /* Configura VBAR */
    mrc p15, 0, r0, c1, c0, 0
    bic r0, #(1 << 13)
    mcr p15, 0, r0, c1, c0, 0
    
    ldr r0, =_vector_table
    mcr p15, 0, r0, c12, c0, 0

    /* Inicializa stacks */
    msr cpsr_c, #(CPSR_SVC | CPSR_I | CPSR_F)
    ldr sp, =_svc_stack_end
    
    msr cpsr_c, #(CPSR_IRQ | CPSR_I | CPSR_F)
    ldr sp, =_irq_stack_end
    
    msr cpsr_c, #(CPSR_SVC | CPSR_F)

    /* Limpa BSS - usa símbolos definidos no linker script */
    ldr r0, =__bss_start
    ldr r1, =__bss_end
    mov r2, #0
bss_loop:
    cmp r0, r1
    strlt r2, [r0], #4
    blt bss_loop

    bl main

loop:
    b loop

/* Handlers básicos */
undef_handler:
swi_handler:
pabt_handler:
dabt_handler:
fiq_handler:
    b .

/* Handler de IRQ */
irq_handler:
    sub lr, lr, #4
    srsdb sp!, #0x13
    cps #0x13
    push {r0-r12, lr}
    
    bl ISR_Handler
    
    pop {r0-r12, lr}
    rfeia sp!

.section .bss
.align 8
_svc_stack:
    .space 1024
_svc_stack_end:

_irq_stack:
    .space 1024
_irq_stack_end:
