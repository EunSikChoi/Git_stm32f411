/*
 * rtc.c
 *
 *  Created on: 2021. 11. 19.
 *      Author: 82109
 */


#include "rtc.h"

RTC_HandleTypeDef hrtc;

bool rtcInit(void)
{
  bool ret = true;

  hrtc.Instance					 		= RTC;
  hrtc.Init.HourFormat 	 		= RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv 		= 127;
  hrtc.Init.SynchPrediv  		= 255;
  hrtc.Init.OutPut 			 		= RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity 	= RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType 			= RTC_OUTPUT_TYPE_OPENDRAIN;

  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  return ret;
}

void rtcbackupRegWrite(uint32_t index , uint32_t data)
{
	HAL_RTCEx_BKUPWrite(&hrtc, index, data);
}

uint32_t rtcbackupRegRead(uint32_t index)
{
	return HAL_RTCEx_BKUPRead(&hrtc, index);
}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */
    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
}
