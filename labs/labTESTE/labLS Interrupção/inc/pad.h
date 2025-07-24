#ifndef PAD_H
#define PAD_H

#include <stdint.h>
#include "control_module.h"

// Alias para facilitar uso dos offsets definidos em control_module.h
typedef uint32_t CONTROL_MODULE;

// Modo de cada pino (0..7)
typedef enum {
    MODE_0 = 0,
    MODE_1,
    MODE_2,
    MODE_3,
    MODE_4,
    MODE_5,
    MODE_6,
    MODE_7
} pinmode_t;

// Configurações de pull-up/pull-down
#define PULL_UP      (1u << 3)    // Bit 3 para pull-up
#define PULL_DOWN    (0u << 3)    // Bit 3 para pull-down
#define PULL_ENABLE  (1u << 4)    // Bit 4 para habilitar pull
#define PULL_DISABLE (0u << 4)    // Bit 4 para desabilitar pull

// Protótipos – implementados em pad.c
void padSetMode(CONTROL_MODULE offset, pinmode_t mode);
pinmode_t padGetMode(CONTROL_MODULE offset);
void padSetPull(CONTROL_MODULE offset, uint32_t pull, uint32_t enable);

#endif // PAD_H