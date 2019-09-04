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

//volatile struct Uart0Info Uart0;
struct Uart0Info Uart0;

volatile uint8 sendMeasurFlg;
volatile uint8 camDataBuf[2559];
volatile uint16 adbuf[8192];
volatile uint16 adCount;

uint8 keyVlu[4];
uint8 ckeyBuf[2];

uint8 sampSpeed = 9;//100khz
uint16 adSum = 1024;
uint8 measureFlg = 0,changeFuntionFlg = 0,selectFunNum = 0;

uint32 timCnt;
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
	EXTMODE = 0x01;//沿触发
	EXTPOLAR = 0x00;//上升沿
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
	EXTMODE = 0x02;//沿触发
	EXTPOLAR = 0x00;//下降沿
	VICIntEnable |= 1 << 15;
}


void Eint1_Exception(void)
{
	

	EXTINT = 0x02;
	VICVectAddr = 0; 
		
}
*/
//struct Uart0Info Uart0;
/*********************************************************************************************************
** Function name:			InitUart0
**
** Descriptions:			Initialize the Uart0
**
** input parameters:		波特率
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


    U0LCR = 0x80;                               /* 允许访问分频因子寄存器 */
    Fdiv = (Fpclk / 16) / bps;                  /* 设置波特率 */
	Fdiv *= 2;	//当不用PLL时，Fpclk快一倍,因为默认PLL参数设置n = 1。
	Fdiv /= 1.5;

	/*
	U0DLM = 0;	//115200
	U0DLL =	0x04;
    U0FDR = 0x85;
	*/

    U0DLM = 0;	//9600
	U0DLL =	0x47;
    U0FDR = 0xa1;

    U0LCR = 0x03;                               /* 禁止访问分频因子寄存器 */
					                            /* 且设置为8,1,n */
	
	
	//U0IER = 0x05;   
	U0IER = 0x00;   
	                         
    U0FCR = 0x87;                              

   	/*
    VICIntSelect = 0;
    VICVectAddr6 = (uint32)Uart0_Handler;
    VICVectPriority6 = 0;//0-15，0为最高优先级
    VICIntEnable |= 1 << 6;
	*/
   
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
                       		if(Uart0.RecvBuf[0] == 0x24)
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
                       else if(Uart0.RecvPoint == 3)
                       {
                       		Uart0.PacketLength =  ((uint32)Uart0.RecvBuf[1]<<8)| Uart0.RecvBuf[2];
                       } 
					   if(Uart0.RecvStart == 1)
                       {    
	                       if((Uart0.RecvCount > 3) && (Uart0.RecvCount >= (Uart0.PacketLength-1))) 
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
                       temp = U0RBR;//空读
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
                        	if(Uart0.RecvBuf[0] == 0x24)
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
                        else if(Uart0.RecvPoint == 3)
                        {
                       		Uart0.PacketLength =  ((uint32)Uart0.RecvBuf[1]<<8)| Uart0.RecvBuf[2];
                    
                        }
                      	if(Uart0.RecvStart == 1)
                       	{
			                if((Uart0.RecvCount > 3) && (Uart0.RecvCount >= (Uart0.PacketLength-1))) 
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
                       temp = U0RBR;//空读
                   }
                }
 				
                temp = U0RBR;//空读   

        
                break;
            default :
             	
                break;
        }
		
    } 

    VICVectAddr = 0;            // 通知中断控制器中断结束

	
	
	return;
	
}
/*********************************************************************************************************
** Function name:			InitUart0
**
** Descriptions:			Initialize the Uart0
**
** input parameters:		波特率
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


    U1LCR = 0x80;                               /* 允许访问分频因子寄存器 */
    Fdiv = (Fpclk / 16) / bps;                  /* 设置波特率 */
	Fdiv *= 2;	//当不用PLL时，Fpclk快一倍,因为默认PLL参数设置n = 1。
	Fdiv /= 1.5;
							
	/*
	U0DLM = 0;	//115200
	U0DLL =	0x04;
    U0FDR = 0x85;
	*/

	U1DLM = 0;	//9600
	U1DLL =	0x47;
    U1FDR = 0xa1;
 

    U1LCR = 0x03;                               /* 禁止访问分频因子寄存器 */
					                            /* 且设置为8,1,n */

	//U1IER = 0x05; 
	U1IER = 0x00;                              
    U1FCR = 0x87;                              

   	/*
    VICIntSelect = 0;
    VICVectAddr7 = (uint32)Uart1_Handler;
    VICVectPriority7 = 0;//0-15，0为最高优先级
    VICIntEnable |= 1 << 7;
	*/
   
    return TRUE;
}

/*********************************************************************************************************
** Function name:			RtcInit
**
** Descriptions:			RTC 初始化
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
    /*
    RTC_CCR &= ~0x01;	
	RTC_YEAR = 2008;
	RTC_MONTH = 10;
	RTC_DOW = 5;
	RTC_DOM = 31;
	RTC_HOUR = 15;
	RTC_MIN = 30;
	RTC_SEC = 0;  	
  	*/
   	RTC_CCR = 0x10;	//选择外部时钟
    RTC_CCR |= 0x02;//复位
    for(i = 0; i < 1000; i++);
    RTC_CCR &= ~0x02;
    RTC_CCR |= 0x01;//启动RTC
    
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
	uint32 lTemp = 2*Fpclk;//当不用PLL时，Fpclk快一倍,因为默认PLL参数设置n = 1。

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
	//VICIntEnable |= 1 << 4;
	
    //T0TCR = 0x03;
    //T0TCR = 0x01; 

	VICIntEnClr |= 1 << 4;
	T0TC = 0;
	T0TCR = 0x00; 
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

   	
   	T0IR = 0x01;
    VICVectAddr = 0;            //interrupt close 通知中断控制器中断结束
    
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
	uint32 lTemp = 2*Fpclk;//当不用PLL时，Fpclk快一倍,因为默认PLL参数设置n = 1。

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
    VICVectAddr = 0;            //interrupt close 通知中断控制器中断结束
    
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

uint8 com0Recive(uint16 waitTim)
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
			recvDlayCnt = 5;
			dataInFlg = 1;
		}
		else
			break;

	}while(1);

    if(dataInFlg == 0)
		return 0 ;
	else
	    return 1;

}
void com0Send(const uint8 * sendStr,uint16 waitTim)
{
	uint8 timOutFlg = 0;
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
		}
		else
			break;

	}while(1);


}
uint8 com1Recive(uint16 waitTim)
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
			recvDlayCnt = 5;
			dataInFlg = 1;
		}
		else
			break;

	}while(1);

	if(dataInFlg == 0)
		return 0 ;
	else
	    return 1;

}
void com1Send(const uint8 * sendStr,uint16 waitTim)
{
	uint8 timOutFlg = 0;
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
		}
		else
			break;

	}while(1);


}
int main (void)
{
	uint8 cxor,erro = 0;
	uint8 ladVluBuf[5],cTemp[20];
	uint8 dpData[8];

	uint8 cTemp1[11];

	uint8 autoChangeItemFlg = 0,autoStartMeasueFlg = 0,backFunNum;
	uint16 iTemp = 0;

	uint32 times,dates;
	uint32 i,j;
	uint32 lTemp;

	uint32 msCnt = 0;

	float dTemp;

	Uart0.RecvOver = 0;
	Uart0.RecvStart = 0;
	Uart0.RecvPoint = 0;
	Uart0.RecvCount = 0;
	Uart0.PacketLength = 0;



	TargetResetInit();
	
	RtcInit();

	InitUart0(9600);
	InitUart1(9600);
	//Timer1Init(20000);
	Timer1Init(1000);

	
	//I2c0Init(300000);
	I2c0Init(75000);

	/*
	for(i = 0; i < 8; i++)
		dpData[i] = 0x6e;

 	//I2C_WriteNByte(0x70, 1, 0x0d, 0x05,1); //显示位数
	I2C_WriteNByte(0x70, 1, 0x10, dpData,8);//显示清0
	//I2C_WriteNByte(0x70, 1, 0x0c, 0x07,1); //闪烁控制

	for(i = 0; i < 8; i++)
		dpData[i] = 0;
	I2C_ReadNByte(0x70, 1, 0x10, dpData,8);

	for(i = 0; i < 8; i++)
	{
		U0THR = dpData[i];
		while((U0LSR & 0x20) == 0);
			 
	}
	*/



	while(1)
	{
		
		if(com0Recive(50))
		{
			//com1Send(Uart0.RecvBuf,50);
			for(i = 0; i < strlen(Uart0.RecvBuf); i++)
			{
				U1THR = Uart0.RecvBuf[i];
				while((U1LSR & 0x20) == 0);
			}
			
			if(com1Recive(500))
			{
				//com0Send(Uart0.RecvBuf,50);
				for(i = 0; i < strlen(Uart0.RecvBuf); i++)
				{
					U0THR = Uart0.RecvBuf[i];
					while((U0LSR & 0x20) == 0);
				}
			}
			
		}


		/*
		cTemp[0] = 0x02;
	    cTemp[1] = 0x20;
		cTemp[2] = 'A';
		cTemp[3] = 'K';
		cTemp[4] = 'O';
		cTemp[5] = 'N';
	    cTemp[6] = 0x20;
		cTemp[7] = 0x4B;
		cTemp[8] = 0x30;
		cTemp[9] = 0x03;
		cTemp[10] = '\0';

		comSend(cTemp,50);
		

		for(i = 0; i < strlen(Uart0.RecvBuf); i++)
		{
			U0THR = Uart0.RecvBuf[i];
			while((U0LSR & 0x20) == 0);
				 
		}
		*/

		/*
		alarmLedOn();

		for(i = 0; i < 1000; i++)
		for(j = 0; j < 600; j++);

		alarmLedOff();

		for(i = 0; i < 1000; i++)
		for(j = 0; j < 6000; j++);
		*/

		/*
		if(strncmp((char *)&Uart0.RecvBuf[2],"AKON",4))
		{
			memset(cTemp,'\0',20);
		    i = 0;
			do
			{
				if(Uart0.RecvBuf[i] != 0x03)
					cTemp[i] = 	Uart0.RecvBuf[i];		
				else
					break;
				i++;
			}while(i < 20) ;

			I2C_WriteNByte(0x70, 1, 0x10, cTemp,6);//显示
			//atof((const char *)cTemp);
		}

		*/


		/*
	    FIO2MASK = 0xffffffff &(~keyInt);
		if((FIO2PIN & keyInt) == 0)
		{
			for(i = 0;i < 500; i++)
			for(j = 0;j < 100; j++);

			FIO2MASK = 0xffffffff &(~keyInt);
			if((FIO2PIN & keyInt) == 0)
			{
				alarmLedOn();

				I2C_ReadNByte(0x70, 1, 0x01, keyVlu,3);
				ckeyBuf[0]= keyVlu[0];
				ckeyBuf[1]= keyVlu[2];

				for(i = 0; i < 8; i++)
				dpData[i] = 0x6e;
			   I2C_WriteNByte(0x70, 1, 0x10, dpData,8);
			   

				do{
					
				I2C_ReadNByte(0x70, 1, 0x01, keyVlu,3);

				}while(ckeyBuf[0] == keyVlu[0]);

				alarmLedOff();
			   
			   for(i = 0; i < 8; i++)
				dpData[i] = 0;
			   I2C_WriteNByte(0x70, 1, 0x10, dpData,8);

			}
		}
		for(i = 0; i < 10; i++)
		{
			U1THR = 0x55;
			while((U1LSR & 0x20) == 0);
				 
		}
		*/



		/*
		for(i = 0; i < 8; i++)
		dpData[i] = 0x55;
		I2C_WriteNByte(0xa0, 2, 0x0, dpData,8);
		for(i = 0; i < 8; i++)
			dpData[i] = 0;
		I2C_ReadNByte(0xa0, 2, 0x0, dpData,8);
	
		U0THR = 0xcc;
		while((U0LSR & 0x20) == 0);
		for(i = 0; i < 8; i++)
		{
			U0THR = dpData[i];
			while((U0LSR & 0x20) == 0);
				 
		}
		*/
		   /*
		   for(i = 0; i < 8; i++)
				dpData[i] = 0x6e;
			I2C_WriteNByte(0x70, 1, 0x10, dpData,8);//显示清0
		
			for(i = 0; i < 8; i++)
				dpData[i] = 0;
			I2C_ReadNByte(0x70, 1, 0x10, dpData,8);
		
			U0THR = 0xbb;
			while((U0LSR & 0x20) == 0);
			for(i = 0; i < 8; i++)
			{
				U0THR = dpData[i];
				while((U0LSR & 0x20) == 0);
					 
			}
			*/



		/*
		if( Uart0.RecvOver == 1)
		{
			msCnt = 0;

			Uart0.RecvOver = 0;
						
			if(Uart0.RecvBuf[0] == 0x24)//命令头正确
			{
				cxor = Uart0.RecvBuf[0];
				for(i = 1; i < Uart0.PacketLength-1; i++)
				{
					cxor ^= Uart0.RecvBuf[i];
				}
	
				if(cxor == Uart0.RecvBuf[i])//校验和正确
				{
	
					if(Uart0.RecvBuf[3] == 0x15)//设置时间
					{
						PCONP |= 0x00000200;//PCRTC = 1;
						RTC_CCR &= ~0x01;	
						RTC_YEAR = 2000 + Uart0.RecvBuf[10];
						RTC_MONTH = Uart0.RecvBuf[9];
						RTC_DOW = Uart0.RecvBuf[8];
						RTC_DOM = Uart0.RecvBuf[7];
						RTC_HOUR = Uart0.RecvBuf[6];
						RTC_MIN = Uart0.RecvBuf[5];
						RTC_SEC = Uart0.RecvBuf[4];
						RtcInit();
						
						cTemp[0] = 0x25;
						cxor = cTemp[0];
						cTemp[1] = 0x05;
						cxor ^= cTemp[1];
						cTemp[2] = Uart0.RecvBuf[3];
						cxor ^= cTemp[2];
						cTemp[3] = 0x0f;
						cxor ^= cTemp[3];
						cTemp[4] = cxor;

						U0FCR |= 0x06;
						for(i = 0; i < cTemp[1]; i++)
						{
							U0THR = cTemp[i];
							while((U0LSR & 0x20) == 0);
						}
						U0IER = 0x05;//允许接收中断

						Uart0.PacketLength = 0;
					
					
						if(erro == 1)
						{
							erro = 0;
						}
						
					}
					else if(Uart0.RecvBuf[3] == 0x16)//读取时间
					{
						times = RTC_CTIME0;
						dates = RTC_CTIME1;

						cTemp[0] = 0x25;
						cxor = cTemp[0];
						cTemp[1] = 12;
						cxor ^= cTemp[1];
						cTemp[2] = Uart0.RecvBuf[3];
						cxor ^= cTemp[2];
						cTemp[3] = 0x0f;
						cxor ^= cTemp[3];
						cTemp[4] = times & 0x3f;		//秒
						cxor ^= cTemp[4];
						cTemp[5] = (times >> 8)&0x3f;	//分
						cxor ^= cTemp[5];
						cTemp[6] = (times >> 16)&0x1f;	//时
						cxor ^= cTemp[6];
						cTemp[7] = dates&0x1f;			//日
						cxor ^= cTemp[7];
						cTemp[8] = (times >> 24)&0x07;	//星期
						cxor ^= cTemp[8];
						cTemp[9] = (dates >> 8)&0x0f;	//月
						cxor ^= cTemp[9];
						cTemp[10] = (dates >> 16)&0x0f; //年
						cxor ^= cTemp[10];
						cTemp[11] = cxor;
						
						U0FCR |= 0x06;
						for(i = 0; i < cTemp[1]; i++)
						{
							U0THR = cTemp[i];
							while((U0LSR & 0x20) == 0);
						}
						U0IER = 0x05;//允许接收中断
						

						Uart0.PacketLength = 0;
					
					
						if(erro == 1)
						{
							erro = 0;
						}

					}
					

					else
					{
						erro = 1;
						Uart0.PacketLength = 0;
						U0IER = 0x05;//允许接收中断

					}
				}
				else
				{
					erro = 1;
					Uart0.PacketLength = 0;
					U0IER = 0x05;//允许接收中断

				}
			}
			else
			{
				erro = 1;
				Uart0.PacketLength = 0;
				U0IER = 0x05;//允许接收中断

			}

		}
		*/
	}
	
    return 0;
}
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
