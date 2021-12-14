/*
 * def.h
 *
 *  Created on: Nov 15, 2021
 *      Author: 82109
 */

#ifndef SRC_COMMON_DEF_H_
#define SRC_COMMON_DEF_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>


#define _DEF_UART1     0
#define _DEF_UART2     1
#define _DEF_UART3     2
#define _DEF_UART4     3

#define _DEF_BUTTON1   		 	0
#define _DEF_BUTTON2   		 	1
#define _DEF_BUTTON3   		 	2
#define _DEF_BUTTON4   		 	3

#define _DEF_INPUT    		 	0
#define _DEF_INPUT_PULLUP  	 	1
#define _DEF_INPUT_PULLDOWN  	2
#define _DEF_OUTPUT    		 	3
#define _DEF_OUTPUT_PULLUP   	4
#define _DEF_OUTPUT_PULLDOWN 	5

#define _DEF_LOW			 	0
#define _DEF_HIGH			 	1

#define _DEF_SPI1             0
#define _DEF_SPI2             1
#define _DEF_SPI3             2
#define _DEF_SPI4             3

#define _485_TX_ENB  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1,  GPIO_PIN_SET)
#define _485_RX_ENB  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1,  GPIO_PIN_RESET)

#define __485_CLI_TX_ENB  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,  GPIO_PIN_SET)
#define __485_CLI_RX_ENB  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,  GPIO_PIN_RESET)


#endif /* SRC_COMMON_DEF_H_ */
