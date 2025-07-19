#include <stdint.h>

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

// GPIO
#define CM_PER_BASE             0x44E00000
#define CM_PER_GPIO1_CLKCTRL    (*(volatile uint32_t*)(CM_PER_BASE + 0xAC))
#define GPIO1_BASE              0x4804C000
#define GPIO1_OE                (*(volatile uint32_t*)(GPIO1_BASE + 0x134))
#define GPIO1_SETDATAOUT        (*(volatile uint32_t*)(GPIO1_BASE + 0x194))
#define GPIO1_CLEARDATAOUT      (*(volatile uint32_t*)(GPIO1_BASE + 0x190))
#define GPIO1_DATAIN            (*(volatile uint32_t*)(GPIO1_BASE + 0x138))

// Pins
#define TRIG_PIN    (1 << 15)    // GPIO1_15 (P8_15)
#define ECHO_PIN    (1 << 14)    // GPIO1_14 (P8_16)
#define LED_R       (1 << 13)    // GPIO1_13 (P8_11)

// Delay
void delay_us(unsigned int us) { volatile unsigned int c = us * 14; while(c--); }
void delay_ms(unsigned int ms) { while(ms--) delay_us(1000); }

void gpio_setup(void) {
    CM_PER_GPIO1_CLKCTRL |= 0x2;
    while (!(CM_PER_GPIO1_CLKCTRL & 0x3));
    GPIO1_OE &= ~(TRIG_PIN | LED_R); // TRIG e LED como saída
    GPIO1_OE |= ECHO_PIN;            // ECHO como entrada
}

// Leitura do sensor
unsigned int hcsr04_read_mm() {
    unsigned int t = 0;
    // Garante TRIG em LOW
    GPIO1_CLEARDATAOUT = TRIG_PIN;
    delay_us(2);

    // Pulso TRIG de 10us
    GPIO1_SETDATAOUT = TRIG_PIN;
    delay_us(10);
    GPIO1_CLEARDATAOUT = TRIG_PIN;

    // Espera ECHO subir
    int timeout = 0;
    while (!(GPIO1_DATAIN & ECHO_PIN)) {
        timeout++; if (timeout > 40000) { uart0_write_str("timeout HIGH\r\n"); return 0; }
    }
    t = 0;
    while (GPIO1_DATAIN & ECHO_PIN) {
        t++; delay_us(1);
        if (t > 30000) { uart0_write_str("timeout LOW\r\n"); return 0; }
    }
    uart0_write_str("Pulse: ");
    uart0_write_uint(t);
    uart0_write_str(" us\r\n");
    return t / 6; // Aproximação em mm
}

void ISR_Handler(void) {} // Dummy

int main(void) {
    disable_watchdog();
    gpio_setup();
    uart0_write_str("\r\n---- HC-SR04 + LED ----\r\n");

    // Liga o LED vermelho ao iniciar
    GPIO1_SETDATAOUT = LED_R;

    while(1) {
        unsigned int distancia = hcsr04_read_mm();

        uart0_write_str("Dist: ");
        uart0_write_uint(distancia);
        uart0_write_str(" mm\r\n");

        if (distancia > 0 && distancia < 150) { 
            GPIO1_CLEARDATAOUT = LED_R;
            uart0_write_str("LED APAGADO\r\n");
        } else {
            GPIO1_SETDATAOUT = LED_R;
        }


        delay_ms(500);
    }
    return 0;
}
