#ifndef CONTROL_MODULE_H
#define CONTROL_MODULE_H

// Base do bloco de controle de pinos (CONTROL_MODULE)
#define CONTROL_MODULE_BASE  0x44E10000u

// Offsets dos pinos do Datasheet AM335x
#define CM_CONF_GPMC_AD15   0x083Cu  // TRIG (GPIO1_15, P8_15)
#define CM_CONF_GPMC_AD14   0x0838u  // ECHO (GPIO1_14, P8_16)
#define CM_CONF_GPMC_AD13   0x0834u  // LED_R (GPIO1_13, P8_11)
#define CM_CONF_GPMC_AD12   0x0830u  // LED_G (GPIO1_12, P8_12)
#define CM_CONF_GPMC_A0     0x0840u  // BUTTON (GPIO1_17, P9_23)

#endif // CONTROL_MODULE_H
