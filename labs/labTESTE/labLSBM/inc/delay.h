#ifndef DELAY_H
#define DELAY_H
#include <stdint.h>

void delay_us(uint32_t us);   // busy-wait básico (~14 cycles/us)
void delay_ms(uint32_t ms);   // wrapper em ms

#endif
