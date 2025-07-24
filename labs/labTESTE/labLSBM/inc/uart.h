#ifndef UART_H
#define UART_H

#include <stdint.h>

// Envia um caractere pela UART0
void uart_putc(char c);
// Envia uma string pela UART0
void uart_write_str(const char *s);
// Envia um inteiro decimal pela UART0
void uart_write_uint(unsigned int n);

#endif // UART_H
