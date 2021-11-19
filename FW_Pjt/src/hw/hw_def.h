/*
 * hw_def.h
 *
 *  Created on: Nov 15, 2021
 *      Author: 82109
 */

#ifndef SRC_HW_HW_DEF_H_
#define SRC_HW_HW_DEF_H_




#include "def.h"
#include "bsp.h"


#define _USE_HW_CDC
#define _USE_HW_FLASH
#define _USE_HW_LED
#define _USE_HW_RTC
#define _USE_HW_RESET
#define _USE_HW_USB
#define _USE_HW_CLI
#define _USE_HW_USB
#define _USE_HW_SD
#define _USE_HW_FATFS
#define _USE_HW_FLASH

#define _USE_HW_UART
#define      HW_UART_MAX_CH         1

#define _USE_HW_CLI
#define      HW_CLI_CMD_NAME_MAX    16
#define      HW_CLI_CMD_LIST_MAX    16
#define      HW_CLI_LINE_HIS_MAX    4
#define      HW_CLI_LINE_BUF_MAX    32

#define _USE_HW_BUTTON
#define      HW_BUTTON_CH           1

#define _USE_HW_GPIO
#define      HW_GPIO_MAX_CH  				1

#define _PIN_GPIO_SDCARD_DETECT     0 // ch 0 : SD DC

#define FLASH_USER_START_ADDR (uint32_t)0x0800EFEA
#define FLASH_USER_LENGTH 32


#endif /* SRC_HW_HW_DEF_H_ */
