#include "delay.h"

void delay_us(uint32_t us)
{
    volatile uint32_t c = us * 14;   // ~14 cycles por µs @ 1 GHz
    while (c--) ;
}

void delay_ms(uint32_t ms)
{
    while (ms--) delay_us(1000);
}
