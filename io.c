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
	if(val == _ON)	LATBbits.LATB2 = _ON; 
	else		LATBbits.LATB2 = _OFF; 
}
void McuLedRedCtrl(INT16U val )
{
	if(val == _ON)	LATCbits.LATC0 = _ON; 
	else		LATCbits.LATC0 = _OFF; 
}
void McuLedBlueCtrl(INT16U val )
{
	if(val == _ON) 	LATCbits.LATC1 = _ON; 
	else		 LATCbits.LATC1 = _OFF; 
}
INT8U McuLedReceiveCtrl(void)
{
	return PORTCbits.RC2; 
}
void McuIcrCtrl(INT16U val )
{
	if(val == _ON)	 LATBbits.LATB1 = _ON; 
	else		 LATBbits.LATB1 = _OFF; 
}
static void Initialize_Io(void)
{
	//disable AD, so ad initialization have to behind Initialze_Io.
	ANSEL = _OFF; 
	ANSELH = _OFF; 
	//====================================================
	// set external IO interrupt INT0
	TRISBbits.TRISB0 = INPUT;	//ir_led receive
	INTCON2bits.INTEDG0 = 1;    	//rising edge
	INTCONbits.INT0IE = 1;		//enalbe int0 sw12v
	//===================================================

	TRISBbits.TRISB2 = OUTPUT;   	//ir_led emit control -> output
	LATBbits.LATB2 = _OFF;       	//ir_led emmit off

	TRISCbits.TRISC0 = OUTPUT;	//red led
        LATCbits.LATC0 = _ON;    

	TRISCbits.TRISC1 = OUTPUT;	//bule led
	LATCbits.LATC1 = _ON;    

	TRISBbits.TRISB1 = OUTPUT;   	//ICR on/off
	LATBbits.LATB1 = _OFF;       	

	TRISBbits.TRISB4 = INPUT;       //ir_led receive, ad an11
	ANSELHbits.ANS11 = _ON; 
	
}

//====================================================================
//for outside connect
void McuIoInit(void)
{
	Initialize_Io();
}
