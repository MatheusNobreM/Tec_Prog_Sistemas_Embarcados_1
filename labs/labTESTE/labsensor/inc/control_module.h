#ifndef CONTROL_MODULE_H
#define CONTROL_MODULE_H

// Base do bloco de controle de pinos (CONTROL_MODULE)
#define CONTROL_MODULE_BASE  0x44E10000u

// Offsets dos pinos que vamos usar (do Datasheet AM335x)
// GPMC_AD15 → offset 0x083C  (GPIO1_15, seu TRIG)
// GPMC_AD14 → offset 0x0838  (GPIO1_14, seu ECHO)
// GPMC_AD13 → offset 0x0834  (GPIO1_13, LED_R se quiser)
// GPMC_AD12 → offset 0x0830  (GPIO1_12, LED_G)
// GPMC_AD0  → offset 0x0800  (GPIO1_0)  etc.

#define CM_CONF_GPMC_AD15   0x083Cu
#define CM_CONF_GPMC_AD14   0x0838u
#define CM_CONF_GPMC_AD13   0x0834u
#define CM_CONF_GPMC_AD12   0x0830u

#endif // CONTROL_MODULE_H
