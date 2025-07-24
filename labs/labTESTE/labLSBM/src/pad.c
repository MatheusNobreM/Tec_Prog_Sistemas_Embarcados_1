#include <stdint.h>
#include "pad.h"

void padSetMode(uint32_t offset, pinmode_t mode) {
    volatile uint32_t *reg = (uint32_t *)(CONTROL_MODULE_BASE + offset);
    uint32_t tmp = *reg;
    tmp &= ~0x7u; //com máscara ~0x7 = 0xFFFFFFF8
    tmp |= (uint32_t)mode;
    *reg = tmp;
}

pinmode_t padGetMode(uint32_t offset) {
    volatile uint32_t *reg = (uint32_t *)(CONTROL_MODULE_BASE + offset);
    return (pinmode_t)(*reg & 0x7u);
}
