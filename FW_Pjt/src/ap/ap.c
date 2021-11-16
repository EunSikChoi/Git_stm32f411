/*
 * ap.c
 *
 *  Created on: Nov 15, 2021
 *      Author: 82109
 */


#include "ap.h"

void apInit(void)
{
		cliOpen(_DEF_UART1, 57600);
}

void apMain(void)
{
  uint32_t pre_time;
  pre_time = millis();

	while(1)
	{
		if(millis()-pre_time >= 100)
		{

		pre_time = millis();
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);


		}

		cliMain();
}

}
