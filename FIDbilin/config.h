/****************************************Copyright (c)**************************************************
**                               Guangzou ZLG-MCU Development Co.,LTD.
**                                      graduate school
**                                 http://www.zlgmcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File Name: config.h
** Last modified Date:  2004-09-17
** Last Version: 1.0
** Descriptions: User Configurable File
**
**------------------------------------------------------------------------------------------------------
** Created By: Chenmingji
** Created date:   2004-09-17
** Version: 1.0
** Descriptions: First version
**
**------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
********************************************************************************************************/
#ifndef __CONFIG_H 
#define __CONFIG_H

//This segment should not be modified
#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

//#define hzIndex 0x0007D000
//#define hzkIndex 0x00058000

//#define dataPointSum 512

//#define hzkMatrix 32 //固定汉字属性
//extern unsigned  char hzkMatrix;
//extern unsigned int  hzIndexLen;

/*
#define OXR_ERRO  0xff
#define LOXR_ERRO 0xfe		//行校验错
#define COXR_ERRO 0xfd		//列校验错
#define HEAD_ERRO 0xfc		//卡信息头错
#define INVALID_DATA 0xfb	//无效数据
*/
//typedef unsigned char  uint8;                   /* defined for unsigned 8-bits integer variable 	无符号8位整型变量  */
typedef signed   char  int8;                    /* defined for signed 8-bits integer variable		有符号8位整型变量  */
//typedef unsigned short uint16;                  /* defined for unsigned 16-bits integer variable 	无符号16位整型变量 */
typedef signed   short int16;                   /* defined for signed 16-bits integer variable 		有符号16位整型变量 */
//typedef unsigned int   uint32;                  /* defined for unsigned 32-bits integer variable 	无符号32位整型变量 */
typedef signed   int   int32;                   /* defined for signed 32-bits integer variable 		有符号32位整型变量 */
typedef float          fp32;                    /* single precision floating point variable (32bits) 单精度浮点数（32位长度） */
typedef double         fp64;                    /* double precision floating point variable (64bits) 双精度浮点数（64位长度） */

#include "LPC23xx.H"  
#include "target.h"
//#include "at45db642.h"
#include "type.h"
//#include "I2CINT.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*
extern BYTE InReport;
extern BYTE OutReport;
extern void GetInReport (void);
extern void SetOutReport (void);
*/

#define			keyInt		 ((uint32)1<<11)		  //P2.11键盘中断线
#define  		alarmLed  ((uint32)1<<13)      		//P2.13报警灯
#define         alarmLedOff()     {FIO2MASK = 0xffffffff & (~alarmLed);FIO2SET |= alarmLed;}  
#define         alarmLedOn()     {FIO2MASK = 0xffffffff & (~alarmLed);FIO2CLR |= alarmLed;}


#define  		P00  ((uint32)1<<0)      		//P0.0
#define         P00Hi()     {FIO0MASK = 0xffffffff & (~P00);FIO0SET |= P00;}  
#define         P00Lo()     {FIO0MASK = 0xffffffff & (~P00);FIO0CLR |= P00;}

#define  		P020  ((uint32)1<<20)      		//P0.20
//#define         P020Hi()     {FIO0MASK = 0xffffffff & (~P020);FIO0SET |= P020;}  
//#define         P020Lo()     {FIO0MASK = 0xffffffff & (~P020);FIO0CLR |= P020;}

#define  		P021  ((uint32)1<<21)      		//P0.21
//#define         P021Hi()     {FIO0MASK = 0xffffffff & (~P021);FIO0SET |= P021;}  
//#define         P021Lo()     {FIO0MASK = 0xffffffff & (~P021);FIO0CLR |= P021;}

#define  		P022  ((uint32)1<<22)      		//P0.22
//#define         P022Hi()     {FIO0MASK = 0xffffffff & (~P022);FIO0SET |= P022;}  
//#define         P022Lo()     {FIO0MASK = 0xffffffff & (~P022);FIO0CLR |= P022;}

#define  		P127  ((uint32)1<<27)      		//P1.27
#define         P127Hi()     {FIO1MASK = 0xffffffff & (~P127);FIO1SET |= P127;}  
#define         P127Lo()     {FIO1MASK = 0xffffffff & (~P127);FIO1CLR |= P127;}

#define  		P128  ((uint32)1<<28)      		//P1.28
#define         P128Hi()     {FIO1MASK = 0xffffffff & (~P128);FIO1SET |= P128;}  
#define         P128Lo()     {FIO1MASK = 0xffffffff & (~P128);FIO1CLR |= P128;}

#define  		P129  ((uint32)1<<29)      		//P1.29
#define         P129Hi()     {FIO1MASK = 0xffffffff & (~P129);FIO1SET |= P129;}  
#define         P129Lo()     {FIO1MASK = 0xffffffff & (~P129);FIO1CLR |= P129;}

#define  		P23  ((uint32)1<<3)      		//P2.3
#define         P23Hi()     {FIO2MASK = 0xffffffff & (~P23);FIO2SET |= P23;}  
#define         P23Lo()     {FIO2MASK = 0xffffffff & (~P23);FIO2CLR |= P23;}

#define  		P24  ((uint32)1<<4)      		//P2.4
#define         P24Hi()     {FIO2MASK = 0xffffffff & (~P24);FIO2SET |= P24;}  
#define         P24Lo()     {FIO2MASK = 0xffffffff & (~P24);FIO2CLR |= P24;}

#define  		P25  ((uint32)1<<5)      		//P2.5
#define         P25Hi()     {FIO2MASK = 0xffffffff & (~P25);FIO2SET |= P25;}  
#define         P25Lo()     {FIO2MASK = 0xffffffff & (~P25);FIO2CLR |= P25;}

#define  		P26  ((uint32)1<<6)      		//P2.6
#define         P26Hi()     {FIO2MASK = 0xffffffff & (~P26);FIO2SET |= P26;}  
#define         P26Lo()     {FIO2MASK = 0xffffffff & (~P26);FIO2CLR |= P26;}

#define  		P27  ((uint32)1<<7)      		//P2.7
#define         P27Hi()     {FIO2MASK = 0xffffffff & (~P27);FIO2SET |= P27;}  
#define         P27Lo()     {FIO2MASK = 0xffffffff & (~P27);FIO2CLR |= P27;}

#define  		P28  ((uint32)1<<8)      		//P2.8
#define         P28Hi()     {FIO2MASK = 0xffffffff & (~P28);FIO2SET |= P28;}  
#define         P28Lo()     {FIO2MASK = 0xffffffff & (~P28);FIO2CLR |= P28;}


#define         P015 ((uint32)1<<15)           //P0.15
//#define         P015_1()     {FIO0MASK = 0xffffffff & (~P015);FIO0SET |= P015;}  
//#define         P015_0()     {FIO0MASK = 0xffffffff & (~P015);FIO0CLR |= P015;}

#define         P016 ((uint32)1<<16)           //P0.16
#define         P016_1()     {FIO0MASK = 0xffffffff & (~P016);FIO0SET |= P016;}  
#define         P016_0()     {FIO0MASK = 0xffffffff & (~P016);FIO0CLR |= P016;}

#define         P017 ((uint32)1<<17)           //P0.17

#define  		P18  ((uint32)1<<8)      		//P1.8
#define         P18Hi()     {FIO1MASK = 0xffffffff & (~P18);FIO1SET |= P18;}  
#define         P18Lo()     {FIO1MASK = 0xffffffff & (~P18);FIO1CLR |= P18;}

#define  		P19  ((uint32)1<<9)      		//P1.9
#define         P19Hi()     {FIO1MASK = 0xffffffff & (~P19);FIO1SET |= P19;}  
#define         P19Lo()     {FIO1MASK = 0xffffffff & (~P19);FIO1CLR |= P19;}

#define  		P110  ((uint32)1<<10)      		//P1.10
#define         P110Hi()     {FIO1MASK = 0xffffffff & (~P110);FIO1SET |= P110;}  
#define         P110Lo()     {FIO1MASK = 0xffffffff & (~P110);FIO1CLR |= P110;}

#define  		P114  ((uint32)1<<14)      		//P1.14
#define         P114Hi()     {FIO1MASK = 0xffffffff & (~P114);FIO1SET |= P114;}  
#define         P114Lo()     {FIO1MASK = 0xffffffff & (~P114);FIO1CLR |= P114;}

#define  		P115  ((uint32)1<<15)      		//P1.15
#define         P115Hi()     {FIO1MASK = 0xffffffff & (~P115);FIO1SET |= P115;}  
#define         P115Lo()     {FIO1MASK = 0xffffffff & (~P115);FIO1CLR |= P115;}

#define  		P116  ((uint32)1<<16)      		//P1.16
#define         P116Hi()     {FIO1MASK = 0xffffffff & (~P116);FIO1SET |= P116;}  
#define         P116Lo()     {FIO1MASK = 0xffffffff & (~P116);FIO1CLR |= P116;}



#define  		P118  ((uint32)1<<18)      		//P1.18
#define         P118Hi()     {FIO1MASK = 0xffffffff & (~P118);FIO1SET |= P118;}  
#define         P118Lo()     {FIO1MASK = 0xffffffff & (~P118);FIO1CLR |= P118;}

#define  		P119  ((uint32)1<<19)      		//P1.19
#define         P119Hi()     {FIO1MASK = 0xffffffff & (~P119);FIO1SET |= P119;}  
#define         P119Lo()     {FIO1MASK = 0xffffffff & (~P119);FIO1CLR |= P119;}

#define  		P120  ((uint32)1<<20)      		//P1.20
#define         P120Hi()     {FIO1MASK = 0xffffffff & (~P120);FIO1SET |= P120;}  
#define         P120Lo()     {FIO1MASK = 0xffffffff & (~P120);FIO1CLR |= P120;}

#define  		P121  ((uint32)1<<21)      		//P1.21
#define         P121Hi()     {FIO1MASK = 0xffffffff & (~P121);FIO1SET |= P121;}  
#define         P121Lo()     {FIO1MASK = 0xffffffff & (~P121);FIO1CLR |= P121;}



#define  		P122  ((uint32)1<<22)      		//P1.22
#define         P122Hi()     {FIO1MASK = 0xffffffff & (~P122);FIO1SET |= P122;}  
#define         P122Lo()     {FIO1MASK = 0xffffffff & (~P122);FIO1CLR |= P122;}

#define  		P123  ((uint32)1<<23)      		//P1.23
#define         P123Hi()     {FIO1MASK = 0xffffffff & (~P123);FIO1SET |= P123;}  
#define         P123Lo()     {FIO1MASK = 0xffffffff & (~P123);FIO1CLR |= P123;}

#define  		P210  ((uint32)1<<10)      		//P2.10

#define	 		ADCNT 		20 //A/D采样次数
/*
#define 		P119		((uint32)1<<19)			//P1.19 +5V电源控制
#define         P119Clr()    {FIO1MASK = 0xffffffff & (~P119);FIO1CLR |= P119;}       
#define         P119Set()    {FIO1MASK = 0xffffffff & (~P119);FIO1SET |= P119;}

#define         P01       	((uint32)1<<1)          //P0.1 模拟通道电源控制
#define         P01Clr()    {FIO0MASK = 0xffffffff & (~P01);FIO0CLR |= P01;}       
#define         P01Set()    {FIO0MASK = 0xffffffff & (~P01);FIO0SET |= P01;}

#define         P016      	((uint32)1<<16)           //P0.16
#define         P016_1()     {FIO0MASK = 0xffffffff & (~P016);FIO0SET |= P016;}  
#define         P016_0()     {FIO0MASK = 0xffffffff & (~P016);FIO0CLR |= P016;}

#define         P019      	((uint32)1<<19)          //P0.19
#define         P019_1()     {FIO0MASK = 0xffffffff & (~P019);FIO0SET |= P019;}  
#define         P019_0()     {FIO0MASK = 0xffffffff & (~P019);FIO0CLR |= P019;}

#define         P020       	((uint32)1<<20)          //P0.20
#define         P020Clr()    {FIO0MASK = 0xffffffff & (~P020);FIO0CLR |= P020;}       
#define         P020Set()    {FIO0MASK = 0xffffffff & (~P020);FIO0SET |= P020;}

#define         P021       	((uint32)1<<21)          //P0.21
#define         P021Clr()    {FIO0MASK = 0xffffffff & (~P021);FIO0CLR |= P021;}       
#define         P021Set()    {FIO0MASK = 0xffffffff & (~P021);FIO0SET |= P021;}
 
#define         P022       	((uint32)1<<22)          //P0.22
#define         P022Clr()    {FIO0MASK = 0xffffffff & (~P022);FIO0CLR |= P022;}       
#define         P022Set()    {FIO0MASK = 0xffffffff & (~P022);FIO0SET |= P022;}

#define         P211       ((uint32)1<<11)          //P2.11	
#define         P211Clr()    {FIO2MASK = 0xffffffff & (~P211);FIO2CLR |= P211;}       
#define         P211Set()    {FIO2MASK = 0xffffffff & (~P211);FIO2SET |= P211;}

#define         P212       ((uint32)1<<12)          //P2.12	
#define         P212Clr()    {FIO2MASK = 0xffffffff & (~P212);FIO2CLR |= P212;}       
#define         P212Set()    {FIO2MASK = 0xffffffff & (~P212);FIO2SET |= P212;}

#define         adCtrA       ((uint32)1<<10)          //P0.10	底板模拟通道控制I/O
#define         P010Clr()    {FIO0MASK = 0xffffffff & (~adCtrA);FIO0CLR |= adCtrA;}       
#define         P010Set()    {FIO0MASK = 0xffffffff & (~adCtrA);FIO0SET |= adCtrA;}

#define         adCtrB       ((uint32)1<<11)          //P0.11	底板模拟通道控制I/O
#define         P011Clr()    {FIO0MASK = 0xffffffff & (~adCtrB);FIO0CLR |= adCtrB;}       
#define         P011Set()    {FIO0MASK = 0xffffffff & (~adCtrB);FIO0SET |= adCtrB;}

#define         adCtrC       ((uint32)1<<0)          //P0.0	底板模拟通道控制I/O
#define         P000Clr()    {FIO0MASK = 0xffffffff & (~adCtrC);FIO0CLR |= adCtrC;}       
#define         P000Set()    {FIO0MASK = 0xffffffff & (~adCtrC);FIO0SET |= adCtrC;}
*/


/*
#define  SPICE0  ((uint32)1<<10)      	//flash 0片选P1.10
#define  SPICE1  ((uint32)1<<9)      	//flash 1片选P1.9
#define  FlashRst  ((uint32)1<<8)      	//P1.8 flash复位控制
#define  FlashRstOff()    {FIO1MASK = 0xffffffff & (~FlashRst);FIO1SET |= FlashRst;}  
#define  FlashRstOn()     {FIO1MASK = 0xffffffff & (~FlashRst);FIO1CLR |= FlashRst;}

#define  RXD0    ((uint32)1<<3)       	//串口0接收P0.3
#define  TXD0    ((uint32)1<<2)       	//串口0发送P0.2


#define LCDRST	((uint32)1<<9)			//P0.9液晶RESET
#define LCDCS	((uint32)1<<8)			//P0.8液晶CS	
#define LCDRS	((uint32)1<<7)			//P0.7液晶RS
#define LCDRD	((uint32)1<<6)			//P0.6液晶RD
#define LCDWR	((uint32)1<<8)			//P2.8液晶WR

#define         RS_1()     {FIO0MASK = 0xffffffff & (~LCDRS);FIO0SET |= LCDRS;}          
#define         RS_0()     {FIO0MASK = 0xffffffff & (~LCDRS);FIO0CLR |= LCDRS;}
#define         WR_1()     {FIO2MASK = 0xffffffff & (~LCDWR);FIO2SET |= LCDWR;}          
#define         WR_0()     {FIO2MASK = 0xffffffff & (~LCDWR);FIO2CLR |= LCDWR;}
#define         CS_1()     {FIO0MASK = 0xffffffff & (~LCDCS);FIO0SET |= LCDCS;}          
#define         CS_0()     {FIO0MASK = 0xffffffff & (~LCDCS);FIO0CLR |= LCDCS;}
#define         RD_1()     {FIO0MASK = 0xffffffff & (~LCDRD);FIO0SET |= LCDRD;}          
#define         RD_0()     {FIO0MASK = 0xffffffff & (~LCDRD);FIO0CLR |= LCDRD;}
#define         RST_1()    {FIO0MASK = 0xffffffff & (~LCDRST);FIO0SET |= LCDRST;}          
#define         RST_0()    {FIO0MASK = 0xffffffff & (~LCDRST);FIO0CLR |= LCDRST;}

#define  		RfPwr_Ctr  ((uint32)1<<4)      		//P1.4 RF电源控制
#define         RfPwrOff()     {FIO1MASK = 0xffffffff & (~RfPwr_Ctr);FIO1SET |= RfPwr_Ctr;}  
#define         RfPwrOn()     {FIO1MASK = 0xffffffff & (~RfPwr_Ctr);FIO1CLR |= RfPwr_Ctr;}

#define         P114       ((uint32)1<<14)          //P1.14 自动关机控制I/O
#define         P28Clr()    {FIO1MASK = 0xffffffff & (~P114);FIO1CLR |= P114;}       
#define         P28Set()    {FIO1MASK = 0xffffffff & (~P114);FIO1SET |= P114;}



#define         P120        ((uint32)1<<20)         //P1.20 +5V测温/转速电源控制
#define         P019Clr()    {P119Clr();FIO1MASK = 0xffffffff & (~P120);FIO1CLR |= P120;}       
#define         P019Set()    {P119Set();FIO1MASK = 0xffffffff & (~P120);FIO1SET |= P120;}

#define 		P122 		((uint32)1<<22)			//P1.22 +5V读卡电源控制
#define         cardPwrCtrOn()     {P119Set();FIO1MASK = 0xffffffff & (~P122);FIO1SET |= P122;}  
#define         cardPwrCtrOff()    {P119Clr();FIO1MASK = 0xffffffff & (~P122);FIO1CLR |= P122;}

#define 		P124 		((uint32)1<<24)			//P1.24	 用作GPRS复位
//#define         gprsRstHi()    {FIO1MASK = 0xffffffff & (~P124);FIO1SET |= P124;}  
//#define         gprsRstLo()    {FIO1MASK = 0xffffffff & (~P124);FIO1CLR |= P124;}

#define 		P128 		((uint32)1<<28)			//P1.28	 闪光灯控制2
#define         flashLedHi()    {FIO1MASK = 0xffffffff & (~P128);FIO1SET |= P128;}  
#define         flashLedLo()    {FIO1MASK = 0xffffffff & (~P128);FIO1CLR |= P128;}

#define 		P129 		((uint32)1<<29)			//P1.29	 闪光灯控制1
#define         flashLedOn()     {FIO1MASK = 0xffffffff & (~P129);FIO1SET |= P129;}  
#define         flashLedOff()    {FIO1MASK = 0xffffffff & (~P129);FIO1CLR |= P129;}

#define			RFDATA		((uint32)1<<21)     	//P1.21 读卡数据线

#define 		P123 ((uint32)1<<23)				//P1.23运行灯
#define         runLedOff()     {FIO1MASK = 0xffffffff & (~P123);FIO1SET |= P123;}  
#define         runLedOn()     {FIO1MASK = 0xffffffff & (~P123);FIO1CLR |= P123;}


#define         P01       	((uint32)1<<1)          //P0.1 模拟通道电源控制
#define         P05Clr()    {FIO0MASK = 0xffffffff & (~P01);FIO0CLR |= P01;}       
#define         P05Set()    {FIO0MASK = 0xffffffff & (~P01);FIO0SET |= P01;}

#define         P04       	((uint32)1<<4)          //P0.4 GPRS ON/OFF控制
#define         GPRSON()    {FIO0MASK = 0xffffffff & (~P04);FIO0SET |= P04;}       
#define         GPRSOFF()   {FIO0MASK = 0xffffffff & (~P04);FIO0CLR |= P04;} 

#define         P05       	((uint32)1<<5)          //P0.5 GPRS电源控制//曲线电源控制
#define         GPRSPwrClr()    {FIO0MASK = 0xffffffff & (~P05);FIO0CLR |= P05;}       
#define         GPRSPwrSet()    {FIO0MASK = 0xffffffff & (~P05);FIO0SET |= P05;}





#define         P023       ((uint32)1<<23)          //测量板模拟通道控制I/O
#define         P023Clr()    {FIO0MASK = 0xffffffff & (~P023);FIO0CLR |= P023;}       
#define         P023Set()    {FIO0MASK = 0xffffffff & (~P023);FIO0SET |= P023;}

#define         P024       ((uint32)1<<24)          //测量板模拟通道控制I/O
#define         P024Clr()    {FIO0MASK = 0xffffffff & (~P024);FIO0CLR |= P024;}       
#define         P024Set()    {FIO0MASK = 0xffffffff & (~P024);FIO0SET |= P024;}

#define         P325       ((uint32)1<<25)          //测量板模拟通道控制I/O
#define         P325Clr()    {FIO3MASK = 0xffffffff & (~P325);FIO3CLR |= P325;}       
#define         P325Set()    {FIO3MASK = 0xffffffff & (~P325);FIO3SET |= P325;}

#define         P326       ((uint32)1<<26)          //测量板模拟通道控制I/O
#define         P326Clr()    {FIO3MASK = 0xffffffff & (~P326);FIO3CLR |= P326;}       
#define         P326Set()    {FIO3MASK = 0xffffffff & (~P326);FIO3SET |= P326;}







#define         P10      	((uint32)1<<0)           //P1.0 CAM 电源控制
#define         P010_1()     {FIO1MASK = 0xffffffff & (~P10);FIO1SET |= P10;}  
#define         P010_0()     {FIO1MASK = 0xffffffff & (~P10);FIO1CLR |= P10;}




  

#define         D10      ((uint32)1<<0)              //P2.0
#define         D11      ((uint32)1<<1)              //P2.1
#define         D12      ((uint32)1<<2)              //P2.2
#define         D13      ((uint32)1<<3)              //P2.3
#define         D14      ((uint32)1<<4)              //P2.4
#define         D15      ((uint32)1<<5)              //P2.5
#define         D16      ((uint32)1<<6)              //P2.6
#define         D17      ((uint32)1<<7)              //P2.7

#define         D10_1()     {FIO2MASK = 0xffffffff & (~D10);FIO2SET |= D10;}  
#define         D10_0()     {FIO2MASK = 0xffffffff & (~D10);FIO2CLR |= D10;}
  
#define         D11_1()     {FIO2MASK = 0xffffffff & (~D11);FIO2SET |= D11;}
#define         D11_0()     {FIO2MASK = 0xffffffff & (~D11);FIO2CLR |= D11;}

#define         D12_1()     {FIO2MASK = 0xffffffff & (~D12);FIO2SET |= D12;}
#define         D12_0()     {FIO2MASK = 0xffffffff & (~D12);FIO2CLR |= D12;}
  
#define         D13_1()     {FIO2MASK = 0xffffffff & (~D13);FIO2SET |= D13;}  
#define         D13_0()     {FIO2MASK = 0xffffffff & (~D13);FIO2CLR |= D13;}

#define         D14_1()     {FIO2MASK = 0xffffffff & (~D14);FIO2SET |= D14;} 
#define         D14_0()     {FIO2MASK = 0xffffffff & (~D14);FIO2CLR |= D14;}
  
#define         D15_1()     {FIO2MASK = 0xffffffff & (~D15);FIO2SET |= D15;} 
#define         D15_0()     {FIO2MASK = 0xffffffff & (~D15);FIO2CLR |= D15;}

#define         D16_1()     {FIO2MASK = 0xffffffff & (~D16);FIO2SET |= D16;}  
#define         D16_0()     {FIO2MASK = 0xffffffff & (~D16);FIO2CLR |= D16;}
  
#define         D17_1()     {FIO2MASK = 0xffffffff & (~D17);FIO2SET |= D17;}
#define         D17_0()     {FIO2MASK = 0xffffffff & (~D17);FIO2CLR |= D17;}


#define         TouchInt       ((uint32)1<<11)         //P2.11	触摸中断信号

#define         camInt       ((uint32)1<<10)          //P2.10	cam中断信号

#define			keyInt		 ((uint32)1<<13)		  //P2.13键盘中断线

#define         P117       ((uint32)1<<17)            //P1.17	键盘背光控制
#define         P117Clr()    {FIO1MASK = 0xffffffff & (~P117);FIO1CLR |= P117;}       
#define         P117Set()    {FIO1MASK = 0xffffffff & (~P117);FIO1SET |= P117;}

#define         P116       ((uint32)1<<16)            //P1.16 液晶背光控制
#define         P04Set()    {FIO1MASK = 0xffffffff & (~P116);FIO1CLR |= P116;}
#define         P04Clr()    {FIO1MASK = 0xffffffff & (~P116);FIO1SET |= P116;}

#define  		alarmLed  ((uint32)1<<1)      		//P1.1报警灯
#define         alarmLedOff()     {FIO1MASK = 0xffffffff & (~alarmLed);FIO1SET |= alarmLed;}  
#define         alarmLedOn()     {FIO1MASK = 0xffffffff & (~alarmLed);FIO1CLR |= alarmLed;}



#define			AD00		0			  	//P0.23 	速度，加速度，噪声，位移
#define			AD05		5			  	//P1.31	速度曲线，加速度曲线，噪声曲线，位移曲线
//#define       AD01        1             //P0.24	超高频曲线
#define         AD01        1 				//P0.24电量
#define 	    AD03		3             	//P0.26	温度
//#define       AD02        2             //P0.25	高频振动曲线
//#define 	    CAP00		0             	//P1.26	转速（capture 计脉冲）
#define       	AD02        2             	//
*/

//extern float pr[dataPointSum];

#define RecBufLength 1500//考虑最大的数据包为字典下载，字典中的每一类最多数据为1024，加上包的其他数据，现为6字节。

struct Uart0Info
{
	 uint8 RecvOver;
	 uint8 RecvStart;
	 uint8 RecvBuf[RecBufLength];                                 
	 uint32 RecvPoint;
	 uint32 RecvCount;
	 uint32 PacketLength;
};
//extern  volatile struct Uart0Info Uart0;
extern  struct Uart0Info Uart0;


#endif




/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
