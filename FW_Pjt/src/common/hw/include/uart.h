/*
 * uart.h
 *
 *  Created on: 2021. 11. 16.
 *      Author: 82109
 */

#ifndef SRC_COMMON_HW_INCLUDE_UART_H_
#define SRC_COMMON_HW_INCLUDE_UART_H_



#include "hw_def.h"
#include "uart.h"
#include "qbuffer.h"

#ifdef _USE_HW_UART

#define UART_MAX_CH          HW_UART_MAX_CH



bool     uartInit(void);
bool     uartOpen(uint8_t ch, uint32_t baud);
uint32_t uartAvailable(uint8_t ch);
uint8_t  uartRead(uint8_t ch);
uint32_t uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length);
uint32_t uartPrintf(uint8_t ch, char *fmt, ...);
uint32_t uartGetBaud(uint8_t ch);

typedef struct
{

  bool is_open;
  bool is_tx_done;
  bool is_tx_error;
  uint8_t ch;
  uint32_t baud;
  qbuffer_t qbuffer;
#if UART_MAX_CH > 1
  UART_HandleTypeDef *p_huart;
  DMA_HandleTypeDef  *p_hdma_rx;
  DMA_HandleTypeDef  *p_hdma_tx;
#endif

}uart_tbl_t;


#endif

#endif /* SRC_COMMON_HW_INCLUDE_UART_H_ */

