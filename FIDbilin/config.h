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

//#define hzkMatrix 32 //�̶���������
//extern unsigned  char hzkMatrix;
//extern unsigned int  hzIndexLen;

/*
#define OXR_ERRO  0xff
#define LOXR_ERRO 0xfe		//��У���
#define COXR_ERRO 0xfd		//��У���
#define HEAD_ERRO 0xfc		//����Ϣͷ��
#define INVALID_DATA 0xfb	//��Ч����
*/
//typedef unsigned char  uint8;                   /* defined for unsigned 8-bits integer variable 	�޷���8λ���ͱ���  */
typedef signed   char  int8;                    /* defined for signed 8-bits integer variable		�з���8λ���ͱ���  */
//typedef unsigned short uint16;                  /* defined for unsigned 16-bits integer variable 	�޷���16λ���ͱ��� */
typedef signed   short int16;                   /* defined for signed 16-bits integer variable 		�з���16λ���ͱ��� */
//typedef unsigned int   uint32;                  /* defined for unsigned 32-bits integer variable 	�޷���32λ���ͱ��� */
typedef signed   int   int32;                   /* defined for signed 32-bits integer variable 		�з���32λ���ͱ��� */
typedef float          fp32;                    /* single precision floating point variable (32bits) �����ȸ�������32λ���ȣ� */
typedef double         fp64;                    /* double precision floating point variable (64bits) ˫���ȸ�������64λ���ȣ� */

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

#define			keyInt		 ((uint32)1<<11)		  //P2.11�����ж���
#define  		alarmLed  ((uint32)1<<13)      		//P2.13������
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

#define	 		ADCNT 		20 //A/D��������
/*
#define 		P119		((uint32)1<<19)			//P1.19 +5V��Դ����
#define         P119Clr()    {FIO1MASK = 0xffffffff & (~P119);FIO1CLR |= P119;}       
#define         P119Set()    {FIO1MASK = 0xffffffff & (~P119);FIO1SET |= P119;}

#define         P01       	((uint32)1<<1)          //P0.1 ģ��ͨ����Դ����
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

#define         adCtrA       ((uint32)1<<10)          //P0.10	�װ�ģ��ͨ������I/O
#define         P010Clr()    {FIO0MASK = 0xffffffff & (~adCtrA);FIO0CLR |= adCtrA;}       
#define         P010Set()    {FIO0MASK = 0xffffffff & (~adCtrA);FIO0SET |= adCtrA;}

#define         adCtrB       ((uint32)1<<11)          //P0.11	�װ�ģ��ͨ������I/O
#define         P011Clr()    {FIO0MASK = 0xffffffff & (~adCtrB);FIO0CLR |= adCtrB;}       
#define         P011Set()    {FIO0MASK = 0xffffffff & (~adCtrB);FIO0SET |= adCtrB;}

#define         adCtrC       ((uint32)1<<0)          //P0.0	�װ�ģ��ͨ������I/O
#define         P000Clr()    {FIO0MASK = 0xffffffff & (~adCtrC);FIO0CLR |= adCtrC;}       
#define         P000Set()    {FIO0MASK = 0xffffffff & (~adCtrC);FIO0SET |= adCtrC;}
*/


/*
#define  SPICE0  ((uint32)1<<10)      	//flash 0ƬѡP1.10
#define  SPICE1  ((uint32)1<<9)      	//flash 1ƬѡP1.9
#define  FlashRst  ((uint32)1<<8)      	//P1.8 flash��λ����
#define  FlashRstOff()    {FIO1MASK = 0xffffffff & (~FlashRst);FIO1SET |= FlashRst;}  
#define  FlashRstOn()     {FIO1MASK = 0xffffffff & (~FlashRst);FIO1CLR |= FlashRst;}

#define  RXD0    ((uint32)1<<3)       	//����0����P0.3
#define  TXD0    ((uint32)1<<2)       	//����0����P0.2


#define LCDRST	((uint32)1<<9)			//P0.9Һ��RESET
#define LCDCS	((uint32)1<<8)			//P0.8Һ��CS	
#define LCDRS	((uint32)1<<7)			//P0.7Һ��RS
#define LCDRD	((uint32)1<<6)			//P0.6Һ��RD
#define LCDWR	((uint32)1<<8)			//P2.8Һ��WR

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

#define  		RfPwr_Ctr  ((uint32)1<<4)      		//P1.4 RF��Դ����
#define         RfPwrOff()     {FIO1MASK = 0xffffffff & (~RfPwr_Ctr);FIO1SET |= RfPwr_Ctr;}  
#define         RfPwrOn()     {FIO1MASK = 0xffffffff & (~RfPwr_Ctr);FIO1CLR |= RfPwr_Ctr;}

#define         P114       ((uint32)1<<14)          //P1.14 �Զ��ػ�����I/O
#define         P28Clr()    {FIO1MASK = 0xffffffff & (~P114);FIO1CLR |= P114;}       
#define         P28Set()    {FIO1MASK = 0xffffffff & (~P114);FIO1SET |= P114;}



#define         P120        ((uint32)1<<20)         //P1.20 +5V����/ת�ٵ�Դ����
#define         P019Clr()    {P119Clr();FIO1MASK = 0xffffffff & (~P120);FIO1CLR |= P120;}       
#define         P019Set()    {P119Set();FIO1MASK = 0xffffffff & (~P120);FIO1SET |= P120;}

#define 		P122 		((uint32)1<<22)			//P1.22 +5V������Դ����
#define         cardPwrCtrOn()     {P119Set();FIO1MASK = 0xffffffff & (~P122);FIO1SET |= P122;}  
#define         cardPwrCtrOff()    {P119Clr();FIO1MASK = 0xffffffff & (~P122);FIO1CLR |= P122;}

#define 		P124 		((uint32)1<<24)			//P1.24	 ����GPRS��λ
//#define         gprsRstHi()    {FIO1MASK = 0xffffffff & (~P124);FIO1SET |= P124;}  
//#define         gprsRstLo()    {FIO1MASK = 0xffffffff & (~P124);FIO1CLR |= P124;}

#define 		P128 		((uint32)1<<28)			//P1.28	 ����ƿ���2
#define         flashLedHi()    {FIO1MASK = 0xffffffff & (~P128);FIO1SET |= P128;}  
#define         flashLedLo()    {FIO1MASK = 0xffffffff & (~P128);FIO1CLR |= P128;}

#define 		P129 		((uint32)1<<29)			//P1.29	 ����ƿ���1
#define         flashLedOn()     {FIO1MASK = 0xffffffff & (~P129);FIO1SET |= P129;}  
#define         flashLedOff()    {FIO1MASK = 0xffffffff & (~P129);FIO1CLR |= P129;}

#define			RFDATA		((uint32)1<<21)     	//P1.21 ����������

#define 		P123 ((uint32)1<<23)				//P1.23���е�
#define         runLedOff()     {FIO1MASK = 0xffffffff & (~P123);FIO1SET |= P123;}  
#define         runLedOn()     {FIO1MASK = 0xffffffff & (~P123);FIO1CLR |= P123;}


#define         P01       	((uint32)1<<1)          //P0.1 ģ��ͨ����Դ����
#define         P05Clr()    {FIO0MASK = 0xffffffff & (~P01);FIO0CLR |= P01;}       
#define         P05Set()    {FIO0MASK = 0xffffffff & (~P01);FIO0SET |= P01;}

#define         P04       	((uint32)1<<4)          //P0.4 GPRS ON/OFF����
#define         GPRSON()    {FIO0MASK = 0xffffffff & (~P04);FIO0SET |= P04;}       
#define         GPRSOFF()   {FIO0MASK = 0xffffffff & (~P04);FIO0CLR |= P04;} 

#define         P05       	((uint32)1<<5)          //P0.5 GPRS��Դ����//���ߵ�Դ����
#define         GPRSPwrClr()    {FIO0MASK = 0xffffffff & (~P05);FIO0CLR |= P05;}       
#define         GPRSPwrSet()    {FIO0MASK = 0xffffffff & (~P05);FIO0SET |= P05;}





#define         P023       ((uint32)1<<23)          //������ģ��ͨ������I/O
#define         P023Clr()    {FIO0MASK = 0xffffffff & (~P023);FIO0CLR |= P023;}       
#define         P023Set()    {FIO0MASK = 0xffffffff & (~P023);FIO0SET |= P023;}

#define         P024       ((uint32)1<<24)          //������ģ��ͨ������I/O
#define         P024Clr()    {FIO0MASK = 0xffffffff & (~P024);FIO0CLR |= P024;}       
#define         P024Set()    {FIO0MASK = 0xffffffff & (~P024);FIO0SET |= P024;}

#define         P325       ((uint32)1<<25)          //������ģ��ͨ������I/O
#define         P325Clr()    {FIO3MASK = 0xffffffff & (~P325);FIO3CLR |= P325;}       
#define         P325Set()    {FIO3MASK = 0xffffffff & (~P325);FIO3SET |= P325;}

#define         P326       ((uint32)1<<26)          //������ģ��ͨ������I/O
#define         P326Clr()    {FIO3MASK = 0xffffffff & (~P326);FIO3CLR |= P326;}       
#define         P326Set()    {FIO3MASK = 0xffffffff & (~P326);FIO3SET |= P326;}







#define         P10      	((uint32)1<<0)           //P1.0 CAM ��Դ����
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


#define         TouchInt       ((uint32)1<<11)         //P2.11	�����ж��ź�

#define         camInt       ((uint32)1<<10)          //P2.10	cam�ж��ź�

#define			keyInt		 ((uint32)1<<13)		  //P2.13�����ж���

#define         P117       ((uint32)1<<17)            //P1.17	���̱������
#define         P117Clr()    {FIO1MASK = 0xffffffff & (~P117);FIO1CLR |= P117;}       
#define         P117Set()    {FIO1MASK = 0xffffffff & (~P117);FIO1SET |= P117;}

#define         P116       ((uint32)1<<16)            //P1.16 Һ���������
#define         P04Set()    {FIO1MASK = 0xffffffff & (~P116);FIO1CLR |= P116;}
#define         P04Clr()    {FIO1MASK = 0xffffffff & (~P116);FIO1SET |= P116;}

#define  		alarmLed  ((uint32)1<<1)      		//P1.1������
#define         alarmLedOff()     {FIO1MASK = 0xffffffff & (~alarmLed);FIO1SET |= alarmLed;}  
#define         alarmLedOn()     {FIO1MASK = 0xffffffff & (~alarmLed);FIO1CLR |= alarmLed;}



#define			AD00		0			  	//P0.23 	�ٶȣ����ٶȣ�������λ��
#define			AD05		5			  	//P1.31	�ٶ����ߣ����ٶ����ߣ��������ߣ�λ������
//#define       AD01        1             //P0.24	����Ƶ����
#define         AD01        1 				//P0.24����
#define 	    AD03		3             	//P0.26	�¶�
//#define       AD02        2             //P0.25	��Ƶ������
//#define 	    CAP00		0             	//P1.26	ת�٣�capture �����壩
#define       	AD02        2             	//
*/

//extern float pr[dataPointSum];

#define RecBufLength 1500//�����������ݰ�Ϊ�ֵ����أ��ֵ��е�ÿһ���������Ϊ1024�����ϰ����������ݣ���Ϊ6�ֽڡ�

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
