/*******************************************************************************
 *@Project: ICR testing 300ms
 *@MCU:Pic18f25K20
 *@Complier:v3.43
 *@Author:Frank.Chiang
 *@Comment: 1. Oscillator:16MHz
 *          2.   {                -------
 *                    I2CDelay();       | using this routine to avoid it mcu 
 *               }                      | cann't get ACK signal from other slave IC.
 *                    I2CDlyClr();-------
 *	    3. RS232 : 9600, 8, N, 1, X,
 *	    4.ä¹?0ï¼šï?x << 3ï¼?+ ï¼ˆx << 1ï¼?  == x*8 + x*2 ==  x*(8+2)
 *	      ??0 : (x >> 3) +   (x >> 1)    == x/8 + x/2 ==  x/(8+2)  
 *v1.0:20140811 by frank - draft
 *******************************************************************************/
#include "Parameter.h"
#if _HI_TECH
#include    <HTC.h>
#elif	_PIC_C18
#include <p18cxxx.h>
#else
//none
#endif

#include "mid_api.h"
#include "mid_aptina.h"
#include "automachine_asx340_para.h"
#include "ldo.h"

#if _HI_TECH
#elif	_PIC_C18

#pragma config	IESO = OFF
#pragma config	FCMEN = OFF
#pragma config	FOSC = HS
#pragma config	XINST = OFF
#pragma config	WDTEN = OFF
//#pragma config	WDTPS = 4096
#pragma config	PWRT  = OFF
#pragma config	BOREN = OFF
#pragma config	STVREN = OFF
#pragma config	LVP = OFF
#else
//none
#endif

INT16U gIcrDlyTime=0;
LdoVol gAnyOutLdo=0;
INT8U gStart=0;
INT8C MSG_Start[]="$f00@";//"$Please push start button#";
INT8C MSG_init[]="$f01@";//"$please input $00(all), $01(0 degree), $02(90 degree), $03(180 degree)#";
INT8C MSG_AllTest[]="$f02@";//"$Starting all test (0, 90, 180 degree)#";
INT8C MSG_0_degree[]="$f03@";//"$Starting 0 degree test#";
INT8C MSG_90_degree[]="$f04@";//"$Starting 90 degree test#";
INT8C MSG_180_degree[]="$f05@";//"$Starting 180 degree test#";
INT8C MSG_Ok[6][6]={"$f06@", "$f16@", "$f26@", "$f36@", "$f46@", "$f56@"};//"$test OK#";
INT8C MSG_Ng[6][6]={"$f07@", "$f17@", "$f27@", "$f37@", "$f47@", "$f57@"};//"$test NG#";
INT8C MSG_Finish[]="$f08@";

//==========================================================================
static void SensorInit( AutoMachine *am_val)
//void SensorInit( AutoMachine *am_val)
{
	am_val->class->Sen_Write( ASX340_hi, ASX340_lo, ASX340_data, ASX340_num, sizeof(ASX340_hi)/sizeof(ASX340_hi[0]) );

	if(am_val->flag.mirror == 1 )//mirror
	{
		am_val->class->Sen_Write(ASX340_mirror_hi, ASX340_mirror_lo, ASX340_mirror_data,\
				ASX340_mirror_num, sizeof(ASX340_mirror_hi)/sizeof(ASX340_mirror_hi[0]) );
	}else//no mirror
	{
		am_val->class->Sen_Write( ASX340_mirror_hi, ASX340_mirror_lo, ASX340_no_mirror_data,\
				ASX340_mirror_num, sizeof(ASX340_mirror_hi)/sizeof(ASX340_mirror_hi[0]) );
	}
}
//==========================================================================
static void McuInit( AutoMachine *am_val)
//void McuInit( AutoMachine *am_val)
{
	INT8U count;

	//peripheral initialization
	for( count=0 ; count < McuPeripheralNum	; count++ )
	{
		am_val->class->Cpu_PeriInit[count]();
	}

	//falg setting
	am_val->flag.mirror = 0;//no mirror
}
//==========================================================================
static void AnyOutLdoInit(AutoMachine *am_val)
{
	INT8U LdoAddr[1]={0x03};
	am_val->class->Ldo_Write( Ldo_cmd, Ldo_data, sizeof(Ldo_cmd)/sizeof(Ldo_cmd[0]) );
	am_val->class->Ldo_Write( LdoAddr, (INT8U*)&gAnyOutLdo, 1);
}

//==========================================================================
INT8U GetR232Value(AutoMachine *am)
{
	INT8U rs232_vlaue[3], temp, count=0, rs232_rx_fifo_not_empty_flag=0 ; 
	INT8U return_value = 100;

	while(1) {
		rs232_rx_fifo_not_empty_flag = am->class->Cpu_GetUartFifoFlag(); 

		if( rs232_rx_fifo_not_empty_flag == 1 ) {
			switch(count) {
				case 0:
					temp = am->class->Cpu_GetUartFifoData();
					if('$' == temp) {
						rs232_vlaue[count] = temp;
						count++;
					}
					break;
				case 1:
					rs232_vlaue[count] = am->class->Cpu_GetUartFifoData();
					count++;
					break;
				case 2:
					rs232_vlaue[count] = am->class->Cpu_GetUartFifoData();
					count++;
					switch(rs232_vlaue[1]) {
						case '0':
							switch(rs232_vlaue[2]) {
								case '0':
									return_value = 0; break;//all test
								case '1':
									return_value = 1; break;//0 degree
								case '2':
									return_value = 2; break;//90 degree 
								case '3':
									return_value = 3; break;//180 degree 
								default:
									return_value = 100; break;//NG
							}
							break;
						case '9':
							switch(rs232_vlaue[2]) {
								case '0':
									return_value = 200; break;//button start
								case '1':
									return_value = 201; break;//button reset
								default:
									break;
							}
							break;
						case 'A':
							switch(rs232_vlaue[2]) {
								case '0': gIcrDlyTime = 100; break;
								case '1': gIcrDlyTime = 200; break;
								case '2': gIcrDlyTime = 300; break;
								case '3': gIcrDlyTime = 400; break;
								case '4': gIcrDlyTime = 500; break;
								case '5': gIcrDlyTime = 600; break;
								case '6': gIcrDlyTime = 700; break;
								case '7': gIcrDlyTime = 800; break;
								case '8': gIcrDlyTime = 900; break;
								case '9': gIcrDlyTime = 1000; break;
								default:		      break;
							}
							break;
						
						case 'B':
							switch(rs232_vlaue[2]) {
								case '0': gAnyOutLdo = V365; break;
								case '1': gAnyOutLdo = V360; break;
								case '2': gAnyOutLdo = V355; break;
								case '3': gAnyOutLdo = V350; break;
								case '4': gAnyOutLdo = V345; break;
								case '5': gAnyOutLdo = V340; break;
								case '6': gAnyOutLdo = V335; break;
								case '7': gAnyOutLdo = V330; break;
								case '8': gAnyOutLdo = V325; break;
								case '9': gAnyOutLdo = V320; break;
								case 'A': gAnyOutLdo = V315; break;
								case 'B': gAnyOutLdo = V310; break;
								case 'C': gAnyOutLdo = V305; break;
								case 'D': gAnyOutLdo = V300; break;
								case 'E': gAnyOutLdo = V295; break;
								case 'F': gAnyOutLdo = V290; break;
								default:		     break;
							}
							AnyOutLdoInit(am);
							am->class->Mcu_Dly(500);
							break;
						case 'C':
							switch(rs232_vlaue[2]) {
								case '0': gAnyOutLdo = V285; break;
								case '1': gAnyOutLdo = V280; break;
								case '2': gAnyOutLdo = V275; break;
								case '3': gAnyOutLdo = V270; break;
								case '4': gAnyOutLdo = V265; break;
								case '5': gAnyOutLdo = V260; break;
								case '6': gAnyOutLdo = V255; break;
								case '7': gAnyOutLdo = V250; break;
								case '8': gAnyOutLdo = V245; break;
								case '9': gAnyOutLdo = V240; break;
								case 'A': gAnyOutLdo = V235; break;
								case 'B': gAnyOutLdo = V230; break;
								case 'C': gAnyOutLdo = V225; break;
								case 'D': gAnyOutLdo = V220; break;
								case 'E': gAnyOutLdo = V215; break;
								case 'F': gAnyOutLdo = V210; break;
								default:		     break;
							}
							AnyOutLdoInit(am);
							am->class->Mcu_Dly(500);
							break;
						case 'D':
							switch(rs232_vlaue[2]) {
								case '0': gAnyOutLdo = V205; break;
								case '1': gAnyOutLdo = V200; break;
								case '2': gAnyOutLdo = V195; break;
								case '3': gAnyOutLdo = V190; break;
								case '4': gAnyOutLdo = V185; break;
								case '5': gAnyOutLdo = V180; break;
								case '6': gAnyOutLdo = V175; break;
								case '7': gAnyOutLdo = V170; break;
								case '8': gAnyOutLdo = V165; break;
								case '9': gAnyOutLdo = V160; break;
								case 'A': gAnyOutLdo = V155; break;
								case 'B': gAnyOutLdo = V150; break;
								case 'C': gAnyOutLdo = V145; break;
								case 'D': gAnyOutLdo = V140; break;
								case 'E': gAnyOutLdo = V135; break;
								case 'F': gAnyOutLdo = V130; break;
								default:		     break;
							}
							AnyOutLdoInit(am);
							am->class->Mcu_Dly(500);
							break;
						case 'E':
							switch(rs232_vlaue[2]) {
								case '0': gAnyOutLdo = V125; break;
								case '1': gAnyOutLdo = V120; break;
								case '2': gAnyOutLdo = V115; break;
								case '3': gAnyOutLdo = V110; break;
								case '4': gAnyOutLdo = V105; break;
								case '5': gAnyOutLdo = V100; break;
								case '6': gAnyOutLdo = V095; break;
								case '7': gAnyOutLdo = V090; break;
								case '8': gAnyOutLdo = V085; break;
								case '9': gAnyOutLdo = V080; break;
								case 'A': gAnyOutLdo = V075; break;
								case 'B': gAnyOutLdo = V070; break;
								case 'C': gAnyOutLdo = V065; break;
								case 'D': gAnyOutLdo = V060; break;
								case 'E': gAnyOutLdo = V055; break;
								case 'F': gAnyOutLdo = V050; break;
								default:		     break;
							}
							AnyOutLdoInit(am);
							am->class->Mcu_Dly(500);
							break;
						default:
							return_value = 100; break;
					}
					return return_value;
					break;
				default:
					return_value = 100; 
					return return_value;
					break;
			}
		}
	}
}
//==========================================================================
INT8U wait_start(AutoMachine *am)
{
	INT8U temp;
	while(1)
	{
		temp = GetR232Value(am);
		  	
		if(temp == 200 )
			return 0;
		else if(temp == 201 )
		{
			Reset();
		}
		else
			return 1;
	}
}
//==========================================================================
void StartIcrTest(AutoMachine *am)
{
	INT8U   ir_count=0;
	INT16S  icr_value=0;
	IcrItem icr_num= ICR_1;
	
	while(1)
	{
		am->class->Icr_Ch(icr_num);
		am->class->Icr_Ctrl(_ON, icr_num);
		am->class->Mcu_Dly(gIcrDlyTime);
		icr_value = am->class->Icr_Read();
		if(icr_value > icr_on_threshold)
			ir_count++;
		else
		{
			am->class->Led_Red(_OFF);//turn on red led => fail
			am->class->Cpu_UartWr(MSG_Ng[icr_num]);//"$ng"
			am->class->Icr_Ctrl(_OFF, icr_num);
			//while(wait_start(am));
			//ir_count = 0;
			//icr_num= ICR_1;
			//am->class->Led_Red(_ON);//turn on red led => fail
			ir_count = 0;
			icr_num += 1;
			if(icr_num > ICR_5)
			{
				am->class->Mcu_Dly(500);
				am->class->Led_Red(_ON);
				break;
			}
			else
				continue;
		}
		
		am->class->Icr_Ctrl(_OFF, icr_num);
		am->class->Mcu_Dly(gIcrDlyTime);
		icr_value = am->class->Icr_Read();
		if(icr_value < icr_off_threshold )
			ir_count++;
		else
		{
			am->class->Led_Red(_OFF);//turn on red led => fail
			am->class->Cpu_UartWr(MSG_Ng[icr_num]);//"$ng"
			am->class->Icr_Ctrl(_OFF, icr_num);
			//while(wait_start(am));
			//ir_count = 0;
			//icr_num= ICR_1;
			//am->class->Led_Red(_ON);//turn on red led => fail
			ir_count = 0;
			icr_num += 1;
			if(icr_num > ICR_5)
			{
				am->class->Mcu_Dly(500);
				am->class->Led_Red(_ON);
				break;
			}
			else
				continue;
		}

		if(ir_count >= 6)
		{
			am->class->Cpu_UartWr(MSG_Ok[icr_num]);
			ir_count = 0;
			am->class->Mcu_Dly(500);
		//	am->class->Led_Red(_ON);

			icr_num += 1;
			if(icr_num > ICR_5)
			{
				am->class->Led_Red(_ON);
				break;
			}
		}
	}
}
//==========================================================================
void main(void)
{
	//AutoMachineClass automachineclass;
	AutoMachine	 am;
	INT8U	temp=0;
 
	am.class = &__automachineclass; 
	am.class->Cpu_Init(&am);

	am.class->Mcu_ServoDC(servo_90DC);
	am.class->Cpu_UartWr(MSG_Start);
	am.class->Ir_Emit(_ON);

	while(wait_start(&am));
	am.class->Cpu_UartWr(MSG_init);
	while(1)
	{
		switch(GetR232Value(&am))
		{
			case 0://start
				am.class->Cpu_UartWr(MSG_AllTest);
				//rotate 0 degree
				am.class->Mcu_ServoDC(servo_90DC);
				am.class->Mcu_Dly(1000);
				am.class->Cpu_UartWr(MSG_0_degree);
				StartIcrTest(&am);
				//rotate 90 degree
				am.class->Mcu_ServoDC(servo_0DC);
				am.class->Mcu_Dly(1000);
				am.class->Cpu_UartWr(MSG_90_degree);
				StartIcrTest(&am);
				//rotate 180 degree
				/*
				am.class->Mcu_ServoDC(servo_n90DC);
				am.class->Mcu_Dly(1000);
				am.class->Cpu_UartWr(MSG_180_degree);
				StartIcrTest(&am);
				*/
				am.class->Cpu_UartWr(MSG_Finish);
				break;
			case 1://moving 0 degree
				//rotate 0 degree
				am.class->Mcu_ServoDC(servo_90DC);
				am.class->Mcu_Dly(1000);
				am.class->Cpu_UartWr(MSG_0_degree);
				StartIcrTest(&am);
				am.class->Cpu_UartWr(MSG_Finish);
				break;
			case 2://moving 90 degree
				//rotate 90 degree
				am.class->Mcu_ServoDC(servo_0DC);
				am.class->Mcu_Dly(1000);
				am.class->Cpu_UartWr(MSG_90_degree);
				StartIcrTest(&am);
				am.class->Cpu_UartWr(MSG_Finish);
				break;
			case 3://moving 180 degree
				//rotate 180 degree
				am.class->Mcu_ServoDC(servo_n90DC);
				am.class->Mcu_Dly(1000);
				am.class->Cpu_UartWr(MSG_180_degree);
				StartIcrTest(&am);
				am.class->Cpu_UartWr(MSG_Finish);
				break;
			case 201:
				Reset();
				break;
			default:
				break;
		}
		//am.class->Cpu_UartWr(MSG_init);
	}
}
/* 
//==========================================================================
void puthexUart(INT8U HEX_Val, AutoMachine *am_val )
{
	INT8C Temp_HEX[]={"0x00"};

	Temp_HEX[2] = (HEX_Val >> 4) & 0x0f ;

	if ( Temp_HEX[2] > 9 )
		Temp_HEX[2] += 0x37 ;
	else 
		Temp_HEX[2] += 0x30 ;

	Temp_HEX[3] = HEX_Val  & 0x0f ;
	if ( Temp_HEX[3] > 9 )
		Temp_HEX[3] += 0x37 ;
	else 
		Temp_HEX[3] += 0x30 ;

	am_val->class->Cpu_UartWr(Temp_HEX);
}
*/
