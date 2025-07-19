#include "pad.h"

void padSetMode(CONTROL_MODULE offset, pinmode_t mode) {
    // Cada registro de configuração de pad é 32-bits,
    // bits [2:0] definem o mode mux (0..7).
    volatile uint32_t *reg = (uint32_t *)(CONTROL_MODULE_BASE + offset);
    uint32_t tmp = *reg;
    tmp &= ~0x7u;         // limpa bits [2:0]
    tmp |= (uint32_t)mode;
    *reg = tmp;
}

pinmode_t padGetMode(CONTROL_MODULE offset) {
    volatile uint32_t *reg = (uint32_t *)(CONTROL_MODULE_BASE + offset);
    return (pinmode_t)(*reg & 0x7u);
}
