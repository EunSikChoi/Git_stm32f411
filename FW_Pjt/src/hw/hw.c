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

	spiInit();

	spiBegin(_DEF_SPI1); // 나중에 w5500이로 이동

}
