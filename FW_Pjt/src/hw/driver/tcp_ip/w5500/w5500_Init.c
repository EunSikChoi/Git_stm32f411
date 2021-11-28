/*
 * w5500.c
 *
 *  Created on: 2021. 3. 26.
 *      Author: 82109
 */


#include "w5500_Init.h"
#include "main.h"
#include "socket.h"
#include "cli.h"


#ifdef _USE_HW_W5500

extern SPI_HandleTypeDef hspi1;
extern wiz_NetInfo gWIZNETINFO;


static uint32_t randomKey;

#ifdef _USE_HW_CLI
extern  void cliW5500(cli_args_t *args);
#endif


uint8_t W5500_dataRead() // SPI 데이터 수신
{
	uint8_t Dummy = 0xFF;
	uint8_t RxData;
	while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);
	HAL_SPI_TransmitReceive(&hspi1,&Dummy,&RxData,1,10); // SPI는 수신시 더미데이터를 보내줘야 클럭이 생성되고 슬레이브가 값을 준다.
	return RxData;
}
void W5500_dataSend(uint8_t Txdata) // SPI 데이터 송신
{
	 while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);
	 HAL_SPI_Transmit_DMA(&hspi1, (uint8_t *) &Txdata, 1);
  // HAL_SPI_Transmit(&hspi1, (uint8_t *) &Txdata, 1, 10);
}

void W5500_CS_select()
{
	//GPIOA->BRR = GPIO_PIN_3; // CS핀을 Low
	gpioPinWrite(1, _DEF_LOW);
}

void W5500_CS_deselect()
{
	//GPIOA->BSRR = GPIO_PIN_3; // CS핀을 High
	gpioPinWrite(1, _DEF_HIGH);
}

int isLinked(void)
{
	return (getPHYCFGR() & PHYCFGR_LNK_ON);
}

void W5500_hw_Rst(void) // 하드웨어 리셋
{
  uint32_t tempTick;
  gpioPinWrite(2, _DEF_LOW);
  tempTick = millis();  //HAL_GetTick
  while((millis() - tempTick) < 1 ); // 1ms 가 지나면
  gpioPinWrite(2, _DEF_HIGH);// RST 핀을 High 로.

#ifdef _USE_HW_CLI
	   cliAdd("W5500", cliW5500);
#endif

}

void W5500_info()
{
	wizchip_getnetinfo(&gWIZNETINFO); // W5500 현재 상태 가져오기
}

uint8_t W5500_init() // 초기화 함수
{
	uint8_t W5500SockBufSize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};
	uint32_t tickStart;

	spiBegin(_DEF_SPI1);//init SPI1//

	/* Register Call back function */
	reg_wizchip_cs_cbfunc(W5500_CS_select, W5500_CS_deselect);

	reg_wizchip_spi_cbfunc(W5500_dataRead, W5500_dataSend);

/*
		if (ctlwizchip(CW_INIT_WIZCHIP, (void*) W5500SockBufSize) == -1) {
	//		printf("WIZCHIP Initialized fail.\r\n");
			 sprintf(UartTx1, "Tx333.\r\n");
			 Userprintf(UartTx1);
			while (1);
		}

		 sprintf(UartTx1, "Tx4.\r\n");
		 Userprintf(UartTx1);



*/

	if(ctlwizchip(CW_INIT_WIZCHIP,(void*)W5500SockBufSize) == -1)
		{
			//printf("W5500 initialized fail.\r\n");
			return -1; // �ʱ�ȭ ����
		}
		/*Set network information by default*/



		wizchip_setnetinfo(&gWIZNETINFO);

		tickStart = millis();

		do{ // PHY ����üũ
			randomKey++;
			if((millis() - tickStart) > 3000 )
			{
				//printf("Ethernet Link timeout.\r\n");
				return -1; // �̴��� Ÿ�Ӿƿ� ����
			}
		}while(!isLinked());
		return 1;



}





#endif

