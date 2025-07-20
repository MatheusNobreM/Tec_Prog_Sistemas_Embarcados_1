#include <stdint.h>

// Watchdog minimal
#define WDT1_BASE 0x44E35000
#define WDT_WSPR (*(volatile uint32_t *)(WDT1_BASE + 0x48))
#define WDT_WWPS (*(volatile uint32_t *)(WDT1_BASE + 0x34))
void disable_watchdog(void) {
    WDT_WSPR = 0xAAAA; while (WDT_WWPS & (1 << 4));
    WDT_WSPR = 0x5555; while (WDT_WWPS & (1 << 4));
}

// Definições GPIO1 para três canais (ajuste se necessário)
#define CM_PER_BASE         0x44E00000
#define CM_PER_GPIO1_CLKCTRL (*(volatile uint32_t*)(CM_PER_BASE + 0xAC))
#define GPIO1_BASE          0x4804C000
#define GPIO1_OE            (*(volatile uint32_t*)(GPIO1_BASE + 0x134))
#define GPIO1_CLEARDATAOUT  (*(volatile uint32_t*)(GPIO1_BASE + 0x190))
#define GPIO1_SETDATAOUT    (*(volatile uint32_t*)(GPIO1_BASE + 0x194))
// LED RGB (catodo comum)
#define LED_R       (1 << 13)    // GPIO1_13 (P8_11)  -> Vermelho
#define LED_G       (1 << 12)    // GPIO1_12 (P8_12)  -> Verde
#define LED_B       (1 << 28)    // GPIO1_28 (P9_12)   -> Azul  

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

void delay_us(unsigned int us) { volatile unsigned int c = us * 14; while(c--); }
void delay_ms(unsigned int ms) { while(ms--) delay_us(1000); }

int main(void) {
    disable_watchdog();
    gpio_setup();
    while(1) {
        // Teste cada LED individualmente
        GPIO1_SETDATAOUT = LED_R; delay_ms(2000);
        GPIO1_CLEARDATAOUT = LED_R; delay_ms(2000);

        GPIO1_SETDATAOUT = LED_G; delay_ms(2000);
        GPIO1_CLEARDATAOUT = LED_G; delay_ms(2000);

        GPIO1_SETDATAOUT = LED_B; delay_ms(2000);
        GPIO1_CLEARDATAOUT = LED_B; delay_ms(2000);
    }
}

