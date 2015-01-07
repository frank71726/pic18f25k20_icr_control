#include    "Parameter.h"
#if _HI_TECH
#include    <HTC.h>
#elif	_PIC_C18
#include    <p18cxxx.h>
#include    <pwm.h>
#include    <timers.h>
#else
//none
#endif


#if _HI_TECH
#elif	_PIC_C18

static void PwmInit(void)
{
	/*
	//void InitializePWM2(void)
	TRISCbits.TRISC1=0;	// setup RC1 for PWM output
	OpenPWM2(0xff); 	// set period to 0xFF

	//void InitializeTMR2(void)
	OpenTimer2(T2_PS_1_4 & T2_POST_1_16);
	SetDCPWM2(1000);
	*/
}

//connect from outside
void McuPwmInit(void)
{
	PwmInit();
}
#else
//none
#endif


