#include "watchdog.h"
#include "gpio.h"
#include "uart.h"
#include "hcsr04.h"
#include "delay.h"

// Registradores de interrupção
#define GPIO1_IRQSTATUS_0     (*(volatile unsigned int*)(0x4804C02C))
#define GPIO1_IRQSTATUS_SET_0 (*(volatile unsigned int*)(0x4804C034))
#define GPIO1_RISINGDETECT    (*(volatile unsigned int*)(0x4804C148))
#define INTC_SIR_IRQ          (*(volatile unsigned int*)(0x48200040))
#define INTC_CONTROL          (*(volatile unsigned int*)(0x48200048))
#define INTC_MIR_CLEAR3       (*(volatile unsigned int*)(0x482000E8))

#define DEBOUNCE_MS 200

volatile int sensor_toggle_flag = 0;

void gpio_enable_button_interrupt(void) {
    GPIO1_RISINGDETECT |= BUTTON_PIN;         // Ativa detecção por borda de subida
    GPIO1_IRQSTATUS_0 = BUTTON_PIN;           // Limpa flag
    GPIO1_IRQSTATUS_SET_0 = BUTTON_PIN;       // Habilita interrupção no pino
    INTC_MIR_CLEAR3 = (1 << 2);               // Habilita IRQ 98 (GPIO1) no controlador
}

void ISR_Handler(void) {
    unsigned int irq_number = INTC_SIR_IRQ & 0x7F;
    if (irq_number == 98) { // IRQ do GPIO1
        delay_ms(DEBOUNCE_MS);
        if (GPIO1_IRQSTATUS_0 & BUTTON_PIN) {
            GPIO1_IRQSTATUS_0 = BUTTON_PIN;   // Limpa flag de interrupção
            sensor_toggle_flag = 1;           // Sinaliza alternância do sensor
        }
    }
    INTC_CONTROL = 0x1; // Reabilita interrupções
}

int main(void) {
    watchdog_disable();
    gpio_setup();
    gpio_enable_button_interrupt();

    uart_write_str("\r\n===== Sensor de Proximidade =====\r\n\n");
    uart_write_str("\r\n-- Aperte o botão para começar --\r\n");

    int sensor_ativo = 0;

    while (1) {
        // Ativa/desativa sensor se flag estiver setada pela interrupção
        if (sensor_toggle_flag) {
            sensor_toggle_flag = 0;
            sensor_ativo = !sensor_ativo;
            if (sensor_ativo) {
                uart_write_str("Sensor LIGADO\r\n");
                rgb_set(0,1);
            } else {
                uart_write_str("Sensor DESLIGADO\r\n");
                rgb_set(0,0);
            }
        }

        
        if (sensor_ativo) {
            uart_write_str("Loop do sensor ativo\r\n");
            unsigned int dist = hcsr04_read_mm(); 
            if (dist) {
                uart_write_str("Dist: ");
                uart_write_uint(dist);
                uart_write_str(" cm\r\n");

                if (dist <= 3) {
                    rgb_set(1,0);
                    uart_write_str("Faixa: Vermelho\r\n");
                } else if (dist >= 4 && dist <= 6) {
                    rgb_set(1,1);
                    uart_write_str("Faixa: Amarelo\r\n");
                } else if (dist <= 7){
                    rgb_set(0,1);
                    uart_write_str("Faixa: Verde\r\n");
                }else {
                    
                }
            }
        }

        delay_ms(100);
    }

    return 0;
}
