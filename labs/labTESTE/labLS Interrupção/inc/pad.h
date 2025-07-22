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

// Protótipos – implementados em pad.c
void padSetMode(CONTROL_MODULE offset, pinmode_t mode);
pinmode_t padGetMode(CONTROL_MODULE offset);

#endif // PAD_H
