/*
 * ap.c
 *
 *  Created on: Nov 15, 2021
 *      Author: 82109
 */


#include "ap.h"
#include "spi.h"
#include "loopback.h"
#include "log.h"

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
	 uartOpen(_DEF_UART2, 57600);
	 uartOpen(_DEF_UART3, 57600);
}

UART_HandleTypeDef huart2;

uint16_t count;

void apMain(void)
{
  uint32_t pre_time, pre_baud;
  uint32_t led_blink_time = 1000;
  uint8_t loopback_Cnt;
	uint8_t buffer[256]= {0,};

  pre_baud = uartGetBaud(_DEF_UART2);
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
	#if 1
	  if(uartAvailable(_DEF_UART3) > 0)
	  {
			uint8_t rx_data;
			pre_time = millis();

			rx_data = uartRead(_DEF_UART3);

			uartPrintf(_DEF_UART3, "RxData1111222233334444444444555555555555 : %c 0x%x\n", rx_data, rx_data);

			uartPrintf(_DEF_UART3, "Time %d ms\n", millis()-pre_time );

			count++;
			logPrintf("No.%d  Time Date   \t: %s: %s \r\n", count, __TIME__,__DATE__ );


	  }
	#endif


	#ifdef _USE_HW_W5500

		 if( loopback_Cnt++ > 10)
		 {
			 loopback_Cnt = 0;

			 for(uint8_t index = 0; index < 4; index++)
			 {
					loopback_tcps(index ,buffer,502);
			 }
		 }
	#endif

		cliMain();

	}

}

