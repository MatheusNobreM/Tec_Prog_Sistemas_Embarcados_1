#include <stdbool.h>

// macros do clock
#define CM_PER 						(*(volatile unsigned int *)0x44E00000)
#define CM_PER_GPIO1_CLKCTRL 		(*(volatile unsigned int *)0x44E000AC)

// macros GPIO
#define GPIO1_EO 					(*(volatile unsigned int *)0x4804C134)
#define GPIO1_CLEARDATAOUT 			(*(volatile unsigned int *)0x4804C190)
#define GPIO1_SETDATAOUT 			(*(volatile unsigned int *)0x4804C194)
#define GPIO1_DATAIN				(*(volatile unsigned int *)0x4804C138)

#define GPIO1_IRQSTATUS_SET_0 		(*(volatile unsigned int *)0x4804C034)
#define GPIO1_IRQSTATUS_0 			(*(volatile unsigned int *)0x4804C02C)
#define GPIO1_IRQSTATUS_CLR_0 		(*(volatile unsigned int *)0x4804C03C)
#define GPIO1_RISINGDETECT 			(*(volatile unsigned int *)0x4804C148)

// Controle dos pinos (Pad control)
#define CM_PER_GPMCBEn1_REGS 		(*(volatile unsigned int *)0x44E10878) // O sinal do GPIO1_28 passa por dentro desse multiplexador
#define CM_PER_GPMCA0_REGS 			(*(volatile unsigned int *)0x44E10840)	// O sinal do GPIO1_16 passa por dentro desse multiplexador
#define CM_PER_GPMCA5_REGS			(*(volatile unsigned int *)0x44E10854)	// O sinal do GPIO1_21 passa por dentro desse multiplexador
#define CM_PER_GPMCA6_REGS 			(*(volatile unsigned int *)0x44E10858)	// O sinal do GPIO1_22 passa por dentro desse multiplexador

// macros INT
#define INTCPS 						(*(volatile unsigned int *)0x48200000)
#define INTC_SIR_IRQ 				(*(volatile unsigned int *)0x48200040)
#define INTC_CONTROL 				(*(volatile unsigned int *)0x48200048)
#define INTC_MIR_CLEAR_3 			(*(volatile unsigned int *)0x482000E8) // GPIOINT1A (5>>98) = 3 -> 98 & 1f = 2

// macros led e botão
#define LED 22 // led interno

#define BTC 16 // pino 15

bool estado_led = false; // False: apagado, true: aceso
bool flag_gpio;

void delay(unsigned int mSec);
void pin_mode();
void gpioSetup();
void gpio_entry_enable();
void gpio_output_enable();
void gpio_interrupt_enable();
void intc_interrupt_enable();
void setup();
void acender_led();
void acender_LED();
void ledON();
void ledOFF();
void ISR_Handler(void);

int main(void){
	setup();
	while (1);
	return 0;
}

void delay(unsigned int mSec){
	volatile unsigned int i;
	for (i = 0; i < mSec; i++);
}

void gpioSetup(){
	// Ativar o clock do GPIO1
	CM_PER_GPIO1_CLKCTRL |= 0x40002;
}

void pin_mode(){
	// Configurar o modo do pino
	// 0000 0000 0000 0000 0000 0000 0110 0111
	// Botões
	CM_PER_GPMCBEn1_REGS |= 0x67; // Modo 7 | Receiver enabled | Pullup / Pulldown config
	CM_PER_GPMCA0_REGS |= 0x67;	  // Modo 7 | Receiver enabled | Pullup / Pulldown config

	// Leds
	CM_PER_GPMCA5_REGS |= 0x7; // Modo 7 | Receiver disabled (output)
	CM_PER_GPMCA6_REGS |= 0x7; // Modo 7 | Receiver disabled (output)
}


void gpio_entry_enable(){
	// Habilitar o gpio1_16 como entrada
	GPIO1_EO |= (1 << BTC); // GPIO1_16 como entrada (bit em 1 = entrada)

	// Habilitar a detecção na borda de subida
	GPIO1_RISINGDETECT |= (1 << BTC);
}

void gpio_output_enable(){
	// Habilitar o gpio1_21 como saida
	GPIO1_EO &= ~(1 << LED); // GPIO1_21 como saida (bit em 0 = saida)
	// Habilitar o gpio1_22 como saida
	// GPIO1_22 como saida (bit em 0 = saida)
}

void gpio_interrupt_enable(){
	// Limpa a interrupção pendente
	GPIO1_IRQSTATUS_CLR_0 = (1 << BTC);

	// Habilitar a interrupção no GPIO
	GPIO1_IRQSTATUS_SET_0 |= (1 << BTC);
}

void intc_interrupt_enable(){
	// O INTC controla interrupções no AM355x.
	// A interrupção do GPIO1 é a n° 98, então está no registrador INTC_MIR_SETx onde x = 3.
	INTC_MIR_CLEAR_3 |= (1 << 2); // vai setar o bit 2 do registrador que é o bit responsável pela interrupção do GPIO1
}

void setup(){
	gpioSetup();
	pin_mode();
	gpio_entry_enable();
	gpio_output_enable();
	gpio_interrupt_enable();
	intc_interrupt_enable();
}

void acender_LED(){
	if (estado_led){
		ledOFF();
	}else{
		ledON();
	}
}

void ledON(){
	GPIO1_SETDATAOUT = (1 << LED);
	estado_led = true;
}

void ledOFF(){
	GPIO1_CLEARDATAOUT = (1 << LED);
	estado_led = false;
}

void ISR_Handler(void){
	unsigned int status = GPIO1_IRQSTATUS_0;
	if(status & (1 << BTC)){
		acender_LED();
		// Limpa a interrupção
		GPIO1_IRQSTATUS_0 = (1 << BTC);
	}
}
