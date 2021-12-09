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
  logInit();

  logPrintf("[ Firmware Begin... ]\r\n");
  logPrintf("Booting..Name \t\t: %s\r\n", _DEF_BOARD_NAME);
  logPrintf("Booting..Ver  \t\t: %s\r\n", _DEF_FIRMWATRE_VERSION);

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
  //spiBegin(_DEF_SPI1); // 나중에 w5500이로 이동

 	W5500_hw_Rst();
	W5500_init();

	logBoot(false); // END Boot log //

}
