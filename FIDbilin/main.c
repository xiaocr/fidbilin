/*****************************************************************************
 *   tmrtest.c:  main C entry file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.09.01  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "config.h"
#include <stdarg.h>   
#include <stdlib.h>
#include "adc.h"
#include "CH376INC.H"
#include "file_sys.h"
//volatile struct Uart0Info Uart0;
struct Uart0Info Uart0;

volatile uint8 sendMeasurFlg;
//volatile uint8 camDataBuf[2559];
//volatile uint16 adbuf[8192];
volatile uint16 adCount;
volatile uint8 secTimFlg,tempCtrFlg,udiskScanFlg;

uint8 adVluBuf[10];
uint8 keyVlu[4];
uint8 ckeyBuf[2];

uint16 tempThreshhold = 0;

uint16 tempDelayCnt;
uint16 autoCalibTimCnt = 0;

uint8 sampSpeed = 9;//100khz
uint8 digiterBuf[13]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xe6,0x02,0x01,0x00};
//uint16 measurVoltageRefBuf[5][2] = {{634,2069},{244,2460},{631,2237},{634,2380},{634,2360}};//0-100,0-10,0-1000,0-10000,0-100000
uint16 measurVoltageRefBuf[5][2] = {{634,2069},{244,2305},{631,2036},{634,2380},{634,2360}};//0-100,0-15,0-1000,0-10000,0-100000
uint32 measurConcentRefBuf[5] = {90,13,900,9000,90000};

uint16 measurMethaneVoltageRefBuf[5][2] = {{634,2069},{244,2305},{631,2036},{634,2380},{634,2360}};//0-100,0-15,0-1000,0-10000,0-100000
uint32 measurMethaneConcentRefBuf[5] = {90,13,900,9000,90000};

uint8 measureFlg = 0,changeFuntionFlg = 0,selectFunNum = 0,pintfItemSel = 0;
uint8 fidSel = 0,tempSel = 0,vaccumSel = 0,autoCalibSel = 0,autoCalibFlg = 0,autoCalibOrder = 0,autoCalibMeasurFlg = 0,lecChar = 0,ledCode = 0,sorftCtrRatFlg = 0;
uint8 ppmRateSel = 0,ppmRateAutoSelFlg = 0;

volatile uint8 lightAquirFlg = 0,lightTim20sDlayFlg = 0,lightLosFlg = 0,lightOnFlg = 0,lightTimFlg = 0,udiskFlg = 0,udiskMountFlg = 0;
volatile uint16 lightLosCnt = 0 , lightTim2sDlay = 0,lightTim20sDlay = 0,preTempFlg = 0,preTemp = 1024;

unsigned char 	nonMethaneTotalHydroCarbonFlg;

unsigned char erroStatus;//�������룬ÿһλ����һ��������  8Ԥ����7�����̣�6У׼������������5У׼��������̫�ߣ�4Ԥ����3�¶ȳ���δ�ﵽ��2Ϩ��1Ԥ��
unsigned int conditionStatus;//״̬���룬ÿһλ����һ��״̬ 1������ŷ�����2������ŷ�����3������ŷ�����4У׼��5���飻6���7Ԥ�ȣ�8����	��9�Զ����̣�
//uint16 gasPreesure = 0;
uint16 adSum = 1024;

uint16 comSendDelayTimeCnt = 0;
unsigned char comSendEnableFlg = 0;

uint32 ppmRate[5] = {10,100,1000,10000,100000};

//double ppmConcent=0.0;

uint32 timCnt;
uint32 nonMethaneTotalHydroCarbonTimCnt;
float totalHydroCarbonVlv, methaneVlv;


//float TempDetctKp = 5.2;
float TempDetctKp = 2.2;
float TempDetctKi = 0.15;
float TempDetctKd = 0.0;

float TempDetctNow ,TempDetctBef; 	
float TempDetctErr_now,TempDetctErr_bef;		
float TempDetctError_add = 0;			
//float TempDetctSet = 1.6;//191�ȶ�Ӧ�ĵ�ѹ

//float TempDetctSet = 512;//191�ȶ�Ӧ�ĵ�ѹ������	   ��4700/((4700/4800)-(512*3.3/1024)/113.3)��- 4700=�趨�¶ȶ�ӦRT100����Ӧ����ֵ��113.3Ϊ�Ŵ���
float TempDetctSet = 522;  

//float TempDetctSet = 488.0;
//float TempDetctSet = 256;

float TempCatalyticKp = 5.2;
float TempCatalyticKi = 0.15;
float TempCatalyticKd = 0.0;

float TempCatalyticNow,TempCatalyticBef; 																				 
float TempCatalyticErr_now,TempCatalyticErr_bef;		
float TempCatalyticError_add = 0;			
//float TempCatalyticSet = 2.8;//290��Ӧ�ĵ�ѹ
//float TempCatalyticSet = 760.8;//290��Ӧ�ĵ�ѹ������
//float TempCatalyticSet = 790;
//float TempCatalyticSet = 522;

//float TempCatalyticSet = 256;
float TempCatalyticSet = 740.0;



extern unsigned char ComCmdPocess();
extern unsigned char KeyFunProcess(uint8 presskeyVlu);
extern uint32 adFilter(uint8 adChannel,uint8 adIndex);

extern void Timer0Init(uint32 uStimLong);
extern void Timer0_Handler(void);
extern void Uart0_Handler(void);
extern void Uart1_Handler(void);
//extern void Eint0_Handler(void);
//extern void Eint1_Handler(void);

/*****************************************************************************
**   Main Function  main()
*****************************************************************************/
/*
uint8 InitEint0 (void)
{
	VICIntSelect = 0;
	VICVectAddr14 = (uint32)Eint0_Handler;
	VICVectPriority14 = 0x01;
	EXTINT = 0x01;
	EXTMODE = 0x01;//�ش���
	EXTPOLAR = 0x00;//������
	VICIntEnable |= 1 << 14;
}
void Eint0_Exception(void)
{

	VICIntEnClr |= 1 << 14;
	


	EXTINT = 0x01;
	VICVectAddr = 0; 
		
}

uint8 InitEint1(void)
{
	VICIntSelect = 0;
	VICVectAddr15 = (uint32)Eint1_Handler;
	VICVectPriority15 = 0x02;
	EXTINT = 0x02;
	EXTMODE = 0x02;//�ش���
	EXTPOLAR = 0x00;//�½���
	VICIntEnable |= 1 << 15;
}


void Eint1_Exception(void)
{
	

	EXTINT = 0x02;
	VICVectAddr = 0; 
		
}
*/


uint8 InitialiseSPI(uint8 Fdiv)
{
    
    S0SPCCR = Fdiv & 0xfe;                    
    //S0SPCR = 0x38;  
	S0SPCR = 0x20;                         
    //S0SPCR &= 0xef;                                           
                                               
     return(1);
}


//struct Uart0Info Uart0;
/*********************************************************************************************************
** Function name:			InitUart0
**
** Descriptions:			Initialize the Uart0
**
** input parameters:		������
** Returned value:			None
**         
** Used global variables:	None
** Calling modules:			None
**
** Created by:				XIAOCR
** Created Date:			2008/03/31
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint8 InitUart0(uint32 bps)
{  	
	//uint32 Fdiv;
    float Fdiv;
	uint8 cTemp;


    U0LCR = 0x80;                               /* �������ʷ�Ƶ���ӼĴ��� */
    Fdiv = (Fpclk / 16) / bps;                  /* ���ò����� */
	Fdiv *= 2;	//������PLLʱ��Fpclk��һ��,��ΪĬ��PLL��������n = 1��
	Fdiv /= 1.5;

	/*
	U0DLM = 0;	//115200
	U0DLL =	0x04;
    U0FDR = 0x85;
	*/

    U0DLM = 0;	//9600
	U0DLL =	0x47;
    U0FDR = 0xa1;

    U0LCR = 0x03;                               /* ��ֹ���ʷ�Ƶ���ӼĴ��� */
					                            /* ������Ϊ8,1,n */
	
	
	U0IER = 0x05;   
	//U0IER = 0x00;   
	                         
    U0FCR = 0x87;                              

   	
    VICIntSelect = 0;
    VICVectAddr6 = (uint32)Uart0_Handler;
    VICVectPriority6 = 0;//0-15��0Ϊ������ȼ�
    VICIntEnable |= 1 << 6;
	
   
    return TRUE;
}
/*********************************************************************************************************
** Function name:			Uart0_Exception
**
** Descriptions:			Uart0 interrupt service function
**
** input parameters:		None
** Returned value:			None
**         
** Used global variables:	None
** Calling modules:			None
**
** Created by:				XIAOCR
** Created Date:			2008/03/31
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void Uart0_Exception(void)
{
	uint8 IIR,temp;
	
    while(((IIR = U0IIR) & 0x01) == 0)
    {                                                  
        switch (IIR & 0x0e)
        {
            case 0x04:                                 
               for(IIR = 0; IIR < 8; IIR ++)
               {
	               if(Uart0.RecvCount < sizeof(Uart0.RecvBuf))
                   {
                       Uart0.RecvBuf[Uart0.RecvPoint] = U0RBR;  
                       if((Uart0.RecvPoint == 0) && (Uart0.RecvStart == 0))
                       {
                       		if(Uart0.RecvBuf[Uart0.RecvPoint] == 0x02)
                       		{
	                       		Uart0.RecvStart = 1;
	                       		//DspPagIndex = SearialComPag;
                       		}
							else
							{
								Uart0.RecvCount = 0;
				                Uart0.RecvStart = 0;
				                Uart0.RecvPoint = 0;
								Uart0.RecvOver = 0;	
							}
                       }
					   /*
                       else if(Uart0.RecvPoint == 3)
                       {
                       		Uart0.PacketLength =  ((uint32)Uart0.RecvBuf[1]<<8)| Uart0.RecvBuf[2];
                       }
					   */ 
					   if(Uart0.RecvStart == 1)
                       {    
	                       //if((Uart0.RecvCount > 3) && (Uart0.RecvCount >= (Uart0.PacketLength-1))) 
						   if(Uart0.RecvBuf[Uart0.RecvPoint] == 0x03)
	                       {
		                   		Uart0.RecvOver = 1;
		                   		U0IER = 0x00;
		                   		Uart0.RecvCount = 0;
				                Uart0.RecvStart = 0;
				                Uart0.RecvPoint = 0;
	
				                break;
		                   }  
		                   else
		                   {                      
			                   Uart0.RecvPoint++;
			                   Uart0.RecvPoint %= sizeof(Uart0.RecvBuf);
			                   Uart0.RecvCount++;
		                   }
					   }
                   }
                   else
                   {
                       temp = U0RBR;//�ն�
                   }
               }
               break;
			
			case 0x06:                                 
                temp = U0LSR;
                break;
                
            //case 0x04:    
            case 0x0c:                                
                while(((temp=U0LSR)&0X01)==0X01)       
                {	
                   //if(Uart0.RecvCount < RecBufLength)
                   if(Uart0.RecvCount < sizeof(Uart0.RecvBuf))
                   {
                   		Uart0.RecvBuf[Uart0.RecvPoint] = U0RBR;
                   		if((Uart0.RecvPoint == 0) && (Uart0.RecvStart == 0))
                        {
                        	if(Uart0.RecvBuf[Uart0.RecvPoint] == 0x02)
                       		{
	                       		Uart0.RecvStart = 1;
	                       		//DspPagIndex = SearialComPag;
                       		}
							else
							{
								Uart0.RecvCount = 0;
				                Uart0.RecvStart = 0;
				                Uart0.RecvPoint = 0;
								Uart0.RecvOver = 0;
							}
                        } 
						/*
                        else if(Uart0.RecvPoint == 3)
                        {
                       		Uart0.PacketLength =  ((uint32)Uart0.RecvBuf[1]<<8)| Uart0.RecvBuf[2];
                    
                        }
						*/
                      	if(Uart0.RecvStart == 1)
                       	{
			                //if((Uart0.RecvCount > 3) && (Uart0.RecvCount >= (Uart0.PacketLength-1))) 
							if(Uart0.RecvBuf[Uart0.RecvPoint] == 0x03)
			                {
			                	Uart0.RecvOver = 1;
			                   	U0IER = 0x00;	
	                   		 	Uart0.RecvCount = 0;
			                 	Uart0.RecvStart = 0;
			                 	Uart0.RecvPoint = 0;
				               	break;
		                   	}
		                     
		                  	else
		                   	{                      
			                   Uart0.RecvPoint++;
			                   Uart0.RecvPoint %= sizeof(Uart0.RecvBuf);
			                   Uart0.RecvCount++;
		                   	}
						}
	                   	
	              
                   }
                   else
                   {
                       temp = U0RBR;//�ն�
                   }
                }
 				
                temp = U0RBR;//�ն�   

        
                break;
            default :
             	
                break;
        }
		
    } 

    VICVectAddr = 0;            // ֪ͨ�жϿ������жϽ���

	
	
	return;
	
}
/*********************************************************************************************************
** Function name:			InitUart0
**
** Descriptions:			Initialize the Uart0
**
** input parameters:		������
** Returned value:			None
**         
** Used global variables:	None
** Calling modules:			None
**
** Created by:				XIAOCR
** Created Date:			2008/03/31
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint8 InitUart1(uint32 bps)
{  	
	//uint32 Fdiv;
    float Fdiv;
	uint8 cTemp;


    U1LCR = 0x80;                               /* �������ʷ�Ƶ���ӼĴ��� */
    Fdiv = (Fpclk / 16) / bps;                  /* ���ò����� */
	Fdiv *= 2;	//������PLLʱ��Fpclk��һ��,��ΪĬ��PLL��������n = 1��
	Fdiv /= 1.5;
							
	/*
	U0DLM = 0;	//115200
	U0DLL =	0x04;
    U0FDR = 0x85;
	*/

	U1DLM = 0;	//9600
	U1DLL =	0x47;
    U1FDR = 0xa1;
 

    U1LCR = 0x03;                               /* ��ֹ���ʷ�Ƶ���ӼĴ��� */
					                            /* ������Ϊ8,1,n */

	//U1IER = 0x05; 
	U1IER = 0x00;                              
    U1FCR = 0x87;                              

   	/*
    VICIntSelect = 0;
    VICVectAddr7 = (uint32)Uart1_Handler;
    VICVectPriority7 = 0;//0-15��0Ϊ������ȼ�
    VICIntEnable |= 1 << 7;
	*/
   
    return TRUE;
}

/*********************************************************************************************************
** Function name:			RtcInit
**
** Descriptions:			RTC ��ʼ��
**
** input parameters:		
** Returned value:			None
**         
** Used global variables:	None
** Calling modules:			None
**
** Created by:				XIAOCR
** Created Date:			2008/03/31
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint8 RtcInit(void)
{  	
	uint32 i;
    
    RTC_CCR &= ~0x01;	
	RTC_YEAR = 2017;
	RTC_MONTH = 7;
	RTC_DOW = 1;
	RTC_DOM = 17;
	RTC_HOUR = 15;
	RTC_MIN = 30;
	RTC_SEC = 0;  	
  	
   	RTC_CCR = 0x10;	//ѡ���ⲿʱ��
    RTC_CCR |= 0x02;//��λ
    for(i = 0; i < 1000; i++);
    RTC_CCR &= ~0x02;
    RTC_CCR |= 0x01;//����RTC
    
    PCONP &= ~0x00000200;//PCRTC = 0;
   
    return TRUE;
}
/*********************************************************************************************************
** Function name:			Timer0Init
**
** Descriptions:			Initialize the Time0
**
** input parameters:		None
** Returned value:			None
**         
** Used global variables:	None
** Calling modules:			None
**
** Created by:				XIAOCR
** Created Date:			2008/03/31
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void Timer0Init(uint32 uStimLong)
{
	uint32 lTemp = 2*Fpclk;//������PLLʱ��Fpclk��һ��,��ΪĬ��PLL��������n = 1��

	/*
	T0TCR = 0x00;
    T0IR = 0xffffffff;
	T0CTCR= 0x00;
    T0TC = 0;
    T0PR = 199;
    T0MCR = 0x03;
   	T0MR0 = ((Fpclk/200)*uStimLong)/1000000;  
	*/
	T0TCR = 0x00;
    T0IR = 0xffffffff;
	T0CTCR= 0x00;
    T0TC = 0;
	T0PR = 0; 
    T0MCR = 0x03;
	T0MR0 = (lTemp/1000000)*uStimLong;  
	
	
	VICVectAddr4 = (uint32)Timer0_Handler;
	VICVectPriority4 = 0x01;
	VICIntEnable |= 1 << 4;
	
    //T0TCR = 0x03;
    //T0TCR = 0x01; 

	/*
	VICIntEnClr |= 1 << 4;
	T0TC = 0;
	T0TCR = 0x00; 
	*/

	/*
	T0TCR = 0x03;
    T0TCR = 0x01;
	*/
 }
/*********************************************************************************************************
** Function name:			Timer0_Exception
**
** Descriptions:			Timer0 interrupt service function
**
** input parameters:		None
** Returned value:			None
**         
** Used global variables:	None
** Calling modules:			None
**
** Created by:				XIAOCR
** Created Date:			2008/03/31
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void Timer0_Exception(void)
{
  	uint16 i,j;

	if((tempDelayCnt%50) == 0)
	{
		secTimFlg = 1;
	}
	if(tempDelayCnt>100)
	{
		tempCtrFlg = 1;
		tempDelayCnt = 0;
	}
	else
	{
		tempDelayCnt++;	
		/*
		if(udiskFlg == 0)
		{
			 if(CH376DiskConnect( ) == USB_INT_SUCCESS)
			 {
			 	udiskFlg = 1;
				udiskMountFlg = 0;
			 }
		}
		*/
		//udiskScanFlg = 1;
	}
	/*
	if((fidSel == 1)&&((erroStatus&0x06) == 0))//��Ǽ�������
	{
		if(nonMethaneTotalHydroCarbonTimCnt >= 3000)//1����
		{
			comSendDelayTimeCnt = 0;
			comSendEnableFlg = 0;

			if(nonMethaneTotalHydroCarbonFlg == 0)
			{
				P27Lo();
				conditionStatus |= 0x0010;
			}
			else
			{
				P27Hi();
				conditionStatus &= ~0x0010;
			}
			nonMethaneTotalHydroCarbonFlg = ~nonMethaneTotalHydroCarbonFlg;	
			nonMethaneTotalHydroCarbonTimCnt = 0;
		}
		else
		{
			nonMethaneTotalHydroCarbonTimCnt++;
		}
		if(comSendDelayTimeCnt >= 1500)
		{
			comSendEnableFlg = 1;
		}
		else
		{
			comSendDelayTimeCnt++;
		}
	}
	*/
	if(lightAquirFlg == 1)//�������
	{
		if(lightTim20sDlayFlg == 1)//ͣ���30����ʱ
		{
			if(lightTim20sDlay >= 1500)	
			{
				
				//P121Lo();//�ر�����
				//P123Lo();//�ر�����	

			    //P118Lo();//�������
				P118Hi();//�������

				

				//P121Hi();//�������
				P123Hi();//�������

				lightTimFlg = 1;
				/*
				alarmLedOn();
				for(j = 0;j < 500; j++)
				for(i = 0;i < 100; i++);
				alarmLedOff();
				*/

				lightTim2sDlay = 0;
				lightTim20sDlay = 0;
				lightTim20sDlayFlg = 0;	
			}
			else 
			{
				if(lightTim20sDlay == 1450) //���������29����������ʼ�¶�
					preTempFlg = 1;

				lightTim20sDlay++;
			}	
		}
		else
		{
			if(lightTim2sDlay >= 100 )//2��
			{
				//P121Lo();//�ر�����
				//P123Lo();//�ر�����
				
				//P118Hi();//���ֹͣ
				P118Lo();//���ֹͣ

				lightTimFlg = 0;

				lightTim2sDlay = 0;
				if(lightLosCnt >= 10)//���10��δ�ɹ�����
				{
					//P121Lo();//�ر�����
					P123Lo();//�ر�����

					lightLosFlg = 1;
					lightAquirFlg = 0;
					P28Hi();//ͣ����	
				}
				else
				{
					lightLosCnt++;
					lightTim20sDlay = 0;
					lightTim20sDlayFlg = 1;
				}

				/* 
				P118Hi();//���ֹͣ
				lightTim2sDlay = 0;
				lightAquirFlg = 0;
				lightOnFlg = 0;
				*/	
			}
			else
			{	
				lightTim2sDlay++;
				
			}
		}
	}
    else if(autoCalibFlg == 1) //У׼
	{
		if(autoCalibTimCnt < 3000)
		{
			if(autoCalibTimCnt == 2900)
			{
				autoCalibMeasurFlg == 1;	
			}
			autoCalibTimCnt++;
		}
		else
		{
			autoCalibTimCnt = 0;
			if(autoCalibOrder == 0)
			{
				autoCalibOrder = 1;
				P23Hi(); //ͨ����
				P24Lo();
				P25Hi();
	
				conditionStatus &= 0x78;//�л���ŷ�ͬʱ�˳�standby״̬
				conditionStatus |= 0x02;
				
			}
			else if(autoCalibOrder == 1)
			{
				autoCalibOrder = 2;
				P23Lo(); //ͨ����
				P24Hi();
				P25Hi();
				conditionStatus &= 0x78;//�л���ŷ�ͬʱ�˳�standby״̬
				conditionStatus |= 0x01;
			}
			else				//У׼����ʱ�䵽
			{
				autoCalibOrder = 0;
				autoCalibFlg = 0;		
			}		
		}	
	}


   	T0IR = 0x01;
    VICVectAddr = 0;            //interrupt close ֪ͨ�жϿ������жϽ���
    
} 
/*********************************************************************************************************
** Function name:			Timer1Init
**
** Descriptions:			Initialize the Time1
**
** input parameters:		None
** Returned value:			None
**         
** Used global variables:	None
** Calling modules:			None
**
** Created by:				XIAOCR
** Created Date:			2008/03/31
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/

void Timer1Init(uint32 uStimLong)
{
	uint32 lTemp = 2*Fpclk;//������PLLʱ��Fpclk��һ��,��ΪĬ��PLL��������n = 1��

	T1TCR = 0x00;
    T1IR = 0xffffffff;
	T1CTCR= 0x00;
    T1TC = 0;
	T1PR = 0;
    T1MCR = 0x03;
	T1MR0 = (lTemp/1000000)*uStimLong;    
    
    T1TCR = 0x03;
    T1TCR = 0x01;
   
    
 }
/*********************************************************************************************************
** Function name:			Timer1_Exception
**
** Descriptions:			Timer1 interrupt service function
**
** input parameters:		None
** Returned value:			None
**         
** Used global variables:	None
** Calling modules:			None
**
** Created by:				XIAOCR
** Created Date:			2008/03/31
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void Timer1_Exception(void)
{

   	T1IR = 0x01;
    VICVectAddr = 0;            //interrupt close ֪ͨ�жϿ������жϽ���
    
}

uint8 *itoa(uint32 num, uint8 *str, int8 radix) 
{ 
	uint8 string[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"; 
	
	int8 temp;
	int8 i; 
	int8 j; 
	uint8* ptr = str; 
	
	if(num == 0)
	{
		*ptr = '0';
		ptr++;
		*ptr = '0';
		ptr++;
		*ptr = '\0';
	}
	else
	{
		while (num) 
		{ 
			*ptr = string[num % radix]; 
			num /= radix; 
			ptr++;
			if (num < radix) 
			{ 
				*ptr = string[num]; 
				ptr++;
				*ptr = '\0'; 
				break; 
			} 
		} 
	}
	j = ptr - str - 1; 

	for (i = 0; i < (ptr - str) / 2; i++ ) 
	{ 

		temp = str[i]; 
		str[i] = str[j]; 
		str[j--] = temp; 
	} 

	return str; 
}
void uart_put(char data)
{
        U0THR = data;
		while((U0LSR & 0x20) == 0);        
}

void printu(const char* fmt,...)
{
         const char* s;
         char ch=0;
         int d;
         char buf[16];
         va_list ap;
         va_start(ap,fmt);
         while (*fmt)
         {
                  if (*fmt != '%')
                  {
                           uart_put(*fmt++);                
                           continue; 
                  }
                  switch (*++fmt)
                  {
                           case 's':                
                                    s = va_arg(ap,const char*);
                                    for (; *s; s++)
                                             uart_put(*s);
                                    break;
                           case 'x':
                                    d = va_arg(ap,int);
                                    itoa(d,buf,16);
                                    for (s = buf; *s; s++)
                                             uart_put(*s);
                                    break;
                           case 'd':
                                    d = va_arg(ap,int);
                                    itoa(d,buf,10);
                                    for (s = buf; *s; s++)
                                             uart_put(*s);
                                    break;
                           case 'c':
                                    ch = va_arg(ap,int);
                                    uart_put(ch);
                                    break;
                           default:
                                    uart_put(*fmt);
                                    break;
                    }
                  fmt++;
              }
         va_end(ap);
}

uint16 com0Recive(uint16 waitTim)
{
	uint8 timOutFlg = 0,dataInFlg = 0;
	uint16 i,recvDlayCnt;
	uint32 RecvPoint;


	recvDlayCnt = waitTim;
	timCnt = 0;
	timOutFlg = 0;
	RecvPoint = 0;
	memset(Uart0.RecvBuf,'\0',sizeof(Uart0.RecvBuf));
	dataInFlg = 0;

	do{
		while((U0LSR & 0x01) == 0)
		{
			
			if((T1IR & 0x01) == 0x01)
			{
				T1IR = 0x01;
				T1TC = 0;
				
				if(timCnt >= recvDlayCnt)
				{
					timCnt = 0;
					timOutFlg = 1;
					break;
				}
				else
					timCnt++;
			}
				
		}
		if(timOutFlg == 0)
		{
			if(RecvPoint > 1000)
				break;
			Uart0.RecvBuf[RecvPoint] = U0RBR;
			RecvPoint++;
			Uart0.RecvBuf[RecvPoint] ='\0';
			timCnt = 0;
			recvDlayCnt = 50;
			dataInFlg = 1;
		}
		else
			break;

	}while(1);

    if(dataInFlg == 0)
		return 0 ;
	else
	    return RecvPoint;

}
uint16 com0Send(const uint8 * sendStr,uint16 waitTim)
{
	uint8 timOutFlg = 0,dataInFlg = 0;
	uint16 i,recvDlayCnt;
	uint32 RecvPoint;


	recvDlayCnt = waitTim;
	timCnt = 0;
	timOutFlg = 0;
	RecvPoint = 0;
	//memset(Uart0.RecvBuf,'\0',sizeof(Uart0.RecvBuf));

	//printu(sendStr);
	
	for(i = 0; i < strlen(sendStr); i++)
	{
		U0THR = sendStr[i];
		while((U0LSR & 0x20) == 0);
	}
	memset(Uart0.RecvBuf,'\0',sizeof(Uart0.RecvBuf));
	dataInFlg = 0;
	do{
		while((U0LSR & 0x01) == 0)
		{
			
			if((T1IR & 0x01) == 0x01)
			{
				T1IR = 0x01;
				T1TC = 0;
				
				if(timCnt >= recvDlayCnt)
				{
					timCnt = 0;
					timOutFlg = 1;
					break;
				}
				else
					timCnt++;
			}
				
		}
		if(timOutFlg == 0)
		{
			if(RecvPoint > 1000)
				break;
			Uart0.RecvBuf[RecvPoint] = U0RBR;
			RecvPoint++;
			Uart0.RecvBuf[RecvPoint] ='\0';
			timCnt = 0;
			recvDlayCnt = 50;
			dataInFlg = 1;
		}
		else
			break;

	}while(1);
    if(dataInFlg == 0)
		return 0 ;
	else
	    return RecvPoint;

}
uint16 com1Recive(uint16 waitTim)
{
	uint8 timOutFlg = 0,dataInFlg = 0;
	uint16 i,recvDlayCnt;
	uint32 RecvPoint;


	recvDlayCnt = waitTim;
	timCnt = 0;
	timOutFlg = 0;
	RecvPoint = 0;
	memset(Uart0.RecvBuf,'\0',sizeof(Uart0.RecvBuf));
	dataInFlg = 0;

	do{
		while((U1LSR & 0x01) == 0)
		{
			
			if((T1IR & 0x01) == 0x01)
			{
				T1IR = 0x01;
				T1TC = 0;
				
				if(timCnt >= recvDlayCnt)
				{
					timCnt = 0;
					timOutFlg = 1;
					break;
				}
				else
					timCnt++;
			}
				
		}
		if(timOutFlg == 0)
		{
			if(RecvPoint > 1000)
				break;
			Uart0.RecvBuf[RecvPoint] = U1RBR;
			RecvPoint++;
			Uart0.RecvBuf[RecvPoint] ='\0';
			timCnt = 0;
			recvDlayCnt = 50;
			dataInFlg = 1;
		}
		else
			break;

	}while(1);

	if(dataInFlg == 0)
		return 0 ;
	else
	    return RecvPoint;

}
uint16 com1Send(const uint8 * sendStr,uint16 waitTim)
{
	uint8 timOutFlg = 0,dataInFlg = 0;
	uint16 i,recvDlayCnt;
	uint32 RecvPoint;


	recvDlayCnt = waitTim;
	timCnt = 0;
	timOutFlg = 0;
	RecvPoint = 0;
	//memset(Uart0.RecvBuf,'\0',sizeof(Uart0.RecvBuf));

	//printu(sendStr);
	
	for(i = 0; i < strlen(sendStr); i++)
	{
		U1THR = sendStr[i];
		while((U1LSR & 0x20) == 0);
	}
	memset(Uart0.RecvBuf,'\0',sizeof(Uart0.RecvBuf));

	dataInFlg = 0;
	do{
		while((U1LSR & 0x01) == 0)
		{
			
			if((T1IR & 0x01) == 0x01)
			{
				T1IR = 0x01;
				T1TC = 0;
				
				if(timCnt >= recvDlayCnt)
				{
					timCnt = 0;
					timOutFlg = 1;
					break;
				}
				else
					timCnt++;
			}
				
		}
		if(timOutFlg == 0)
		{
			if(RecvPoint > 1000)
				break;
			Uart0.RecvBuf[RecvPoint] = U1RBR;
			RecvPoint++;
			Uart0.RecvBuf[RecvPoint] ='\0';
			timCnt = 0;
			recvDlayCnt = 50;
			dataInFlg = 1;
		}
		else
			break;

	}while(1);
	if(dataInFlg == 0)
		return 0 ;
	else
	    return RecvPoint;

}
void hct595sendByte(uint8 date)
{
	uint8 i,j;

	for(i = 0; i < 8; i++)
	{
		if((date>>i)&0x01)
		{
			P129Hi();
		}
		else
		{
			P129Lo();
		}

		P128Lo();
		for(j = 0; j < 20; j++);
		P128Hi();

	}
	P127Lo();
	for(j = 0; j < 20; j++);
	P127Hi();
}


int TempDetctPid()	
{
	
	int num = 0;
	
	TempDetctErr_now = TempDetctSet - TempDetctNow;
	TempDetctErr_bef = TempDetctSet - TempDetctBef;
	
	TempDetctError_add = TempDetctError_add + TempDetctErr_now; 
	
	num = TempDetctKp*TempDetctErr_now + TempDetctKi*TempDetctError_add + TempDetctKd*(TempDetctErr_now - TempDetctErr_bef);
	
	if(num > 100)
		num = 100;
	else if(num < 0)
		num = 0;

	num = 100 - num;	
	return(num);
}



int TempCatalyticPid()	
{
	
	int num = 0;
	
	TempCatalyticErr_now = TempCatalyticSet - TempCatalyticNow;
	TempCatalyticErr_bef = TempCatalyticSet - TempCatalyticBef;
	
	TempCatalyticError_add = TempCatalyticError_add + TempCatalyticErr_now; 
	
	num = TempCatalyticKp*TempCatalyticErr_now + TempCatalyticKi*TempCatalyticError_add + TempCatalyticKd*(TempCatalyticErr_now - TempCatalyticErr_bef);
	
	if(num > 100)
		num = 100;
	else if(num < 0)
		num = 0;

	num = 100 - num;
	return(num);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void	xWriteCH376Cmd( UINT8 mCmd )  /* ��CH376д���� */
{
	
	U1THR = SER_SYNC_CODE1;  /* ���������ĵ�1������ͬ���� */
	while((U1LSR & 0x20) == 0);
	
	U1THR = SER_SYNC_CODE2;  /* ���������ĵ�2������ͬ���� */
	while((U1LSR & 0x20) == 0);
	
	U1THR = mCmd;  /* ������� */
	while((U1LSR & 0x20) == 0);

}

void	xWriteCH376Data( UINT8 mData )  /* ��CH376д���� */
{
	U1THR = mData;  /* ������� */
	while((U1LSR & 0x20) == 0);
}

UINT8	xReadCH376Data( void )  /* ��CH376������ */
{
	UINT32	i;
	for ( i = 0; i < 500000; i ++ ) {  /* ������ֹ��ʱ */
		if ((U1LSR & 0x01) != 0) {  /* ���ڽ��յ� */
			return(U1RBR);  /* �������� */
		}
	}
	return( 0 );  /* ��Ӧ�÷�������� */
}
/* ��ѯCH376�ж�(INT#�͵�ƽ) */
UINT8	Query376Interrupt( void )
{
	UINT8 cTemp;

	/*
	if ((U1LSR & 0x01) != 0)
	{  // ���δ����CH376���ж��������ѯ�����ж�״̬�� //
		cTemp = U1RBR;
		return( TRUE );
	}
	else return( FALSE );
	*/
   	
	FIO2MASK = 0xffffffff &(~P210);
	if((FIO2PIN & (P210)) != P210)
	{
		while((U1LSR & 0x01) == 0);
		cTemp = U1RBR;
		return( TRUE );	
	}
	else return( FALSE );

}
UINT8	mInitCH376Host( void )  /* ��ʼ��CH376 */
{
	UINT8	res;
	xWriteCH376Cmd( CMD11_CHECK_EXIST );  /* ���Ե�Ƭ����CH376֮���ͨѶ�ӿ� */
	xWriteCH376Data( 0x65 );
	res = xReadCH376Data( );

	
	//U0THR = res;
	//while((U0LSR & 0x20) == 0);	//����

	if ( res != 0x9A ) return( 0xFA  );  /* ͨѶ�ӿڲ�����,����ԭ����:�ӿ������쳣,�����豸Ӱ��(Ƭѡ��Ψһ),���ڲ�����,һֱ�ڸ�λ,���񲻹��� */

	xWriteCH376Cmd( CMD11_SET_USB_MODE );  /* �豸USB����ģʽ */
	//xWriteCH376Data( 0x05 );
	xWriteCH376Data( 0x05 );
	res = xReadCH376Data( );

	
	//U0THR = res;
	//while((U0LSR & 0x20) == 0);	 //����

	if ( res == CMD_RET_SUCCESS ) return( USB_INT_SUCCESS );
	else return( 0xFA  );  /* ����ģʽ���� */
}

UINT8	buf[64];
/* Ϊָ���ļ����ô������ں�ʱ�� */
/* �������:   ԭʼ�ļ�����iFileName��, �µĴ������ں�ʱ��: iCreateDate, iCreateTime */
/* ����״̬��: USB_INT_SUCCESS = ���óɹ�,
               ����״̬��ο�CH376INC.H */
UINT8	SetFileCreateTime( PUINT8 iFileName, UINT16 iCreateDate, UINT16 iCreateTime )
{
	UINT8	s;
	s = CH376FileOpen( iFileName );  /* ���ļ� */
	if ( s == USB_INT_SUCCESS ) {
		/* �ļ���д������... */
		s = CH376DirInfoRead( );  /* ��ȡ��ǰ�ļ���Ŀ¼��ϢFAT_DIR_INFO,��������ݵ����ڴ��� */
		if ( s == USB_INT_SUCCESS ) {
			buf[0] = (UINT8)iCreateTime;  /* �ļ�������ʱ��,��8λ��ǰ */
			buf[1] = (UINT8)(iCreateTime>>8);  /* �ļ�������ʱ��,��8λ�ں� */
			CH376WriteOfsBlock( buf, STRUCT_OFFSET( FAT_DIR_INFO, DIR_CrtTime ), 2 );  /* ���ڲ�������ƫ�Ƶ�ַDIR_CrtTimeд��2���ֽ� */
			buf[0] = (UINT8)iCreateDate;  /* �ļ�����������,��8λ��ǰ */
			buf[1] = (UINT8)(iCreateDate>>8);  /* �ļ�����������,��8λ�ں� */
			CH376WriteOfsBlock( buf, STRUCT_OFFSET( FAT_DIR_INFO, DIR_CrtDate ), 2 );  /* ���ڲ�������ƫ�Ƶ�ַDIR_CrtDateд��2���ֽ� */
			s = CH376DirInfoSave( );  /* �����ļ���Ŀ¼��Ϣ */
			if ( s == USB_INT_SUCCESS ) {  /* �ɹ��޸Ĳ����� */
				/* �ļ���д������... */
			}
		}
		CH376FileClose( FALSE );  /* �ر��ļ� */
	}
	return( s );
}
/* Ϊָ���ļ���������޸ĵ����ں�ʱ�� */
UINT8	SetFileWrtTime( PUINT8 iFileName, UINT16 iWrtDate, UINT16 iWrtTime )
{
	UINT8	s;
	s = CH376FileOpen( iFileName );  /* ���ļ� */
	if ( s == USB_INT_SUCCESS ) {
		/* �ļ���д������... */
		s = CH376DirInfoRead( );  /* ��ȡ��ǰ�ļ���Ŀ¼��ϢFAT_DIR_INFO,��������ݵ����ڴ��� */
		if ( s == USB_INT_SUCCESS ) {
			buf[0] = (UINT8)iWrtTime;  /* �ļ�������ʱ��,��8λ��ǰ */
			buf[1] = (UINT8)(iWrtTime>>8);  /* �ļ�������ʱ��,��8λ�ں� */
			CH376WriteOfsBlock( buf, STRUCT_OFFSET( FAT_DIR_INFO, DIR_WrtTime ), 2 );  /* ���ڲ�������ƫ�Ƶ�ַDIR_WrtTimeд��2���ֽ� */
			buf[0] = (UINT8)iWrtDate;  /* �ļ�����������,��8λ��ǰ */
			buf[1] = (UINT8)(iWrtDate>>8);  /* �ļ�����������,��8λ�ں� */
			CH376WriteOfsBlock( buf, STRUCT_OFFSET( FAT_DIR_INFO, DIR_WrtDate ), 2 );  /* ���ڲ�������ƫ�Ƶ�ַDIR_WrtDateд��2���ֽ� */
			s = CH376DirInfoSave( );  /* �����ļ���Ŀ¼��Ϣ */
			if ( s == USB_INT_SUCCESS ) {  /* �ɹ��޸Ĳ����� */
				/* �ļ���д������... */
			}
		}
		CH376FileClose( FALSE );  /* �ر��ļ� */
	}
	return( s );
}
void	host( ) {
	UINT8	i, s,creatFlg;
	UINT8	TotalCount;
	UINT8	FileDataBuf[512];
	UINT16	RealCount,iTemp;
	P_FAT_DIR_INFO	pDir;

	s = mInitCH376Host( );  /* ��ʼ��CH376 */

	while ( 1 ) 
	{
		com0Send( "---�ȴ�U�̲���---\n",100 );
		while ( CH376DiskConnect( ) != USB_INT_SUCCESS ) {  /* ���U���Ƿ�����,�ȴ�U�̲���,����SD��,�����ɵ�Ƭ��ֱ�Ӳ�ѯSD�����Ĳ��״̬���� */
			
			mDelaymS( 100 );  /* û��ҪƵ����ѯ */
		}
		mDelaymS( 200 );  /* ��ʱ,��ѡ����,�е�USB�洢����Ҫ��ʮ�������ʱ */
		com0Send( "�Ѽ�⵽U��\n",100 );

/* ���ڼ�⵽USB�豸��,���ȴ�100*50mS,��Ҫ�����ЩMP3̫��,���ڼ�⵽USB�豸��������DISK_MOUNTED��,���ȴ�5*50mS,��Ҫ���DiskReady������ */
		for ( i = 0; i < 100; i ++ ) 
		{  /* ��ȴ�ʱ��,100*50mS */
			mDelaymS( 50 );
			com0Send( "Ready ?\n",100 );
			s = CH376DiskMount( );  /* ��ʼ�����̲����Դ����Ƿ���� */
			if ( s == USB_INT_SUCCESS )
			{ 
				com0Send( "OK\n",100 );

				break;  /* ׼���� */
			}
			else if ( s == ERR_DISK_DISCON ) 
				break;  /* ��⵽�Ͽ�,���¼�Ⲣ��ʱ */
			if ( CH376GetDiskStatus( ) >= DEF_DISK_MOUNTED && i >= 5 )
				 break;  /* �е�U�����Ƿ���δ׼����,�������Ժ���,ֻҪ�佨������MOUNTED�ҳ���5*50mS */
		}
		if ( s == ERR_DISK_DISCON ) {  /* ��⵽�Ͽ�,���¼�Ⲣ��ʱ */
			com0Send( "Device gone\n",100 );
			continue;
		}
		if ( CH376GetDiskStatus( ) < DEF_DISK_MOUNTED ) {  /* δ֪USB�豸,����USB���̡���ӡ���� */
			com0Send( "Unknown device\n",100);
			goto UnknownUsbDevice;
		}
		//i = CH376ReadBlock( buf );  /* �����Ҫ,���Զ�ȡ���ݿ�CH376_CMD_DATA.DiskMountInq,���س��� */
		//if ( i == sizeof( INQUIRY_DATA ) )
		//{  /* U�̵ĳ��̺Ͳ�Ʒ��Ϣ */
			//buf[ i ] = 0;
			//com0Send(((P_INQUIRY_DATA)buf) -> VendorIdStr,100 );
		//}
		
		mDelaymS( 10000 ); 

		// ��ʼд���ļ� 
		FileDataBuf[0]='/';
		FileDataBuf[1]='8'; 
		FileDataBuf[2]='8'; 
		FileDataBuf[3]='8'; 
		FileDataBuf[4]='8'; 
		FileDataBuf[5]='.';	// ���ļ���չ�� 
		FileDataBuf[6]='T'; 
		FileDataBuf[7]='X'; 
		FileDataBuf[8]='T'; 
		FileDataBuf[9]=0;	// ��ǰ�ļ�������·���Ľ����� 
		
		s = CH376FileOpen(FileDataBuf); 
		if( s == USB_INT_SUCCESS )	// ���ļ��ɹ� 
		{ 
			//CH376FileErase(FileDataBuf);	// ԭ����ͬ���ļ�����ɾ�����½� 
			//CH376FileCreate( NULL );	// �½��ļ�����,���ļ���������ɾ�������½�,�������ṩ 
			creatFlg = 0;
		} 
		else
		{
			creatFlg = 1;
			CH376FileCreate( NULL );	// �½��ļ�����,���ļ���������ɾ�������½�,�������ṩ 
										// �ļ���,�ղ��Ѿ��ṩ��CH376FileOpen 
		}

		s = CH376ByteLocate( 0xFFFFFFFF );  /* �Ƶ��ļ���β�� */
		s = sprintf( buf, "��ȡ��ǰ�ļ�����= %ld �ֽ�\xd\xa", iTemp);  /* ע���ַ������Ȳ������buf,����Ӵ󻺳������߷ֶ��д�� */
		s = CH376ByteWrite( buf, s, NULL );  /* ���ֽ�Ϊ��λ���ļ�д������ */

		s = CH376FileClose( TRUE );

		if(creatFlg == 1)
			s = SetFileCreateTime( FileDataBuf, MAKE_FILE_DATE( 2017, 6, 8 ), MAKE_FILE_TIME( 15, 39, 20 ) );  /* Ϊָ���ļ����ô������ں�ʱ�� */
		else
			s = SetFileWrtTime( FileDataBuf, MAKE_FILE_DATE( 2017, 6, 8 ), MAKE_FILE_TIME( 16, 10, 10 ) );  /* Ϊָ���ļ����ô������ں�ʱ�� */
	


UnknownUsbDevice:
		//printf( "Take out\n" );
		com0Send( "---�ȴ�U�̰γ�---\n",100 );
		while ( CH376DiskConnect( ) == USB_INT_SUCCESS ) {  /* ���U���Ƿ�����,�ȴ�U�̰γ� */
		//while (  Wait376Interrupt( ) != USB_INT_DISCONNECT ) {  /* ���U���Ƿ�����,�ȴ�U�̰γ� */
			mDelaymS( 100 );
		}
		//LED_UDISK_OUT( );  /* LED�� */
		mDelaymS( 100 );
		com0Send( "---U���Ѱγ�---\n",100 );
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////
int main (void)
{
	uint8 cxor = 0,erro = 0,scalSel = 0;
	//uint8 tempFlg = 0,tim0Flg = 0;
	
	uint8 cTemp;
	uint8 dpData[8];
	uint8 cTemp1[11],ladVluBuf[10];
	uint8 cachBuf[1024],uDiskBuf[2048];

	UINT8	s,creatFlg = 0;
	UINT8	FileDataBuf[512];

	uint8 autoIgnitFlg = 0,fileOpenFlg = 0;

	uint8 TempDetctDuty,TempCatalyticDuty,pidTempDetctFlg,pidTempCatalyticFlg;
	//uint16 tempDelayCnt;


	uint16 iTemp = 0,tim0Cnt = 0,ADC0Value = 0,lightOnCnt = 0,autoIgnitCnt = 0;
    int16 int16Temp;

	uint32 times,dates;
	uint32 i,j;
	uint32 lTemp,lTemp1;

	uint32 msCnt = 0;

	float fTemp,dTemp;
	double ppmConcent=0.0,ppmConcentBuf[3];

    nonMethaneTotalHydroCarbonTimCnt = 0;
	nonMethaneTotalHydroCarbonFlg = 0;
	totalHydroCarbonVlv = 0.0;
	methaneVlv = 0.0;

	pintfItemSel = 10;//Ĭ���ϵ������κ���Ϣ

	comSendDelayTimeCnt = 0;
	comSendEnableFlg = 0;

	Uart0.RecvOver = 0;
	Uart0.RecvStart = 0;
	Uart0.RecvPoint = 0;
	Uart0.RecvCount = 0;
	Uart0.PacketLength = 0;

 /*
			Uart0.RecvBuf[1] = 0x00;
		    Uart0.RecvBuf[7] = 0x30;
			Uart0.RecvBuf[8] = 0x20;
			Uart0.RecvBuf[9] = 'M';
			Uart0.RecvBuf[10] = 0x31;
			Uart0.RecvBuf[11] = 0x20;
			sprintf((unsigned char *)&Uart0.RecvBuf[12],"%d", ppmRate[0]);
			Uart0.RecvCount = strlen((unsigned char *)&Uart0.RecvBuf[12]);
			Uart0.RecvCount += 12;
			Uart0.RecvBuf[Uart0.RecvCount] = 0x20;
			Uart0.RecvBuf[Uart0.RecvCount+1] = 'M';
			Uart0.RecvBuf[Uart0.RecvCount+2] = 0x32;
			Uart0.RecvBuf[Uart0.RecvCount+3] = 0x20;
			sprintf((unsigned char *)Uart0.RecvBuf[Uart0.RecvCount+4],"%d", ppmRate[1]);
			Uart0.RecvCount += strlen((unsigned char *)&Uart0.RecvBuf[12]);
			//iTemp += 12;
			Uart0.RecvBuf[Uart0.RecvCount] = 0x20;
			Uart0.RecvBuf[Uart0.RecvCount+1] = 'M';
			Uart0.RecvBuf[Uart0.RecvCount+2] = 0x33;
			Uart0.RecvBuf[Uart0.RecvCount+3] = 0x20;
			sprintf((unsigned char *)Uart0.RecvBuf[Uart0.RecvCount+4],"%d", ppmRate[2]);
			Uart0.RecvCount += strlen((unsigned char *)&Uart0.RecvBuf[12]);
			//iTemp += 12;
			Uart0.RecvBuf[Uart0.RecvCount] = 0x20;
			Uart0.RecvBuf[Uart0.RecvCount+1] = 'M';
			Uart0.RecvBuf[Uart0.RecvCount+2] = 0x34;
			Uart0.RecvBuf[Uart0.RecvCount+3] = 0x20;
			sprintf((unsigned char *)Uart0.RecvBuf[Uart0.RecvCount+4],"%d", ppmRate[3]);
			Uart0.RecvCount += strlen((unsigned char *)&Uart0.RecvBuf[12]);
			//iTemp += 12;
			Uart0.RecvBuf[Uart0.RecvCount] = 0x20;
			Uart0.RecvBuf[Uart0.RecvCount+1] = 'M';
			Uart0.RecvBuf[Uart0.RecvCount+2] = 0x35;
			Uart0.RecvBuf[Uart0.RecvCount+3] = 0x20;
			sprintf((unsigned char *)Uart0.RecvBuf[Uart0.RecvCount+4],"%d", ppmRate[4]);
			Uart0.RecvCount += strlen((unsigned char *)&Uart0.RecvBuf[12]);
			//iTemp += 12;
			Uart0.RecvBuf[Uart0.RecvCount] = 0x03;

 */
	tim0Cnt = 0;
	//tim0Flg = 1;
	autoCalibFlg = 0;
	tempSel = 0;

	udiskScanFlg = 0;

	TargetResetInit();
	
	RtcInit();

	InitUart0(9600);
	InitUart1(9600);
	//Timer1Init(20000);
	//Timer1Init(1000);

	//Timer1Init(500);

	Timer0Init(20000);

	//I2c0Init(300000);
	I2c0Init(75000);

	InitialiseSPI(100);


	PCONP |= (1 << 12);
	AD0CR = ( 0x01 << 0 ) | 	/* SEL=1,select channel 0~7 on ADC0 */
		( ( (2*Fpclk) / 1000000 - 1 ) << 8 ) |  /* CLKDIV = Fpclk / 1000000 - 1 */ 
		( 0 << 16 ) | 		/* BURST = 0, no BURST, software controlled */
		( 0 << 17 ) |  		/* CLKS = 0, 11 clocks/10 bits */
		( 1 << 21 ) |  		/* PDN = 1, normal operation */
		( 0 << 22 ) |  		/* TEST1:0 = 00 */
		( 0 << 24 ) |  		/* START = 0 A/D conversion stops */
		( 0 << 27 );		/* EDGE = 0 (CAP/MAT singal falling,trigger A/D conversion) */ 

	//ADCInit( ADC_CLK );
	/*
	for(i = 0; i < 8; i++)
		dpData[i] = 0x6e;

 	//I2C_WriteNByte(0x70, 1, 0x0d, 0x05,1); //��ʾλ��
	I2C_WriteNByte(0x70, 1, 0x10, dpData,8);//��ʾ��0
	//I2C_WriteNByte(0x70, 1, 0x0c, 0x07,1); //��˸����

	for(i = 0; i < 8; i++)
		dpData[i] = 0;
	I2C_ReadNByte(0x70, 1, 0x10, dpData,8);

	for(i = 0; i < 8; i++)
	{
		U0THR = dpData[i];
		while((U0LSR & 0x20) == 0);
			 
	}

	*/
	
    



	/* ������ʼ�� */
	/*
	for(i = 0; i < 5;i++)
		I2C_WriteNByte(0xa0, 2, i*20,(uint8 *)&measurVoltageRefBuf[i][0],4);

	for(i = 0; i < 5;i++)
   	{
		for(j = 0; j < 2;j++)
		{
			measurVoltageRefBuf[i][j] = 0;
		}
	}
	for(i = 0; i < 5;i++)
		I2C_ReadNByte(0xa0, 2, i*20,(uint8 *)&measurVoltageRefBuf[i][0],4);

	for(i = 0; i < 5; i++)
	{
		for(j = 0; j < 2;j++)
		{
			U0THR = (measurVoltageRefBuf[i][j]>>8)&0x00ff;
			while((U0LSR & 0x20) == 0);
			U0THR = measurVoltageRefBuf[i][j]&0x00ff;
			while((U0LSR & 0x20) == 0);
		}
			 
	}
	*/
	


	for(i = 0; i < 5;i++)
	{
		I2C_ReadNByte(0xa0, 2, i*20,(uint8 *)&measurVoltageRefBuf[i][0],2);
		I2C_ReadNByte(0xa0, 2, (i*20)+2,(uint8 *)&measurVoltageRefBuf[i][1],2);
	}
	
    for(i = 0; i < 5;i++)
		I2C_ReadNByte(0xa0, 2, (i*20)+4,(uint8 *)&measurConcentRefBuf[i],4);

	I2C_ReadNByte(0xa0, 2, 120,(uint8 *)&tempThreshhold,2);

	for(i = 0; i < 5;i++)
	{
		I2C_ReadNByte(0xa0, 2, i*20+130,(uint8 *)&measurMethaneVoltageRefBuf[i][0],2);
		I2C_ReadNByte(0xa0, 2, (i*20+130)+2,(uint8 *)&measurMethaneVoltageRefBuf[i][1],2);
	}
	
    for(i = 0; i < 5;i++)
		I2C_ReadNByte(0xa0, 2, (i*20+130)+4,(uint8 *)&measurMethaneConcentRefBuf[i],4);


	for(i = 0; i < 5; i++)
	{
		for(j = 0; j < 2;j++)
		{
			U0THR = (measurVoltageRefBuf[i][j]>>8)&0x00ff;
			while((U0LSR & 0x20) == 0);
			U0THR = measurVoltageRefBuf[i][j]&0x00ff;
			while((U0LSR & 0x20) == 0);
		}
		printu("\r\n");	 
	}
    printu("\r\n");

    for(i = 0; i < 5; i++)
	{
		
		U0THR = (measurConcentRefBuf[i]>>24)&0x000000ff;
		while((U0LSR & 0x20) == 0);
		U0THR = (measurConcentRefBuf[i]>>16)&0x000000ff;
		while((U0LSR & 0x20) == 0);
		U0THR = (measurConcentRefBuf[i]>>8)&0x000000ff;
		while((U0LSR & 0x20) == 0);
		U0THR = measurConcentRefBuf[i]&0x000000ff;
		while((U0LSR & 0x20) == 0);
		printu("\r\n"); 
	}

	for(i = 0; i < 5; i++)
	{
		for(j = 0; j < 2;j++)
		{
			U0THR = (measurMethaneVoltageRefBuf[i][j]>>8)&0x00ff;
			while((U0LSR & 0x20) == 0);
			U0THR = measurMethaneVoltageRefBuf[i][j]&0x00ff;
			while((U0LSR & 0x20) == 0);
		}
		printu("\r\n");	 
	}
    printu("\r\n");

    for(i = 0; i < 5; i++)
	{
		
		U0THR = (measurMethaneConcentRefBuf[i]>>24)&0x000000ff;
		while((U0LSR & 0x20) == 0);
		U0THR = (measurMethaneConcentRefBuf[i]>>16)&0x000000ff;
		while((U0LSR & 0x20) == 0);
		U0THR = (measurMethaneConcentRefBuf[i]>>8)&0x000000ff;
		while((U0LSR & 0x20) == 0);
		U0THR = measurMethaneConcentRefBuf[i]&0x000000ff;
		while((U0LSR & 0x20) == 0);
		printu("\r\n"); 
	}

	U0THR = (tempThreshhold>>8)&0x00ff;
	while((U0LSR & 0x20) == 0);
	U0THR = tempThreshhold&0x00ff;
	while((U0LSR & 0x20) == 0);


	tempThreshhold = 30;//����

	erroStatus = 0x06;//�¶�δ����Ϩ��
	conditionStatus = 0x0144;//����Ԥ�ȣ�ͨ����
	ppmRateSel = 0x31;//���������̴���С��ʼ

	P18Lo();//���̿���
	P19Lo();
	P110Lo();
	P114Lo();
	P115Lo();
	P116Lo();

	//FIO1MASK = 0xffffffff &(~P115);//��������ť
	//if((FIO1PIN & P115) != 0)

	//FIO0MASK = 0xffffffff &(~(P015|P016|P017));//������
	FIO0MASK = 0xffffffff &(~(P020|P021|P022));//������
	//if((FIO0PIN & (P015|P016|P017)) == 0x00)
	if((FIO0PIN & (P020|P021|P022)) == 0x00)
	{
		ppmRateAutoSelFlg = 1;
		conditionStatus |= 0x0100;
		ppmRateSel = 0x31;		
	}
	else
	{
		ppmRateAutoSelFlg = 0;
		conditionStatus &= ~0x0100;
	}
	


	I2C_WriteNByte(0x70, 1, 0x0d, 0x05,1); //��ʾλ��


	

	//fidSel = 1;	//�����
	fidSel = 0;	//������

	ledCode = 0xb7;
	nonMethaneTotalHydroCarbonTimCnt = 0;
	nonMethaneTotalHydroCarbonFlg = 0;

	//lecChar = 0xef;
	lecChar = 0xff;//û��standbyģʽ
    //ledCode = 0x77;
	hct595sendByte(ledCode&lecChar);

	P00Lo();
	P25Lo();//����




	P119Lo();	//ѹ��ģ��ͨ���л�
  	//P120Lo();
	P122Lo();

	scalSel = 0x30;

    tempDelayCnt = 0;
	tempCtrFlg = 0;
	secTimFlg = 0;

	//gasPreesure = 0;

	TempDetctErr_now = 0.0;
	TempDetctErr_bef = 0.0;
	TempDetctError_add = 0.0;
	TempDetctBef = 0.0;
	TempDetctDuty = 50;

	pidTempDetctFlg = 0;

	TempCatalyticErr_now = 0.0;
	TempCatalyticErr_bef = 0.0;
	TempCatalyticError_add = 0.0;
	TempCatalyticBef = 0.0;
	TempCatalyticDuty = 50;
	
	pidTempCatalyticFlg = 0;

	PINSEL7 = 0x003c0000;
	PWM1PR = 0x0; // ����Ƶ������Ƶ��ΪFpclk
	PWM1MCR = 0x02; // ����PWMMR0ƥ��ʱ��λPWMTC
	PWM1PCR = 0x0c00; // ����PWM2,3���������PWM
	PWM1TCR = 0x02; // ��λPWMTC

	memset(uDiskBuf,'\0',2048);

	for(i = 0; i < 1000;i++)
	for(j = 0; j < 1000;j++); 
	udiskFlg = 2;
	if(udiskFlg == 0)
		mInitCH376Host( );  /* ��ʼ��CH376 */



	T0TCR = 0x03;
    T0TCR = 0x01;


	while(1)
	{
		/*
		if((T0IR & 0x01) == 0x01)
		{
			T0IR = 0x01;
			T0TC = 0;
		*/	
			if((udiskScanFlg == 1)&&(lightAquirFlg == 0)) //�������в�ɨ��U�̣���ֹӰ����
			{
				udiskScanFlg = 0;

				if(CH376DiskConnect( ) == USB_INT_SUCCESS)
				 {
				 	
				 	udiskFlg = 1;
		
					if(udiskMountFlg == 0)
					{
						for(i = 0; i < 20; i++)
						{
							if(CH376DiskMount( ) == USB_INT_SUCCESS)//����U�̣�����U��
							{
								udiskMountFlg = 1;
								alarmLedOn();
								for(j = 0;j < 500; j++)
								for(i = 0;i < 100; i++);
								alarmLedOff();
								break;
							}
							else
								for(j = 0; j < 500; j++);
						}
						udiskMountFlg = 1; //��ЩU��û��ʾ���سɹ���ʵ���Ѿ����Է�����
					}
					
				 }
			 }
			
			//if((tempDelayCnt%50) == 0)//1s��һ���¶�
			if(secTimFlg == 1)
			{
				secTimFlg = 0;
				/*
				if((udiskFlg == 1)&&(udiskMountFlg == 0))//����U�̣�����U��
				{
					if(CH376DiskMount( ) == USB_INT_SUCCESS)
					{
						udiskMountFlg = 1;
					
					}
				}
				*/

				

				AdProcess(&lTemp,0);//�¶�
				TempCatalyticNow = lTemp;

				if(pintfItemSel == 2)
				{
					memset(ladVluBuf,'\0',10);
					sprintf(ladVluBuf,"%d",lTemp);
					U0THR = 0x0d;
					while((U0LSR & 0x20) == 0);
					U0THR = 0x0a;
					while((U0LSR & 0x20) == 0);
					for(i = 0; i < strlen(ladVluBuf);i++)
					{
						U0THR = ladVluBuf[i];
						while((U0LSR & 0x20) == 0);
					}
				}

				AdProcess(&lTemp,2);//�¶�

				TempDetctNow = lTemp;

				if(pintfItemSel == 1)
				{
					memset(ladVluBuf,'\0',10);
					sprintf(ladVluBuf,"%d",lTemp);
					U0THR = 0x0d;
					while((U0LSR & 0x20) == 0);
					U0THR = 0x0a;
					while((U0LSR & 0x20) == 0);
					for(i = 0; i < strlen(ladVluBuf);i++)
					{
						U0THR = ladVluBuf[i];
						while((U0LSR & 0x20) == 0);
					}
				}

					
				
				if((TempDetctNow>(TempDetctSet-10))&&(TempCatalyticNow>(TempCatalyticSet-10)))//�¶ȵ������¶�
				{
					erroStatus &= 0xfb;	
					conditionStatus &= 0xbf;//�˳�Ԥ��״̬	

					if((autoIgnitFlg == 0)&&(lightAquirFlg == 0))
					{
						//P121Hi(); //ͨ����
						//P123Hi(); //ͨ����

						if(autoIgnitCnt >= 180)	//�¶ȵ��ˣ�3���Ӻ��Զ����
						{
							autoIgnitCnt = 0;
							autoIgnitFlg = 1;

							P28Lo();//��������
	
							//P118Lo();//������
							//for(j = 0;j < 1000; j++)
							//for(i = 0;i < 1000; i++);
							//P121Hi();//�������
							P123Hi();//�������
							
							lightTim2sDlay = 0;
							lightLosCnt = 0;
							lightAquirFlg = 1;
							lightTim20sDlay = 0;
							lightTim20sDlayFlg = 0;	
						}
						else
							autoIgnitCnt++;
					}

				}
				

				
				AdProcess(&lTemp,1);//����¶�

				if(preTempFlg == 1)
				{
					preTemp = lTemp;
					preTempFlg = 0;
					memset(ladVluBuf,'\0',10);
					sprintf(ladVluBuf,"%d",lTemp);
					U0THR = 0x55;
					while((U0LSR & 0x20) == 0);
					U0THR = 0xaa;
					while((U0LSR & 0x20) == 0);
					U0THR = 0x0d;
					while((U0LSR & 0x20) == 0);
					U0THR = 0x0a;
					while((U0LSR & 0x20) == 0);
					for(i = 0; i < strlen(ladVluBuf);i++)
					{
						U0THR = ladVluBuf[i];
						while((U0LSR & 0x20) == 0);
					}
					U0THR = 0x55;
					while((U0LSR & 0x20) == 0);
					U0THR = 0xaa;
					while((U0LSR & 0x20) == 0);
				}
						
				if(pintfItemSel == 0)
				{
					memset(ladVluBuf,'\0',10);
					sprintf(ladVluBuf,"%d",lTemp);
					U0THR = 0x0d;
					while((U0LSR & 0x20) == 0);
					U0THR = 0x0a;
					while((U0LSR & 0x20) == 0);
					for(i = 0; i < strlen(ladVluBuf);i++)
					{
						U0THR = ladVluBuf[i];
						while((U0LSR & 0x20) == 0);
					}
				}

				if(lightTimFlg == 0)//������շ������󣬷�ֹ���𻨵����¶����������жϵ��ɹ�
				{
					//if(lTemp > 125)//���ɹ� (���ɹ�ʱ�ȵ�ż��ӦΪ10mV)
					//if(lTemp >= (preTemp+60))
					if(lTemp >= (preTemp+tempThreshhold))

					//if((lTemp > 150)||(lightLosCnt == 2))//���ɹ�
					{
						if(lightOnFlg == 0)
						{
							lightOnCnt++;
	
							if(lightOnCnt > 5)
							{
								lightOnCnt = 0;
								
								//P118Hi();//���ֹͣ
								P118Lo();//���ֹͣ

								P26Lo();
								lightOnFlg = 1;
								lightAquirFlg = 0;
								lightLosCnt = 0;
								lightOnCnt = 0;
								erroStatus &= 0xfd;	
								conditionStatus |= 0x80;
								autoIgnitFlg = 1;//�ر��Զ����
							}
						}
						else
							preTemp = lTemp - tempThreshhold; //��ֹ��շ�����������ѹ��𣬸���ѹ�����������¶ȵı仯����Ϩ����ж�
						
					}
					else if(lTemp < (preTemp-tempThreshhold))
					{
						if(	lightOnFlg == 1)//�Զ�Ϩ��
						{
							lightOnCnt++;
	
							if(lightOnCnt > 30)
							{
								P28Hi();//����ֹͣ
								//P121Lo();//�ر�����
								P123Lo();//�ر�����	

								P26Hi();
								erroStatus |= 0x02;

								lightOnFlg = 0;	
								lightOnCnt = 0;
								autoCalibFlg = 0;//��ֹ�Զ�У׼��Ϩ��
							}
						}
						else
							lightOnCnt = 0;
								
						//P26Hi();
						//erroStatus |= 0x02;
	
					}
				}
				else  //��ֹδͨȼ��ʱ������� ���ٵ��ɹ�
				{
					lightOnCnt = 0;
				}

				FIO2MASK = 0xffffffff &(~keyInt);
				if((FIO2PIN & keyInt) != 0)
				{
					/*
					AdProcess(&lTemp1,4);//PPM
					ppmConcent = lTemp1*3300;
					ppmConcent /= 1024;
					*/
					
					lTemp1 = adFilter(4,5);
					ppmConcent = lTemp1*4500;
					ppmConcent /= 4096;
					


					
					if(fidSel == 0)
						ledCode = 0x77;
					else
						ledCode = 0xb7;
					hct595sendByte(ledCode&lecChar);
					//fidSel = 0;
			
					/*
					//FIO1MASK = 0xffffffff &(~P115);//��������ť
					//if((FIO1PIN & P115) != 0)
					//FIO0MASK = 0xffffffff &(~(P015|P016|P017));//������
					FIO0MASK = 0xffffffff &(~(P020|P021|P022));//������
					//if((FIO0PIN & (P015|P016|P017)) == 0x00)
					if((FIO0PIN & (P020|P021|P022)) == 0x00)
					{
						ppmRateAutoSelFlg = 1;	
						conditionStatus |= 0x0100;
						//ppmRateSel = 0x32;	
					}
					else 
					{
						ppmRateAutoSelFlg = 0;
						conditionStatus &= ~0x0100;
		
						//FIO0MASK = 0xffffffff &(~(P015|P016|P017));//������
						FIO0MASK = 0xffffffff &(~(P020|P021|P022));//������
						//if((FIO0PIN & (P015|P016|P017)) == P016)
						if((FIO0PIN & (P020|P021|P022)) == P021)
						{
							ppmRateSel = 0x31;		
						}
						//else if((FIO0PIN & (P015|P016|P017)) == (P015|P016))
						else if((FIO0PIN & (P020|P021|P022)) == (P020|P021))
						{
							ppmRateSel = 0x32;		
						}
					   	//else if((FIO0PIN & (P015|P016|P017)) == (P015|P017))
						else if((FIO0PIN & (P020|P021|P022)) == (P020|P022))
						{
							ppmRateSel = 0x33;		
						}
						//else if((FIO0PIN & (P015|P016|P017)) == (P016|P017))
						else if((FIO0PIN & (P020|P021|P022)) == (P021|P022))
						{
							ppmRateSel = 0x34;		
						}
						//else if((FIO0PIN & (P015|P016|P017)) == (P015|P016|P017))
						else if((FIO0PIN & (P020|P021|P022)) == (P020|P021|P022))
						{
							ppmRateSel = 0x35;		
						}
						P18Lo();//���̿���
						P19Lo();
						P110Lo();
						P114Lo();
						P115Lo();
						P116Lo();
					}
					*/
					FIO0MASK = 0xffffffff &(~(P020|P021|P022));//������
					if((FIO0PIN & (P020|P021|P022)) == P021)
					{
						ppmRateAutoSelFlg = 1;	
						conditionStatus |= 0x0100;
					}
					else 
					{
						ppmRateAutoSelFlg = 0;
						conditionStatus &= ~0x0100;
		
						FIO0MASK = 0xffffffff &(~(P020|P021|P022));//������
						if((FIO0PIN & (P020|P021|P022)) == P022)
						{
							ppmRateSel = 0x31;		
						}
						else if((FIO0PIN & (P020|P021|P022)) == (P020|P021))
						{
							ppmRateSel = 0x32;		
						}
						else if((FIO0PIN & (P020|P021|P022)) == (P020|P022))
						{
							ppmRateSel = 0x33;		
						}
						else if((FIO0PIN & (P020|P021|P022)) == (P021|P022))
						{
							ppmRateSel = 0x34;		
						}
						else if((FIO0PIN & (P020|P021|P022)) == (P020|P021|P022))
						{
							ppmRateSel = 0x35;		
						}
						P18Lo();//���̿���
						P19Lo();
						P110Lo();
						P114Lo();
						P115Lo();
						P116Lo();
					}

					if((erroStatus&0x06) == 0)
					{
						if((ppmRateAutoSelFlg == 1)&&(sorftCtrRatFlg == 0))//�����Զ�����,����ѡ�����̹ر�
						{
			
							switch(ppmRateSel)
							{
								
								case 0x31:
								P18Hi();//���̿���
								P19Lo();
								P110Lo();
								P114Lo();
								P116Lo();
		
								for(i = 0 ;i < 500; i++)
								for(j = 0 ;j < 100; j++);
								/*
								AdProcess(&lTemp,4);//PPM
								ppmConcent = lTemp*3300;
								ppmConcent /= 1024;
							    */
								
								lTemp = adFilter(4,5);
								ppmConcent = lTemp*4500;
								ppmConcent /= 4096;

								if(fidSel == 0)//��������
								{
									//if(ppmConcent > measurVoltageRefBuf[0][1])
									fTemp = ppmConcent -measurVoltageRefBuf[0][0];	
									fTemp *= measurConcentRefBuf[0];
									fTemp /= (measurVoltageRefBuf[0][1]- measurVoltageRefBuf[0][0]);
								}
								else
								{
									//if(ppmConcent > measurVoltageRefBuf[0][1])
									fTemp = ppmConcent -measurMethaneVoltageRefBuf[0][0];	
									fTemp *= measurMethaneConcentRefBuf[0];
									fTemp /= (measurMethaneVoltageRefBuf[0][1]- measurMethaneVoltageRefBuf[0][0]);
								}
								//if(ppmConcent > 28.0)	
								if(fTemp > 9.0)
								{
									P18Lo();
									ppmRateSel = 0x32;
								}
							   	break;
	
								case 0x32:
								P18Lo();//���̿���
								P19Lo();
								P110Hi();
								P114Lo();
								P116Lo();
		
								for(i = 0 ;i < 500; i++)
								for(j = 0 ;j < 100; j++);
		
								//AdProcess(&lTemp,4);//PPM
								//ppmConcent = lTemp*3300;
								//ppmConcent /= 1024;
								lTemp = adFilter(4,5);
								ppmConcent = lTemp*4500;
								ppmConcent /= 4096;
								
								if(fidSel == 0)//��������
								{
									//if(ppmConcent > measurVoltageRefBuf[1][1])
									fTemp = ppmConcent -measurVoltageRefBuf[1][0];	
									fTemp *= measurConcentRefBuf[1];
									fTemp /= (measurVoltageRefBuf[1][1]- measurVoltageRefBuf[1][0]);
								}
								else
								{
									//if(ppmConcent > measurVoltageRefBuf[1][1])
									fTemp = ppmConcent -measurMethaneVoltageRefBuf[1][0];	
									fTemp *= measurMethaneConcentRefBuf[1];
									fTemp /= (measurMethaneVoltageRefBuf[1][1]- measurMethaneVoltageRefBuf[1][0]);
								}
								//if(fTemp > 98.0)
								if(fTemp > 90.0)	
								{
									P110Lo();
									ppmRateSel = 0x33;
								}
								//else if(ppmConcent < (measurVoltageRefBuf[1][0])+(measurVoltageRefBuf[1][1]-measurVoltageRefBuf[1][0])*0.1)
								//else if(ppmConcent < 25.0)
								else if(fTemp < 8.0)
								{
									P110Lo();
									ppmRateSel = 0x31;	
								}
								break;
	
								case 0x33:
								P18Lo();//���̿���
								P19Lo();
								P110Lo();
								P114Lo();
								P116Hi();
		
								for(i = 0 ;i < 500; i++)
								for(j = 0 ;j < 100; j++);
		
								//AdProcess(&lTemp,4);//PPM
								//ppmConcent = lTemp*3300;
								//ppmConcent /= 1024;
								lTemp = adFilter(4,5);
								ppmConcent = lTemp*4500;
								ppmConcent /= 4096;

								if(fidSel == 0)//��������
								{
									//if(ppmConcent > measurVoltageRefBuf[2][1])
									fTemp = ppmConcent -measurVoltageRefBuf[2][0];	
									fTemp *= measurConcentRefBuf[2];
									fTemp /= (measurVoltageRefBuf[2][1]- measurVoltageRefBuf[2][0]);
								}
								else
								{
									//if(ppmConcent > measurVoltageRefBuf[2][1])
									fTemp = ppmConcent -measurMethaneVoltageRefBuf[2][0];	
									fTemp *= measurMethaneConcentRefBuf[2];
									fTemp /= (measurMethaneVoltageRefBuf[2][1]- measurMethaneVoltageRefBuf[2][0]);
								}
								//if(fTemp > 280.0)
								if(fTemp > 900.0)
								{
									P116Lo();
									ppmRateSel = 0x34;
								}
								//else if(ppmConcent < (measurVoltageRefBuf[2][0])+(measurVoltageRefBuf[2][1]-measurVoltageRefBuf[2][0])*0.1)
								//else if(ppmConcent < 95.0)
								else if(fTemp < 80.0)
								{
									P116Lo();
									ppmRateSel = 0x32;	
								}	
								break;
		
								case 0x34:
								P18Lo();//���̿���
								P19Lo();
								P110Lo();
								P114Hi();
								P116Lo();
		
								for(i = 0 ;i < 500; i++)
								for(j = 0 ;j < 100; j++);
		
								//AdProcess(&lTemp,4);//PPM
								//ppmConcent = lTemp*3300;
								//ppmConcent /= 1024;
								lTemp = adFilter(4,5);
								ppmConcent = lTemp*4500;
								ppmConcent /= 4096;

								if(fidSel == 0)//��������
								{
									//if(ppmConcent > measurVoltageRefBuf[3][1])
									fTemp = ppmConcent -measurVoltageRefBuf[3][0];	
									fTemp *= measurConcentRefBuf[3];
									fTemp /= (measurVoltageRefBuf[3][1]- measurVoltageRefBuf[3][0]);
								}
								else
								{
									//if(ppmConcent > measurVoltageRefBuf[3][1])
									fTemp = ppmConcent -measurMethaneVoltageRefBuf[3][0];	
									fTemp *= measurMethaneConcentRefBuf[3];
									fTemp /= (measurMethaneVoltageRefBuf[3][1]- measurMethaneVoltageRefBuf[3][0]);
								}
								//if(ppmConcent > 980.0)
								if(fTemp > 9000.0)
								{
									P114Lo();
									ppmRateSel = 0x35;
								}
								//else if(ppmConcent < (measurVoltageRefBuf[3][0])+(measurVoltageRefBuf[3][1]-measurVoltageRefBuf[3][0])*0.1)
								//else if(ppmConcent < 250.0)
								else if(fTemp < 800.0)
								{
									P114Lo();
									ppmRateSel = 0x33;	
								}
								break;
		
								case 0x35:
								P18Lo();//���̿���
								P19Hi();
								P110Lo();
								P114Lo();
								P116Lo();
		
								for(i = 0 ;i < 500; i++)
								for(j = 0 ;j < 100; j++);
		
								//AdProcess(&lTemp,4);//PPM
								//ppmConcent = lTemp*3300;
								//ppmConcent /= 1024;
								lTemp = adFilter(4,5);
								ppmConcent = lTemp*4500;
								ppmConcent /= 4096;

							   	if(fidSel == 0)//��������
								{
									//if(ppmConcent < (measurVoltageRefBuf[4][0])+(measurVoltageRefBuf[4][1]-measurVoltageRefBuf[4][0])*0.1)
									fTemp = ppmConcent -measurVoltageRefBuf[4][0];	
									fTemp *= measurConcentRefBuf[4];
									fTemp /= (measurVoltageRefBuf[4][1]- measurVoltageRefBuf[4][0]);
								}
								else
								{
									//if(ppmConcent < (measurVoltageRefBuf[4][0])+(measurVoltageRefBuf[4][1]-measurVoltageRefBuf[4][0])*0.1)
									fTemp = ppmConcent -measurMethaneVoltageRefBuf[4][0];	
									fTemp *= measurMethaneConcentRefBuf[4];
									fTemp /= (measurMethaneVoltageRefBuf[4][1]- measurMethaneVoltageRefBuf[4][0]);
								}
								//if(fTemp < 950.0)
								if(fTemp < 8000.0)
								{
									P19Lo();
									ppmRateSel = 0x34;	
								}
								break;
		
								default:
								break;
							}
							
						}
					}
					if(pintfItemSel == 3)
					{
						memset(ladVluBuf,'\0',10);
						sprintf(ladVluBuf,"%.1f",ppmConcent);
						U0THR = 0x0d;
						while((U0LSR & 0x20) == 0);
						U0THR = 0x0a;
						while((U0LSR & 0x20) == 0);
						for(i = 0; i < strlen(ladVluBuf);i++)
						{
							U0THR = ladVluBuf[i];
							while((U0LSR & 0x20) == 0);
						}
					}

					if(autoCalibMeasurFlg == 1)	//�Զ�У׼�����в���
					{
						autoCalibMeasurFlg = 0;

						if(autoCalibOrder == 0)
						{
							int16Temp =  measurVoltageRefBuf[ppmRateSel-0x31][0] - ppmConcent;
							if((int16Temp >500)||(int16Temp < -500))//����Ĭ�����ò������ޣ��򱨴�
							{
								erroStatus |= 0x10;

								cTemp1[0] =0x9e;
								I2C_WriteNByte(0x70, 1, 0x15,cTemp1,1);//��ʾE
							}
							else
							{
								measurVoltageRefBuf[ppmRateSel-0x31][0] =  ppmConcent;
								erroStatus &= ~0x10;
							}
						}	
						else if(autoCalibOrder == 1)
						{
							if(ppmConcent <= measurVoltageRefBuf[ppmRateSel-0x31][0])	//���С�ڵ�����㣬�򱨴�
							{
								erroStatus |= 0x20;	
								cTemp1[0] =0x9e;
								I2C_WriteNByte(0x70, 1, 0x15,cTemp1,1);//��ʾE
							}
							else if( ppmConcent > 3100)//�ӽ������̣��򱨴�
							{
								erroStatus |= 0x40;
								cTemp1[0] =0x9e;
								I2C_WriteNByte(0x70, 1, 0x15,cTemp1,1);//��ʾE
							}
							else
							{
								int16Temp =  measurVoltageRefBuf[ppmRateSel-0x31][1] - ppmConcent;
								if((int16Temp >1000)||(int16Temp < -1000))//����Ĭ�����ò������ޣ��򱨴�
								{
								 	erroStatus |= 0x20;
									cTemp1[0] =0x9e;
									I2C_WriteNByte(0x70, 1, 0x15,cTemp1,1);//��ʾE
								}
								else
								{
									measurVoltageRefBuf[ppmRateSel-0x31][1] = ppmConcent;	
									erroStatus &= ~0x60;
								}
							}
						}
						else
						{
							int16Temp = measurVoltageRefBuf[ppmRateSel-0x31][0] - ppmConcent;
							if((int16Temp < 50)||(int16Temp > -50))//���α궨���������ڷ�Χ��
							{
								erroStatus &= ~0x10;
								autoCalibFlg = 0;//�ɹ��궨���������±궨����

								iTemp = (ppmRateSel-0x31)*20;
								I2C_WriteNByte(0xa0, 2, iTemp,(uint8 *)&measurVoltageRefBuf[ppmRateSel-0x31][0],4);
								 	
							}
							else
							{
								erroStatus |= 0x10;

								cTemp1[0] =0x9e;
								I2C_WriteNByte(0x70, 1, 0x15,cTemp1,1);//��ʾE

								autoCalibFlg = 0;//�궨�������Զ�����
							}
						}
						if(autoCalibFlg == 0) //�궨�������л�������
						{
							P23Hi();
							P24Hi();
							P25Lo();
							
							conditionStatus &= 0x78;//�л���ŷ�ͬʱ�˳�standby״̬
							conditionStatus |= 0x04;

							cTemp1[0] =0;
							I2C_WriteNByte(0x70, 1, 0x15,cTemp1,1);//��ʾE
						}
					}
					switch(ppmRateSel)
					{
						case 0x31:
						/*
						ppmConcent -= 525;
						ppmConcent *= 79.7;
						ppmConcent /= 2000;
						*/
						/*
					    ppmConcent -= 535;
						ppmConcent *= 79.7;
						ppmConcent /= 973;
						*/

						/*
						ppmConcent -= 535;	   //((V-V0)/10)*K=PPM
						ppmConcent *= 79.7;
						ppmConcent /= 675;
						*/
						/*
						ppmConcent -= 535;
						ppmConcent /= 10.0;
						ppmConcent *= 1.205;
						*/
						/*
						ppmConcent -= 551;	 //12-20
						ppmConcent *= 90.0;
						ppmConcent /= 1746.0;
						*/
						/*
						ppmConcent -= 551;	 //12-20
						ppmConcent *= 79.7;
						ppmConcent /= 799.0;
						*/
						/*
						ppmConcent -= 554;	 //01-04
						ppmConcent *= 80.0;
						ppmConcent /= 921.0;
						*/

						/*
					    ppmConcent -= 634;	 //01-08
						ppmConcent *= 79.7;
						ppmConcent /= 2069.0;
						*/
						
						if(fidSel == 0)//��������
						{
							ppmConcent -= measurVoltageRefBuf[0][0];	 //01-17
							//ppmConcent *= 90.0;
							ppmConcent *= measurConcentRefBuf[0];
							ppmConcent /= (measurVoltageRefBuf[0][1]- measurVoltageRefBuf[0][0]);
						}
						else
						{
							ppmConcent -= measurMethaneVoltageRefBuf[0][0];	 //01-17
							//ppmConcent *= 90.0;
							ppmConcent *= measurMethaneConcentRefBuf[0];
							ppmConcent /= (measurMethaneVoltageRefBuf[0][1]- measurMethaneVoltageRefBuf[0][0]);	
						}
						break;
						case 0x32: 
						
						/*
						ppmConcent -= 500;
						ppmConcent *= 8.2;
						ppmConcent /= 300;
					    */

						/*
						ppmConcent -= 506;	   //((V-V0)/30)*K=PPM
						ppmConcent *= 8.2;
						ppmConcent /= 200;
						*/

						/*
						ppmConcent -= 506;
						ppmConcent /= 30.0;
						ppmConcent *= 1.205;
						*/
						/*
						ppmConcent -= 506;
						ppmConcent *= 20.0;
						ppmConcent /= 1694.0;
						*/
						/*
						ppmConcent -= 506;
						ppmConcent *= 9.0;
						ppmConcent /= 692.0;
						*/
						/*
						ppmConcent -= 518;
						ppmConcent *= 9.0;
						ppmConcent /= 1604.0;
						*/
						/*
						ppmConcent -= 551; //12-20
						ppmConcent *= 9.0;
						ppmConcent /= 1634.0;
						*/
						/*
					    ppmConcent -= 551;	 //12-21
						ppmConcent *= 20.0;
						ppmConcent /= 1901.0;
						*/
						/*
						ppmConcent -= 551;	 //12-21-1
						ppmConcent *= 9.0;
						ppmConcent /= 799.0;
						*/
						/*
						ppmConcent -= 551;	 //01-03
						ppmConcent *= 7.9;
						ppmConcent /= 782.0;
						*/
						/*
					    ppmConcent -= 551;	 //01-04
						ppmConcent *= 9.0;
						ppmConcent /= 892.0;
						*/
						/*/
						ppmConcent -= 551;	 //01-04
						ppmConcent *= 9.0;
						ppmConcent /= 1066.0;
						*/
						/*
						ppmConcent -= 244;	 //01-08
						ppmConcent *= 10.0;
						ppmConcent /= 2556.0;
						*/
						if(fidSel == 0)//��������
						{
							ppmConcent -= measurVoltageRefBuf[1][0];	 //01-17
							//ppmConcent *= 9.0;
							//ppmConcent *= 13.5;
							ppmConcent *= measurConcentRefBuf[1];
							ppmConcent /= (measurVoltageRefBuf[1][1]- measurVoltageRefBuf[1][0]);
						}
						else
						{
							ppmConcent -= measurMethaneVoltageRefBuf[1][0];	 //01-17
							//ppmConcent *= 9.0;
							//ppmConcent *= 13.5;
							ppmConcent *= measurMethaneConcentRefBuf[1];
							ppmConcent /= (measurMethaneVoltageRefBuf[1][1]- measurMethaneVoltageRefBuf[1][0]);	
						}
						break;
						case 0x33:	//û�ã�������Ϊ300ppm
						/*
						ppmConcent -= 547;
						ppmConcent *= 797;
						ppmConcent /= 1676;
						*/
						/*
						ppmConcent -= 547;	   //((V-V0)/3)*K=PPM
						ppmConcent *= 797;
						ppmConcent /= 1983;
						*/ 

						/*
						ppmConcent -= 547;	
						ppmConcent /= 3.0;
						ppmConcent *= 1.205;
						*/
						/*
						ppmConcent -= 551;	 //12-21-1
						ppmConcent *= 797.0;
						ppmConcent /= 2333.0;
						*/
						/*
						ppmConcent -= 551;	 //12-21-2
						ppmConcent *= 300.0;
						ppmConcent /= 921.0;
						*/
						/*
					    ppmConcent -= 554;	 //01-04
						ppmConcent *= 240.0;
						ppmConcent /= 841.0;
						*/
						/*
						ppmConcent -= 631;	 //01-08
						ppmConcent *= 900.0;
						ppmConcent /= 2237.0;	 
						*/
						if(fidSel == 0)//��������
						{
							ppmConcent -= measurVoltageRefBuf[2][0];	 //01-17
							//ppmConcent *= 900.0;
							ppmConcent *= measurConcentRefBuf[2];
							ppmConcent /= (measurVoltageRefBuf[2][1]- measurVoltageRefBuf[2][0]);
						}
						else
						{
							ppmConcent -= measurMethaneVoltageRefBuf[2][0];	 //01-17
							//ppmConcent *= 900.0;
							ppmConcent *= measurMethaneConcentRefBuf[2];
							ppmConcent /= (measurMethaneVoltageRefBuf[2][1]- measurMethaneVoltageRefBuf[2][0]);
						}
						break;
						
						case 0x34:
						/*
						ppmConcent -= 547;
						ppmConcent *= 2990;
						ppmConcent /= 2060;
					    */					  
						/*
						ppmConcent -= 547;	  //((V-V0)/1)*K=PPM
						ppmConcent *= 1.205;
					    */
						/*
						ppmConcent -= 551;	  //12-20
						ppmConcent *= 900;
						ppmConcent /= 1666.0;
						*/
						/*
						ppmConcent -= 551;	  //12-20-2
						ppmConcent *= 797;
						ppmConcent /= 809.0;
					    */
						/*
						ppmConcent -= 551;	  //01-04
						ppmConcent *= 800;
						ppmConcent /= 902.0;
						*/
						/*
						ppmConcent -= 634;	  //01-08
						ppmConcent *= 9030;
						ppmConcent /= 2380.0;
						*/
						if(fidSel == 0)//��������
						{
							ppmConcent -= measurVoltageRefBuf[3][0];	 //01-17
							//ppmConcent *= 9000.0;
							ppmConcent *= measurConcentRefBuf[3];
							ppmConcent /= (measurVoltageRefBuf[3][1]- measurVoltageRefBuf[3][0]);
						}
						else
						{
							ppmConcent -= measurMethaneVoltageRefBuf[3][0];	 //01-17
							//ppmConcent *= 9000.0;
							ppmConcent *= measurMethaneConcentRefBuf[3];
							ppmConcent /= (measurMethaneVoltageRefBuf[3][1]- measurMethaneVoltageRefBuf[3][0]);
						}
						break;
						case 0x35:
						/*
						ppmConcent -= 547;	 //((V-V0)/0.1)*K=PPM
						ppmConcent /= 0.1;
						ppmConcent *= 1.205;  
						*/
						/*
						ppmConcent -= 538;	  //12-22
						ppmConcent *= 2990;
						ppmConcent /= 876.0;
						*/
						/*
						ppmConcent -= 538;	  //01-04
						ppmConcent *= 7000;
						ppmConcent /= 2384.0;
						*/
						if(fidSel == 0)//��������
						{
							ppmConcent -= measurVoltageRefBuf[4][0];	 //01-17
							//ppmConcent *= 90000.0;
							ppmConcent *= measurConcentRefBuf[4];
							ppmConcent /= (measurVoltageRefBuf[4][1]- measurVoltageRefBuf[4][0]);
						}
						else
						{
							ppmConcent -= measurMethaneVoltageRefBuf[4][0];	 //01-17
							//ppmConcent *= 90000.0;
							ppmConcent *= measurMethaneConcentRefBuf[4];
							ppmConcent /= (measurMethaneVoltageRefBuf[4][1]- measurMethaneVoltageRefBuf[4][0]);
						}
						break;
						
						default:
						break;
					}
					

					
					if(pintfItemSel == 4)
					{
						memset(ladVluBuf,'\0',10);
						sprintf(ladVluBuf,"%.1f",ppmConcent);
						U0THR = 0x0d;
						while((U0LSR & 0x20) == 0);
						U0THR = 0x0a;
						while((U0LSR & 0x20) == 0);
						for(i = 0; i < strlen(ladVluBuf);i++)
						{
							U0THR = ladVluBuf[i];
							while((U0LSR & 0x20) == 0);
						}
						
					}
					ppmConcentBuf[0] = ppmConcent;
					ppmConcentBuf[1] = ppmConcentBuf[0];
					ppmConcentBuf[2] = ppmConcentBuf[1];
					ppmConcent = (ppmConcentBuf[0]+ppmConcentBuf[1]+ppmConcentBuf[2])/3;
					 
					if(fidSel == 0)//��������
					{
						totalHydroCarbonVlv = ppmConcent;	
					}
					else
					{
						/*
						if(comSendEnableFlg == 1)
						{
							if(nonMethaneTotalHydroCarbonFlg == 0)
							{
								totalHydroCarbonVlv = ppmConcent;	
							}
							else
							{
								methaneVlv = ppmConcent; 	
							}
						}
						*/

						methaneVlv = ppmConcent;

					}
				    if((erroStatus&0x06) == 0)
					{
						memset(adVluBuf,'\0',10);
						if(ppmRateSel == 0x31)
							sprintf(adVluBuf,"%.2f",ppmConcent);
						else if(ppmRateSel == 0x32)
							sprintf(adVluBuf,"%.1f",ppmConcent);
						else
						{
						   lTemp = ppmConcent;
							sprintf(adVluBuf,"%d",lTemp);
						}

			
						for(i = 0;i < 6; i++)
						{
							cTemp1[i] = 0x00;
						}
						if(adVluBuf[0] == '-')
						{
						 	cTemp1[0] = digiterBuf[10];
							i = 1;
							j = 1;
						}
						else
						{
							i = 1;
							j = 0;
						}
						do{
							if(adVluBuf[j] >= 0x30)
							{
								cTemp =  adVluBuf[j]- 0x30;
								cTemp1[i] = digiterBuf[cTemp]; 
							}
							else if	(adVluBuf[j] == '.')
							{
								i--;
								cTemp1[i] |= 0x01;
							}
							
							i++;
							j++;
						}while(	adVluBuf[j] != '\0');
						I2C_WriteNByte(0x70, 1, 0x10,cTemp1,5);//��ʾ
					}
					
					else
					{
						memset(ladVluBuf,'\0',10);
						if((erroStatus&0x02) == 0x02)//Ϩ��
						{
							ladVluBuf[1] = 0x32;
						}
						if((erroStatus&0x04) == 0x04)//�¶�δ��
						{
							ladVluBuf[2] = 0x20;
						    ladVluBuf[3] = 0x33;
						}
						else
						{
							ladVluBuf[2] = 0x20;
						    ladVluBuf[3] = 0x31;	
						}
						for(i = 0;i < 6; i++)
						{
							cTemp1[i] = 0x00;
						}
						if(ladVluBuf[0] == '-')
						{
						 	cTemp1[0] = digiterBuf[10];
							i = 1;
							j = 1;
						}
						else
						{
							i = 1;
							j = 0;
						}
						do{
							if(ladVluBuf[j] >= 0x30)
							{
								cTemp =  ladVluBuf[j]- 0x30;
								cTemp1[i] = digiterBuf[cTemp]; 
							}
							else if	(ladVluBuf[j] == '.')
							{
								i--;
								cTemp1[i] |= 0x01;
							}
							
							i++;
							j++;
						}while(	ladVluBuf[j] != '\0');
						I2C_WriteNByte(0x70, 1, 0x10,cTemp1,5);//��ʾ	
					}

					if((udiskFlg == 1)&&(udiskMountFlg == 1))	//д�����¶����ݵ�U���ļ�
					{
						memset(cachBuf,'\0',1024);

						if((erroStatus&0x06) == 0)
						{
							if(fidSel == 0)
							{
								s = sprintf( cachBuf, "ChamberTemp= %.1f;CatalyticTemp= %.1f;TotalHydrocarbonVlu= %1.f;Date��%d-%d-%d %d:%d:%d\r\n", TempDetctNow,TempCatalyticNow,totalHydroCarbonVlv,RTC_YEAR,RTC_MONTH,RTC_DOM,RTC_HOUR,RTC_MIN,RTC_SEC);
							}
							else
							{
								s = sprintf( cachBuf, "ChamberTemp= %.1f;CatalyticTemp= %.1f;MethaneVlu= %1.f;Date��%d-%d-%d %d:%d:%d\r\n", TempDetctNow,TempCatalyticNow,methaneVlv,RTC_YEAR,RTC_MONTH,RTC_DOM,RTC_HOUR,RTC_MIN,RTC_SEC);
							}
						}
						else
						{
							s = sprintf( cachBuf, "ChamberTemp= %.1f;CatalyticTemp= %.1f;AlarmCode= %c %c;Date��%d-%d-%d %d:%d:%d\r\n", TempDetctNow,TempCatalyticNow,ladVluBuf[1],ladVluBuf[3],RTC_YEAR,RTC_MONTH,RTC_DOM,RTC_HOUR,RTC_MIN,RTC_SEC);
							
						}
						strcat(uDiskBuf,cachBuf);

						if(strlen(uDiskBuf) >  1024)
						{
							FileDataBuf[0]='/';
							FileDataBuf[1]='F'; 
							FileDataBuf[2]='I'; 
							FileDataBuf[3]='D';
							FileDataBuf[4]=RTC_DOM/10+0x30; 
							FileDataBuf[5]=RTC_DOM%10+0x30;
							FileDataBuf[6]=RTC_HOUR/10+0x30;
							FileDataBuf[7]=RTC_HOUR%10+0x30; //ÿСʱ�½�һ�洢�ļ�
							FileDataBuf[8]='.'; 
							FileDataBuf[9]='T'; 
							FileDataBuf[10]='X'; 
							FileDataBuf[11]='T'; 
							FileDataBuf[12]=0;
		
							s = CH376FileOpen(FileDataBuf); 
							if( s == USB_INT_SUCCESS )
							{
								creatFlg = 0;
								fileOpenFlg = 1;
							} 
							else
							{
								s = CH376FileCreate( NULL );
								if ( s != USB_INT_SUCCESS )
								{	 
									creatFlg = 0;
								  	fileOpenFlg = 0;
								}
								else
								{
									creatFlg = 1;
									fileOpenFlg = 1;
								}
							}
							if(fileOpenFlg == 1)
							{
								s = CH376ByteLocate( 0xFFFFFFFF );	
		
								alarmLedOn();
								for(j = 0;j < 500; j++)
								for(i = 0;i < 1000; i++);
								alarmLedOff();
								
								s = CH376ByteWrite( uDiskBuf, strlen(uDiskBuf), NULL );
								s = CH376FileClose( TRUE );	
								
								if(creatFlg == 1)
								{
									creatFlg = 0;
									s = SetFileCreateTime( FileDataBuf, MAKE_FILE_DATE( RTC_YEAR, RTC_MONTH, RTC_DOM ), MAKE_FILE_TIME( RTC_HOUR, RTC_MIN, RTC_SEC ) );  // Ϊָ���ļ����ô������ں�ʱ�� 
								}
								else
									s= SetFileWrtTime( FileDataBuf, MAKE_FILE_DATE( RTC_YEAR, RTC_MONTH, RTC_DOM ), MAKE_FILE_TIME( RTC_HOUR, RTC_MIN, RTC_SEC ) );
								
							}

							memset(uDiskBuf,'\0',2048);
						}
						/*
						FileDataBuf[0]='/';
						FileDataBuf[1]='F'; 
						FileDataBuf[2]='I'; 
						FileDataBuf[3]='D';
						FileDataBuf[4]=(RTC_YEAR%100)/10+0x30; 
						FileDataBuf[5]=(RTC_YEAR%100)%10+0x30;
						FileDataBuf[6]=RTC_MONTH/10+0x30;
						FileDataBuf[7]=RTC_MONTH%10+0x30;
					    FileDataBuf[8]=RTC_DOM/10+0x30; 
						FileDataBuf[9]=RTC_DOM%10+0x30;
						FileDataBuf[10]=RTC_HOUR/10+0x30;
						FileDataBuf[11]=RTC_HOUR%10+0x30;
	
	
						FileDataBuf[12]='.'; 
						FileDataBuf[13]='T'; 
						FileDataBuf[14]='X'; 
						FileDataBuf[15]='T'; 
						FileDataBuf[16]=0;
	
						s = CH376FileOpen(FileDataBuf); 
						if( s != USB_INT_SUCCESS )
						{
							creatFlg = 1;
							CH376FileCreate( NULL );
						}
						s = CH376ByteLocate( 0xFFFFFFFF );	

						alarmLedOn();
						for(j = 0;j < 500; j++)
						for(i = 0;i < 1000; i++);
						alarmLedOff();


						if((erroStatus&0x06) == 0)
						{
							if(fidSel == 0)
							{
								s = sprintf( buf, "ChamberTemp= %.1f;CatalyticTemp= %.1f;TotalHydrocarbonVlu= %1.f;Date��%d-%d-%d %d:d%:%d\xd\xa", TempDetctNow,TempCatalyticNow,totalHydroCarbonVlv,RTC_YEAR,RTC_MONTH,RTC_DOM,RTC_HOUR,RTC_MIN,RTC_SEC);
							}
							else
							{
								s = sprintf( buf, "ChamberTemp= %.1f;CatalyticTemp= %.1f;MethaneVlu= %1.f;Date��%d-%d-%d %d:d%:%d\xd\xa", TempDetctNow,TempCatalyticNow,methaneVlv,RTC_YEAR,RTC_MONTH,RTC_DOM,RTC_HOUR,RTC_MIN,RTC_SEC);
							}
						}
						else
						{
							s = sprintf( buf, "ChamberTemp= %.1f;CatalyticTemp= %.1f;AlarmCode= %d %d;Date��%d-%d-%d %d:d%:%d\xd\xa", TempDetctNow,TempCatalyticNow,ladVluBuf[1],ladVluBuf[3],RTC_YEAR,RTC_MONTH,RTC_DOM,RTC_HOUR,RTC_MIN,RTC_SEC);
							
						}
						s = CH376ByteWrite( buf, s, NULL );
						s = CH376FileClose( TRUE );
						
						if(creatFlg == 1)
						{
							creatFlg = 0;
							s = SetFileCreateTime( FileDataBuf, MAKE_FILE_DATE( RTC_YEAR, RTC_MONTH, RTC_DOM ), MAKE_FILE_TIME( RTC_HOUR, RTC_MIN, RTC_SEC ) );  // Ϊָ���ļ����ô������ں�ʱ�� 
						}
						else
							s= SetFileWrtTime( FileDataBuf, MAKE_FILE_DATE( RTC_YEAR, RTC_MONTH, RTC_DOM ), MAKE_FILE_TIME( RTC_HOUR, RTC_MIN, RTC_SEC ) );
						*/
	
					}
				/////////////////////////////////////////////////////////	 
				}
			}

			/*
			if(tempDelayCnt > 100) //2s����һ���¶�
			{
				tempCtrFlg = 1;
				tempDelayCnt = 0;	
			}
			else
				tempDelayCnt++;
			*/

		//}
		
		if(tempCtrFlg == 1)
		{	
			tempCtrFlg = 0;

			if(pidTempDetctFlg == 0)
			{
				if(TempDetctNow >= TempDetctSet)
				{
					pidTempDetctFlg = 1;
				}
				TempDetctDuty = 0;
			}
			else
			{
				TempDetctDuty = TempDetctPid();
				TempDetctBef = 	TempDetctNow;
			}
			if(pidTempCatalyticFlg == 0)
			{
				if(TempCatalyticNow >= TempCatalyticSet)
				{
					pidTempCatalyticFlg = 1;
				}
				TempCatalyticDuty = 0;
			}
			else
			{
				TempCatalyticDuty = TempCatalyticPid();	
			 	TempCatalyticBef = TempCatalyticNow;
			}

			/*
			memset(ladVluBuf,'\0',10);
			sprintf(ladVluBuf,"%d",TempCatalyticDuty);
			for(i = 0; i < strlen(ladVluBuf);i++)
			{
				U0THR = ladVluBuf[i];
				while((U0LSR & 0x20) == 0);
			}
			memset(ladVluBuf,'\0',10);
			sprintf(ladVluBuf,"%d",TempDetctDuty);
			for(i = 0; i < strlen(ladVluBuf);i++)
			{
				U0THR = ladVluBuf[i];
				while((U0LSR & 0x20) == 0);
			}
			*/


			PWM1TCR = 0x02; // ��λPWMTC
			PWM1MR0 = 2*Fpclk;//1���������
			PWM1MR2 = PWM1MR0* TempCatalyticDuty/100; //ռ�ձ�
			PWM1MR3 = PWM1MR0* TempDetctDuty/100; //ռ�ձ�
			PWM1LER = 0x0d; // PWM0��PWM2,PWM3ƥ������
			PWM1TCR = 0x09; // ����PWM��
			}
		

		
		if(Uart0.RecvOver == 1)//ͨѶ��Ӧ
		{
			ComCmdPocess();

			Uart0.RecvOver = 0;
			Uart0.RecvStart = 0;
			Uart0.RecvPoint = 0;
			Uart0.RecvCount = 0;
			Uart0.PacketLength = 0;	
			U0IER = 0x05;
		}
		
		if(autoCalibFlg == 0)//У׼ʱ����Ӧ����
		{
			FIO2MASK = 0xffffffff &(~keyInt);
			if((FIO2PIN & keyInt) == 0)
			{
				for(i = 0;i < 50; i++)
				for(j = 0;j < 10; j++);
	
				FIO2MASK = 0xffffffff &(~keyInt);
				if((FIO2PIN & keyInt) == 0)
				{
					I2C_ReadNByte(0x70, 1, 0x01, keyVlu,3);
					ckeyBuf[0]= keyVlu[0];
					ckeyBuf[1]= keyVlu[2];
					KeyFunProcess(ckeyBuf[0]);
				}
			}
		}		
			
		
	}
	
    return 0;
}
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/