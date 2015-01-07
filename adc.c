#include    "Parameter.h"
#if _HI_TECH
#include    <HTC.h>
#elif	_PIC_C18
#include    <p18cxxx.h>
#include    <adc.h>
#else
//none
#endif

extern void Time0Count( INT16U num);

int AdcRead(void)
{
	ConvertADC();
	while(BusyADC( ));
	return ReadADC();
}
//************************************************
//* PIC18F4520 ¦b ADC Peripheral Library ªºÂkÃþ  *
//* ¬° V5 , ¦b¨Ï¥Î OpenADC() ®É­n°Ñ¦Ò 			 *
//* Function: InitializeAD                       *
//*        - Fosc/32 clock source                *
//*        - Left justified                      *
//*        - 1 analog channels, 0 references     *
//*        - Channel 0                           *
//*        - Disable A/D interrupt               *
//************************************************
//DW pic18f25k20 #define ADC_V8 => pconfig.h #16458
static void Initialize_Adc(void)
{
	OpenADC(  ADC_FOSC_32 & ADC_RIGHT_JUST & ADC_20_TAD,
			ADC_CH11 & ADC_INT_OFF & ADC_REF_VDD_VSS,
			0x0);
	Time0Count(2);
	ConvertADC();
}
//====================================================================
//for outside connect
void McuAdcInit(void)
{
	Initialize_Adc();
}


