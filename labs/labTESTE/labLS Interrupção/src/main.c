#include <stdint.h>
#include "pad.h"
#include "control_module.h"

// Watchdog minimal
#define WDT1_BASE 0x44E35000
#define WDT_WSPR   (*(volatile uint32_t *)(WDT1_BASE + 0x48))
#define WDT_WWPS   (*(volatile uint32_t *)(WDT1_BASE + 0x34))
void disable_watchdog(void) {
    WDT_WSPR = 0xAAAA; while (WDT_WWPS & (1 << 4));
    WDT_WSPR = 0x5555; while (WDT_WWPS & (1 << 4));
}

// UART0 minimal
#define UART0_BASE      0x44E09000
#define UART0_THR       (*(volatile unsigned int *)(UART0_BASE + 0x00))
#define UART0_LSR       (*(volatile unsigned int *)(UART0_BASE + 0x14))
#define UART0_LSR_THRE  (1 << 5)
void uart0_putc(char c)  { while (!(UART0_LSR & UART0_LSR_THRE)); UART0_THR = c; }
void uart0_write_str(const char *s) { while (*s) uart0_putc(*s++); }
void uart0_write_uint(unsigned int n) {
    char buf[16]; int i = 0;
    if (n == 0) { uart0_putc('0'); return; }
    while (n && i < 15) { buf[i++] = '0' + (n % 10); n /= 10; }
    while (i--) uart0_putc(buf[i]);
}

// Clock & GPIO
#define CM_PER_BASE           0x44E00000
#define CM_PER_GPIO1_CLKCTRL  (*(volatile uint32_t*)(CM_PER_BASE + 0xAC))
#define GPIO1_BASE            0x4804C000
#define GPIO1_OE              (*(volatile uint32_t*)(GPIO1_BASE + 0x134))
#define GPIO1_SETDATAOUT      (*(volatile uint32_t*)(GPIO1_BASE + 0x194))
#define GPIO1_CLEARDATAOUT    (*(volatile uint32_t*)(GPIO1_BASE + 0x190))
#define GPIO1_DATAIN          (*(volatile uint32_t*)(GPIO1_BASE + 0x138))
#define GPIO1_IRQSTATUS_0     (*(volatile uint32_t*)(GPIO1_BASE + 0x2C))
#define GPIO1_IRQSTATUS_SET_0 (*(volatile uint32_t*)(GPIO1_BASE + 0x34))
#define GPIO1_RISINGDETECT    (*(volatile uint32_t*)(GPIO1_BASE + 0x148))

// INTC
#define INTC_BASE           0x48200000
#define INTC_SYSCONFIG      (*(volatile uint32_t*)(INTC_BASE + 0x10))
#define INTC_CONTROL        (*(volatile uint32_t*)(INTC_BASE + 0x48))
#define INTC_MIR_CLEAR_3    (*(volatile uint32_t*)(INTC_BASE + 0xE8))

// Pins
#define TRIG_PIN    (1 << 15) // GPIO1_15 → P8_15
#define ECHO_PIN    (1 << 14) // GPIO1_14 → P8_16
#define LED_R       (1 << 13) // GPIO1_13 → P8_11
#define LED_G       (1 << 12) // GPIO1_12 → P8_12
#define BUTTON_PIN  (1 << 17) // GPIO1_17 → P9_23

volatile int sensor_ativo = 0;
volatile uint32_t last_button_time = 0;
#define DEBOUNCE_MS 200

void delay_us(unsigned int us) { volatile unsigned int c = us * 14; while (c--); }
void delay_ms(unsigned int ms) { while (ms--) delay_us(1000); }

void gpio_setup(void) {
    // Ativar clock do GPIO1
    CM_PER_GPIO1_CLKCTRL |= 0x2;
    while (!(CM_PER_GPIO1_CLKCTRL & 0x3));

    // Mapeamento de pinos via padSetMode
    padSetMode(CM_CONF_GPMC_AD15, MODE_7); // TRIG → P8_15
    padSetMode(CM_CONF_GPMC_AD14, MODE_7); // ECHO → P8_16
    padSetMode(CM_CONF_GPMC_AD13, MODE_7); // LED_R → P8_11
    padSetMode(CM_CONF_GPMC_AD12, MODE_7); // LED_G → P8_12
    padSetMode(CM_CONF_GPMC_A0,   MODE_7); // BUTTON → P9_23

    // Direções: saídas e entradas
    GPIO1_OE &= ~(TRIG_PIN | LED_R | LED_G); // Config como saída
    GPIO1_OE |= (ECHO_PIN | BUTTON_PIN);     // Config como entrada

    // Inicializar LEDs e TRIG desligados
    GPIO1_CLEARDATAOUT = LED_R | LED_G | TRIG_PIN; // Garante TRIG em LOW

    // Delay inicial para estabilizar o sensor
    delay_ms(100);
}

void gpio_button_interrupt_enable(void) {
    GPIO1_IRQSTATUS_0 = BUTTON_PIN;        // Limpa status inicial
    GPIO1_RISINGDETECT |= BUTTON_PIN;      // Detecta borda de subida
    GPIO1_IRQSTATUS_SET_0 |= BUTTON_PIN;   // Habilita interrupção
}

void intc_interrupt_enable(void) {
    INTC_SYSCONFIG |= (1 << 2);  // Soft reset
    while (INTC_SYSCONFIG & (1 << 2));
    INTC_CONTROL |= (1 << 0);    // Habilita interrupções globais
    INTC_MIR_CLEAR_3 |= (1 << 2); // Habilita IRQ98 (GPIO1)
}

void rgb_set(int r, int g) {
    if (r) GPIO1_SETDATAOUT = LED_R; else GPIO1_CLEARDATAOUT = LED_R;
    if (g) GPIO1_SETDATAOUT = LED_G; else GPIO1_CLEARDATAOUT = LED_G;
    uart0_write_str("LEDs: R="); uart0_write_uint(r); uart0_write_str(" G="); uart0_write_uint(g); uart0_write_str("\r\n");
}

unsigned int hcsr04_read_mm(void) {
    unsigned int t = 0, to = 0;

    // Depuração: indicar início da leitura
    uart0_write_str("Iniciando leitura do sensor...\r\n");

    // Garante TRIG em LOW por 2us
    GPIO1_CLEARDATAOUT = TRIG_PIN;
    delay_us(20); // Aumentado para garantir estado estável

    // Pulso de 10us no TRIG
    GPIO1_SETDATAOUT = TRIG_PIN;
    delay_us(10);
    GPIO1_CLEARDATAOUT = TRIG_PIN;

    // Aguarda ECHO subir
    to = 0;
    while (!(GPIO1_DATAIN & ECHO_PIN)) {
        if (++to > 60000) { // Timeout aumentado
            uart0_write_str("timeout HIGH\r\n");
            return 0;
        }
        delay_us(1);
    }

    // Depuração: ECHO subiu
    uart0_write_str("ECHO HIGH detectado\r\n");

    // Conta tempo do pulso ECHO
    t = 0; to = 0;
    while (GPIO1_DATAIN & ECHO_PIN) {
        delay_us(1);
        if (++to > 60000) { // Timeout aumentado
            uart0_write_str("timeout LOW\r\n");
            return 0;
        }
        t++;
    }

    // Depuração: ECHO desceu
    uart0_write_str("ECHO LOW detectado\r\n");

    // Exibe tempo do pulso
    uart0_write_str("Pulse: "); uart0_write_uint(t); uart0_write_str(" us\r\n");

    // Converte para milímetros (velocidade do som ~343 m/s)
    return t / 6; // Aproximação em mm
}

void ISR_Handler(void) {
    uint32_t st = GPIO1_IRQSTATUS_0;

    if (st & BUTTON_PIN) {
        delay_ms(DEBOUNCE_MS);
        if (GPIO1_DATAIN & BUTTON_PIN) { // Verifica se o botão ainda está pressionado
            sensor_ativo = !sensor_ativo;
            last_button_time = 0;
            if (sensor_ativo) {
                uart0_write_str("Sensor LIGADO\r\n");
                rgb_set(0, 1); // Acende LED verde
            } else {
                uart0_write_str("Sensor DESLIGADO\r\n");
                rgb_set(0, 0); // Desliga LEDs
            }
        }
        GPIO1_IRQSTATUS_0 = BUTTON_PIN; // Limpa interrupção
    }
}

int main(void) {
    disable_watchdog();
    gpio_setup();
    uart0_write_str("\r\n-- HC-SR04 + LED RGB + Botão Liga/Desliga --\r\n");
    uart0_write_str("Botão em P9_23 (GPIO1_17)\r\n");

    gpio_button_interrupt_enable();
    intc_interrupt_enable();

    while (1) {
        if (sensor_ativo) {
            // Depuração: confirmar que o loop do sensor está ativo
            uart0_write_str("Loop do sensor ativo\r\n");
            unsigned int distancia = hcsr04_read_mm();
            if (distancia > 0) { // Só processa leituras válidas
                uart0_write_str("Dist: "); uart0_write_uint(distancia); uart0_write_str(" mm\r\n");

                if (distancia < 200) {
                    rgb_set(1, 0); // Vermelho
                    uart0_write_str("Faixa: Vermelho\r\n");
                } else if (distancia < 500) {
                    rgb_set(1, 1); // Amarelo
                    uart0_write_str("Faixa: Amarelo\r\n");
                } else {
                    rgb_set(0, 1); // Verde
                    uart0_write_str("Faixa: Verde\r\n");
                }
            } else {
                uart0_write_str("Leitura inválida, verificando sensor...\r\n");
            }
        }
        delay_ms(500);
    }
    return 0;
}