/*
 * uart.c
 *
 *  Created on: 2021. 11. 16.
 *      Author: 82109
 */






#include "uart.h"
#include "cdc.h"
#include "qbuffer.h"
#include "cli.h"
#include "modbus.h"
#include "ModProt.h"


uart_tbl_t uart_tbl[UART_MAX_CH];

static bool is_open[UART_MAX_CH];

static qbuffer_t qbuffer[UART_MAX_CH];

static uint8_t rx_buf[256]; 	 	  		// qbuffer buf
static uint8_t rx_data[UART_MAX_CH];  // rx INT buf


UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;

#ifdef _USE_HW_CLI

static void cliUart(cli_args_t *args);

#endif

data_t data;


bool uartInit(void)
{
  for (int i =0; i < UART_MAX_CH; i++)
  {
		uart_tbl[i].is_open 	 = false;
		uart_tbl[i].baud 			 = 57600;
		uart_tbl[i].is_tx_done 	= true;
		uart_tbl[i].is_tx_error = false;
		uart_tbl[i].ch				  = i;
  }


	#ifdef _USE_HW_CLI

		cliAdd("uart",cliUart);

	#endif

  return true;
}


bool uartOpen(uint8_t ch, uint32_t baud)
{

  bool ret = false;
  switch(ch)
  {
    case _DEF_UART1:
			 uart_tbl[ch].is_open = true;
			 uart_tbl[ch].baud    = baud;
			ret = true;
	  break;

    case _DEF_UART2:

    	uart_tbl[ch].p_huart 			= &huart1;
    	uart_tbl[ch].is_open 		 	= true;

			huart1.Instance 				 = USART1;
      huart1.Init.BaudRate	 	 = baud;
      huart1.Init.WordLength 	 = UART_WORDLENGTH_8B;
      huart1.Init.StopBits 		 = UART_STOPBITS_1;
      huart1.Init.Parity 			 = UART_PARITY_NONE;
      huart1.Init.Mode 				 = UART_MODE_TX_RX;
      huart1.Init.HwFlowCtl 	 = UART_HWCONTROL_NONE;
      huart1.Init.OverSampling = UART_OVERSAMPLING_16;

  		_485_RX_ENB;

      if (HAL_UART_Init(uart_tbl[ch].p_huart) != HAL_OK)
			{
      	ret = false;
			}
			else
			{
				 __HAL_UART_ENABLE_IT(uart_tbl[ch].p_huart, UART_IT_RXNE); //UART RX INT Enable//
				 ret = true;
			}
		break;

  }

  return ret;
}


uint32_t uartAvailable(uint8_t ch)
{
  uint32_t ret = 0;

  switch(ch)
  {
    case _DEF_UART1:
    	ret = cdcAvailable();
    break;

    case _DEF_UART2:

		break;
  }

  return ret;
}


uint8_t  uartRead(uint8_t ch)
{
  uint8_t ret = 0;

   switch(ch)
   {
     case _DEF_UART1:
       ret = cdcRead();
     break;

     case _DEF_UART2:
    	 data.bRx2Buff[BufIndexGet()] = (huart1.Instance->DR & 0x00FF);
    	 ret = true;
     break;

   }

   return ret;

}


uint32_t uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length)
{
  uint32_t ret = 0;
  HAL_StatusTypeDef status;
  switch(ch)
  {
    case _DEF_UART1:
    	ret = cdcWrite(p_data, length);
    break;

    case _DEF_UART2:

    	_485_TX_ENB;
    	//status =  HAL_UART_Transmit_IT(&huart1, p_data, length);
    	status =  HAL_UART_Transmit_DMA(&huart1, p_data, length);

			if(status == HAL_OK)
			{
				ret = true;
			}
		break;
  }

  return ret;

}


uint32_t uartPrintf(uint8_t ch, char *fmt, ...)  // #include <stdarg.h> 추가해야 가능 //
{
  char buf[256];
  va_list args;  						// memory address , args 이름으로 포인트 설정 // 포인터 변수 생성
  int len;
  uint32_t ret;

  va_start(args, fmt); 					// 가변인자중 첫번째 인자의 주소를 가르킴 // 즉 시작 주소 지정 //
  len = vsnprintf(buf, 256, fmt, args); // 가변인자 문자 출력  //(fmt에 만들어진 내용이 담길 버퍼, 최대크키 , 포멧, 가변인자 시작주소) // 리턴값은 길이 //
  ret = uartWrite(ch, (uint8_t *)buf, len);

  va_end(args); 						//args 목록 초기화

  return ret;

}

uint32_t uartGetBaud(uint8_t ch)
{
  uint32_t ret = 0;

  switch(ch)
  {
    case _DEF_UART1:
      ret = cdcGetBaud();
    break;

    case _DEF_UART2:
    	ret = huart1.Init.BaudRate;
		break;
  }

  return ret;


}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART1)
  {

  }

}

uint8_t Rx_INT_Cnt, Tx_INT_Cnt;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART1)
  {
  	//-- Check for INT Counter -- //
    //  Rx_INT_Cnt++;
    //  cliPrintf("RXCP %d\n",Rx_INT_Cnt );

  }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	 if (huart->Instance == USART1)
	 {
		 //-- Check for INT Counter -- //
		 //   Tx_INT_Cnt++;
		 //   cliPrintf("TXCP %d\n",Tx_INT_Cnt );

	   __HAL_UART_CLEAR_FLAG(&huart1,UART_FLAG_RXNE);
		 __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);

		 _485_RX_ENB;

	 }
}


void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_DMA2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA15     ------> USART1_TX
    PB3     ------> USART1_RX
    */

    /* DMA2_Stream7_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 4, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);

    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_TX Init */
    hdma_usart1_tx.Instance = DMA2_Stream7;
    hdma_usart1_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA15     ------> USART1_TX
    PB3     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_15);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

static void cliUart(cli_args_t *args)
{

  bool ret = false;
  uint32_t baudrate = 0 ;

  if(args->argc == 1 && args->isStr(0, "info") == true) //여기서 설정한 toggle 과 동일 문자가 입력하면 아래 Loop가 실행됨//
  {

  	uartPrintf(_DEF_UART1, "Uart(485) BaudRate : %d\n", uartGetBaud(_DEF_UART2));
  	uartPrintf(_DEF_UART1, "ID    : DEC: %d, HEX: %x\n", STATION_ID_HMI, STATION_ID_HMI);

  	ret = true;
  }

  if(args->argc == 2 && args->isStr(0, "set") == true) //여기서 설정한 toggle 과 동일 문자가 입력하면 아래 Loop가 실행됨//
  {

  	baudrate  = (uint32_t) args->getData(1);

  	uartOpen(_DEF_UART2, baudrate);

  	ret = true;
  }

  if(args->argc == 1 && args->isStr(0, "reset") == true) //여기서 설정한 toggle 과 동일 문자가 입력하면 아래 Loop가 실행됨//
  {

    HAL_UART_Init(uart_tbl[1].p_huart);
  	uartPrintf(_DEF_UART1, "Uart(485) reset OK\n");

  	ret = true;
  }



  if(ret != true )
  {

    cliPrintf("uart info\n");
    cliPrintf("uart set baud\n");
  }

}


