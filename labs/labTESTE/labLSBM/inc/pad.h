#ifndef PAD_H
#define PAD_H

#include <stdint.h>
#include "control_module.h"

// Modo de cada pino (0..7)
typedef enum {
    MODE_0 = 0, MODE_1, MODE_2, MODE_3,
    MODE_4, MODE_5, MODE_6, MODE_7
} pinmode_t;

// Configura o modo de um pino via offset
void padSetMode(uint32_t offset, pinmode_t mode);
pinmode_t padGetMode(uint32_t offset);

#endif // PAD_H
