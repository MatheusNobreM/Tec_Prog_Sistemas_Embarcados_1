
#include <stdbool.h>
#include <stdint.h>


// Definições de registradores
#define WDT1_BASE 0x44E35000
#define WDT_WSPR (*(volatile uint32_t *)(WDT1_BASE + 0x48))
#define WDT_WWPS (*(volatile uint32_t *)(WDT1_BASE + 0x34))

void disable_watchdog(void) {
	WDT_WSPR = 0xAAAA;
	while (WDT_WWPS & (1 << 4));
	WDT_WSPR = 0x5555;
	while (WDT_WWPS & (1 << 4));
}

#define CM_PER					(*(volatile unsigned int*)0x44E00000)
#define CM_PER_GPIO1_CLKCTRL	(*(volatile unsigned int*)0x44E000AC)
#define CM_PER_TIMER7_CLKCTRL	(*(volatile unsigned int*)0x44E0007C)

#define GPIO1					(*(volatile unsigned int*)0x4804C000)
#define GPIO1_OE				(*(volatile unsigned int*)0x4804C134)
#define GPIO1_CLEARDATAOUT		(*(volatile unsigned int*)0x4804C190)
#define GPIO1_SETDATAOUT		(*(volatile unsigned int*)0x4804C194)

#define CM_PER_GPMCA5_REGS    	(*(volatile unsigned int*)0x44E10854)

#define INTC_SIR_IRQ			(*(volatile unsigned int*)0x48200040)
#define INTC_ISR_CLEAR2			(*(volatile unsigned int*)0x482000C8)
#define INTC_CONTROL			(*(volatile unsigned int*)0x48200048)

#define DMTIMER7				(*(volatile unsigned int*)0x4804A000)
#define DMTIMER7_TCLR			(*(volatile unsigned int*)0x4804A038)
#define DMTIMER7_TCRR			(*(volatile unsigned int*)0x4804A03C)
#define DMTIMER7_TLDR			(*(volatile unsigned int*)0x4804A040)
#define DMTIMER7_TTGR			(*(volatile unsigned int*)0x4804A044)
#define DMTIMER7_TWPS			(*(volatile unsigned int*)0x4804A048)
#define DMTIMER7_TSICR			(*(volatile unsigned int*)0x4804A054)
#define DMTIMER_IRQSTATUS		(*(volatile unsigned int*)0x4804A028)
#define DMTIMER_IRQENABLE_SET	(*(volatile unsigned int*)0x4804A02C)
#define DMTIMER_IRQENABLE_CLR	(*(volatile unsigned int*)0x4804A030)

/* 
 * O clock de entrada foi selecionado como 24 MHz. Portanto, para um atraso de 1 ms, 
 * configure a contagem para 0x5DC0. Se o clock de entrada for alterado para uma fonte 
 * diferente, este valor deverá ser atualizado de acordo.
 * 24MHz = 24.000.000 HZ (ciclos por segundo) = 24.000 mHZ (ciclos por milissegundo)
 * 0x5DC0 = 24.000 em hexadecimal --> corresponde a 1 ms de contagem.
 */
#define TIMER_1MS_COUNT 	0x5DC0u
#define TIMER_OVERFLOW		0xFFFFFFFFu

/**
 * \brief   This macro will check for write POSTED status
 *
 * \param   reg          Register whose status has to be checked
 *
 * 'reg' can take the following values \n
 * 	bit - description (When equal to 1, a write is pending in the register)
 *    0 - DMTIMER_WRITE_POST_TCLR - Timer Control register \n 
 *    1 - DMTIMER_WRITE_POST_TCRR - Timer Counter register \n
 *    2 - DMTIMER_WRITE_POST_TLDR - Timer Load register \n
 *    3 - DMTIMER_WRITE_POST_TTGR - Timer Trigger register \n
 *    4 - DMTIMER_WRITE_POST_TMAR - Timer Match register \n
 *		
 *    DMTIMER7_TSICR - PIFREQRATIO
		0h = Posted mode inactive: will delay the command accept output signal.
		1h = Posted mode active (clocks ratio needs to fit freq (timer) less than freq (OCP)/4 frequency requirement)
 **/
#define DMTimerWaitForWrite(reg)	\
			if(DMTIMER7_TSICR & 0x4) \
			while(DMTIMER7_TWPS & reg);

#define LED 21



typedef enum timerType{
	POLLING = 0,
	INTERRUPT
}timerType;


int flag_timer = false;
unsigned int contDelay = 3000;

void clocksetup(){
	CM_PER_TIMER7_CLKCTRL |=0x2;

}

void timerInterruptEnabled(){
	DMTIMER_IRQENABLE_SET = 0x02;
}

void timerInterruptDisable(){
	DMTIMER_IRQENABLE_CLR = 0x02;
}

// void timerDisable(){
// 	DMTimerWaitForWrite(0x1);
// 	DMTIMER7_TCLR &= ~(0x1);
// }

void timerEnable(){
	DMTimerWaitForWrite(0x1);
	DMTIMER7_TCLR = 0x0; //desliga para config
	DMTIMER7_TLDR = 0x00000000; // reset
	timerInterruptEnabled();
	DMTimerWaitForWrite(0x1);
	DMTIMER7_TCLR = 0x3; // controle ligar e habilitar alto recarga
}

void gpioSetup(void){
	CM_PER_GPIO1_CLKCTRL |= 0x40002;
}

void ledSetup(){
	CM_PER_GPMCA5_REGS |= 0x7;

	GPIO1_OE &= ~(1<<LED);
}

void timerSetup(timerType type){
	clocksetup();


	if(type == INTERRUPT){
		INTC_ISR_CLEAR2 |= (1<<31);
	}
}

void ledON(void){
	GPIO1_SETDATAOUT |= (1<<LED);
}

void ledOFF(void){
	GPIO1_CLEARDATAOUT |= (1<<LED);
}

// void timerEnable(){
// 	DMTimerWaitForWrite(0x1);

// 	DMTIMER7_TCLR |= 0x1;
// }

void timerDisable(){
	DMTimerWaitForWrite(0x1);

	DMTIMER7_TCLR &= ~(0x1);
}

void delay(unsigned int mSec, timerType type){
	if(type == INTERRUPT){
		unsigned int contValue = TIMER_OVERFLOW - (mSec * TIMER_1MS_COUNT);

		DMTimerWaitForWrite(0x2);

		DMTIMER7_TCRR = contValue;

		flag_timer = false;

		timerInterruptEnabled();

		timerEnable();

		while(flag_timer == false);

		DMTIMER_IRQENABLE_CLR = 0x2;

	}else{
		while(mSec != 0){
			DMTimerWaitForWrite(0x2);
	
			DMTIMER7_TCRR = 0x0;
	
			timerEnable();
	
			while(DMTIMER7_TCRR < TIMER_1MS_COUNT);
	
			DMTIMER7_TCLR &= ~(0x00000001u);
	
			mSec--;
		}
	}
}

void timerIrqHandler(){
	DMTIMER_IRQSTATUS |= 0x2;

	flag_timer = true;

	timerDisable();
}

void ISR_Handler(void){
	unsigned int irq_number = INTC_SIR_IRQ & 0x7F;

	if(irq_number == 95){
		timerIrqHandler();
	}

	INTC_CONTROL |= 0x1;
}

int main(void){
	disable_watchdog();
	timerType type = INTERRUPT;


	gpioSetup();
	ledSetup();
	timerSetup(type);

	while(1){
		if(flag_timer){
			ledON();
			delay(contDelay,type);
			flag_timer = false;
		}else{
			ledOFF();
			delay(contDelay,type);
		}
	}

	return 0;
}