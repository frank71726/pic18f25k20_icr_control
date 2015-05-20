#include    "Parameter.h"
#if _HI_TECH
#include    <HTC.h>
#elif	_PIC_C18
#include    <p18cxxx.h>
#else
//none
#endif

void McuIrEmitCtrl(INT16U val )
{
	if(val == _ON)	LATCbits.LATC0 = _ON; 
	else		LATCbits.LATC0 = _OFF; 
}
void McuLedRedCtrl(INT16U val )
{
	if(val == _ON)	LATAbits.LATA4 = _ON; 
	else		LATAbits.LATA4 = _OFF; 
}


INT8U McuLedReceiveCtrl(void)
{
	return PORTCbits.RC2; 
}
void McuIcrCtrl(INT16U val, IcrItem item )
{
	switch(item)
	{
		case ICR_1:
			if(val == _ON)	 LATBbits.LATB2 = _ON; 
			else		 LATBbits.LATB2 = _OFF; 
			break;
		case ICR_2:
			if(val == _ON)	 LATBbits.LATB3 = _ON; 
			else		 LATBbits.LATB3 = _OFF; 
			break;
		case ICR_3:
			if(val == _ON)	 LATBbits.LATB1 = _ON; 
			else		 LATBbits.LATB1 = _OFF; 
			break;
		case ICR_4:
			if(val == _ON)	 LATBbits.LATB4 = _ON; 
			else		 LATBbits.LATB4 = _OFF; 
			break;
		case ICR_5:
			if(val == _ON)	 LATBbits.LATB0 = _ON; 
			else		 LATBbits.LATB0 = _OFF; 
			break;
		default:
			break;
	}
}
static void Initialize_Io(void)
{
	//disable AD, so ad initialization have to behind Initialze_Io.
	ANSEL = _OFF; 
	ANSELH = _OFF; 
	//====================================================
	// set external IO interrupt INT0
	//TRISBbits.TRISB0 = INPUT;	//ir_led receive
	//INTCON2bits.INTEDG0 = 1;    	//rising edge
	//INTCONbits.INT0IE = 1;		//enalbe int0 sw12v
	//===================================================

	TRISCbits.TRISC0 = OUTPUT;   	//ir_led emit control -> output
	LATCbits.LATC0 = _OFF;       	//ir_led emmit off
	
	TRISBbits.TRISB2 = OUTPUT;   	//ICR on/off
	LATBbits.LATB2 = _OFF;       	
	TRISBbits.TRISB3 = OUTPUT;   	//ICR on/off
	LATBbits.LATB3 = _OFF;       	
	TRISBbits.TRISB1 = OUTPUT;   	//ICR on/off
	LATBbits.LATB1 = _OFF;       	
	TRISBbits.TRISB4 = OUTPUT;   	//ICR on/off
	LATBbits.LATB4 = _OFF;       	
	TRISBbits.TRISB0 = OUTPUT;   	//ICR on/off
	LATBbits.LATB0 = _OFF;       	

	TRISAbits.TRISA0 = INPUT;       //ir_led receive, ad an0
	ANSELbits.ANS0 = _ON; 
	TRISAbits.TRISA1 = INPUT;       //ir_led receive, ad an1
	ANSELbits.ANS1 = _ON; 
	TRISAbits.TRISA2 = INPUT;       //ir_led receive, ad an2
	ANSELbits.ANS2 = _ON; 
	TRISAbits.TRISA3 = INPUT;       //ir_led receive, ad an3
	ANSELbits.ANS3 = _ON; 
	TRISAbits.TRISA4 = INPUT;       //ir_led receive, ad an4
	ANSELbits.ANS4 = _ON; 
	
	TRISCbits.TRISC1 = OUTPUT;	//pwm for servo
        LATCbits.LATC1 = _ON;    

	TRISAbits.TRISA4 = OUTPUT;	//red led
	LATAbits.LATA4 = _ON;    
}

//====================================================================
//for outside connect
void McuIoInit(void)
{
	Initialize_Io();
}
