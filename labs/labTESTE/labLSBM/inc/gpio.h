#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "control_module.h"
#include "pad.h"

// ——————————————————————————————————————————————
// Clock & registradores GPIO (GPIO1)
// — base CM_PER + offset GPIO1
#define CM_PER_BASE           0x44E00000u
#define CM_PER_GPIO1_CLKCTRL  (*(volatile uint32_t*)(CM_PER_BASE + 0xAC))
#define GPIO1_BASE            0x4804C000u

// — offsets dentro de GPIO1
#define GPIO1_OE              (*(volatile uint32_t*)(GPIO1_BASE + 0x134))
#define GPIO1_SETDATAOUT      (*(volatile uint32_t*)(GPIO1_BASE + 0x194))
#define GPIO1_CLEARDATAOUT    (*(volatile uint32_t*)(GPIO1_BASE + 0x190))
#define GPIO1_DATAIN          (*(volatile uint32_t*)(GPIO1_BASE + 0x138))

// ——————————————————————————————————————————————
// Definição dos pinos (GPIO1_x)
// ——————————————————————————————————————————————
#define TRIG_PIN    (1 << 15) // P8_15
#define ECHO_PIN    (1 << 14) // P8_16
#define LED_R       (1 << 13) // P8_11
#define LED_G       (1 << 12) // P8_12
#define BUTTON_PIN  (1 << 17) // P9_23

// ——————————————————————————————————————————————
// Protótipos
// ——————————————————————————————————————————————
void gpio_setup(void);
void rgb_set(int r, int g);
// lê nível lógico do registrador DATAIN
uint32_t gpio_read(void);

#endif // GPIO_H
