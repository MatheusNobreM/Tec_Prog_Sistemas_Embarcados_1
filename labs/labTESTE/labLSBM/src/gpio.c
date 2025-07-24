#include <stdint.h>
#include "gpio.h"
#include "control_module.h"
#include "pad.h"
#include "delay.h"

// Clock & registradores GPIO
#define CM_PER_BASE           0x44E00000u
#define CM_PER_GPIO1_CLKCTRL  (*(volatile uint32_t*)(CM_PER_BASE + 0xAC))
#define GPIO1_BASE            0x4804C000u
#define GPIO1_OE              (*(volatile uint32_t*)(GPIO1_BASE + 0x134))
#define GPIO1_SETDATAOUT      (*(volatile uint32_t*)(GPIO1_BASE + 0x194))
#define GPIO1_CLEARDATAOUT    (*(volatile uint32_t*)(GPIO1_BASE + 0x190))
#define GPIO1_DATAIN          (*(volatile uint32_t*)(GPIO1_BASE + 0x138))


void gpio_setup(void) {
    // habilita clock GPIO1
    CM_PER_GPIO1_CLKCTRL |= 0x2;
    while (!(CM_PER_GPIO1_CLKCTRL & 0x3));

    // padmux
    padSetMode(CM_CONF_GPMC_AD15, MODE_7); // TRIG
    padSetMode(CM_CONF_GPMC_AD14, MODE_7); // ECHO
    padSetMode(CM_CONF_GPMC_AD13, MODE_7); // LED_R
    padSetMode(CM_CONF_GPMC_AD12, MODE_7); // LED_G
    padSetMode(CM_CONF_GPMC_A0,   MODE_7); // BUTTON

    // direções
    GPIO1_OE &= ~(TRIG_PIN | LED_R | LED_G); // saida
    GPIO1_OE |=  (ECHO_PIN | BUTTON_PIN); // entrada

    // pinos low
    GPIO1_CLEARDATAOUT = TRIG_PIN | LED_R | LED_G; // low
}

void rgb_set(int r, int g) {
    if (r) GPIO1_SETDATAOUT = LED_R; else GPIO1_CLEARDATAOUT = LED_R;
    if (g) GPIO1_SETDATAOUT = LED_G; else GPIO1_CLEARDATAOUT = LED_G;
}

uint32_t gpio_read(void) {
    return GPIO1_DATAIN;
}
