/*
 * ap.c
 *
 *  Created on: Nov 15, 2021
 *      Author: 82109
 */


#include "ap.h"
#include "spi.h"
#include "loopback.h"

const uint8_t spi_ch1 = _DEF_SPI1; // Wiz5500

#ifdef _USE_HW_W5500
BYTE    bTxTcpFrameSize;
BYTE    bTxTcpFrameSend;
WORD wSelCmdMode;
WORD wRefModeVoltage, wRefModeVoltage1, wRefModeVoltage2, wRefModeVoltage3, wRefModeVoltage4, wRefModeVoltage5;
WORD wRefModeVoltage6, wRefModeVoltage7, wRefModeVoltage8, wRefModeVoltage9, wRefModeVoltage10;
char UartTx[32] ={0};

wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x08, 0xdc, 0xab, 0xcd, 0x49},

            .ip = {192, 168, 0, 125},

            .sn = {255, 255, 255, 0},

            .gw = {192, 168, 0, 1},

            .dns = {8, 8, 8, 8},

            .dhcp = NETINFO_STATIC};
#endif

void apInit(void)
{
		cliOpen(_DEF_UART1, 57600);
}

void apMain(void)
{
  uint32_t pre_time;
  uint32_t led_blink_time = 1000;


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

		}

	#ifdef _USE_HW_W5500
		uint8_t loopback_Cnt;

		 if( loopback_Cnt++ > 4)
		 {
			 loopback_Cnt = 0;
			uint8_t buffer[256]= {0,};

			 for(uint8_t index = 0; index < 4; index++)
			 {
					loopback_tcps(index ,buffer,502);
			 }
		 }
	#endif

		cliMain();

	}

}

