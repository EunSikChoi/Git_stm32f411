/*
 * w5500.h
 *
 *  Created on: 2021. 3. 26.
 *      Author: 82109
 */

#ifndef SRC_COMMON_HW_INCLUDE_W5500_INIT_H_
#define SRC_COMMON_HW_INCLUDE_W5500_INIT_H_

#include "stm32f4xx_hal.h"

uint8_t W5500_dataRead();
void W5500_dataSend(uint8_t Txdata);
void W5500_CS_select();
void W5500_CS_deselect();
int isLinked(void);
void W5500_hw_Rst(void);
void W5500_info();
uint8_t W5500_init();

#endif /* SRC_COMMON_HW_INCLUDE_W5500_INIT_H_ */
