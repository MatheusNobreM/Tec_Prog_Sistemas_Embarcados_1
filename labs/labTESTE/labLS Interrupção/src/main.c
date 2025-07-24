#include <stdint.h>
#include "pad.h"
#include "control_module.h"

// Watchdog minimal
#define WDT1_BASE 0x44E35000
#define WDT_WSPR   (*(volatile uint32_t *)(WDT1_BASE + 0x48))
#define WDT_WWPS   (*(volatile uint32_t *)(WDT1_BASE + 0x34))
static void disable_watchdog(void) {
    WDT_WSPR = 0xAAAA; while (WDT_WWPS & (1 << 4));
    WDT_WSPR = 0x5555; while (WDT_WWPS & (1 << 4));
}

// UART0 minimal
#define UART0_BASE     0x44E09000
#define UART0_THR      (*(volatile unsigned int *)(UART0_BASE + 0x00))
#define UART0_LSR      (*(volatile unsigned int *)(UART0_BASE + 0x14))
#define UART0_LSR_THRE (1 << 5)
static void uart0_putc(char c) { 
    while (!(UART0_LSR & UART0_LSR_THRE)); 
    UART0_THR = c; 
}
static void uart0_write_str(const char *s) { 
    while (*s) uart0_putc(*s++); 
}
static void uart0_write_uint(unsigned int n) {
    char buf[16]; int i = 0;
    if (n == 0) { uart0_putc('0'); return; }
    while (n && i < 15) { buf[i++] = '0' + (n % 10); n /= 10; }
    while (i--) uart0_putc(buf[i]);
}

// GPIO e clock
#define CM_PER_BASE           0x44E00000
#define CM_PER_GPIO1_CLKCTRL  (*(volatile uint32_t*)(CM_PER_BASE + 0xAC))
#define GPIO1_BASE            0x4804C000
#define GPIO1_OE              (*(volatile uint32_t*)(GPIO1_BASE + 0x134))
#define GPIO1_SETDATAOUT      (*(volatile uint32_t*)(GPIO1_BASE + 0x194))
#define GPIO1_CLEARDATAOUT    (*(volatile uint32_t*)(GPIO1_BASE + 0x190))
#define GPIO1_DATAIN          (*(volatile uint32_t*)(GPIO1_BASE + 0x138))
#define GPIO1_IRQSTATUS_SET_0 (*(volatile uint32_t*)(GPIO1_BASE + 0x34))
#define GPIO1_IRQSTATUS_0     (*(volatile uint32_t*)(GPIO1_BASE + 0x2C))
#define GPIO1_RISINGDETECT    (*(volatile uint32_t*)(GPIO1_BASE + 0x148))

// INTC (interrupt controller)
#define INTC_BASE         0x48200000
#define INTC_MIR_CLEAR3   (*(volatile uint32_t*)(INTC_BASE + 0xE8))
#define INTC_SIR_IRQ      (*(volatile uint32_t*)(INTC_BASE + 0x40))
#define INTC_CONTROL      (*(volatile uint32_t*)(INTC_BASE + 0x48))

// Pinos
#define TRIG_PIN    (1 << 15) // P8_15
#define ECHO_PIN    (1 << 14) // P8_16
#define LED_R       (1 << 13) // P8_11
#define LED_G       (1 << 12) // P8_12
#define BUTTON_PIN  (1 << 17) // P9_23

volatile int sensor_ativo = 0;
#define DEBOUNCE_MS 200

static void delay_us(unsigned int us)   { volatile unsigned int c = us * 14; while(c--); }
static void delay_ms(unsigned int ms)   { while(ms--) delay_us(1000); }

// LEDs RGB
static void rgb_set(int r, int g) {
    if (r) GPIO1_SETDATAOUT = LED_R; else GPIO1_CLEARDATAOUT = LED_R;
    if (g) GPIO1_SETDATAOUT = LED_G; else GPIO1_CLEARDATAOUT = LED_G;
}

// Sensor ultrassônico
static unsigned int hcsr04_read_mm(void) {
    unsigned int t = 0, to = 0;
    GPIO1_CLEARDATAOUT = TRIG_PIN;
    delay_us(2);
    GPIO1_SETDATAOUT = TRIG_PIN;
    delay_us(10);
    GPIO1_CLEARDATAOUT = TRIG_PIN;

    while (!(GPIO1_DATAIN & ECHO_PIN)) {
        if (++to > 40000) { uart0_write_str("timeout HIGH\r\n"); return 0; }
        delay_us(1);
    }

    t = 0; to = 0;
    while (GPIO1_DATAIN & ECHO_PIN) {
        delay_us(1);
        if (++to > 30000) { uart0_write_str("timeout LOW\r\n"); return 0; }
        t++;
    }

    uart0_write_str("Pulse: "); uart0_write_uint(t); uart0_write_str(" us\r\n");
    return t / 6; // retorna em milímetros
}

// Configuração inicial dos GPIOs e interrupções
static void gpio_setup(void) {
    CM_PER_GPIO1_CLKCTRL |= 0x2;
    while (!(CM_PER_GPIO1_CLKCTRL & 0x3));

    padSetMode(CM_CONF_GPMC_AD15, MODE_7); // TRIG
    padSetMode(CM_CONF_GPMC_AD14, MODE_7); // ECHO
    padSetMode(CM_CONF_GPMC_AD13, MODE_7); // LED_R
    padSetMode(CM_CONF_GPMC_AD12, MODE_7); // LED_G
    padSetMode(CM_CONF_GPMC_A0,   MODE_7); // BUTTON

    GPIO1_OE &= ~(TRIG_PIN | LED_R | LED_G);  // saída
    GPIO1_OE |=  (ECHO_PIN | BUTTON_PIN);     // entrada

    GPIO1_CLEARDATAOUT = TRIG_PIN | LED_R | LED_G;

    // Interrupção no botão (borda de subida)
    GPIO1_RISINGDETECT |= BUTTON_PIN;
    GPIO1_IRQSTATUS_0 = BUTTON_PIN;
    GPIO1_IRQSTATUS_SET_0 = BUTTON_PIN;

    // Habilita IRQ 98 no controlador
    INTC_MIR_CLEAR3 = (1 << 2);
}

// Trata o botão via interrupção
void gpio_isr_handler(void) {
    delay_ms(200); // debounce
    if (GPIO1_IRQSTATUS_0 & BUTTON_PIN) {
        GPIO1_IRQSTATUS_0 = BUTTON_PIN; // limpa flag
        sensor_ativo = !sensor_ativo;

        if (sensor_ativo) {
            uart0_write_str("Sensor LIGADO (via interrupção)\r\n");
            rgb_set(0,1);
        } else {
            uart0_write_str("Sensor DESLIGADO (via interrupção)\r\n");
            rgb_set(0,0);
        }
    }
}

// Handler geral da interrupção
void ISR_Handler(void) {
    uint32_t irq_number = INTC_SIR_IRQ & 0x7F;
    if (irq_number == 98) {
        gpio_isr_handler();
    }
    INTC_CONTROL = 0x1;
}

// Main
int main(void) {
    disable_watchdog();
    gpio_setup();

    uart0_write_str("\r\n===== Sensor de Proximidade =====\r\n\n");
    uart0_write_str("-- Use o botão para ligar/desligar o sensor --\r\n");

    while (1) {
        if (sensor_ativo) {
            uart0_write_str("Loop do sensor ativo\r\n");
            unsigned int dist = hcsr04_read_mm();
            if (dist) {
                uart0_write_str("Dist: "); uart0_write_uint(dist); uart0_write_str(" mm\r\n");
                if (dist < 150) {
                    rgb_set(1,0); uart0_write_str("Faixa: Vermelho\r\n");
                }
                else if (dist > 200 && dist < 600) {
                    rgb_set(1,1); uart0_write_str("Faixa: Amarelo\r\n");
                }
                else {
                    rgb_set(0,1); uart0_write_str("Faixa: Verde\r\n");
                }
            }
        }

        delay_ms(100);
    }

    return 0;
}
