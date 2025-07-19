#include <stdint.h>

// Definições GPIO1 para três canais (ajuste se necessário)
#define CM_PER_BASE         0x44E00000
#define CM_PER_GPIO1_CLKCTRL (*(volatile uint32_t*)(CM_PER_BASE + 0xAC))
#define GPIO1_BASE          0x4804C000
#define GPIO1_OE            (*(volatile uint32_t*)(GPIO1_BASE + 0x134))
#define GPIO1_CLEARDATAOUT  (*(volatile uint32_t*)(GPIO1_BASE + 0x190))
#define GPIO1_SETDATAOUT    (*(volatile uint32_t*)(GPIO1_BASE + 0x194))

#define LED_R (1 << 13)   // P8_11 = GPIO1_13
#define LED_G (1 << 12)   // P8_12 = GPIO1_12
#define LED_B (1 << 14)   // P8_16 = GPIO1_14 (confira!)

void delay(volatile unsigned int c) {
    while(c--);
}

void gpio_setup() {
    // Liga clock do GPIO1
    CM_PER_GPIO1_CLKCTRL |= 0x2;
    while(!(CM_PER_GPIO1_CLKCTRL & 0x3));
    // Configura GPIO1_12/13/14 como saída
    GPIO1_OE &= ~(LED_R | LED_G | LED_B);
}

void rgb_color(int r, int g, int b) {
    // Agora: para acender a cor, coloca o pino em HIGH (nível alto)
    if(r) GPIO1_SETDATAOUT = LED_R;
    else  GPIO1_CLEARDATAOUT = LED_R;

    if(g) GPIO1_SETDATAOUT = LED_G;
    else  GPIO1_CLEARDATAOUT = LED_G;

    if(b) GPIO1_SETDATAOUT = LED_B;
    else  GPIO1_CLEARDATAOUT = LED_B;
}


void ISR_Handler(unsigned int interrupcao) {
    // Seu código de interrupção aqui.
}

int main(void) {
    gpio_setup();
    while(1) {
        rgb_color(1,0,0);   // Vermelho
        // UART opcional, mas pode printar
        delay(10000000);
        rgb_color(0,1,0);   // Verde
        delay(10000000);
        rgb_color(0,0,1);   // Azul
        delay(10000000);
        rgb_color(1,1,0);   // Amarelo (R+G)
        delay(10000000);
        rgb_color(1,0,1);   // Magenta (R+B)
        delay(10000000);
        rgb_color(0,1,1);   // Ciano (G+B)
        delay(10000000);
        rgb_color(1,1,1);   // Branco (R+G+B)
        delay(10000000);
        rgb_color(0,0,0);   // Tudo apagado
        delay(10000000);
    }
    return 0;
}
