/*
 * hw.c
 *
 *  Created on: Nov 15, 2021
 *      Author: 82109
 */


#include "hw.h"

void hwInit(void)
{

	bspInit();

	rtcInit();
	resetInit();

  if( resetGetCount() == 2)
  {
    resetToSysBoot(); //jump to system bootloader //
  }

	cliInit();
	ledInit();

	usbInit();

	flashInit();

	uartInit();

	buttonInit();

	gpioInit();

	if (sdInit() == true)
	{
	  fatfsInit();
	}


}
