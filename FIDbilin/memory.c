/*----------------------------------------------------------------------------
 *      Name:    MEMORY.C
 *      Purpose: USB Mass Storage Demo
 *      Version: V1.10
 *----------------------------------------------------------------------------
 *      This file is part of the uVision/ARM development tools.
 *      This software may only be used under the terms of a valid, current,
 *      end user licence from KEIL for a compatible version of KEIL software
 *      development tools. Nothing else gives you the right to use it.
 *
 *      Copyright (c) 2005-2007 Keil Software.
 *---------------------------------------------------------------------------*/

#include <LPC23xx.H>                        /* LPC23xx definitions */

#include "type.h"

#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "usbcore.h"
#include "mscuser.h"

#include "memory.h"
#include "LCD.h"
#include "config.h"

extern BYTE Memory[MSC_MemorySize];         /* MSC Memory in RAM */


/* Main Program */

int main (void) {

  unsigned int  i,j;

   SCS |= 0x01;



  PINSEL10 = 0;                             /* Disable ETM interface */

  FIO1DIR |= alarmLed|runLed;

  alarmLedOn();
  runLedOn();

   for(i = 0; i < 50000; i++)
	for(j = 0; j < 800; j++);
   alarmLedOff();
  runLedOff();
  /* LED Setup */
  //FIO2DIR |= LED_MSK;                       /* LEDs, port 2, bit 0~7 output only */
/*
  lcd_init();
  lcd_clear();
  lcd_print ("MCB2300 MSD Demo");
  set_cursor (0, 1);
  lcd_print ("  www.keil.com  ");
*/
  USB_Init();                               /* USB Initialization */
  USB_Connect(TRUE);                        /* USB Connect */

  while (1);                                /* Loop forever */
}
