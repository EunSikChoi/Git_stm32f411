/*
 * ap.c
 *
 *  Created on: Nov 15, 2021
 *      Author: 82109
 */


#include "ap.h"


typedef struct
{
  uint32_t pre_time;
  uint16_t x_time;
  uint8_t  mode;
} args_t;

void sdMain(args_t *p_args);


void apInit(void)
{
		cliOpen(_DEF_UART1, 57600);
}

void apMain(void)
{
  uint32_t pre_time;
  pre_time = millis();

  args_t args;

  args.mode = 0;
  args.x_time = 0;
  args.pre_time = millis();

	while(1)
	{
		if(millis()-pre_time >= 100)
		{

			pre_time = millis();
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

		}

		cliMain();

    sdMain(&args);
	}

}

void sdMain(args_t *p_args)
{
  sd_state_t sd_state;


  sd_state = sdUpdate();
  if (sd_state == SDCARD_CONNECTED)
  {
    cliPrintf("\nSDCARD_CONNECTED\n");
  }
  if (sd_state == SDCARD_DISCONNECTED)
  {
    cliPrintf("\nSDCARD_DISCONNECTED\n");
  }
}
