/*
 * modbus.h
 *
 *  Created on: 2021. 3. 29.
 *      Author: 82109
 */

#ifndef SRC_COMMON_HW_INCLUDE_MODBUS_H_
#define SRC_COMMON_HW_INCLUDE_MODBUS_H_

#include "hw_def.h"
#include "uart.h"

#ifdef _USE_HW_W5500

typedef unsigned int        WORD;
typedef unsigned						BOOL;
typedef unsigned char				BYTE;
typedef char								SBYTE;
typedef int									SWORD;
typedef unsigned int				WORD;
typedef long								SLONG;
typedef unsigned long				LONG;
typedef float								REAL;

#define RX0_SIZE 50
#define TX0_SIZE 40
#define RX1_SIZE 260
#define TX1_SIZE 260

#define RX2_SIZE 20  //260�ε� ������ �޴� �����Ͱ� 8����.. 20���� �۰���//
#define TX2_SIZE 260


BOOL CommEOTReceived(BYTE bPort);
BYTE* CommGetRxBuff(BYTE bPort);
BYTE CommGetRxSize(BYTE bPort);
BYTE* CommGetTxBuff(BYTE bPort);

BOOL CommGetRxFlag(BYTE bPort, uart_tbl_t *p_uart_tbl);
BOOL CommGetTxFlag(BYTE bPort, uart_tbl_t *p_uart_tbl);

void CommClearTxBuff(BYTE bPort);
void CommErrSetCnt(void);
WORD CommErrGetCnt(void);

#define TRUE 1
#define FALSE 0

#define PORT_SCIC 0    // ������ 0 ����
#define wSelCommMode 1 // HMI == 1 ������ 1 ����
#define REMOTE_MODE 1
#define HMI_MODE 0

BYTE	bRx2Buff[RX2_SIZE];	// ���� ����  //
BYTE	bTx2Buff[TX2_SIZE]; // �۽� ����  //


#define	MAKE_WORD(hi, lo) (WORD)(((WORD)(LO_BYTE(hi)) << 8) + (lo & 0xff))

#define	HI_BYTE(data)	((BYTE)(((data) >> 8) & 0xff))
#define	LO_BYTE(data)	((BYTE)((data) & 0xff))

#define ACK		0x06
#define EOT		0x04


#define DEFAULT_BAUD_IDX	3				/* 9600 */

typedef struct
{
	BYTE	bRx2Buff[RX2_SIZE];
	BYTE	bTx2Buff[TX2_SIZE];
} data_t;

typedef struct {  //2011.10.24
	WORD	bBaudIdx:8;
	WORD	bRxIdx:8;
	WORD	bTxIdx:8;
	WORD	bRxSize:8;
	WORD	bTxSize:8;

	WORD	fTx:1;
	WORD	fEOTReceived:1;
	WORD	fIsBroadCastID:1;
	WORD	fIsReceiveState:1;

	WORD	bReserved:4;
	WORD	bCntEOF;
} COMM_PORT_DATA;

typedef struct {
	WORD	wRcvFrameNum;
	WORD	wNAKFrameNum;
	WORD	wErrorFrameNum;
} COMM_PORT_STATE;

COMM_PORT_DATA commPortData[4];
COMM_PORT_STATE commPortState[4];

#endif

#endif /* SRC_COMMON_HW_INCLUDE_MODBUS_H_ */
