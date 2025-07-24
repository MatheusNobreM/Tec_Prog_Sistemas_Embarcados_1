#include <stdint.h>
#include "hcsr04.h"
#include "gpio.h"   
#include "uart.h"
#include "delay.h"

unsigned int hcsr04_read_mm(void) {
    unsigned int t = 0, to = 0;

    // Trigger
    GPIO1_CLEARDATAOUT = TRIG_PIN; // low
    delay_us(2);
    GPIO1_SETDATAOUT = TRIG_PIN; //high
    delay_us(10);
    GPIO1_CLEARDATAOUT = TRIG_PIN; // low

    // espera ECHO high
    while (!(GPIO1_DATAIN & ECHO_PIN)) {
        if (++to > 40000) {
            uart_write_str("timeout HIGH\r\n");
            return 0;
        }
        delay_us(1);
    }

    // mede tempo high
    t = 0; to = 0;
    while (GPIO1_DATAIN & ECHO_PIN) {
        delay_us(1);
        if (++to > 30000) {
            uart_write_str("timeout LOW\r\n");
            return 0;
        }
        t++;
    }

     uart_write_str("Pulse: ");
    uart_write_uint(t);
    uart_write_str(" us\r\n");

    return (t/95) / 2; 
}
