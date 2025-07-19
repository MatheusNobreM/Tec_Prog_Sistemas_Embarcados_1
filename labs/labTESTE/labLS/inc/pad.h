#ifndef PAD_H
#define PAD_H

#include <stdint.h>
#include "control_module.h"


// Base do bloco CONTROL_MODULE no AM335x
#define CONTROL_MODULE_BASE 0x44E10000u

// Os offsets de pino estão em control_module.h, por exemplo:
//   #define CM_conf_gpmc_ad0    0x0800
//   #define CM_conf_gpmc_ad1    0x0804
//   …
//   #define CM_conf_gpmc_ad15   0x083C
//   #define CM_conf_gpmc_a0     0x0840
//   etc.

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

// Usa o offset definido em control_module.h
typedef uint32_t CONTROL_MODULE;

// Seta o MUX do pino para o modo desejado (0..7)
void padSetMode(CONTROL_MODULE offset, pinmode_t mode);

// Lê o modo atualmente programado
pinmode_t padGetMode(CONTROL_MODULE offset);

#endif // PAD_H
