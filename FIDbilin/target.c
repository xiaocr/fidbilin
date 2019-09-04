/****************************************Copyright (c)**************************************************
**                               Guangzou ZLG-MCU Development Co.,LTD.
**                                      graduate school
**                                 http://www.zlgmcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			target.c
** Last modified Date:	2004-09-17
** Last Version:		1.0
** Descriptions:		header file of the specific codes for LPC2100 target boards
**						Every project should include a copy of this file, user may modify it as needed
**------------------------------------------------------------------------------------------------------
** Created by:			Chenmingji
** Created date:		2004-02-02
** Version:				1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			Chenmingji
** Modified date:		2004-09-17
** Version:				1.01
** Descriptions:		Renewed the template, added more compiler supports 
**
**------------------------------------------------------------------------------------------------------
** Modified by: 
** Modified date:
** Version:	
** Descriptions: 
**
********************************************************************************************************/

#define IN_TARGET
#include "config.h"

#define VIC_SIZE		32

#define VECT_ADDR_INDEX	0x100
#define VECT_PRIO_INDEX 0x200


/*********************************************************************************************************
** Function name:			IRQ_Exception
**
** Descriptions:			interrupt exceptional handler , change it as needed
**
** input parameters:		None
** Returned value:			None
**         
** Used global variables:	None
** Calling modules:			None
**
** Created by:				Chenmingji
** Created Date:			2004/02/02
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void __irq IRQ_Exception(void)
{
    while(1);                   //  change it to your code 这一句替换为自己的代码
}

/*********************************************************************************************************
** Function name:			FIQ_Exception
**
** Descriptions:			Fast interrupt exceptional handler , change it as needed
**
** input parameters:		None
** Returned value:			None
**         
** Used global variables:	None
** Calling modules:			None
**
** Created by:				Chenmingji
** Created Date:			2004/02/02
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void FIQ_Exception(void)
{
    while(1);                   // change it to your code  这一句替换为自己的代码
}

/* Initialize the interrupt controller */
/******************************************************************************
** Function name:		init_VIC
**
** Descriptions:		Initialize VIC interrupt controller.
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void init_VIC(void) 
{
    unsigned long i = 0;
    unsigned long *vect_addr, *vect_prio;
   	
    /* initialize VIC*/
    VICIntEnClr = 0xffffffff;
    VICVectAddr = 0;
    VICIntSelect = 0;

    /* set all the vector and vector control register to 0 */
    for ( i = 0; i < VIC_SIZE; i++ )
    {
		vect_addr = ( unsigned long *)(VIC_BASE_ADDR + VECT_ADDR_INDEX + i*4);
		vect_prio = ( unsigned long *)(VIC_BASE_ADDR + VECT_PRIO_INDEX + i*4);
		*vect_addr = 0x0;	
		*vect_prio = 0xF;
    }
    return;
}

/*********************************************************************************************************
** Function name:			InitIO
**
** Descriptions:			I/O Initial , change it as needed
**
** input parameters:		None
** Returned value:			None
**         
** Used global variables:	None
** Calling modules:			None
**
** Created by:				xcr
** Created Date:			2008/03/20
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
 void GPIOResetInit(void)
{
   uint16 i;
	/* Reset all GPIO pins to default: primary function */
  SCS |= 0x01;

  //PCONP = 0x002003de;//关闭没用到的外设，降低功耗

  PINSEL0 = 0x00000000;
  PINSEL1 = 0x00000000;
  PINSEL2 = 0x00000000;
  PINSEL3 = 0x00000000;
  PINSEL4 = 0x00000000;
  PINSEL5 = 0x00000000;
  PINSEL6 = 0x00000000;
  PINSEL7 = 0x00000000;
  PINSEL8 = 0x00000000;
  PINSEL9 = 0x00000000;
  PINSEL10 = 0x00000000;
    
  IODIR0 = 0x00000000;
  IODIR1 = 0x00000000;
  IOSET0 = 0x00000000;
  IOSET1 = 0x00000000;
    
  FIO0DIR = 0x00000000;
  FIO1DIR = 0x00000000;
  FIO2DIR = 0x00000000;
  FIO3DIR = 0x00000000;
  FIO4DIR = 0x00000000;
    
  FIO0SET = 0x00000000;
  FIO1SET = 0x00000000;
  FIO2SET = 0x00000000;
  FIO3SET = 0x00000000;
  FIO4SET = 0x00000000;


  PINSEL1 |= 0x01400000;	//IIC0定义P0.27,P0.28
  PINSEL0 |= 0xc0000000;	//串行FLASH接口定义P0.15
  PINSEL1 |= 0x0000003c;	//串行FLASH接口定义P0.17,P0.18
  
  PINSEL0 |= 0x00000050;    //串口0接收P0.3,发送P0.2
  
  PINSEL1 |= 0x00154000;	//AD0.0-3---P0.23,P0.24,P0.25,P0.26
  //PINSEL1 |= 0x00140000;	//AD0.2-3---P0.25,P0.26

  PINSEL3 |= 0xF0000000;	/* P1.30~31, A0.4~5, function 11 */

  //PINSEL3 |= 0xf0300000;	//AD0.4-5---P1.30,P1.31;CAP0.0---P1.26
  //PINSEL3 |= 0xc0300000;	//AD05---P1.31;CAP0.0---P1.26

  PINSEL4 |= 0x0000000a;	//P2.0,P2.1-RXD,TXD	  

  /*
  FIO0DIR |= P01|P016|P019|P021|P020|P022|adCtrA|adCtrB|adCtrC;
  FIO1DIR |= P119;
  FIO2DIR |= P211|P212;
  */
  FIO0DIR |= P00|P016;
  FIO1DIR |= P18|P19|P110|P114|P115|P116|P118|P119|P120|P121|P122|P123|P127|P128|P129;

  //FIO0DIR &= ~(P015|P016|P017);
  FIO0DIR &= ~(P020|P021|P022);

  FIO2DIR |= alarmLed|P23|P24|P25|P26|P27|P28;
  FIO2DIR &= ~(keyInt|P210);

  alarmLedOff();
  P23Hi();	//电磁阀控制
  P24Hi();
  P25Hi();
  P26Hi();
  P27Hi();
  P28Hi();
  //P28Lo();//气泵启动

  P122Hi();//预留控制
  P123Hi();

  P00Hi(); //显示板指示灯控制


  P18Lo();	//量程切换控制
  P19Lo();
  P110Lo();
  P114Lo();
  P115Lo();
  P116Lo();

  //P118Hi();//点火控制
  P118Lo();//点火控制

  P119Lo();	//压力模拟通道切换
  //P120Lo();
  P122Lo();

  //P121Lo();//关闭氢气
  P123Lo();//关闭氢气

  return; 
}
/******************************************************************************
** Function name:		ConfigurePLL
**
** Descriptions:		Configure PLL switching to main OSC instead of IRC
**						at power up and wake up from power down. 
**						This routine is used in TargetResetInit() and those
**						examples using power down and wake up such as
**						USB suspend to resume, ethernet WOL, and power management
**						example
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void ConfigurePLL ( void )
{
  uint32 MValue, NValue;

  if ( PLLSTAT & (1 << 25) )
  {
	PLLCON = 1;			/* Enable PLL, disconnected */
	PLLFEED = 0xaa;
	PLLFEED = 0x55;
  }

  PLLCON = 0;				/* Disable PLL, disconnected */
  PLLFEED = 0xaa;
  PLLFEED = 0x55;
    
  SCS |= 0x20;			/* Enable main OSC */
  while( !(SCS & 0x40) );	/* Wait until main OSC is usable */

  CLKSRCSEL = 0x1;		/* select main OSC, 12MHz, as the PLL clock source */

  PLLCFG = PLL_MValue | (PLL_NValue << 16);
  PLLFEED = 0xaa;
  PLLFEED = 0x55;
      
  PLLCON = 1;				/* Enable PLL, disconnected */
  PLLFEED = 0xaa;
  PLLFEED = 0x55;

  CCLKCFG = CCLKDivValue;	/* Set clock divider */
#if USE_USB
  USBCLKCFG = USBCLKDivValue;		/* usbclk = 288 MHz/6 = 48 MHz */
#endif

  while ( ((PLLSTAT & (1 << 26)) == 0) );	/* Check lock bit status */
    
  MValue = PLLSTAT & 0x00007FFF;
  NValue = (PLLSTAT & 0x00FF0000) >> 16;
  while ((MValue != PLL_MValue) && ( NValue != PLL_NValue) );

  PLLCON = 3;				/* enable and connect */
  PLLFEED = 0xaa;
  PLLFEED = 0x55;
  while ( ((PLLSTAT & (1 << 25)) == 0) );	/* Check connect bit status */
  return;
}

/******************************************************************************
** Function name:		TargetResetInit
**
** Descriptions:		Initialize the target board before running the main() 
**						function; User may change it as needed, but may not 
**						deleted it.
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void TargetResetInit(void)
{
#ifdef __DEBUG_RAM    
  MEMMAP = 0x2;			/* remap to internal RAM */
#endif

#ifdef __DEBUG_FLASH    
  MEMMAP = 0x1;			/* remap to internal flash */
#endif

//#if USE_USB
  //PCONP |= 0x80000000;		/* Turn On USB PCLK */
//#endif
  /* Configure PLL, switch from IRC to Main OSC */
  //ConfigurePLL();

  /* Set system timers for each component */
#if (Fpclk / (Fcclk / 4)) == 1
  PCLKSEL0 = 0x00000000;	/* PCLK is 1/4 CCLK */
  PCLKSEL1 = 0x00000000;
#endif
#if (Fpclk / (Fcclk / 4)) == 2
  PCLKSEL0 = 0xAAAAAAAA;	/* PCLK is 1/2 CCLK */
  //PCLKSEL0 = 0xAAA9AAAA;
  PCLKSEL1 = 0xAAAAAAAA;	 
#endif
#if (Fpclk / (Fcclk / 4)) == 4
  PCLKSEL0 = 0x55555555;	/* PCLK is the same as CCLK */
  PCLKSEL1 = 0x55555555;	
#endif

  /* Set memory accelerater module*/
  MAMCR = 0;
#if Fcclk < 20000000
  MAMTIM = 1;
#else
#if Fcclk < 40000000
  MAMTIM = 2;
#else
  MAMTIM = 3;
#endif
#endif
  MAMCR = 2;

  GPIOResetInit();

  init_VIC();
  return;
}
/*********************************************************************************************************
**                  以下为一些与系统相关的库函数的实现
**                  具体作用请ads的参考编译器与库函数手册
**                  用户可以根据自己的要求修改        
********************************************************************************************************/
/*********************************************************************************************************
**                  The implementations for some library functions
**                  For more details, please refer to the ADS compiler handbook and The library 
**					function manual
**                  User could change it as needed       
********************************************************************************************************/
#include <rt_sys.h>
#include <stdio.h>

#pragma import(__use_no_semihosting_swi)

        int __rt_div0(int a)
{
    a = a;
    return 0;
}

        int fputc(int ch,FILE *f)
{
    ch = ch;
    f = f;
    return 0;
}

    int fgetc(FILE *f)
{
    f = f;
    return 0;
}


        int _sys_close(FILEHANDLE fh)
{
    fh = fh;
    return 0;
}

        int _sys_write(FILEHANDLE fh, const unsigned char * buf,
                      unsigned len, int mode)
{
    fh = fh;
    buf = buf;
    len =len;
    mode = mode;
    return 0;
}
        int _sys_read(FILEHANDLE fh, unsigned char * buf,
                     unsigned len, int mode)
{
    fh = fh;
    buf = buf;
    len =len;
    mode = mode;
    
    return 0;
}

       void _ttywrch(int ch)
{
    ch = ch;
}

        int _sys_istty(FILEHANDLE fh)
{
    fh = fh;
    return 0;
}
        int _sys_seek(FILEHANDLE fh, long pos)
{
    fh = fh;
    return 0;
}
        int _sys_ensure(FILEHANDLE fh)
{
    fh = fh;
    return 0;
}

        long _sys_flen(FILEHANDLE fh)
{
    fh = fh;
    return 0;
}
       int _sys_tmpnam(char * name, int sig, unsigned maxlen)
{
    name = name;
    sig = sig;
    maxlen = maxlen;
    return 0;
}

        void _sys_exit(int returncode)
{
    returncode = returncode;
}

        char *_sys_command_string(char * cmd, int len)
{
    cmd = cmd;
    len = len;
    return 0;
}


/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
