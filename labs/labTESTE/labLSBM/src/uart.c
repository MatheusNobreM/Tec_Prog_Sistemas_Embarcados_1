#include <stdint.h>
#include "uart.h"

#define UART0_BASE     0x44E09000u
#define UART0_THR      (*(volatile unsigned int  *)(UART0_BASE + 0x00))
#define UART0_LSR      (*(volatile unsigned int  *)(UART0_BASE + 0x14))
#define UART0_LSR_THRE (1 << 5)

void uart_putc(char c) {
    while (!(UART0_LSR & UART0_LSR_THRE));
    UART0_THR = c;
}

void uart_write_str(const char *s) {
    while (*s) uart_putc(*s++);
}

void uart_write_uint(unsigned int n) {
    char buf[16];
    int i = 0;
    if (n == 0) {
        uart_putc('0');
        return;
    }
    while (n && i < 15) {
        //converte para ASCII e armazena
        buf[i++] = '0' + (n % 10);
        //remove o último dígito
        n /= 10;
    }
    while (i--) {
        //imprime o caractere de trás para frente
        uart_putc(buf[i]);
    }
}
