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

#define _DEF_FIRMWATRE_VERSION    "V211013R2"
#define _DEF_BOARD_NAME           "STM32G431"


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
#define _USE_HW_RTC
#define _USE_HW_RESET

#define _USE_HW_UART
#define      HW_UART_MAX_CH         3

#define _USE_HW_CLI
#define      HW_CLI_CMD_NAME_MAX    16
#define      HW_CLI_CMD_LIST_MAX    16
#define      HW_CLI_LINE_HIS_MAX    4
#define      HW_CLI_LINE_BUF_MAX    64

#define _USE_HW_BUTTON
#define      HW_BUTTON_CH           1

#define _USE_HW_GPIO
#define      HW_GPIO_MAX_CH  				4

#define _PIN_GPIO_SDCARD_DETECT     0 // ch 0 : SD DC

#define _USE_HW_SPI
#define      HW_SPI_MAX_CH    1  //1: Wiz5500 //

#define _USE_HW_LOG
#define      HW_LOG_CH              _DEF_UART1 // CLI채널과 같아야함 //
#define      HW_LOG_BOOT_BUF_MAX    1024
#define      HW_LOG_LIST_BUF_MAX    2048

#define FLASH_USER_START_ADDR (uint32_t)0x0800EFEA
#define FLASH_USER_LENGTH 32

#define _USE_HW_W5500

#endif /* SRC_HW_HW_DEF_H_ */
