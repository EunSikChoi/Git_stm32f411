/*
 * ModProt_HMI.c
 *
 *  Created on: 2021. 5. 6.
 *      Author: 82109
 */





#include "modbus.H"
#include "ModProt.H"
#include "RS_Para.H"
#include "ModProt_HMI.H"
#include "stm32f4xx_hal.h"
#include "cli.h"



UART_HandleTypeDef huart1;
//DMA_HandleTypeDef hdma_usart6_tx;


extern WORD wSelCmdMode;

WORD wHmiErrCnt     = 0;
WORD wHmiResetCnt = 0;
WORD g_wFlagHmiErr  = 0;
WORD wBufferIndex_C;
static uint32_t pre_time_rtu;





/***********************************************/
/* Operation List for                          */
/* [0x04] Read Input Register                  */
/***********************************************/
static pFUNCParaI wInputRegTBL1[] = {

/*00*/	paraSoftware_ver, //Softwarw Ver
/*01*/	paraParameter_ver, //Parameter Ver
/*02*/  paraSourceInfo,
/*03*/  paraSysCapaInfo,
/*04*/  paraReserved1,
/*05*/  paraReserved1,
/*06*/  paraReserved1,
/*07*/  paraReserved1,
/*08*/  paraReserved1,
/*09*/  paraReserved1,
/*10*/	paraInfoDoStatus,
/*11*/	paraInfoDiStatus,
/*12*/	paraProtDigitalErrStatus1,
/*13*/	paraProtDigitalErrStatus2,
/*14*/	paraProtAnalogErrStatus1,
/*15*/	paraProtAnalogErrStatus2,
/*16*/	paraProtHwErrStatus,
/*17*/ 	paraProtExtErrStatus,
/*18*/	parawProtWarnStatus,
/*19*/	paraOperation_State,
/*20*/	paraCharge_State,
/*21*/  paraControl_Mode_State,
/*22*/	paraGrid_Uan,
/*23*/	paraGrid_Ubn,
/*24*/	paraGrid_Ucn,
/*25*/	paraInv_Ian,
/*26*/	paraInv_Ibn,
/*27*/	paraInv_Icn,
/*28*/	paraSysWe,
/*29*/	pararInv_Kw_rms,
/*30*/  pararInv_Kva_rms,
/*31*/	parapowerFactor,
/*32*/	pararVdcCheck,
/*33*/	paraDcVTG,
/*34*/  pararIbatt_rms,
/*35*/  pararVdc_Kw_rms,
/*36*/	paraPCS_Temper_1,
/*37*/	paraPCS_Temper_2,
/*38*/	paraPCS_Temper_3,
/*39*/	paraPCS_Temper_4,
/*40*/	paraPCS_Temper_5,
/*41*/	paraPCS_Temper_6,
/*42*/	paraPCS_Temper_7,
/*43*/	paraAcChaPwrKwh,
/*44*/	paraAcChaPwrMwh,
/*45*/	paraAcChaPwrGwh,
/*46*/	paraAcDisChaPwrKwh,
/*47*/	paraAcDisChaPwrMwh,
/*48*/	paraAcDisChaPwrGwh,
/*49*/	paraDcChaPwrKwh,
/*50*/	paraDcChaPwrMwh,
/*51*/	paraDcChaPwrGwh,
/*52*/	paraDcDisChaPwrKwh,
/*53*/	paraDcDisChaPwrMwh,
/*54*/	paraDcDisChaPwrGwh,
/*55*/  paraErrAllStatus,
/*56*/	paraDailyChaPwrKwh,
/*57*/	paraDailyDisCharPwrKwh,
/*58*/	paraTwoagoChaPwrKwh,
/*59*/	paraTwoagoDisChaPwrKwh,
/*60*/  paraDccbState,
/*61*/  paraHour0708Kwh,
/*62*/  paraHour0809Kwh,
/*63*/  paraHour0910Kwh,
/*64*/  paraHour1011Kwh,
/*65*/  paraHour1112Kwh,
/*66*/  paraHour1213Kwh,
/*67*/  paraHour1314Kwh,
/*68*/  paraHour1415Kwh,
/*69*/  paraHour1516Kwh,
/*70*/  paraHour1617Kwh,
/*71*/  paraHour1718Kwh,
/*71*/  paraHour1819Kwh,
/*73*/  paraReserved1,
/*74*/  paraReserved1,
/*75*/  paraReserved1,
/*76*/  paraReserved1,
/*77*/  paraReserved1,
/*78*/  paraReserved1,
/*79*/  paraReserved1


};


static pFUNCParaF fInputRegTBL1[] = {
/*60*/	paraReserved1,	/* float */
/*61*/	paraReserved1,	/* float */
};


/***********************************************/
/* Operation List for                          */
/* [0x03] Read Holding Register                */
/***********************************************/
static pFUNCParaRWI wHoldingRegTBL1[] = {

/*1000*/ paraKwh_Reset,
/*1001*/ paraINV_errReset,
/*1002*/ paraRemote_Sel,
/*1003*/ paraEmergencyStop,
/*1004*/ parawAutoCVMode,
/*1005*/ paraRefModeVoltage,
/*1006*/ paraCP_Step_Flag,
/*1007*/ paraTarget_Gradient,
/*1008*/ paraRunCmd,
/*1009*/ parafStateControl,
/*1010*/ paraiActiveCmd,
/*1011*/ paraiCVCmd,
/*1012*/ paraiCCCmd,
/*1013*/ paraiReActiveCmd,
/*1014*/ paraiUpsVoltCmd,
/*1015*/ paraIdseRefCmd,
/*1016*/ parawCheckComm,
/*1017*/ paraTotalComAlarm,
/*1018*/ paraInfoExternal_Status,
/*1019*/ paraPVAutoRunEnable,
/*1020*/ paraSetRunHourTime,
/*1021*/ paraSetRunMinTime,
/*1022*/ paraSetStopHourTime,
/*1023*/ paraSetStopMinTime,
/*1024*/ paraPVdc_Start,
/*1025*/ paraPVidc_Standby,
/*1026*/ paraPVDelayCount,
/*1027*/ paraReserved,
/*1028*/ paraReserved,
/*1029*/ paraReserved,
/*1030*/ paraFrtEnable,
/*1031*/ paraHvrt01_Lvl,
/*1032*/ paraHvrt02_Lvl,
/*1033*/ paraHvrt03_Lvl,
/*1034*/ paraHvrt01_PERIOD,
/*1035*/ paraHvrt02_PERIOD,
/*1036*/ paraHvrt03_PERIOD,
/*1037*/ paraLvrt01_Lvl,
/*1038*/ paraLvrt02_Lvl,
/*1039*/ paraLvrt03_Lvl,
/*1040*/ paraLvrt01_PERIOD,
/*1041*/ paraLvrt02_PERIOD,
/*1042*/ paraLvrt03_PERIOD,
/*1043*/ paraHfrt01_Lvl,
/*1044*/ paraHfrt02_Lvl,
/*1045*/ paraHfrt03_Lvl,
/*1046*/ paraHfrt01_PERIOD,
/*1047*/ paraHfrt02_PERIOD,
/*1048*/ paraHfrt03_PERIOD,
/*1049*/ paraLfrt01_Lvl,
/*1050*/ paraLfrt02_Lvl,
/*1051*/ paraLfrt03_Lvl,
/*1052*/ paraLfrt01_PERIOD,
/*1053*/ paraLfrt02_PERIOD,
/*1054*/ paraLfrt03_PERIOD,
/*1055*/ paraReserved,
/*1056*/ paraReserved,
/*1057*/ paraReserved,
/*1058*/ paraReserved,
/*1059*/ paraReserved,
/*1060*/ paraDC_OV_cmd,
/*1061*/ paraDC_UV_cmd,
/*1062*/ paraDC_OC_cmd,
/*1063*/ paraGrid_OV_cmd,
/*1064*/ paraGrid_UV_cmd,
/*1065*/ paraGrid_OC_cmd,
/*1066*/ paraGrid_OF_cmd,
/*1067*/ paraGrid_UF_cmd,
/*1068*/ paraReserved,
/*1069*/ paraReserved,
/*1070*/ paraSocUpperLimitSet,
/*1071*/ paraSocLowerLimitSet,
/*1072*/ paraBankSOC,
/*1073*/ paraBankSOH,
/*1074*/ paraBankMinVolt,
/*1075*/ paraBankMaxVolt,
/*1076*/ paraBankMaxTemp,
/*1077*/ paraBankStatus,
/*1078*/ paraBankWarning,
/*1079*/ paraBankFault,
/*1080*/ paraPVWarning,
/*1081*/ paraPVFault,
/*1082*/ paraReserved,
/*1083*/ paraReserved,
/*1084*/ paraReserved,
/*1085*/ paraReserved,
/*1086*/ paraReserved,
/*1087*/ paraReserved,
/*1088*/ paraReserved,
/*1089*/ paraReserved,
/*1090*/ paraVdcPerc,
/*1091*/ paraVbattPerc,
/*1092*/ paraIbattCharPerc,
/*1093*/ paraIbattPerc,
/*1094*/ paraVsaPerc,
/*1095*/ paraVsbPerc,
/*1096*/ paraVscPerc,
/*1097*/ paraIoutCharPerc,
/*1098*/ paraIoutPerc,
/*1099*/ paraIoutR2Perc,
/*1100*/ paraIoutR3Perc,
/*1101*/ paraInvKwCharPerc,
/*1102*/ paraInvKwPerc,
/*1103*/ paraInvKvaPerc,
/*1104*/ paraReserved,
/*1105*/ paraReserved,
/*1106*/ paraReserved,
/*1107*/ paraReserved,
/*1108*/ paraReserved,
/*1109*/ paraReserved,
/*1110*/ paraKpVgdse,
/*1111*/ paraKiVgdse,
/*1112*/ paraKpVgqse,
/*1113*/ paraKiVgqse,
/*1114*/ paraVgdseLimit ,
/*1115*/ paraVgqseLimit,
/*1116*/ paraKpReactive,
/*1117*/ paraKiReactive,
/*1118*/ paraReactiveLimit,
/*1119*/ paraKpActive,
/*1120*/ paraKiActive,
/*1121*/ paraActiveLimit,
/*1122*/ paraKpIdc,
/*1123*/ paraKiIdc,
/*1124*/ paraIdcLimit,
/*1125*/ paraKpIdse,
/*1126*/ paraKiIdse,
/*1127*/ paraKpIqse,
/*1128*/ paraKiIqse,
/*1129*/ paraIdseLimit,
/*1130*/ paraIqseLimit,
/*1131*/ paraKpVdc,
/*1132*/ paraKiVdc,
/*1133*/ paraVdcLimit,
/*1134*/ paraKpAngle,
/*1135*/ paraKiAngle,
/*1136*/ paraAngleLimit,
/*1137*/ paraReserved,
/*1138*/ paraReserved,
/*1139*/ paraReserved,
/*1140*/ paraReserved,
/*1141*/ paraReserved,
/*1142*/ paraReserved,
/*1143*/ paraReserved,
/*1144*/ paraReserved,
/*1145*/ paraReserved,
/*1146*/ paraReserved,
/*1147*/ paraReserved,
/*1148*/ paraReserved,
/*1149*/ paraReserved,
/*1150*/ paraFacWriteEnable,
/*1151*/ paraVariInitEnable,
/*1152*/ paraFactoryMode,
/*1153*/ paraSelctDoNum,
/*1154*/ paraDcRelayEnable,
/*1155*/ paraFactoryVoltCmd,
/*1156*/ paraFactoryFanDuty,
/*1157*/ paraDigitalFLT1_Enable,
/*1158*/ paraDigitalFLT2_Enable,
/*1159*/ paraAnalogFLT1_Enable,
/*1160*/ paraAnalogFLT2_Enable,
/*1161*/ paraExternalFLT1_Enable,
/*1162*/ paraWarning_Enable,
/*1163*/ paraRunningTime,
/*1164*/ paraRTC_Enable,
/*1165*/ paraRTC_1sec,
/*1166*/ paraRTC_10sec,
/*1167*/ paraRTC_1min,
/*1168*/ paraRTC_10min,
/*1169*/ paraRTC_1hour,
/*1170*/ paraRTC_10hour,
/*1171*/ paraRTC_1day,
/*1172*/ paraRTC_10day,
/*1173*/ paraRTC_1month,
/*1174*/ paraRTC_10month,
/*1175*/ paraRTC_1year,
/*1176*/ paraRTC_10year,
/*1177*/ paraRTC_week,
/*1178*/ paraTimeSyncSet,
/*1179*/ paraAngleTest,
/*1180*/ paraPLLSet,
/*1181*/ paraDbCompSet,
/*1182*/ paraAutoResetNum,
/*1183*/ paraTimeSyncEnable,
/*1184*/ paraTimeSync_1sec,
/*1185*/ paraTimeSync_10sec,
/*1186*/ paraTimeSync_1min,
/*1187*/ paraTimeSync_10min,
/*1188*/ paraTimeSync_1hour,
/*1189*/ paraTimeSync_10hour,
/*1190*/ paraTimeSync_1day,
/*1191*/ paraTimeSync_10day,
/*1192*/ paraTimeSync_1month,
/*1193*/ paraTimeSync_10month,
/*1194*/ paraFacReserved1,
/*1195*/ paraFacReserved2,
/*1196*/ paraFacReserved3,
/*1197*/ paraFacReserved4,
/*1198*/ paraFacReserved5,
/*1199*/ paraFacReserved6

};


static pFUNCParaRWF fHoldingRegTBL1[] = {
    /*560*/ paraReserved

};


/***********************************************/
/* Operation List for                          */
/* [0x05] Write Single Coil                    */
/***********************************************/
static pFUNCParaRWI wWriteSingleCoilTBL1[] = {
/*300*/ paraReserved,
/*301*/ paraReserved,
/*302*/ paraReserved,
/*303*/ paraReserved,
/*304*/ paraReserved,
/*305*/ paraReserved
};



/***********************************************/
/* Operation List for                          */
/* [0x06, 0x10] Write Single/Multiple Register */
/***********************************************/

static pFUNCParaRWI wWriteRegTBL1[] = {

/*1000*/ paraKwh_Reset,
/*1001*/ paraINV_errReset,
/*1002*/ paraRemote_Sel,
/*1003*/ paraEmergencyStop,
/*1004*/ parawAutoCVMode,
/*1005*/ paraRefModeVoltage,
/*1006*/ paraCP_Step_Flag,
/*1007*/ paraTarget_Gradient,
/*1008*/ paraRunCmd,
/*1009*/ parafStateControl,
/*1010*/ paraiActiveCmd,
/*1011*/ paraiCVCmd,
/*1012*/ paraiCCCmd,
/*1013*/ paraiReActiveCmd,
/*1014*/ paraiUpsVoltCmd,
/*1015*/ paraIdseRefCmd,
/*1016*/ parawCheckComm,
/*1017*/ paraTotalComAlarm,
/*1018*/ paraInfoExternal_Status,
/*1019*/ paraPVAutoRunEnable,
/*1020*/ paraSetRunHourTime,
/*1021*/ paraSetRunMinTime,
/*1022*/ paraSetStopHourTime,
/*1023*/ paraSetStopMinTime,
/*1024*/ paraPVdc_Start,
/*1025*/ paraPVidc_Standby,
/*1026*/ paraPVDelayCount,
/*1027*/ paraReserved,
/*1028*/ paraReserved,
/*1029*/ paraReserved,
/*1030*/ paraFrtEnable,
/*1031*/ paraHvrt01_Lvl,
/*1032*/ paraHvrt02_Lvl,
/*1033*/ paraHvrt03_Lvl,
/*1034*/ paraHvrt01_PERIOD,
/*1035*/ paraHvrt02_PERIOD,
/*1036*/ paraHvrt03_PERIOD,
/*1037*/ paraLvrt01_Lvl,
/*1038*/ paraLvrt02_Lvl,
/*1039*/ paraLvrt03_Lvl,
/*1040*/ paraLvrt01_PERIOD,
/*1041*/ paraLvrt02_PERIOD,
/*1042*/ paraLvrt03_PERIOD,
/*1043*/ paraHfrt01_Lvl,
/*1044*/ paraHfrt02_Lvl,
/*1045*/ paraHfrt03_Lvl,
/*1046*/ paraHfrt01_PERIOD,
/*1047*/ paraHfrt02_PERIOD,
/*1048*/ paraHfrt03_PERIOD,
/*1049*/ paraLfrt01_Lvl,
/*1050*/ paraLfrt02_Lvl,
/*1051*/ paraLfrt03_Lvl,
/*1052*/ paraLfrt01_PERIOD,
/*1053*/ paraLfrt02_PERIOD,
/*1054*/ paraLfrt03_PERIOD,
/*1055*/ paraReserved,
/*1056*/ paraReserved,
/*1057*/ paraReserved,
/*1058*/ paraReserved,
/*1059*/ paraReserved,
/*1060*/ paraDC_OV_cmd,
/*1061*/ paraDC_UV_cmd,
/*1062*/ paraDC_OC_cmd,
/*1063*/ paraGrid_OV_cmd,
/*1064*/ paraGrid_UV_cmd,
/*1065*/ paraGrid_OC_cmd,
/*1066*/ paraGrid_OF_cmd,
/*1067*/ paraGrid_UF_cmd,
/*1068*/ paraReserved,
/*1069*/ paraReserved,
/*1070*/ paraSocUpperLimitSet,
/*1071*/ paraSocLowerLimitSet,
/*1072*/ paraBankSOC,
/*1073*/ paraBankSOH,
/*1074*/ paraBankMinVolt,
/*1075*/ paraBankMaxVolt,
/*1076*/ paraBankMaxTemp,
/*1077*/ paraBankStatus,
/*1078*/ paraBankWarning,
/*1079*/ paraBankFault,
/*1080*/ paraPVWarning,
/*1081*/ paraPVFault,
/*1082*/ paraReserved,
/*1083*/ paraReserved,
/*1084*/ paraReserved,
/*1085*/ paraReserved,
/*1086*/ paraReserved,
/*1087*/ paraReserved,
/*1088*/ paraReserved,
/*1089*/ paraReserved,
/*1090*/ paraVdcPerc,
/*1091*/ paraVbattPerc,
/*1092*/ paraIbattCharPerc,
/*1093*/ paraIbattPerc,
/*1094*/ paraVsaPerc,
/*1095*/ paraVsbPerc,
/*1096*/ paraVscPerc,
/*1097*/ paraIoutCharPerc,
/*1098*/ paraIoutPerc,
/*1099*/ paraIoutR2Perc,
/*1100*/ paraIoutR3Perc,
/*1101*/ paraInvKwCharPerc,
/*1102*/ paraInvKwPerc,
/*1103*/ paraInvKvaPerc,
/*1104*/ paraReserved,
/*1105*/ paraReserved,
/*1106*/ paraReserved,
/*1107*/ paraReserved,
/*1108*/ paraReserved,
/*1109*/ paraReserved,
/*1110*/ paraKpVgdse,
/*1111*/ paraKiVgdse,
/*1112*/ paraKpVgqse,
/*1113*/ paraKiVgqse,
/*1114*/ paraVgdseLimit ,
/*1115*/ paraVgqseLimit,
/*1116*/ paraKpReactive,
/*1117*/ paraKiReactive,
/*1118*/ paraReactiveLimit,
/*1119*/ paraKpActive,
/*1120*/ paraKiActive,
/*1121*/ paraActiveLimit,
/*1122*/ paraKpIdc,
/*1123*/ paraKiIdc,
/*1124*/ paraIdcLimit,
/*1125*/ paraKpIdse,
/*1126*/ paraKiIdse,
/*1127*/ paraKpIqse,
/*1128*/ paraKiIqse,
/*1129*/ paraIdseLimit,
/*1130*/ paraIqseLimit,
/*1131*/ paraKpVdc,
/*1132*/ paraKiVdc,
/*1133*/ paraVdcLimit,
/*1134*/ paraKpAngle,
/*1135*/ paraKiAngle,
/*1136*/ paraAngleLimit,
/*1137*/ paraReserved,
/*1138*/ paraReserved,
/*1139*/ paraReserved,
/*1140*/ paraReserved,
/*1141*/ paraReserved,
/*1142*/ paraReserved,
/*1143*/ paraReserved,
/*1144*/ paraReserved,
/*1145*/ paraReserved,
/*1146*/ paraReserved,
/*1147*/ paraReserved,
/*1148*/ paraReserved,
/*1149*/ paraReserved,
/*1150*/ paraFacWriteEnable,
/*1151*/ paraVariInitEnable,
/*1152*/ paraFactoryMode,
/*1153*/ paraSelctDoNum,
/*1154*/ paraDcRelayEnable,
/*1155*/ paraFactoryVoltCmd,
/*1156*/ paraFactoryFanDuty,
/*1157*/ paraDigitalFLT1_Enable,
/*1158*/ paraDigitalFLT2_Enable,
/*1159*/ paraAnalogFLT1_Enable,
/*1160*/ paraAnalogFLT2_Enable,
/*1161*/ paraExternalFLT1_Enable,
/*1162*/ paraWarning_Enable,
/*1163*/ paraRunningTime,
/*1164*/ paraRTC_Enable,
/*1165*/ paraRTC_1sec,
/*1166*/ paraRTC_10sec,
/*1167*/ paraRTC_1min,
/*1168*/ paraRTC_10min,
/*1169*/ paraRTC_1hour,
/*1170*/ paraRTC_10hour,
/*1171*/ paraRTC_1day,
/*1172*/ paraRTC_10day,
/*1173*/ paraRTC_1month,
/*1174*/ paraRTC_10month,
/*1175*/ paraRTC_1year,
/*1176*/ paraRTC_10year,
/*1177*/ paraRTC_week,
/*1178*/ paraTimeSyncSet,
/*1179*/ paraAngleTest,
/*1180*/ paraPLLSet,
/*1181*/ paraDbCompSet,
/*1182*/ paraAutoResetNum,
/*1183*/ paraTimeSyncEnable,
/*1184*/ paraTimeSync_1sec,
/*1185*/ paraTimeSync_10sec,
/*1186*/ paraTimeSync_1min,
/*1187*/ paraTimeSync_10min,
/*1188*/ paraTimeSync_1hour,
/*1189*/ paraTimeSync_10hour,
/*1190*/ paraTimeSync_1day,
/*1191*/ paraTimeSync_10day,
/*1192*/ paraTimeSync_1month,
/*1193*/ paraTimeSync_10month,
/*1194*/ paraFacReserved1,
/*1195*/ paraFacReserved2,
/*1196*/ paraFacReserved3,
/*1197*/ paraFacReserved4,
/*1198*/ paraFacReserved5,
/*1199*/ paraFacReserved6


};

static pFUNCParaRWF fWriteRegTBL1[] = {
    /*360*/ paraReserved
};



//bRxSize�� CRC�� ������ ���� ����Ÿ�� ��
/**
 * @brief Parse the command field of Modbus RTU
 */

static BYTE CommandParse (BYTE *pBRx, BYTE *pBTx)
{
	//  = (((RDRES_FRAME*)pBTx)->bData[0][0]);
	//  = (((RDRES_FRAME*)pBTx)->bData[1][0]);
	//  = (((RDRES_FRAME*)pBTx)->bData[2][0]);
	//wCESCheck4  = (((RDRES_FRAME*)pBTx)->bData[3][0]);

    switch (((GEN_FRAME*)pBRx)->bCmd) {
		case RD_HOLD_CMD:   /* Read Holding Registers [03] */
			return CommandHoldReg((RDCMD_FRAME*)pBRx, (RDRES_FRAME*)pBTx);

		case RD_INPUT_CMD:  /* Read Input Register [04] */
			return CommandRead((RDCMD_FRAME*)pBRx, (RDRES_FRAME*)pBTx);

		case WR_COIL_CMD:   /* Write Single Coil [05] */
			return CommandWriteCoil((WRCMD_FRAME*)pBRx, (WRRES_FRAME*)pBTx);

		case WR_SINGLE_CMD: /* Write Single Register [06] */
			return CommandWriteOne((WRCMD_FRAME*)pBRx, (WRRES_FRAME*)pBTx);

		case WR_MANY_CMD:   /* Write Multiple Registers [16] */
			return CommandWriteMany((WRCMD_FRAME_MANY*)pBRx, (WRRES_FRAME_MANY*)pBTx);

		default:
			return ILLEGAL_FUNCTION;
	}
}


static void ErrorParse (BYTE *pBRx, BYTE *pBTx, BYTE bErr)
{
	ERROR_FRAME* pRes = (ERROR_FRAME*)pBTx;

	pRes->bNumber = ((GEN_FRAME*)pBRx)->bNumber;
	pRes->bCmd	  = ((GEN_FRAME*)pBRx)->bCmd | 0x80;
	pRes->bExCode = bErr;

	bTxFrameSize = SendRTUFrame((BYTE *)pRes, ERROR_SIZE);
}

static BYTE SendRTUFrame(BYTE* pRes, BYTE bSize)
{
	WORD wCrc;
	wCrc = CRC16(pRes, bSize);
	pRes[bSize++] = LO_BYTE(wCrc);
	pRes[bSize++] = HI_BYTE(wCrc);
	return bSize;
}

static WORD wDBG_Size;


/**
 * @brief Process for Read Holding Registers [03]
 *
 * @remarks
 * - This function makes the response frame for the request also.
 */
static BYTE CommandHoldReg(RDCMD_FRAME* pCmd, RDRES_FRAME* pRes)
{
	BYTE bCount, bRegCnt;
	BYTE i, j, bSize;
	BYTE bErr;
	WORD wAddr, wTempAddr;
	WORD wParaArr[RD_HOLD_WGRP_MAX_NUM];
	float fParaArr[RD_HOLD_FGRP_MAX_NUM];

	BYTE bType;
	FLOAT_DATA fTemp;   //Tx:65 03 03 E8 00 04 CC 5D
	                      // 101�ּ� 03��� 1000(�����ּ�)  4(����Ʈ�ټ�)  1000���� 4�� �о�//

	bRegCnt = pCmd->bCnt[1]; // ī��Ʈ�� ����Ʈ ����  //4
	//RTUFrameFlag =  pCmd->bCnt[1];

	if ((bRegCnt>0) && (bRegCnt<MAX_IN_REG)) { //Modbus Maximum data numbers
		wAddr = MAKE_WORD(pCmd->bAddr[0], pCmd->bAddr[1]);
		//wCESCheck3 =wAddr;
		// Make Response data
		pRes->bNumber = pCmd->bNumber; // �ּ� // 101
		pRes->bCmd = pCmd->bCmd;

		//RTUFrameFlag1 = pCmd->bNumber;
		//RTUFrameFlag =pCmd->bAddr[0];// pCmd->bCmd;
		//RTUFrameFlag1 = pCmd->bAddr[1];//bErr;
		//wCESCheck4 = wAddr;
		bErr = PARA_NO_ERROR;

		/* 16-bit integer */
		if ((wAddr>=RD_HOLD_WGRP1_INIT_OFF) && (wAddr<=RD_HOLD_WGRP1_END_OFF)) {
			if((wAddr+(bRegCnt-1)) <= RD_HOLD_WGRP1_END_OFF) {
				wTempAddr = wAddr - RD_HOLD_WGRP1_INIT_OFF; //wAddr 1000
				//wCESCheck4 = wTempAddr;

				bType = MB_TYPE_W;
				bCount = bRegCnt; // �о���� ����


				for(i=0 ; i<bCount ; i++) {
					wHoldingRegTBL1[wTempAddr+i](MB_READ, wParaArr+i);
					//RTUFrameFlag++;
					//�Լ��迭[�迭](�Ű�����)
					//RTUFrameFlag
					//RTUFrameFlag1
					//RTUFrameFlag2
					//RTUFrameFlag3

				}
			} else {
				bErr = PARA_ADDRRANGE_ERROR;
			}
		}
		/* 32-bit float */
		else if ((wAddr>=RD_HOLD_FGRP1_INIT_OFF) && (wAddr<=RD_HOLD_FGRP1_END_OFF)) {
			if ((wAddr+(bRegCnt-1)) <= RD_HOLD_FGRP1_END_OFF) {
                wTempAddr = (wAddr-RD_HOLD_FGRP1_INIT_OFF) >> 1;
                bType = MB_TYPE_F;
                bCount = bRegCnt >> 1;

                for(i=0 ; i<bCount ; i++) {
                    fHoldingRegTBL1[wTempAddr+i](MB_READ, fParaArr+i);
                }
            } else {
                		bErr = PARA_ADDRRANGE_ERROR;
            }
        }
		else {
			bErr = PARA_ADDRRANGE_ERROR;
		}

		switch((char)bErr) {
			case PARA_NO_ERROR:
				if(bType == MB_TYPE_W) {
					for (i=0 ; i<bCount ; i++) {
						//RTUFrameFlag1++;
						pRes->bData[i][0] = HI_BYTE(wParaArr[i]);
						pRes->bData[i][1] = LO_BYTE(wParaArr[i]);

					/*
					    pRes->bData[0][0] = (WORD)0;//HI_BYTE(wParaArr[i]);
						pRes->bData[0][1] = (WORD)7;//LO_BYTE(wParaArr[i]);

						pRes->bData[1][0] = (WORD)0;//HI_BYTE(wParaArr[i]);
						pRes->bData[1][1] = (WORD)6;//LO_BYTE(wParaArr[i]);

						pRes->bData[2][0] = (WORD)0;//HI_BYTE(wParaArr[i]);
						pRes->bData[2][1] = (WORD)5;//LO_BYTE(wParaArr[i]);

						pRes->bData[3][0] = (WORD)0;//HI_BYTE(wParaArr[i]);
						pRes->bData[3][1] = (WORD)4;//LO_BYTE(wParaArr[i]);

						wCESCheck1  = (pRes->bData[0][0]);
						wCESCheck2  = (pRes->bData[0][1]);
						wCESCheck3  = (pRes->bData[1][0]);
						wCESCheck4  = (pRes->bData[1][1]);
*/

					}
				} else {  //float�� ���
					j = 0;
					for (i=0 ; i<bCount ; i++) {
						fTemp.fData = fParaArr[i];
						fDBG_Para =  fTemp.fData;

						wDBG_Data0 = pRes->bData[j][0]   = HI_BYTE(fTemp.wData[0]);
						wDBG_Data1 = pRes->bData[j][1]   = LO_BYTE(fTemp.wData[0]);
						wDBG_Data2 = pRes->bData[j+1][0] = HI_BYTE(fTemp.wData[1]);
						wDBG_Data3 = pRes->bData[j+1][1] = LO_BYTE(fTemp.wData[1]);

						j += 2;
					}
				}

				bSize = RDRES_SIZE(bRegCnt);// ���䰹��  4����Ʈ�ϱ� �ּ� 3 + 2*4 = 11����Ʈ +2����Ʈ(CRC)
				pRes->bByteCnt = 2*bRegCnt;
				wDBG_Size = bSize;

				//wCESCheck1  = (pRes->bData[0][0]);
				//wCESCheck2  = (pRes->bData[0][1]);
				//wCESCheck3  = (pRes->bData[1][0]);
				//wCESCheck4  = (pRes->bData[1][1]);


				bTxFrameSize = SendRTUFrame((BYTE *)pRes,bSize);

				return COM_NO_ERR;

			case PARA_ADDRRANGE_ERROR:
					return ILLEGAL_DATA_ADDRESS;
			default:
					return UNDEFINE_CONDITION;		// User Define : 0X15
		}
	}

	return ILLEGAl_DATA_VALUE;
}


/**
 * @brief Process for Read Input Register [04]
 *
 * @remarks
 * - This function makes the response frame for the request also.
 */
static BYTE test_bRegCnt;
static WORD test_wAddr;
static BYTE CommandRead (RDCMD_FRAME* pCmd, RDRES_FRAME* pRes)
{
	BYTE bCount, bRegCnt;
	BYTE i, j, bSize;
	BYTE bErr;
	WORD wAddr, wTempAddr;
	WORD wParaArr[RD_INPUT_WGRP_MAX_NUM];  /* number of input registers(integer) */
	float fParaArr[RD_INPUT_FGRP_MAX_NUM];	/* number of input registers(float) */

	BYTE bType;
	FLOAT_DATA fTemp;

	bRegCnt = pCmd->bCnt[1];
	test_bRegCnt = bRegCnt;


	if ((bRegCnt>0) && (bRegCnt<MAX_IN_REG)) { //Modbus Maximum data numbers
		wAddr = MAKE_WORD(pCmd->bAddr[0], pCmd->bAddr[1]);

		test_wAddr = 	wAddr;

		// Make Response data
		pRes->bNumber = pCmd->bNumber;
		pRes->bCmd = pCmd->bCmd;

		bErr = PARA_NO_ERROR;

		/* 16-bit integer */
		if ((wAddr>=RD_INPUT_WGRP1_INIT_OFF) && (wAddr<=RD_INPUT_WGRP1_END_OFF)) {
            if ((wAddr+(bRegCnt-1)) <= RD_INPUT_WGRP1_END_OFF) {
                wTempAddr = wAddr - RD_INPUT_WGRP1_INIT_OFF;
                bType = MB_TYPE_W;
                bCount = bRegCnt;

                for(i=0 ; i<bCount ; i++) {
                    wInputRegTBL1[wTempAddr+i](wParaArr+i);
                }
            } else {
                bErr = PARA_ADDRRANGE_ERROR;
            }
        }

		/* 32-bit float */
		else if ((wAddr>=RD_INPUT_FGRP1_INIT_OFF) && (wAddr<=RD_INPUT_FGRP1_END_OFF)) {
			if ((wAddr+(bRegCnt-1)) <= RD_INPUT_FGRP1_END_OFF) {
                wTempAddr = (wAddr-RD_INPUT_FGRP1_INIT_OFF) >> 1;
                bType = MB_TYPE_F;
                bCount = bRegCnt >> 1;

                for(i=0 ; i<bCount ; i++) {
                    fInputRegTBL1[wTempAddr+i](fParaArr+i);
                }
            } else {
                		bErr = PARA_ADDRRANGE_ERROR;
            }
        }

		else {
			bErr = PARA_ADDRRANGE_ERROR;
		}

		switch((char)bErr) {
			case PARA_NO_ERROR:
				if (bType == MB_TYPE_W) {
					/* 16-bit integer */
    					for (i=0 ; i<bCount ; i++) {
						pRes->bData[i][0] = HI_BYTE(wParaArr[i]);
						pRes->bData[i][1] = LO_BYTE(wParaArr[i]);
					}
				} else {
					/* 32-bit float */
					j = 0;
					for (i=0 ; i<bCount ; i++) {
						fTemp.fData = fParaArr[i];
						wDBG_Data0 = pRes->bData[j][0]   = HI_BYTE(fTemp.wData[0]);
						wDBG_Data1 = pRes->bData[j][1]   = LO_BYTE(fTemp.wData[0]);
						wDBG_Data2 = pRes->bData[j+1][0] = HI_BYTE(fTemp.wData[1]);
						wDBG_Data3 = pRes->bData[j+1][1] = LO_BYTE(fTemp.wData[1]);

						j += 2;
					}
				}

				bSize = RDRES_SIZE(bRegCnt);
				pRes->bByteCnt = 2*bRegCnt;
				bTxFrameSize = SendRTUFrame((BYTE *)pRes,bSize);

				return COM_NO_ERR;

			case PARA_ADDRRANGE_ERROR:
				return ILLEGAL_DATA_ADDRESS;
			default:
				return UNDEFINE_CONDITION;		// User Define : 0X15
		}
	}

	return ILLEGAl_DATA_VALUE;
}


/**
 * @brief Process for Write Single Coil [05]
 *
 * @remarks
 * - This function makes the response frame for the request also.
 */
static BYTE CommandWriteCoil(WRCMD_FRAME* pCmd, WRRES_FRAME* pRes)
{
	BYTE bErr;
	WORD wAddr, wTempAddr;
	WORD wParaArr[WR_COIL_WGRP_MAX_NUM];

	wAddr = MAKE_WORD(pCmd->bAddr[0], pCmd->bAddr[1]);

	// Make Response data
	pRes->bNumber  = pCmd->bNumber;
	pRes->bCmd     = WR_COIL_CMD;
	pRes->bAddr[0] = pCmd->bAddr[0];
	pRes->bAddr[1] = pCmd->bAddr[1];

	bErr = PARA_NO_ERROR;

	if((wSelCmdMode == REMOTE_MODE ) || (wSelCmdMode == HMI_MODE )){

		if ((wAddr>=WR_COIL_WGRP1_INIT_OFF) && (wAddr<=WR_COIL_WGRP1_END_OFF)) {
			wTempAddr = wAddr - WR_COIL_WGRP1_INIT_OFF;
			wParaArr[0] = MAKE_WORD(pCmd->bData[0], pCmd->bData[1]);
			wWriteSingleCoilTBL1[wTempAddr](MB_WRITE, wParaArr);
		} else {
			bErr = PARA_ADDRRANGE_ERROR;
			}
	} else {
		bErr = PARA_COMMAND_ERROR;
	}

	switch((char)bErr) {
		case PARA_NO_ERROR:
			pRes->bData[0] = HI_BYTE(wParaArr[0]);
			pRes->bData[1] = LO_BYTE(wParaArr[0]);

			bTxFrameSize = SendRTUFrame((BYTE *)pRes, 6);

			return COM_NO_ERR;

		case PARA_ADDRRANGE_ERROR:
			return ILLEGAL_DATA_ADDRESS;

		case PARA_COMMAND_ERROR:
			return ILLEGAl_DATA_VALUE;

		default:
			return UNDEFINE_CONDITION;		// User Define : 0X15
	}
}


/**
 * @brief Process for Write Single Register [06]
 *
 * @remarks
 * - This function makes the response frame for the request also.
 */
static BYTE CommandWriteOne(WRCMD_FRAME* pCmd, WRRES_FRAME* pRes)  ///6
{
	BYTE bErr;
	WORD wAddr, wTempAddr;
	WORD wParaArr[WR_REG_WGRP_MAX_NUM];

	wAddr = MAKE_WORD(pCmd->bAddr[0], pCmd->bAddr[1]);

	// Make Response data
	pRes->bNumber  = pCmd->bNumber;  //ID��
	pRes->bCmd     = WR_SINGLE_CMD;   // ����ڵ�//
	pRes->bAddr[0] = pCmd->bAddr[0];
	pRes->bAddr[1] = pCmd->bAddr[1];

	//RTUFrameFlag  = pCmd->bAddr[0];  //������ �ּ� //
	//RTUFrameFlag1  = pCmd->bAddr[1];
	//wCESCheck3= wAddr;
	//wCESCheck4= pCmd->bAddr[1];



	bErr = PARA_NO_ERROR;

	if((wSelCmdMode == REMOTE_MODE ) || (wSelCmdMode == HMI_MODE  )){

		if((wAddr>=WR_REG_WGRP1_INIT_OFF) && (wAddr<=WR_REG_WGRP1_END_OFF)) {// wAddr ���� �ּ� //
			wTempAddr = wAddr - WR_REG_WGRP1_INIT_OFF;
			wParaArr[0] = MAKE_WORD(pCmd->bData[0], pCmd->bData[1]);
				if(wSelCmdMode == REMOTE_MODE) //190826
			  {
				    if ((wTempAddr == 2 ) || ((wTempAddr >= 184 ) && (wTempAddr <= 193 )) ) {// Remote Sel, HMI Time Sync AnyTime Write //
					    wWriteRegTBL1[wTempAddr](MB_WRITE, wParaArr);
				    }
			  }
			  else // ������ �����  0 ����//
			  {
			  		wWriteRegTBL1[wTempAddr](MB_WRITE, wParaArr);
			  }

		}
		/* Write single�� 16bit�̱� ������ float���� ������ ���� */
		else {
			bErr = PARA_ADDRRANGE_ERROR;
		}
	} else {
		bErr = PARA_COMMAND_ERROR;
	}

	switch((char)bErr) {
		case PARA_NO_ERROR:
			pRes->bData[0] = HI_BYTE(wParaArr[0]); // ���� ������ �״�� ȸ��
			pRes->bData[1] = LO_BYTE(wParaArr[0]);

			//wCESCheck3 =  MAKE_WORD(pRes->bData[0], pRes->bData[1]);

			bTxFrameSize = SendRTUFrame((BYTE *)pRes, 6); // 6�� ������ + CRC ȸ�� ����

			return COM_NO_ERR;

		case PARA_ADDRRANGE_ERROR:
			return ILLEGAL_DATA_ADDRESS;

		default:
			return UNDEFINE_CONDITION;		// User Define : 0X15
	}
}


/**
 * @brief Process for Write Multiple Registers [16]
 *
 * @remarks
 * - This function makes the response frame for the request also.
 */
static BYTE CommandWriteMany (WRCMD_FRAME_MANY* pCmd, WRRES_FRAME_MANY* pRes)
{
	WORD bRegCnt, bCount;
	BYTE bErr;
	WORD wAddr, wTempAddr;
	WORD wParaArr[WR_REG_WGRP_MAX_NUM];

	FLOAT_DATA fTemp;
	float fParaArr[WR_REG_FGRP_MAX_NUM];

	WORD i, j;

	bRegCnt = pCmd->bCnt[1];

	if ((bRegCnt>0) && (bRegCnt<MAX_IN_REG)) { //Modbus Maximum data numbers

		wAddr = MAKE_WORD(pCmd->bAddr[0], pCmd->bAddr[1]);

		// Make Response data
		pRes->bNumber  = pCmd->bNumber;
		pRes->bCmd     = WR_MANY_CMD;
		pRes->bAddr[0] = pCmd->bAddr[0];
		pRes->bAddr[1] = pCmd->bAddr[1];
		pRes->bCnt[0]  = 0;
		pRes->bCnt[1]  = pCmd->bCnt[1];

		bErr = PARA_NO_ERROR;

		if((wSelCmdMode == REMOTE_MODE ) || (wSelCmdMode == HMI_MODE )){

			/* 16-bit integer */
			if ((wAddr>=WR_REG_WGRP1_INIT_OFF) && (wAddr<=WR_REG_WGRP1_END_OFF)) {

				if ((wAddr+(bRegCnt-1) <= WR_REG_WGRP1_END_OFF)) {
					wTempAddr = wAddr - WR_REG_WGRP1_INIT_OFF;
					bCount = bRegCnt;

					for(i=0 ; i<bCount ; i++) {
						wParaArr[0] = MAKE_WORD(pCmd->bData[i][0], pCmd->bData[i][1]);
						wWriteRegTBL1[wTempAddr+i](MB_WRITE, wParaArr);
					}
				}
				else {
					bErr = PARA_ADDRRANGE_ERROR;
				}
			}


			/* 32-bit float */
			 else if((wAddr>=WR_REG_FGRP1_INIT_OFF) && (wAddr<=WR_REG_FGRP1_END_OFF)) {//
				if ((wAddr+(bRegCnt-1) <= WR_REG_FGRP1_END_OFF)) {
					wTempAddr = (wAddr-WR_REG_FGRP1_INIT_OFF) >> 1;
					bCount = bRegCnt >> 1;

					j = 0;
					for(i=0 ; i<bCount ; i++) {
						fTemp.wData[0] = MAKE_WORD(pCmd->bData[j][0], pCmd->bData[j][1]);
						fTemp.wData[1] = MAKE_WORD(pCmd->bData[j+1][0], pCmd->bData[j+1][1]);
						j+=2;
						fParaArr[0] = fTemp.fData;
						fWriteRegTBL1[wTempAddr+i](MB_WRITE, fParaArr);
					}
				}
				else {
					bErr = PARA_ADDRRANGE_ERROR;
				}

			}
			else {
				bErr = PARA_ADDRRANGE_ERROR;
			}
		}
		else {

			bErr = PARA_COMMAND_ERROR;
		}


		switch((char)bErr) {
			case PARA_NO_ERROR:
				bTxFrameSize = SendRTUFrame((BYTE *)pRes,6);
				return COM_NO_ERR;

			case PARA_ADDRRANGE_ERROR:
				return ILLEGAL_DATA_ADDRESS;

			default:
				return UNDEFINE_CONDITION;		// User Define : 0X15
		}
	}

	return ILLEGAl_DATA_VALUE;
}



static WORD CRC16 (BYTE *buf, BYTE numbytes)
{
    WORD crc;
    BYTE i;
    BYTE carrybit;
    BYTE *ptr = buf;
    crc = 0xffff;
    do
    {
        crc = crc ^ ((WORD)*ptr++);
        i = 8;
        do
        {
            carrybit = (crc & 0x0001) ? 1 : 0;
            crc >>= 1;
	        if (carrybit) crc ^= M16;
        } while(--i);
    } while (--numbytes);

   return crc;
}

static BYTE CRCok(BYTE *msg, BYTE len)
{
	WORD chk;
	BYTE *msgbuf = msg;

	if(len > 2){
		chk = CRC16(msg,len-2);
		if (chk == (((WORD)*(msgbuf+(len-1)) << 8) + *(msgbuf+(len-2))))
    		return(TRUE);
    }
    return FALSE;
}


/**
 * @brief Check the frame of Modbus RTU
 *
 * @return TRUE Valid frame
 * @return FALSE Invalid frame
 */
static BYTE RTUFrameCheck(BYTE bPort)
{
	WORD wTempMacID;
	BYTE ret;

	wTempMacID = STATION_ID_HMI;

	//if ((commPortData[bPort].fIsReceiveState == TRUE) && (commPortData[bPort].bCntEOF >= MB_TIMEOUT))//  {
	if ((commPortData[bPort].fIsReceiveState == TRUE) )//  {
	{

		BYTE* pBRx  = CommGetRxBuff(bPort); //
		BYTE  bSize = CommGetRxSize(bPort);	// bRxIdx:�������� ����Ÿ�� ��(CRC����)

		commPortData[bPort].fIsReceiveState = FALSE;  // reset the flag to receive a frame

		if(wTempMacID == pBRx[0] || MODBUS_BROADCAST == pBRx[0]) {
			commPortState[bPort].wRcvFrameNum++;

			if(CRCok(pBRx, bSize)) {

				if(MODBUS_BROADCAST == pBRx[0]){

					commPortData[bPort].fIsBroadCastID = TRUE;
				}

				ret = TRUE;  /* valid frame */
			} else {
				commPortState[bPort].wErrorFrameNum++;
				ret = FALSE;  /* invalid frame; CRC error */
			}
		} else {
			ret = FALSE;  /* invalid frame; not interesting ID */
		}
	} else {
		ret = FALSE;  /* invalid frame; no recieved data */
	}

	return ret;
}


/**
 * @par
 *
 * 2015-08-19 kyryu
 * - Created for Jeju BSS
 */

void HmiLoop(BYTE bPort, WORD wSel, uart_tbl_t *p_uart_tbl)
{
    //BYTE  bSize;
	BYTE bErr, RTUFrameFlag;

	BYTE* pBRx = CommGetRxBuff(bPort);	// bRxBuff  //bRx2Buff
	BYTE* pBTx = CommGetTxBuff(bPort);	// bTxBuff


	if((CommErrGetCnt()) >= 100)
	{

				wHmiErrCnt = 0;

				 __HAL_UART_CLEAR_FLAG(p_uart_tbl->p_huart, UART_FLAG_RXNE);
				 __HAL_UART_CLEAR_FLAG(p_uart_tbl->p_huart, UART_FLAG_TC);
				 __HAL_UART_CLEAR_FLAG(p_uart_tbl->p_huart, UART_FLAG_LBD);

				 HAL_UART_Init(p_uart_tbl->p_huart);

				 _485_RX_ENB;
				 __HAL_UART_ENABLE_IT(p_uart_tbl->p_huart, UART_IT_RXNE); //UART RX INT Enable//

  	    CommClearTxBuff(0);
      	commPortData[PORT_SCIC].fTx = 0;
      	BufIndexClear();
        commPortData[PORT_SCIC].bRxIdx = 0; // first message come
        commPortData[PORT_SCIC].fIsReceiveState = 0;
        commPortData[PORT_SCIC].bCntEOF = 0;
        commPortData[PORT_SCIC].bTxIdx  = 0;
        commPortData[PORT_SCIC].bTxSize = 0;
        commPortData[PORT_SCIC].bRxIdx  = 0;
        commPortData[PORT_SCIC].bRxSize = 0;

        wHmiResetCnt++;

				if (wHmiResetCnt >= 4) // 나중에 폴트 및 보호 동작 추가해야함 // 리트라이 5번함 //
				{  // count 4 PI_FUSE ����
					g_wFlagHmiErr = 1;
					wHmiResetCnt = 5;  // max limit
				}
	}
	else
	{
		if (wHmiResetCnt < 4)
		{  // count 3
			g_wFlagHmiErr = 0;
		}
	}

	//_485_RX_ENB;

	if (RTUFrameCheck(bPort))
	{

		wHmiErrCnt = 0;
		wHmiResetCnt = 0;

		commPortData[bPort].fTx = TRUE;

		bErr = CommandParse(pBRx, pBTx);

	//	_485_TX_ENB;

		__HAL_UART_ENABLE_IT(p_uart_tbl->p_huart, UART_IT_TC);   //UART TX INT Enable//

		if (bErr)
		{
			ErrorParse(pBRx, pBTx, bErr);
		}

		if(commPortData[bPort].fTx == TRUE)
		{
			commPortData[bPort].bTxSize = bTxFrameSize;

		}
	}
}

// Check EOT : Check One Frame in internal RX buffer
BOOL CommEOTReceived(BYTE bPort)
{
   /* if(wSelCommMode == SEL_KEYPAD){ //For HMI

    	if (fEOTReceived[bPort]) {
    		fEOTReceived[bPort] = FALSE;
    		return TRUE;
    	}
    	return FALSE;
    }
    return FALSE; */

	BOOL ret = 0;

	return ret;
}

data_t data;

BYTE* CommGetRxBuff(BYTE bPort)
{
	    return data.bRx2Buff;
}

BYTE CommGetRxSize(BYTE bPort)
{
	    return commPortData[bPort].bRxIdx;
}

BYTE* CommGetTxBuff(BYTE bPort)
{
	    return data.bTx2Buff;
}

BOOL CommIsTxEmpty(BYTE bPort)
{
	BOOL ret = 0;

	return ret;
}

void CommClearTxBuff(BYTE bPort)
{
	commPortData[bPort].bTxSize = commPortData[bPort].bTxIdx = 0;
}

void CommTxDataBuffer (BYTE bPort)
{
	commPortData[bPort].bTxIdx = 0;

}

BOOL CommGetRxFlag(BYTE bPort, uart_tbl_t *p_uart_tbl)
{
	bool ret = false;
	ret = __HAL_UART_GET_FLAG(p_uart_tbl->p_huart, UART_FLAG_RXNE);
	return ret;
}


BOOL CommGetTxFlag(BYTE bPort, uart_tbl_t *p_uart_tbl)
{
	bool ret = false;
	ret = __HAL_UART_GET_FLAG(p_uart_tbl->p_huart, UART_FLAG_TC);
	return ret;
}


void CommErrSetCnt(void)
{

	if( wHmiErrCnt++ == 0xFFFF)
	{
		wHmiErrCnt = 30000;
	}

}

WORD CommErrGetCnt(void)
{
	WORD ret = 0;

	ret = wHmiErrCnt;

	return ret;

}

void BufIndexSet(void)
{

	if( wBufferIndex_C++ == 0xFFFF)
	{
		wBufferIndex_C = 30000;
	}

}

WORD BufIndexGet(void)
{

	WORD ret = 0;

	ret = wBufferIndex_C;

	return ret;

}

WORD BufIndexClear(void)
{

	WORD ret = 0;

	wBufferIndex_C = 0;
	ret = wBufferIndex_C;

	return ret;

}

uart_tbl_t uart_tbl[1];

void RtuLoopBack(uart_tbl_t *p_uart_tbl)
{

	if(millis()-pre_time_rtu >= 100)
	  {
		  pre_time_rtu = millis();

		   HmiLoop(PORT_SCIC, wSelCommMode, p_uart_tbl);

		   if(commPortData[PORT_SCIC].bCntEOF < 0xFFFF)
		   {
			   commPortData[PORT_SCIC].bCntEOF++;
		   }

		   CommErrSetCnt();

	  }

}


void RtuRxTxloop(uart_tbl_t *p_uart_tbl)
{

	uint8_t rxd = 0;
	bool temp_Rx_flag = 0;
	bool temp_Tx_flag = 0;
	uint8_t ret = 0;

	temp_Rx_flag	 	 = CommGetRxFlag(PORT_SCIC, p_uart_tbl);
	temp_Tx_flag		 = CommGetTxFlag(PORT_SCIC, p_uart_tbl);

	COMM_PORT_DATA  *port_c;
	port_c = &commPortData[PORT_SCIC];


	if((temp_Rx_flag == 1)) /* Start Rx Data */
	{

		if(port_c->fIsReceiveState)
		{
			port_c->bCntEOF = 0;

			 if (BufIndexGet() < (RX2_SIZE-1))
			 {
				  ret = uartRead(p_uart_tbl->ch);

				  if ( ret == true )
				  {
					  BufIndexSet();
					  port_c->bRxIdx = BufIndexGet();
				  }
				  else
				  {
					  /* do fault */
				  }
			 }
		 }
		 else
		 {
			   port_c->bRxIdx 					= 0;   // first message come
			   data.bRx2Buff[port_c->bRxIdx++] 	= rxd;
			   port_c->fIsReceiveState 					= TRUE;
			   port_c->bCntEOF 					= 0;
			   port_c->bTxIdx 					= 0;
			   port_c->bTxSize 					= 0;
			   BufIndexClear();
		 }
	}
	else
	{
		  BufIndexClear();
		  port_c->fIsReceiveState = FALSE;
		  __HAL_UART_DISABLE_IT(p_uart_tbl->p_huart, UART_IT_RXNE);
		  __HAL_UART_CLEAR_FLAG(p_uart_tbl->p_huart, UART_FLAG_RXNE);
	}


	if((temp_Tx_flag == 1)) /* Start Tx Data */
	{

		if (port_c->bTxIdx < port_c->bTxSize)
		{

			 ret = uartWrite(p_uart_tbl->ch, &data.bTx2Buff[port_c->bTxIdx], port_c->bTxSize);

			 if (ret == true)
			 {
				__HAL_UART_DISABLE_IT(p_uart_tbl->p_huart, UART_IT_TC);
				__HAL_UART_CLEAR_FLAG(p_uart_tbl->p_huart, UART_FLAG_TC);
				port_c->bTxIdx = 0; // first message come //= CommClearTxBuff(0);
				commPortData[0].fTx = 0;
				temp_Tx_flag = 0;
			  }
			 else
			 {
				 /* do fault */
			 }
		}
	}
	else
	{
		port_c->bTxIdx = 0; // first message come
		port_c->fTx = 0;
	}

}




