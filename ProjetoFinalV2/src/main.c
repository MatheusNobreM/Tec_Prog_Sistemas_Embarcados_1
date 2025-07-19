#include "interrupt.h"
#include "gpio.h"
#include "timer.h"
#include "hw_types.h"
#include "soc_AM335x.h"
#include "uart.h"

// Pinos
#define TRIGGER_PIN     15  // GPIO1_15 → TRIGGER
#define ECHO_PIN        14  // GPIO1_14 → ECHO
#define LED_R_PIN       16  // GPIO1_16 → LED RGB - RED
#define LED_G_PIN       13  // GPIO1_13 → LED RGB - GREEN
#define LED_B_PIN       12  // GPIO1_12 → LED RGB - BLUE

#define VELOCIDADE_DO_SOM 343 // m/s

// Flags de medição
volatile int start_time = 0;
volatile int end_time = 0;
volatile int echo_state = 0; // 0: aguardando subida, 1: aguardando descida
volatile int ready = 0;      // Flag de medição completa

int get_timer_value() {
    return HWREG(SOC_DMTIMER_2_REGS + DMTIMER_TCRR);
}

void delay1(volatile unsigned int count) {
    while (count--);
}

// Envia pulso no TRIGGER do sensor
void mandar_pulso() {
    outState(GPIO1, TRIGGER_PIN, LOW);
    delay1(20000);
    outState(GPIO1, TRIGGER_PIN, HIGH);
    delay1(100000);
    outState(GPIO1, TRIGGER_PIN, LOW);
}

// Configura LED RGB
void rgb_set(State r, State g, State b) {
    outState(GPIO1, LED_R_PIN, r);
    outState(GPIO1, LED_G_PIN, g);
    outState(GPIO1, LED_B_PIN, b);
}

// Interrupção do ECHO
void ISR_Handler(unsigned int interrupcao) {
    unsigned int irq_number = HWREG(INTC_BASE + INTC_SIR_IRQ) & 0x7f;

    if (irq_number == 98) { // GPIO1 interrupt
        if (HWREG(SOC_GPIO_1_REGS + GPIO_IRQSTATUS_0) & (1 << ECHO_PIN)) {
            if (gpio_Input(GPIO1, ECHO_PIN)) {
                // Borda de subida
                timerEnable(TIMER2);
                start_time = get_timer_value();
                echo_state = 1;
                HWREG(SOC_GPIO_1_REGS + GPIO_FALLINGDETECT) |= (1 << ECHO_PIN);
                HWREG(SOC_GPIO_1_REGS + GPIO_RISINGDETECT) &= ~(1 << ECHO_PIN);
            } else {
                // Borda de descida
                end_time = get_timer_value();
                timerDisable(TIMER2);
                HWREG(SOC_GPIO_1_REGS + GPIO_RISINGDETECT) |= (1 << ECHO_PIN);
                HWREG(SOC_GPIO_1_REGS + GPIO_FALLINGDETECT) &= ~(1 << ECHO_PIN);
                ready = 1; // Medição completa
                echo_state = 0;
            }

            gpioIsrHandler(gpio_Input(GPIO1, ECHO_PIN), ECHO_PIN);
        }
    }

    HWREG(INTC_BASE + INTC_CONTROL) = 0x1; // Limpa IRQ
}


int main(void) {
    watchdog();

    Init_gpio(GPIO1);
    timerSetup(TIMER2);

    // Pinos do sensor
    Init(TRIGGER_PIN, A15, GPIO1, OUTPUT, 7);
    Init(ECHO_PIN, A14, GPIO1, INPUT, 7);

    // RGB: R, G, B
    Init(LED_R_PIN, A16, GPIO1, OUTPUT, 7);
    Init(LED_G_PIN, A13, GPIO1, OUTPUT, 7);
    Init(LED_B_PIN, A12, GPIO1, OUTPUT, 7);
    rgb_set(LOW, LOW, LOW);

    // UART0
    uartInitModule(UART0, 115200, STOP1, PARITY_NONE, FLOW_OFF);

    // Configura interrupção GPIO1_14 (ECHO)
    pinInterrup(0, GPIO1, ECHO_PIN);
    Interrupt_Setup(98); // IRQ para GPIO1
    HWREG(INTC_BASE + INTC_MIR_CLEAR2) |= (1 << (98 % 32));
    HWREG(INTC_BASE + INTC_ILR(98)) = 0; // IRQ level-sensitive
    HWREG(INTC_BASE + INTC_SYSCONFIG) |= (1 << 1); // Auto idle

    int tempo, distancia;

    while (1) {
        mandar_pulso();

        // Espera interrupções completarem
        while (!ready);

        tempo = end_time - start_time;
        distancia = divu((tempo * VELOCIDADE_DO_SOM), 2000); // em mm

        // UART: envia distância
        uartPutString(UART0, "Distancia: ", 11);
        uartPutInt(UART0, distancia);
        uartPutString(UART0, " mm\r\n", 5);

        // LED RGB por faixa
        if (distancia < 5000) {
            rgb_set(HIGH, LOW, LOW);     // Vermelho
        } else if (distancia < 10000) {
            rgb_set(HIGH, HIGH, LOW);    // Amarelo
        } else {
            rgb_set(LOW, HIGH, LOW);     // Verde
        }

        ready = 0;
        delay(50, TIMER2);
    }

    return 0;
}
