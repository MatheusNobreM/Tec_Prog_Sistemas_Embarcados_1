.equ INTC_BASE, 0x48200000

/* CPSR */
.equ CPSR_I,   0x80
.equ CPSR_F,   0x40
.equ CPSR_IRQ, 0x12
.equ CPSR_USR, 0x10
.equ CPSR_FIQ, 0x11
.equ CPSR_SVC, 0x13
.equ CPSR_ABT, 0x17
.equ CPSR_UND, 0x1B
.equ CPSR_SYS, 0x1F

/* Vector table */
_vector_table:
    ldr   pc, _reset     /* reset - _start           */
    ldr   pc, _undf      /* undefined - _undf        */
    ldr   pc, _swi       /* SWI - _swi               */
    ldr   pc, _pabt      /* program abort - _pabt    */
    ldr   pc, _dabt      /* data abort - _dabt       */
    nop                  /* reserved                 */
    ldr   pc, _irq       /* IRQ - read the VIC       */
    ldr   pc, _fiq       /* FIQ - _fiq               */

_reset: .word _start
_undf:  .word _undef_handler /* undefined              */
_swi:   .word _swi_handler /* SWI                      */
_pabt:  .word _pabt_handler /* program abort           */
_dabt:  .word _dabt_handler /* data abort              */
_irq:   .word .irq_handler  /* IRQ                     */
_fiq:   .word _fiq_handler  /* FIQ                       */

/* Handlers não usados(loop infinito) */
_undef_handler: b _undef_handler
_swi_handler: b _swi_handler
_pabt_handler: b _pabt_handler
_dabt_handler: b _dabt_handler
_fiq_handler: b _fiq_handler

/* Startup Code */
_start:

    /* Set V=0 in CP15 SCTRL register - for VBAR to point to vector */
    mrc    p15, 0, r0, c1, c0, 0    // Read CP15 SCTRL Register
    bic    r0, #(1 << 13)           // V = 0
    mcr    p15, 0, r0, c1, c0, 0    // Write CP15 SCTRL Register

    /* Set vector address in CP15 VBAR register */
    ldr     r0, =_vector_table
    mcr     p15, 0, r0, c12, c0, 0  //Set VBAR

    /* init */
    mrs r0, cpsr
    bic r0, r0, #0x1F            // clear mode bits
    orr r0, r0, #CPSR_SVC        // set SVC mode
    orr r0, r0, #(CPSR_F)        // disable FIQ
    and r0, r0, #~(CPSR_I)       // enable IRQ
    msr cpsr, r0

    bl main

.loop:    b .loop 


.irq_handler:
    stmfd sp!, {r0-r12, lr}
    mrs r11, spsr
	bl ISR_Handler 
    dsb
    msr spsr, r11   
    ldmfd sp!, {r0-r12, lr}
    subs pc, lr, #4