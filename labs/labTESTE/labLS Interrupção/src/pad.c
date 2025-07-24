#include <stdint.h>
#include "pad.h"

// Cada registro de configuração de pad é 32-bits,
// bits [2:0] definem o modo (0..7).
void padSetMode(CONTROL_MODULE offset, pinmode_t mode) {
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

// Configura pull-up/pull-down
void padSetPull(CONTROL_MODULE offset, uint32_t pull, uint32_t enable) {
    volatile uint32_t *reg = (uint32_t *)(CONTROL_MODULE_BASE + offset);
    uint32_t tmp = *reg;
    tmp &= ~(0x3u << 3);  // Limpa bits [4:3] (pull-up/down e enable)
    tmp |= pull;          // Define pull-up ou pull-down
    tmp |= enable;        // Habilita ou desabilita pull
    *reg = tmp;
}