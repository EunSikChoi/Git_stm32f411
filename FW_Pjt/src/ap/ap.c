/*
 * ap.c
 *
 *  Created on: Nov 15, 2021
 *      Author: 82109
 */


#include "ap.h"
#include "spi.h"

const uint8_t spi_ch1 = _DEF_SPI1; // Wiz5500

void apInit(void)
{
		cliOpen(_DEF_UART1, 57600);
}

void apMain(void)
{
  uint32_t pre_time;
  uint32_t led_blink_time = 1000;
  uint8_t buf[0];

  pre_time = millis();

  if(resetGetCount() >= 2)
  {
    led_blink_time = 200;
  }


	while(1)
	{
		if(millis()-pre_time >= led_blink_time) //
		{

			pre_time = millis();
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

			buf[0] = 0x02;
			gpioPinWrite(1, _DEF_LOW);
			spiTransfer(spi_ch1, buf, buf, 1, 100);
			gpioPinWrite(1, _DEF_HIGH);

		}

		cliMain();

	}

}

