#include "uart.h"

// Validação de número de UART
static bool checkValidUart(UART_t uart) {
   if ((uart < UART0) || (uart > UART5)) {
      return false;
   }
   return true;
}

void uartInitModule(UART_t uart, unsigned int baudrate, STOP_BIT_t stopBit, PARITY_BIT_t parity, FLOW_t flowControl) {
   if (!checkValidUart(uart)) return;

   switch (uart) {
      case UART0:
         // Mux dos pinos
         cmSetCtrlModule(CM_conf_uart0_txd, 0);                    // TX sem alterações
         cmSetCtrlModule(CM_conf_uart0_rxd, (1 << 4) | (1 << 5));  // Pull-up + input enabled

         // Configura pinos para modo UART
         padSetMode(CM_conf_uart0_txd, MODE_0);
         padSetMode(CM_conf_uart0_rxd, MODE_0);

         // Ativa clock da UART0
         HWREG(SOC_CM_WKUP_REGS + CM_WKUP_UART0_CLKCTRL) = 0x2;
         while ((HWREG(SOC_CM_WKUP_REGS + CM_WKUP_UART0_CLKCTRL) & 0x3) != 0x2);

         // Soft reset da UART
         HWREG(UART0_BASE + 0x54) = 0x01;
         while ((HWREG(UART0_BASE + 0x58) & 0x01) != 0);

         // Configura UART (desabilita acesso a registros divisor)
         HWREG(UART0_BASE + 0x8C) = 0x83;

         // Clock: 48MHz / divisor = baud
         unsigned int divisor = 48000000 / (16 * baudrate);
         HWREG(UART0_BASE + 0x0C) = divisor & 0xFF;          // DLL
         HWREG(UART0_BASE + 0x04) = (divisor >> 8) & 0x3F;   // DLH

         // Configura linha (8 bits, 1 stop, sem paridade)
         HWREG(UART0_BASE + 0x0C) = 0x03;  // 8 bits

         // Desativa divisor access
         HWREG(UART0_BASE + 0x8C) = 0x03;

         // Habilita FIFO
         HWREG(UART0_BASE + 0x08) = 0x07;

         break;

      // UART1–5 (pode ser implementado depois)
      default:
         break;
   }
}

void uartPutC(UART_t uart, char c) {
   unsigned int base = UART_ARRAY_BASE[uart];
   while (!(HWREG(base + 0x14) & 0x20)); // THR empty
   HWREG(base + 0x00) = c;
}

char uartGetC(UART_t uart) {
   unsigned int base = UART_ARRAY_BASE[uart];
   while (!(HWREG(base + 0x14) & 0x01)); // RBR has data
   return HWREG(base + 0x00);
}

int uartPutString(UART_t uart, char *str, unsigned int length) {
   for (int i = 0; i < length; i++) {
      uartPutC(uart, str[i]);
   }
   return length;
}

int uartGetString(UART_t uart, char *buf, unsigned int length) {
   for (int i = 0; i < length; i++) {
      buf[i] = uartGetC(uart);
   }
   return length;
}
