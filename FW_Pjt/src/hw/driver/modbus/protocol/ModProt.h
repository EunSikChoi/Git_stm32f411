/*
 * ModProt.h
 *
 *  Created on: 2021. 3. 29.
 *      Author: 82109
 */

#ifndef SRC_COMMON_HW_INCLUDE_MODPROT_H_
#define SRC_COMMON_HW_INCLUDE_MODPROT_H_

#include "ModProt.H"
#include <stdio.h>
#include "modbus.h"
#include "uart.h"



#ifdef _USE_HW_W5500

//#include <sys/_intsup.h>
//#include <sys/_stdint.h>


#define MODBUS_BROADCAST	0

//For HMI
#define M16             0xA001
#define STATION_ID_HMI  101 // SCI_B // KEYPAD or HMI
#define STATION_ID  0x30    //SCI_C// Ex Ethernet


#define RD_HOLD_CMD		0x03
#define RD_INPUT_CMD		0x04
#define WR_COIL_CMD		0x05
#define WR_SINGLE_CMD		0x06
#define WR_MANY_CMD		0x10

#define MAX_IN_REG		0x7E // modbus maximum quantity : 0x7D


enum {
	COM_NO_ERR,
	ILLEGAL_FUNCTION,		// used
	ILLEGAL_DATA_ADDRESS,	// used
	ILLEGAl_DATA_VALUE,		// used
	SLAVE_DEVICE_FAILURE,	// Not used
	ACKNOWLEDGE,			// Not used
	SLAVE_DEVICE_BUSY,		// used
	NEGATIVE_ACKNOWLEDGE,	// Not used
	MEMORY_PARATY_ERROR,	// Not used
	WRITE_PERMITION_ERROR = 20,	// User Define
	UNDEFINE_CONDITION    = 21	// User Define
};

#define RDRES_SIZE(i)	(3 + i*2)
#define WRCMD_SIZE(i)	(8 + i*4 + 2)
#define REGRES_SIZE(i)	(sizeof(REGRES_FRAME))
#define EXERES_SIZE(i)	(4 + i*4 )

#define ERROR_SIZE		3



typedef struct {
	BYTE	bNumber;
	BYTE	bCmd;
} GEN_FRAME;

typedef struct {
	BYTE	bNumber;    //Slave ID
	BYTE	bCmd;       //Function Code
	BYTE	bAddr[2];   //Starting Address
	BYTE	bCnt[2];    //Register Count
} RDCMD_FRAME;

typedef struct {
	BYTE	bNumber;
	BYTE	bCmd;
	BYTE	bByteCnt;
	BYTE	bData[50][2];		/* Maximun Data Size *///????
} RDRES_FRAME;

typedef struct {
	BYTE	bNumber;
	BYTE	bCmd;
	BYTE	bAddr[2];
	BYTE	bData[2];		/* Maximun Data Size */
} WRCMD_FRAME;

typedef struct {
	BYTE	bNumber;
	BYTE	bCmd;
	BYTE	bAddr[2];
	BYTE	bData[2];		/* Maximun Data Size */
} WRRES_FRAME;

typedef struct {
	BYTE	bNumber;
	BYTE	bCmd;
	BYTE	bAddr[2];
	BYTE	bCnt[2];
	BYTE	bByteCnt;
	BYTE	bData[50][2];		/* Maximun Data Size */
} WRCMD_FRAME_MANY;

typedef struct {
	BYTE	bNumber;
	BYTE	bCmd;
	BYTE	bAddr[2];
	BYTE	bCnt[2];		/* Maximun Data Size */
} WRRES_FRAME_MANY;

typedef struct {
	BYTE	bNumber;
	BYTE	bCmd;
	BYTE	bExCode;
} ERROR_FRAME;

void RTU485Loop (BYTE bPort);
void RTUtcpLoop(uint8_t* bRXbuf, uint16_t rxLen, uint8_t* bTXbuf);


//void HmiLoop(BYTE bPort, WORD wSel);
void HmiLoop(BYTE bPort, WORD wSel, uart_tbl_t *p_uart_tbl);

#endif

#endif /* SRC_COMMON_HW_INCLUDE_MODPROT_H_ */
