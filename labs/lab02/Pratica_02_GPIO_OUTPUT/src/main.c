#include	"hw_types.h"
#include	"soc_AM335x.h"

/*****************************************************************************
**                INTERNAL MACRO DEFINITIONS
*****************************************************************************/
#define TIME													1000000
#define TOGGLE          										(0x01u)

#define CM_PER_GPIO1											0xAC
#define CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE   				(0x2u)
#define CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK   			(0x00040000u)

#define CM_conf_gpmc_be1n      	 							0x0878 //expansor_P9_com_pino_12
#define CM_conf_gpmc_a5         								0x0854

#define GPIO_OE                 								0x134
#define GPIO_CLEARDATAOUT       								0x190
#define GPIO_SETDATAOUT         								0x194


unsigned int flagBlink;
unsigned int flagBlink21;
unsigned int flagBlink22;
unsigned int flagBlink23;
unsigned int flagBlink24;

/*****************************************************************************
**                INTERNAL FUNCTION PROTOTYPES
*****************************************************************************/
static void delay();
static void ledInit();
static void ledToggle();
static void ledToggle21();
static void ledToggle22();
static void ledToggle23();
static void ledToggle24();


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int _main(void){

	flagBlink=0;
	flagBlink21=0;
	flagBlink22=0;
	flagBlink23=0;
	flagBlink24=0;	//init flag
  	
	/* Configure the green LED control pin. */
  	ledInit();
  
  	while (1){
    	/* Change the state of the green LED. */
    	ledToggle();
		delay();

		ledToggle21();
		delay();

		ledToggle22();
		delay();

		ledToggle23();
		delay();

		ledToggle24();
		delay();
	}

	return(0);
} /* ----------  end of function main  ---------- */


/*FUNCTION*-------------------------------------------------------
*
* Function Name : Delay
* Comments      :
*END*-----------------------------------------------------------*/
static void delay(){
	volatile unsigned int ra;
	for(ra = 0; ra < TIME; ra ++);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ledInit
 *  Description:  
 * =====================================================================================
 */
void ledInit( ){
	
	unsigned int val_temp; 	
	/*-----------------------------------------------------------------------------
	 *  configure clock GPIO in clock module
	 *-----------------------------------------------------------------------------*/
	HWREG(SOC_CM_PER_REGS+CM_PER_GPIO1) |= CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK | CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE;
	
	/*-----------------------------------------------------------------------------
	 * configure mux pin in control module
	 *-----------------------------------------------------------------------------*/
	HWREG(SOC_CONTROL_REGS+CM_conf_gpmc_a5) |= 0x7;
	 HWREG(SOC_CONTROL_REGS+CM_conf_gpmc_be1n) |= 0x7;
 
	/*-----------------------------------------------------------------------------
	 *  set pin direction 
	 *-----------------------------------------------------------------------------*/
	val_temp = HWREG(SOC_GPIO_1_REGS+GPIO_OE);
	val_temp &= ~(1<<21);
	HWREG(SOC_GPIO_1_REGS+GPIO_OE) = val_temp;

	val_temp = HWREG(SOC_GPIO_1_REGS+GPIO_OE);
	val_temp &= ~(1<<22);
	HWREG(SOC_GPIO_1_REGS+GPIO_OE) = val_temp;

	val_temp = HWREG(SOC_GPIO_1_REGS+GPIO_OE);
	val_temp &= ~(1<<23);
	HWREG(SOC_GPIO_1_REGS+GPIO_OE) = val_temp;

	val_temp = HWREG(SOC_GPIO_1_REGS+GPIO_OE);
	val_temp &= ~(1<<24);
	HWREG(SOC_GPIO_1_REGS+GPIO_OE) = val_temp;

	val_temp = HWREG(SOC_GPIO_1_REGS+GPIO_OE);
	val_temp &= ~(1<<28);
	HWREG(SOC_GPIO_1_REGS+GPIO_OE) = val_temp;
		
}/* -----  end of function ledInit  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ledToggle
 *  Description:  
 * =====================================================================================
 */

void ledToggle( ){
		
		flagBlink ^= TOGGLE;

		if(flagBlink){
			HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) = (1<<28);
		}else{
			HWREG(SOC_GPIO_1_REGS+GPIO_CLEARDATAOUT) = (1<<28);
		}
		

}/* -----  end of function ledToggle  ----- */

void ledToggle21( ){
		
	flagBlink21 ^= TOGGLE;

	if(flagBlink21){
		HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) = (1<<21);
	}else{
		HWREG(SOC_GPIO_1_REGS+GPIO_CLEARDATAOUT) = (1<<21);
	}
	

}/* -----  end of function ledToggle  ----- */

void ledToggle22( ){
		
	flagBlink22 ^= TOGGLE;

	if(flagBlink22){
		HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) = (1<<22);
	}else{
		HWREG(SOC_GPIO_1_REGS+GPIO_CLEARDATAOUT) = (1<<22);
	}
	

}/* -----  end of function ledToggle  ----- */

void ledToggle23( ){
		
	flagBlink23 ^= TOGGLE;

	if(flagBlink23){
		HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) = (1<<23);
	}else{
		HWREG(SOC_GPIO_1_REGS+GPIO_CLEARDATAOUT) = (1<<23);
	}
	

}/* -----  end of function ledToggle  ----- */

void ledToggle24( ){
		
	flagBlink24 ^= TOGGLE;

	if(flagBlink24){
		HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) = (1<<24);
	}else{
		HWREG(SOC_GPIO_1_REGS+GPIO_CLEARDATAOUT) = (1<<24);
	}
	

}/* -----  end of function ledToggle  ----- */









