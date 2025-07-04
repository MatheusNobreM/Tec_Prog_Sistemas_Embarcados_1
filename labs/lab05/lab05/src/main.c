#include <stdbool.h>
#include <stdint.h>

// Definições de registradores
#define WDT1_BASE 0x44E35000
#define WDT_WSPR (*(volatile uint32_t *)(WDT1_BASE + 0x48))
#define WDT_WWPS (*(volatile uint32_t *)(WDT1_BASE + 0x34))

#define CM_PER_BASE 0x44E00000
#define CM_PER_GPIO1_CLKCTRL (*(volatile uint32_t*)(CM_PER_BASE + 0xAC))

#define GPIO1_BASE 0x4804C000
#define GPIO1_OE (*(volatile uint32_t*)(GPIO1_BASE + 0x134))
#define GPIO1_CLEARDATAOUT (*(volatile uint32_t*)(GPIO1_BASE + 0x190))
#define GPIO1_SETDATAOUT (*(volatile uint32_t*)(GPIO1_BASE + 0x194))
#define GPIO1_DATAIN (*(volatile uint32_t*)(GPIO1_BASE + 0x138))
#define GPIO1_IRQSTATUS_SET_0 (*(volatile uint32_t*)(GPIO1_BASE + 0x34))
#define GPIO1_IRQSTATUS_0 (*(volatile uint32_t*)(GPIO1_BASE + 0x2C))
#define GPIO1_RISINGDETECT (*(volatile uint32_t*)(GPIO1_BASE + 0x148))

#define CONTROL_MODULE_BASE 0x44E10000
#define CONF_GPMC_AD6 (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x818))  //MUX DO BOTÃO INT
#define CONF_GPMC_AD7 (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x81C))  //MUX DO BOTÃO WHITE
#define CONF_GPMC_AD3 (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x80C))  //MUX DO BOTÃO RED
#define CONF_GPMC_AD13 (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x834)) //MUX DO LED RED
#define CONF_GPMC_AD12 (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x830)) //MUX DO LED WHITE


#define INTC_BASE 0x48200000
#define INTC_MIR_CLEAR3 (*(volatile uint32_t*)(INTC_BASE + 0xE8))
#define INTC_SIR_IRQ (*(volatile uint32_t*)(INTC_BASE + 0x40))
#define INTC_CONTROL (*(volatile uint32_t*)(INTC_BASE + 0x48))

#define UART0_BASE 0x44E09000
#define UART0_THR (*(volatile unsigned int *)(UART0_BASE + 0x00))     // Registrador de Transmissão (que é o mesmo que o de Recepção)
#define UART0_LSR (*(volatile unsigned int *)(UART0_BASE + 0x14))     // Base do LSR
#define UART0_LSR_THRE (1 << 5)                                       // Avisa que registrador THR transmite dados
#define UART0_LSR_RHRE (1 << 0)                                       // Avisa que registrador RHR recebe dados

void uart0_putc(char c) {
    while (!(UART0_LSR & UART0_LSR_THRE));
    UART0_THR = c;
}

unsigned char uart0_getc(){
  while (!(UART0_LSR & UART0_LSR_RHRE));
  return UART0_THR;
}

void uart0_write_str(const char *str) {
    while (*str) {
        uart0_putc(*str++);
    }
}
// Definições de pinos
#define LED_INT (1 << 21)  // P8_21 (LED INT)
#define BUTTON1_PIN (1 << 6) // P8_3(BOTÃO INT)
#define BUTTON2_PIN (1 << 3)// P8_6 
#define BUTTON3_PIN (1 << 7)// P8_4 
#define LED1 (1 << 13)// P8_11 PINO DO LED 1
#define LED2 (1 << 12)// P8_12 PINO DO LED 2


void uart0_write_int(unsigned char inteiro) {
    int resto = 0;
    int contador = 0;
    char final[100];
    char temp[100];

    while (inteiro){
        resto = inteiro % 10;
        temp[contador] = resto + '0';
        inteiro /= 10;
        contador++;
    }

    for (int i = 0; i < contador; i++){
        final[i] = temp[contador - i - 1];
    }

    final[contador] = '\r';
    final[contador+1] = '\n';

    uart0_write_str(final);
}


void disable_watchdog(void) {
    WDT_WSPR = 0xAAAA;
    while (WDT_WWPS & (1 << 4));
    WDT_WSPR = 0x5555;
    while (WDT_WWPS & (1 << 4));
}

volatile bool button_int_pressed = false;
volatile bool button_2_pressed = false;
volatile bool button_3_pressed = false;

void delay(uint32_t cycles) {
    volatile uint32_t i;
    for(i = 0; i < cycles; i++);
}

void gpio_setup(void) {
    // Ativa clock do GPIO1
    CM_PER_GPIO1_CLKCTRL |= 0x2;
    while(!(CM_PER_GPIO1_CLKCTRL & 0x3)); // Espera ativação
    
    CONF_GPMC_AD6 |= (7 | (1 << 5));//SET O BOTÃO INT MOD7 E RECEPTOR ATIVO
    CONF_GPMC_AD3 |= (7 | (1 << 5));//SET O BOTÃO 1 MOD7 E RECEPTOR ATIVO
    CONF_GPMC_AD7 |= (7 | (1 << 5));//SET O BOTÃO 2 MOD7 E RECEPTOR ATIVO
    CONF_GPMC_AD13 |= 7;//SET LED 1 NO MOD7
    CONF_GPMC_AD12 |= 7;//SET LED 2 NO MOD7

    // Configura pinos
    GPIO1_OE &= ~LED_INT;       // LED INT como saída
    GPIO1_OE &= ~LED2;     // LED 2 como saida
    GPIO1_OE &= ~LED1;       // LED 1 como saida 
    GPIO1_OE |= BUTTON1_PIN;    // Botão LED INT como entrada
    GPIO1_OE |= BUTTON2_PIN;    // Botão LED 1 como entrada
    GPIO1_OE |= BUTTON3_PIN;    // Botão LED 2 como entrada
    
    
    // Configura interrupção

    //BOTÃO INT
    GPIO1_RISINGDETECT |= BUTTON1_PIN;  // Borda de subida
    GPIO1_IRQSTATUS_0 = BUTTON1_PIN;    // Limpa flag pendente
    GPIO1_IRQSTATUS_SET_0 = BUTTON1_PIN; // Habilita interrupção

    //BOTÃO RED
    GPIO1_RISINGDETECT |= BUTTON2_PIN;  // Borda de subida
    GPIO1_IRQSTATUS_0 = BUTTON2_PIN;    // Limpa flag pendente
    GPIO1_IRQSTATUS_SET_0 = BUTTON2_PIN; // Habilita interrupção

    //BOTÃO WHITE
    GPIO1_RISINGDETECT |= BUTTON3_PIN;  // Borda de subida
    GPIO1_IRQSTATUS_0 = BUTTON3_PIN;    // Limpa flag pendente
    GPIO1_IRQSTATUS_SET_0 = BUTTON3_PIN; // Habilita interrupção
    
    // Configura INTC
    INTC_MIR_CLEAR3 = (1 << 2);  // Habilita IRQ 98 (GPIO1)
  }
  
void printTextInt(){
  if (button_int_pressed){
    uart0_write_str("LED INTERNO ACESO\r\n");
  } else {
    uart0_write_str("LED INTERNO APAGADO\r\n");
  }
}
void printText1(){
  if (button_2_pressed){
    uart0_write_str("LED 1 ACESO\r\n");
  } else {
    uart0_write_str("LED 1 APAGADO\r\n");
  }
}
void printText2(){
  if (button_3_pressed){
    uart0_write_str("LED 2 ACESO\r\n");
  } else {
    uart0_write_str("LED 2 APAGADO\r\n");
  }
}

void changeInt(){
  if (button_int_pressed){
    GPIO1_SETDATAOUT = LED_INT;
  } else {
    GPIO1_CLEARDATAOUT = LED_INT;
  }
}

void change1(){
  if (button_2_pressed){
    GPIO1_SETDATAOUT = LED1;
  } else {
    GPIO1_CLEARDATAOUT = LED1;
  }
}

void change2(){
  if (button_3_pressed){
    GPIO1_SETDATAOUT = LED2;
  } else {
    GPIO1_CLEARDATAOUT = LED2;
  }
}


void gpio_isr_handler(void) {
    // if(GPIO1_IRQSTATUS_0 & BUTTON1_PIN) {
    //     delay(1000000);
    //     GPIO1_IRQSTATUS_0 = BUTTON1_PIN; // Limpa flag
    //     button_int_pressed = !button_int_pressed;
    //     uart0_write_str("<Botão 1>\r\n");
    //     printTextInt();
    //     changeInt();
    // }
    if(GPIO1_IRQSTATUS_0 & BUTTON2_PIN) {
        delay(1000000);
        GPIO1_IRQSTATUS_0 = BUTTON2_PIN; // Limpa flag
        button_2_pressed = !button_2_pressed;
        uart0_write_str("<Botão 2>\r\n");
        printText1();
        change1();
        
    }
    if(GPIO1_IRQSTATUS_0 & BUTTON3_PIN) {
        delay(1000000);
        GPIO1_IRQSTATUS_0 = BUTTON3_PIN; // Limpa flag
        button_3_pressed = !button_3_pressed;
        uart0_write_str("<Botão 3>\r\n");
        printText2();
        change2();
    }
    uart0_write_str("\r\n");
}


void ISR_Handler(void) {
    uint32_t irq_number = INTC_SIR_IRQ & 0x7F;
    if(irq_number == 98) {  // IRQ do GPIO1
        gpio_isr_handler();
    }
    INTC_CONTROL = 0x1;     // Reabilita interrupções
}

int main(void) {
    disable_watchdog();

    gpio_setup();
    uart0_write_str("****************************************\r\n");
    uart0_write_str("             PROJETO\r\n");
    uart0_write_str("****************************************\r\n\n");
    uart0_write_str("BOTÃO 1 - LED INTERNO\r\n");
    uart0_write_str("BOTÃO 2 - LED 1\r\n");
    uart0_write_str("BOTÃO 3 - LED 2\n\n");
    
    // Loop principal
	while(1){
		unsigned char entrada = uart0_getc();
    uart0_write_str("\r\n");
		if (entrada == '1'){
			uart0_write_str("LED INTERNO LIGADO\r\n");
			GPIO1_SETDATAOUT = LED_INT;
		} else if (entrada == '0'){
			uart0_write_str("LED INTERNO DESLIGADO\r\n");
			GPIO1_CLEARDATAOUT = LED_INT;
		} else if (entrada - '0' == 2){
      uart0_write_int(2);
      delay(1000000);
      button_2_pressed = !button_2_pressed;
      change1();
      printText1();
    } else if (entrada - '0' == 3){
      uart0_write_int(3);
      delay(1000000);
      button_3_pressed = !button_3_pressed;
      change2();
      printText2();
    }
    uart0_write_str("\r\n");
    
	}
    
    return 0;
}