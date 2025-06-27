#include <stdbool.h>

//macros do clock
#define CM_PER 					(*(volatile unsigned int*)0x44E00000)
#define CM_PER_GPIO1_CLKCTRL	(*(volatile unsigned int*)0x44E000AC)

//macros GPIO
#define GPIO1_EO				(*(volatile unsigned int*)0x4804C134)
#define GPIO1_CLEARDATAOUT		(*(volatile unsigned int*)0x4804C190)
#define GPIO1_SETDATAOUT		(*(volatile unsigned int*)0x4804C191)
#define GPIO1_DATAIN			(*(volatile unsigned int*)0x4804C138)
#define GPIO1_IRQSTATUS_SET_0	(*(volatile unsigned int*)0x4804C034)
#define GPIO1_IRQSTATUS_0		(*(volatile unsigned int*)0x4804C02C)
#define GPIO1_RISINGDETECT		(*(volatile unsigned int*)0x4804C148)

#define CM_PER_GPMCBEn1_REGS	(*(volatile unsigned int*)0x4804C878)
#define CM_PER_GPMCA5_REGS    	(*(volatile unsigned int*)0x44E10854)

//macros INT
#define INTCPS					(*(volatile unsigned int*)0x48200000)
#define INTC_SIR_IRQ			(*(volatile unsigned int*)0x48200040)
#define INTC_CONTROL			(*(volatile unsigned int*)0x48200048)
#define INTC_MIR_CLEAR_3		(*(volatile unsigned int*)0x482000E8) //GPIOINT1A (5>>98) = 3 -> 98 & 1f = 2

//macros led e botão
#define LED 21	// led interno

#define BTC 28  // pino 12

bool flag_gpio;

void delay(unsigned int mSec){
	volatile unsigned int i;
	for(i = 0; i < mSec; i++);
}

void gpioSetup(void){
	CM_PER_GPIO1_CLKCTRL |= 0x40002;

	// mascara daa interrupção
	INTC_MIR_CLEAR_3 |= (1<<2);
}

void butConfig(void){
	CM_PER_GPMCBEn1_REGS |= 0x2F;

	GPIO1_EO |= (1<<BTC);

	flag_gpio = false;

	//configura a INTC do registrador
	GPIO1_IRQSTATUS_SET_0 |= (1<<BTC);

	GPIO1_RISINGDETECT |= (1<<BTC);
}

void ledSetup(void){
	CM_PER_GPMCA5_REGS |= 0x7;

	GPIO1_EO &= ~(1<<LED);
}

unsigned int readBut(){
	unsigned int temp = GPIO1_DATAIN & (0x1000);

	return temp;
}

void ledON(){
	GPIO1_SETDATAOUT	|= (1<<21);
}

void ledOFF(){
	GPIO1_CLEARDATAOUT	|= (1<<21);
}

void gpioIsrHandler(){
	GPIO1_IRQSTATUS_0 = (1<<BTC);

	flag_gpio = true;
}

void ISR_Handler(void){
	/* Verify active IRQ number */
	unsigned int irq_number = INTC_SIR_IRQ & 0x7f;
	
	if(irq_number == 98)
		gpioIsrHandler();
    
	/* acknowledge IRQ */
	INTC_CONTROL = 0x1;
}

int _main(void){

	gpioSetup();
	butConfig();
	ledSetup();

	ledOFF();
	delay(10000);

	if(flag_gpio){
		ledON();
		delay(100000);
		ledOFF();
		delay(100000);
		flag_gpio = false;
	}else{
		ledON();
		delay(1000000);
		ledOFF();
		delay(1000000);		
	}

	return 0;
}