#include <stdint.h>
#include "pad.h"
#include "control_module.h"  // seus offsets de pad

// Watchdog minimal
#define WDT1_BASE 0x44E35000
#define WDT_WSPR (*(volatile uint32_t *)(WDT1_BASE + 0x48))
#define WDT_WWPS (*(volatile uint32_t *)(WDT1_BASE + 0x34))

void disable_watchdog(void) {
    WDT_WSPR = 0xAAAA; while (WDT_WWPS & (1 << 4));
    WDT_WSPR = 0x5555; while (WDT_WWPS & (1 << 4));
}

// UART0 minimal
#define UART0_BASE 0x44E09000
#define UART0_THR (*(volatile unsigned int *)(UART0_BASE + 0x00))
#define UART0_LSR (*(volatile unsigned int *)(UART0_BASE + 0x14))
#define UART0_LSR_THRE (1 << 5)
void uart0_putc(char c) { while (!(UART0_LSR & UART0_LSR_THRE)); UART0_THR = c; }
void uart0_write_str(const char *str) { while (*str) uart0_putc(*str++); }
void uart0_write_uint(unsigned int n) {
    char buf[16]; int i = 0;
    if (n == 0) { uart0_putc('0'); return; }
    while (n && i < 15) { buf[i++] = '0' + (n % 10); n /= 10; }
    while (i--) uart0_putc(buf[i]);
}

// GPIO minimal
#define CM_PER_BASE             0x44E00000
#define CM_PER_GPIO1_CLKCTRL    (*(volatile uint32_t*)(CM_PER_BASE + 0xAC))
#define CM_PER_GPIO0_CLKCTRL    (*(volatile uint32_t*)(CM_PER_BASE + 0x08))
#define GPIO1_BASE              0x4804C000
#define GPIO1_OE                (*(volatile uint32_t*)(GPIO1_BASE + 0x134))
#define GPIO1_CLEARDATAOUT      (*(volatile uint32_t*)(GPIO1_BASE + 0x190))
#define GPIO1_SETDATAOUT        (*(volatile uint32_t*)(GPIO1_BASE + 0x194))
#define GPIO0_BASE              0x44E07000
#define GPIO0_OE                (*(volatile uint32_t*)(GPIO0_BASE + 0x134))
#define GPIO0_DATAIN            (*(volatile uint32_t*)(GPIO0_BASE + 0x138))

// Defina conforme sua ligação!
#define TRIG_PIN    (1 << 15)    // GPIO1_15 (P8_15)
#define ECHO_PIN    (1 << 27)    // GPIO0_27 (P8_17) - AJUSTE se usar outro pino!

void delay_us(unsigned int us) { volatile unsigned int c = us * 14; while(c--); }
void delay_ms(unsigned int ms) { while(ms--) delay_us(1000); }

void gpio_setup(void) {
    // Muda o multiplexador dos pads para modo GPIO (MODE_7)
    padSetMode(CM_CONF_GPMC_AD15, MODE_7);  // TRIG em GPMC_AD15 (pin P8_15)
    padSetMode(CM_CONF_GPMC_AD14, MODE_7);  // ECHO em GPMC_AD14 (pin P8_17)

    // Ativa clock de GPIO1 e GPIO0
    CM_PER_GPIO1_CLKCTRL |= 0x2;
    while (!(CM_PER_GPIO1_CLKCTRL & 0x3));
    CM_PER_GPIO0_CLKCTRL |= 0x2;
    while (!(CM_PER_GPIO0_CLKCTRL & 0x3));

    // Define direções
    GPIO1_OE &= ~TRIG_PIN;   // TRIG como saída
    GPIO0_OE |= ECHO_PIN;    // ECHO como entrada
}


// Apenas HC-SR04 + UART (sem LED!)
unsigned int hcsr04_read_mm() {
    unsigned int t;

    // TRIG LOW
    GPIO1_CLEARDATAOUT = TRIG_PIN;
    delay_us(2);

    // Pulso TRIG 10us
    GPIO1_SETDATAOUT = TRIG_PIN;
    delay_us(10);
    GPIO1_CLEARDATAOUT = TRIG_PIN;

    // Espera ECHO subir (começar a medir)
    int timeout = 0;
    while(!(GPIO0_DATAIN & ECHO_PIN)) {
        timeout++; if (timeout > 40000) { uart0_write_str("timeout HIGH\r\n"); return 0; }
    }
    t = 0;
    while(GPIO0_DATAIN & ECHO_PIN) {
        t++; delay_us(1);
        if (t > 30000) { uart0_write_str("timeout LOW\r\n"); return 0; }
    }

    uart0_write_str("Pulse: ");
    uart0_write_uint(t);
    uart0_write_str(" us\r\n");

    return t / 6; // Aproximação de mm
}

void ISR_Handler(void) {} // Handler vazio, só para link

int main(void) {
    disable_watchdog();
    gpio_setup();
    uart0_write_str("\r\n---- HC-SR04 UART ONLY ----\r\n");

    while(1) {
        unsigned int distancia = hcsr04_read_mm();

        uart0_write_str("Dist: ");
        uart0_write_uint(distancia);
        uart0_write_str(" mm\r\n");

        delay_ms(500);
    }
    return 0;
}
