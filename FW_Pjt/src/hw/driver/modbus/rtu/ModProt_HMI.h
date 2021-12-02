/*
 * ModProt_HMI.h
 *
 *  Created on: 2021. 5. 6.
 *      Author: 82109
 */

#ifndef SRC_COMMON_HW_INCLUDE_MODPROT_HMI_H_
#define SRC_COMMON_HW_INCLUDE_MODPROT_HMI_H_



#include "ModProt.h"
#include "uart.h"


void RtuRxTxloop(uart_tbl_t *p_uart_tbl);
void RtuLoopBack(uart_tbl_t *p_uart_tbl);

static BYTE bTxFrameSize;

static void ErrorParse (BYTE *pBRx, BYTE *pBTx, BYTE bErr);
static WORD CRC16 (BYTE *buf, BYTE numbytes);


static BYTE SendRTUFrame(BYTE* pRes, BYTE bSize);


static BYTE CommandRead (RDCMD_FRAME* pCmd, RDRES_FRAME* pRes);
static BYTE CommandHoldReg(RDCMD_FRAME* pCmd, RDRES_FRAME* pRes);
static BYTE CommandWriteCoil (WRCMD_FRAME* pCmd, WRRES_FRAME* pRes);
static BYTE CommandWriteOne (WRCMD_FRAME* pCmd, WRRES_FRAME* pRes);
static BYTE CommandWriteMany (WRCMD_FRAME_MANY* pCmd, WRRES_FRAME_MANY* pRes);
static void CmdSingleWrite(BYTE*, BYTE, WORD, WORD);
static void CmdSingleRead(BYTE*, BYTE, WORD);


void BufIndexSet(void);
WORD BufIndexGet(void);
WORD BufIndexClear(void);







/////////0x04 Read Input ���� �ּ� /////

#define RD_INPUT_WGRP1_INIT_OFF	0
#define RD_INPUT_WGRP1_END_OFF	79//45 //21
#define RD_INPUT_WGRP_MAX_NUM	 80 //46   //22

///////////////////////////////////////

#define RD_INPUT_FGRP1_INIT_OFF	60
#define RD_INPUT_FGRP1_END_OFF	63
#define RD_INPUT_FGRP_MAX_NUM	2


///////0x04 Read Hold ���� �ּ� ///////
////���� 500(0x03) �д°Ÿ� -> 1000������ ���� ���� �а� ������//

#define RD_HOLD_WGRP1_INIT_OFF	1000
#define RD_HOLD_WGRP1_END_OFF	1199
#define RD_HOLD_WGRP_MAX_NUM	199

//////////////////////////////////////


#define RD_HOLD_FGRP1_INIT_OFF	700
#define RD_HOLD_FGRP1_END_OFF	701
#define RD_HOLD_FGRP_MAX_NUM	1

#define WR_COIL_WGRP1_INIT_OFF	200
#define WR_COIL_WGRP1_END_OFF	205
#define WR_COIL_WGRP_MAX_NUM	6

//0x06, 0x10 // Write �ּ� ����
#define WR_REG_WGRP1_INIT_OFF	1000
#define WR_REG_WGRP1_END_OFF	1199
#define WR_REG_WGRP_MAX_NUM		199
/////////////////////////////////



#define WR_REG_FGRP1_INIT_OFF	710
#define WR_REG_FGRP1_END_OFF	711
#define WR_REG_FGRP_MAX_NUM		1

#define MB_TIMEOUT		5 // 10msec

#define MB_TYPE_F		0x55
#define MB_TYPE_W		0xAA


static WORD wDBG_CommandRead;
static BYTE wDBG_Data0;
static BYTE wDBG_Data1;
static BYTE wDBG_Data2;
static BYTE wDBG_Data3;
static float fDBG_Data3 = 0.2;
static float fDBG_Para;

typedef union {
	float fData;
	WORD  wData[2];
} FLOAT_DATA;

typedef struct {
	WORD baud;
	BYTE ch2;	// Char. time out is 1.5Char time
	BYTE ch4;	// Response time out is 4Char time
} BAUD_DATA;

enum baudRatetype {
	BAUD_1200BPS,
	BAUD_2400BPS,
	BAUD_9600BPS,
	BAUD_19200BPS,
	BAUD_38400BPS,
	BAUD_56000BPS,
	BAUD_115200BPS
};






#endif /* SRC_COMMON_HW_INCLUDE_MODPROT_HMI_H_ */
