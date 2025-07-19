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

void delay1(volatile unsigned int count) {
    while (count--);
}

void mandar_pulso() {
    outState(GPIO1, TRIGGER_PIN, LOW);
    delay1(20000);
    outState(GPIO1, TRIGGER_PIN, HIGH);
    delay1(100000);
    outState(GPIO1, TRIGGER_PIN, LOW);
}

void rgb_set(State r, State g, State b) {
    outState(GPIO1, LED_R_PIN, r);
    outState(GPIO1, LED_G_PIN, g);
    outState(GPIO1, LED_B_PIN, b);
}

void ISR_Handler(unsigned int interrupcao) {
    // Para debug:
    // uartPutString(UART0, "Interrupcao!\r\n", 14);
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

    int tempo, distancia;
    int start_time, end_time;

    

    while (1) {
        // Dispara o pulso TRIGGER
        mandar_pulso();

        // Aguarda a borda de subida no ECHO (eco começou)
        while (!gpio_Input(GPIO1, ECHO_PIN));
        timerEnable(TIMER2);
        start_time = HWREG(SOC_DMTIMER_2_REGS + DMTIMER_TCRR);

        // Aguarda a borda de descida no ECHO (eco terminou)
        while (gpio_Input(GPIO1, ECHO_PIN));
        end_time = HWREG(SOC_DMTIMER_2_REGS + DMTIMER_TCRR);
        timerDisable(TIMER2);

        tempo = end_time - start_time;
        distancia = divu((tempo * VELOCIDADE_DO_SOM), 2000); // mm

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

        delay(50, TIMER2);
    }

    return 0;
}
