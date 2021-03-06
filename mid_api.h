#ifndef _MID_API_H
#define	_MID_API_H

//================================================================================================================
//cmos object
#define	CmosClassMember_Init(OBJ)	void (*Sen_Init)(struct OBJ* )
#define CmosClassMember_Read		void (*Sen_Read)( INT8U const, INT8U const, INT8U * const, INT8U const)
#define CmosClassMember_Delay		void (*Delay)(INT16U const)
#define CmosClassMember_MirrorCtr	void (*MirrorCtr)(INT8U const)
//#define CmosMembers(OBJ)	//c18 need put value, gcc don't need value			

#if _HI_TECH
#define CmosClassMember_Write		void (*Sen_Write)( INT8U const *, INT8U const *, INT8U const *,  INT8U const *, INT16U )
#elif _PIC_C18
#define CmosClassMember_Write		void (*Sen_Write)( rom INT8U const *, rom INT8U const *, rom INT8U const *, rom INT8U const *, INT16U )
#else
//none
#endif

#define CmosClassMember_RamWrite	void (*Sen_RamWrite)( INT8U *, INT8U *, INT8U *, INT8U *, INT16U )

//===================================================================================
//mcu object
#define McuPeripheralNum			6//peripheral(depend on project),timer2,i2c,eusart,io(rb1),adc,pwm
#define McuClassMember_Init(OBJ)		void (*Cpu_Init)(struct OBJ* ) 
#define McuClassMember_GetUartFifoFalg		volatile INT8U (*Cpu_GetUartFifoFlag)(void)  
#define McuClassMember_GetUartFifoData		INT8U (*Cpu_GetUartFifoData)(void)  
#define McuClassMember_Ir_EmCtrl(OBJ)		void (*Ir_Emit)(OBJ) 
#define McuClassMember_LedRedCtrl(OBJ)		void (*Led_Red)(OBJ) 
#define McuClassMember_IcrCtrl(OBJ, OBJ1)	void (*Icr_Ctrl)(OBJ, OBJ1) 
#define McuClassMember_IcrCh(OBJ)		void (*Icr_Ch)(OBJ) 
#define McuClassMember_IrReCtrl			INT8U (*Ir_Receive)(void) 
#define McuClassMember_Delay(OBJ)		void (*Mcu_Dly)(OBJ) 
#define McuClassMember_ServoDC(OBJ)		void (*Mcu_ServoDC)(OBJ) 
#define McuClassMember_AdcRead			int (*Icr_Read)(void) 
#define McuClassMember_LdoWrite			void (*Ldo_Write)(INT8U *,INT8U *,INT16U )
#define McuClassMember_Peripheral		void (*Cpu_PeriInit[McuPeripheralNum])(void) 

#if _HI_TECH
#define McuClassMember_UartWr		void (*Cpu_UartWr)( INT8S const * )  
#elif _PIC_C18
#define McuClassMember_UartWr		void (*Cpu_UartWr)( INT8C * )  
#else
//none
#endif

//================================================================================================================

struct	_AutoMachine;

typedef struct	_AutoMachineClass
{
	CmosClassMember_Init( _AutoMachine );	//1
	CmosClassMember_Read;                  	//2
	CmosClassMember_Write;		       	//3	
	CmosClassMember_RamWrite;		//4
	McuClassMember_Init( _AutoMachine );	//5
	McuClassMember_UartWr;			//6
	McuClassMember_GetUartFifoFalg;		//7
	McuClassMember_GetUartFifoData;		//8
	McuClassMember_Ir_EmCtrl(INT16U );	//9
	McuClassMember_LedRedCtrl(INT16U ); 	//10
	McuClassMember_IcrCh(IcrItem );		//11
	McuClassMember_IcrCtrl(INT16U, IcrItem );//12
	McuClassMember_IrReCtrl;		//13
	McuClassMember_Delay(INT16U);		//14
	McuClassMember_ServoDC(ServoDC);	//15
	McuClassMember_AdcRead;			//16
	McuClassMember_LdoWrite;			
	McuClassMember_Peripheral;		//17
}AutoMachineClass;

typedef struct _AutoMachine
{
	rom AutoMachineClass *class;
	
	//CmosMembers(_AutoMachine);
	INT8U	gainval[2];
	INT8U	expoval[2];

	struct _flag{
		INT8U	mirror : 1;
		INT8U	heater : 1;
		INT8U	       : 6;
	}flag;
	
}AutoMachine;

/* for gcc
AutoMachineClass automachineclass = 
{
	.Init = aaa,
	.Read = aaa
};
*/
#if _HI_TECH
void WriteCmosConfig( INT8U const *hiaddr, INT8U const *loaddr, INT8U const *data,  INT8U const *dnum, INT16U item_num);
#elif  _PIC_C18
void WriteCmosConfig( rom INT8U const *hiaddr, rom INT8U const *loaddr, rom INT8U const *data, rom INT8U const *dnum, INT16U item_num);
#endif

void RamWriteCmosConfig( INT8U *hiaddr, INT8U *loaddr, INT8U *data, INT8U *dnum, INT16U item_num);
void ASX340Read( INT8U const hiaddr , INT8U const loaddr , INT8U * const redata, INT8U const size);

static void SensorInit( AutoMachine *am_val);
void SensorInit( AutoMachine *am_val);
//mcu peripheral function initialization
static void McuInit( AutoMachine *am_val);
void McuInit( AutoMachine *am_val);
void McuI2CInit(void);
void McuTimerInit(void);
void McuEusartWr( INT8C *);
void McuEusartInit(void);
INT8U McuGetUartFifoData(void);
volatile INT8U McuGetUartFifoFlag(void);
void Time0Count( INT16U num);
void Time2Count( ServoDC item);
void McuIoInit(void);
void McuIrEmitCtrl(INT16U val );
void McuLedRedCtrl(INT16U val );
void McuIcrCtrl(INT16U val, IcrItem item );
INT8U McuLedReceiveCtrl(void);
void McuAdcInit(void);
int AdcRead(void);
void McuPwmInit(void);
void AdcIcrCh( IcrItem item );
void WriteLdoConfig( INT8U *cmd, INT8U *data, INT16U item_num);

#pragma romdata
rom AutoMachineClass __automachineclass = 
{
	SensorInit,						//1 
	ASX340Read, //not ready                                 //2 
	WriteCmosConfig,                                        //3 
	RamWriteCmosConfig,                                     //4 
	McuInit,                                                //5 
	McuEusartWr,//peripheral->Eusart wirte                  //6 
	McuGetUartFifoFlag,//peripheral->get uard fifo flag     //7 
	McuGetUartFifoData,//peripheral->get uard fifo data     //8 
	McuIrEmitCtrl,//ir led emit control                     //9 
	McuLedRedCtrl,                                          //10
	AdcIcrCh,						//11
	McuIcrCtrl,                                             //12
	McuLedReceiveCtrl,                                      //13
	Time0Count,//mcu timer2 delay                           //14
	Time2Count,//mcu timer2 delay                           //15
	AdcRead,                                                //16
	WriteLdoConfig,
	McuI2CInit,//peripheral->I2C                            //17-1
	McuTimerInit,//peripheral->Timer2			//17-2
	McuEusartInit,//peripheral->Eusart			//17-3
	McuIoInit,						//17-4
	McuAdcInit,						//17-5
	McuPwmInit						//17-6
};

#pragma romdata

#endif
