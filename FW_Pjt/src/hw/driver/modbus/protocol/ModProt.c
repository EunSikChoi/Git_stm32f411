/*
 * ModProt.c
 *
 *  Created on: 2021. 3. 29.
 *      Author: 82109
 */



#include "modbus.h"
#include "ModProt.h"
#include "RS_Para.h"
#include "stm32f4xx_hal.h"

#ifdef _USE_HW_W5500

//FOR HMI
//#define M16			0xA001 //11.10.25
//#define STATION_ID		0x30


//////////0x04 Read Input 영역 주소 //////////

#define RD_INPUT_WGRP1_INIT_OFF	0
#define RD_INPUT_WGRP1_END_OFF	79
#define RD_INPUT_WGRP_MAX_NUM	80

////////////////////////////////////////////

#define RD_INPUT_FGRP1_INIT_OFF	60
#define RD_INPUT_FGRP1_END_OFF	63
#define RD_INPUT_FGRP_MAX_NUM	3

///////0x04 Read Hold 영역 주소 //////////
////수정 500(0x03) 읽는거를 -> 1000번지로 같이 쓰고 읽게 수정함//

#define RD_HOLD_WGRP1_INIT_OFF	1000
#define RD_HOLD_WGRP1_END_OFF	1199 //1169
#define RD_HOLD_WGRP_MAX_NUM	199 //170

///////////////////////////////////////////////


#define RD_HOLD_FGRP1_INIT_OFF	700
#define RD_HOLD_FGRP1_END_OFF	701
#define RD_HOLD_FGRP_MAX_NUM	1



#define WR_COIL_WGRP1_INIT_OFF	200
#define WR_COIL_WGRP1_END_OFF	205
#define WR_COIL_WGRP_MAX_NUM	6

//////0x06, 0x10 // Write 주소 영역//////

#define WR_REG_WGRP1_INIT_OFF	1000
#define WR_REG_WGRP1_END_OFF	1199
#define WR_REG_WGRP_MAX_NUM		199

////////////////////////////////////////

#define WR_REG_WGRP2_INIT_OFF	100 //@juho
#define WR_REG_WGRP2_END_OFF	150
#define WR_REG_WGRP_MAX_NUM2	51

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

extern const BAUD_DATA baudData[];
extern WORD Tcp_Test;

enum baudRatetype {
	BAUD_1200BPS,
	BAUD_2400BPS,
	BAUD_9600BPS,
	BAUD_19200BPS,
	BAUD_38400BPS,
	BAUD_56000BPS,
	BAUD_115200BPS
};

//extern pFUNCParaF fInputRegTBL1[];
//extern pFUNCParaI wInputRegTBL1[];
//extern pFUNCParaI wInputRegTBL2[];
//extern pFUNCParaI wInputRegTBL3[];
//
//extern pFUNCParaRWI wHoldingRegTBL1[];
//extern pFUNCParaRWF fHoldingRegTBL1[];
//
//extern pFUNCParaRWI wWriteSingleCoilTBL1[];
//extern pFUNCParaRWI wWriteRegTBL1[];
//extern pFUNCParaRWI wWriteRegTBL2[]; //@juho
//extern pFUNCParaRWF fWriteRegTBL1[];

static BYTE bTxFrameSize; //11.10.25
BYTE    bTxTcpFrameSize = 0; //JSJ
BYTE    bTxTcpFrameSend = 0; //JSJ

static void ErrorParse (BYTE *pBRx, BYTE *pBTx, BYTE bErr);
static WORD CRC16 (BYTE *buf, BYTE numbytes); //11.10.25



extern WORD wSelCmdMode;

//상위 통신 에러 체크
WORD wRS485ErrCnt     = 0;
WORD wRS485ResetCnt = 0;  // by DUKE  20120710

WORD wEthErrCnt    = 0;
WORD wEthResetCnt = 0;

extern WORD g_wFlagRS485Err  = 0;
extern WORD test_2, test_3;

static  BYTE SendRTUFrame(BYTE* pRes, BYTE bSize); // For HMI
static  BYTE CommandRead (RDCMD_FRAME* pCmd, RDRES_FRAME* pRes); //11.10.25
static  BYTE CommandHoldReg(RDCMD_FRAME* pCmd, RDRES_FRAME* pRes);
static  BYTE CommandWriteCoil (WRCMD_FRAME* pCmd, WRRES_FRAME* pRes); //11.10.25
static  BYTE CommandWriteOne (WRCMD_FRAME* pCmd, WRRES_FRAME* pRes); //11.10.25
static  BYTE CommandWriteMany (WRCMD_FRAME_MANY* pCmd, WRRES_FRAME_MANY* pRes); //11.10.25
static  void CmdSingleWrite(BYTE*, BYTE, WORD, WORD);
static  void CmdSingleRead(BYTE*, BYTE, WORD);



//WORD wTblInReg[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};





/***********************************************/
/* Operation List for                          */
/* [0x04] Read Input Register                  */
/***********************************************/
static pFUNCParaI wInputRegTBL1[] = { //paraReserved1

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
/*62*/	paraReserved1	/* float */
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
/*560*/	paraReserved

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

static pFUNCParaRWI wWriteRegTBL2[] = { //@juho
/*100*/ paraReserved,
/*101*/ paraReserved,
/*102*/ paraReserved,
/*103*/ paraReserved,
/*104*/ paraReserved,
/*105*/ paraReserved,
/*106*/ paraReserved,
/*107*/ paraReserved,
/*108*/ paraReserved,
/*109*/ paraReserved,
/*110*/ paraReserved,
/*111*/ paraReserved,
/*112*/ paraReserved,
/*113*/ paraReserved,
/*114*/ paraReserved,
/*115*/ paraReserved,
/*116*/ paraReserved,
/*117*/ paraReserved,
/*118*/ paraReserved,
/*119*/ paraReserved,
/*120*/ paraReserved,
/*121*/ paraReserved,
/*122*/ paraReserved,
/*123*/ paraReserved,
/*124*/ paraReserved,
/*125*/ paraReserved,
/*126*/ paraReserved,
/*127*/ paraReserved,
/*128*/ paraReserved,
/*129*/ paraReserved,
/*130*/ paraReserved,
/*131*/ paraReserved,
/*132*/ paraReserved,
/*133*/ paraReserved,
/*134*/ paraReserved,
/*135*/ paraReserved,
/*136*/ paraReserved,
/*137*/ paraReserved,
/*138*/ paraReserved,
/*139*/ paraReserved,
/*140*/ paraReserved,
/*141*/ paraReserved,
/*142*/ paraReserved,
/*143*/ paraReserved,
/*144*/ paraReserved,
/*145*/ paraReserved,
/*146*/ paraReserved,
/*147*/ paraReserved,
/*148*/ paraReserved,
/*149*/ paraReserved,
/*150*/ paraReserved
};

static pFUNCParaRWF fWriteRegTBL1[] = {
/*360*/	paraReserved
};















//bRxSize는 CRC를 제외한 수신 데이타의 수
/**
 * @brief Parse the command field of Modbus RTU
 */
extern char UartTx1[200];

static BYTE CommandParse (BYTE *pBRx, BYTE *pBTx)
{

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
	FLOAT_DATA fTemp;

	bRegCnt = pCmd->bCnt[1];

	if ((bRegCnt>0) && (bRegCnt<MAX_IN_REG)) { //Modbus Maximum data numbers
		wAddr = MAKE_WORD(pCmd->bAddr[0], pCmd->bAddr[1]);



		// Make Response data
		pRes->bNumber = pCmd->bNumber;
		pRes->bCmd = pCmd->bCmd;

		bErr = PARA_NO_ERROR;

		/* 16-bit integer */
		if ((wAddr>=RD_HOLD_WGRP1_INIT_OFF) && (wAddr<=RD_HOLD_WGRP1_END_OFF)) {
			if((wAddr+(bRegCnt-1)) <= RD_HOLD_WGRP1_END_OFF) {
				wTempAddr = wAddr - RD_HOLD_WGRP1_INIT_OFF;
				bType = MB_TYPE_W;
				bCount = bRegCnt;

				for(i=0 ; i<bCount ; i++) {
					wHoldingRegTBL1[wTempAddr+i](MB_READ, wParaArr+i);
					//함수배열[배열](매개변수)
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
						pRes->bData[i][0] = HI_BYTE(wParaArr[i]);
						pRes->bData[i][1] = LO_BYTE(wParaArr[i]);
					}
				} else {  //float인 경우
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

				bSize = RDRES_SIZE(bRegCnt);
				pRes->bByteCnt = 2*bRegCnt;
				wDBG_Size = bSize;

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
extern char UartTx1[200];

static BYTE CommandWriteOne(WRCMD_FRAME* pCmd, WRRES_FRAME* pRes)
{
	BYTE bErr;
	WORD wAddr, wTempAddr;
	WORD wParaArr[WR_REG_WGRP_MAX_NUM];
	WORD wParaArr2[WR_REG_WGRP_MAX_NUM2];

	wAddr = MAKE_WORD(pCmd->bAddr[0], pCmd->bAddr[1]);

	// Make Response data
	pRes->bNumber  = pCmd->bNumber;
	pRes->bCmd     = WR_SINGLE_CMD;
	pRes->bAddr[0] = pCmd->bAddr[0];
	pRes->bAddr[1] = pCmd->bAddr[1];

	bErr = PARA_NO_ERROR;


	if((wSelCmdMode == REMOTE_MODE ) || (wSelCmdMode == HMI_MODE )){

		if((wAddr>=WR_REG_WGRP1_INIT_OFF) && (wAddr<=WR_REG_WGRP1_END_OFF)) {
			wTempAddr = wAddr - WR_REG_WGRP1_INIT_OFF;
			wParaArr[0] = MAKE_WORD(pCmd->bData[0], pCmd->bData[1]);


				if(wSelCmdMode == HMI_MODE)//190826
			  {
				   if ((wTempAddr == 2 ) || ((wTempAddr >= 184 ) && (wTempAddr <= 193 )) ) {// Remote Sel, HMI Time Sync AnyTime Write //
					    wWriteRegTBL1[wTempAddr](MB_WRITE, wParaArr);

				    }
			  }
			  else
			  {
			  		wWriteRegTBL1[wTempAddr](MB_WRITE, wParaArr);

			  }

		}
		else if((wAddr>=WR_REG_WGRP2_INIT_OFF) && (wAddr<=WR_REG_WGRP2_END_OFF)) { //@juho
			wTempAddr = wAddr - WR_REG_WGRP2_INIT_OFF;
			wParaArr2[0] = MAKE_WORD(pCmd->bData[0], pCmd->bData[1]);
			wWriteRegTBL2[wTempAddr](MB_WRITE, wParaArr2);
		}
		/* Write single은 16bit이기 때문에 float에는 적용을 못함 */
		else {
			bErr = PARA_ADDRRANGE_ERROR;
		}
	} else {
		bErr = PARA_COMMAND_ERROR;
	}

	if((wAddr>=WR_REG_WGRP1_INIT_OFF) && (wAddr<=WR_REG_WGRP1_END_OFF)){
		switch((char)bErr) {
			case PARA_NO_ERROR:
				pRes->bData[0] = HI_BYTE(wParaArr[0]);
				pRes->bData[1] = LO_BYTE(wParaArr[0]);


				bTxFrameSize = SendRTUFrame((BYTE *)pRes, 6);

				return COM_NO_ERR;

			case PARA_ADDRRANGE_ERROR:

				return ILLEGAL_DATA_ADDRESS;

			default:;
				return UNDEFINE_CONDITION;		// User Define : 0X15
		}
	}
	else{
		switch((char)bErr) {
			case PARA_NO_ERROR:
				pRes->bData[0] = HI_BYTE(wParaArr2[0]);
				pRes->bData[1] = LO_BYTE(wParaArr2[0]);

				bTxFrameSize = SendRTUFrame((BYTE *)pRes, 6);

				return COM_NO_ERR;

			case PARA_ADDRRANGE_ERROR:
				return ILLEGAL_DATA_ADDRESS;

			default:
				return UNDEFINE_CONDITION;		// User Define : 0X15
		}
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
//
//	wTempInt485BaudRate = BAUD_19200BPS;			// Index
//	wTempCommRespDelay = 0;// CommRespDelay;				// 5msec

	wTempMacID = STATION_ID;

	if ((commPortData[bPort].fIsReceiveState == TRUE)
		&& (commPortData[bPort].bCntEOF >= MB_TIMEOUT))// (baudData[wTempInt485BaudRate].ch4 + wTempCommRespDelay))) {
	{
		BYTE* pBRx  = CommGetRxBuff(bPort);
		BYTE  bSize = CommGetRxSize(bPort);	// bRxIdx:실제받은 데이타의 수(CRC포함)

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
 * 2013-09-01 DUKE
 * - Modified for H2 project
 */
void RTU485Loop (BYTE bPort)
{
	//BYTE  bSize;
	BYTE bErr;

	BYTE* pBRx = CommGetRxBuff(bPort);	// bRxBuff
	BYTE* pBTx = CommGetTxBuff(bPort);	// bTxBuff

	//2012.04.04
	//통신 에러 체크
	// by DUKE  20120710
	// (기존) 3총 동안 통신 두절 시, PI_FUSE 차단
	// [1] 3초 동안 통신 두절 시, 강제로 SCI B를 SW reset 시킴
	// [2] [1]번을 4회 반복 후, PI_FUSE 차단
	if(wRS485ErrCnt >= 3000) { // 3sec동안 통신이 없는 경우
		wRS485ErrCnt = 0;  // 다시 count by DUKE   20120710
		// reset SCI B
		Comm485SWReset();  // SW reset  by DUKE  20120710
		wRS485ResetCnt++;
		if (wRS485ResetCnt >= 4) {  // count 4 (약 12 초)후, PI_FUSE 차단
			g_wFlagRS485Err = 1;
			wRS485ResetCnt = 5;  // max limit
		}
	} else {
		if (wRS485ResetCnt < 4) {  // count 3 까지만 복귀 가능  20120711 추가
			g_wFlagRS485Err = 0;
		}
	}

	if(Is485PortTxEmpty()) {
		//RTS_RECEIVE;
	}

	if (RTUFrameCheck(bPort)) {
		// RS485 통신 에러 카운트 reset
		wRS485ErrCnt = 0;
		wRS485ResetCnt = 0;  // 정상이면 다시 카운트  by DUKE  20120710

		commPortData[bPort].fTx = TRUE;

		bErr = CommandParse(pBRx, pBTx);

		if (bErr) {
			ErrorParse(pBRx, pBTx, bErr);
		}

		if(commPortData[bPort].fTx == TRUE) {
			commPortData[bPort].bTxSize = bTxFrameSize;
			CommTxDataBuffer(bPort);	// Send bTx0Buff[] with size bTxSize
		}
	}
}


static BYTE RTUtcpFrameCheck(BYTE *bRXbuf, uint16_t rxLen, BYTE *bTXbuf)
{
#if 0
	WORD wTempMacID;
	BYTE ret;

	wTempMacID = STATION_ID;

	if ((commPortData[bPort].fIsReceiveState == TRUE)
		&& (commPortData[bPort].bCntEOF >= MB_TIMEOUT))// (baudData[wTempInt485BaudRate].ch4 + wTempCommRespDelay))) {
	{
		BYTE* pBRx  = bRXbuf;
		BYTE  bSize = CommGetRxSize(bPort);	// bRxIdx:실제받은 데이타의 수(CRC포함)

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
	#endif

	return TRUE; //JSJ
}
/**
 * @par
 * - Modified for Wiz TCP/IP project
 */

// Frame Compare
// CC 5C 00 00 00 06 01 03 00 00 00 0A        TCP/IP
//                   01 03 00 00 00 0A C5 CD  RTU


BYTE	bRxTcpBuff[RX1_SIZE];
BYTE	bTxTcpBuff[TX1_SIZE];

extern char UartTx1[200];
//BYTE    bTxTcpFrameSize;
//BYTE    bTxTcpFrameSend;

	BYTE* pBRx = bRxTcpBuff;	// Received Buffer
	BYTE* pBTx = bTxTcpBuff; 	// Send Buffer
void RTUtcpLoop(uint8_t* bRXbuf, uint16_t rxLen, uint8_t* bTXbuf)
{
	uint16_t sendSize = 0;
	BYTE bErr;
//	BYTE* pBRx = bRxTcpBuff;	// Received Buffer
//	BYTE* pBTx = bTxTcpBuff;	// Send Buffer


	if(rxLen > 0)
	{
		memcpy(pBRx, bRXbuf+6, rxLen-3);
		if(RTUtcpFrameCheck(pBRx, rxLen, pBTx)) {

			wEthErrCnt = 0;
			wEthResetCnt = 0;

			bErr = CommandParse(pBRx, pBTx);

		//	sprintf(UartTx1, "Err1 Event  %d: \r\n",bErr);
		//	Userprintf(UartTx1);

			if (bErr) {
				ErrorParse(pBRx, pBTx, bErr);

		}

/*		if((bTxFrameSize > 0) && (bTxTcpFrameSend == 0)) {

			memcpy(bTXbuf+6, pBTx, bTxFrameSize);

			bTXbuf[sendSize++] = bRXbuf[0]; //Transaction ID_H
			bTXbuf[sendSize++] = bRXbuf[1]; //Transaction ID_L
			bTXbuf[sendSize++] = bRXbuf[2]; //Protocol Identifier_H
			bTXbuf[sendSize++] = bRXbuf[3]; //Protocol Identifier_L
			bTXbuf[sendSize++] = bRXbuf[4]; //Length_H
			bTXbuf[sendSize++] = bTxFrameSize - 2; //bRXbuf[5]; //Length_L
			bTXbuf[sendSize++] = bRXbuf[6]; //Unit Identifier

			bTxTcpFrameSize = bTxFrameSize + 16;
			bTxTcpFrameSend = TRUE;
			bTxFrameSize = 0;
		}
*/
		}
	}

	if((bTxFrameSize > 0) /*&& (bTxTcpFrameSend == 0) && (bTxTcpFrameSize == 0)*/) {

		bTXbuf[sendSize++] = bRXbuf[0]; //Transaction ID_H
		bTXbuf[sendSize++] = bRXbuf[1]; //Transaction ID_L
		bTXbuf[sendSize++] = bRXbuf[2]; //Protocol Identifier_H
		bTXbuf[sendSize++] = bRXbuf[3]; //Protocol Identifier_L
		bTXbuf[sendSize++] = bRXbuf[4]; //Length_H
		bTXbuf[sendSize++] = bTxFrameSize - 2; //bRXbuf[5]; //Length_L
		memcpy(bTXbuf + sendSize, pBTx, bTxFrameSize);

		bTxTcpFrameSize = 6 + bTxFrameSize - 2;

//		bTxTcpFrameSize = bTxFrameSize + 16;
//		if(bTxTcpFrameSize < 16)
//			bTxTcpFrameSize = 16;
		bTxTcpFrameSend = TRUE;
		bTxFrameSize = 0;
	}

}


//=================================================================//
//=============         Modbus Master Fn.             =============//
//=================================================================//


static void CmdSingleWrite(BYTE* pCmd, BYTE bSlaveID, WORD wAddr, WORD wData)
{
    BYTE idx = 0;
    WORD wCrc;

    pCmd[idx++] = bSlaveID;
    pCmd[idx++] = WR_SINGLE_CMD;

    pCmd[idx++] = (wAddr>>8)& 0x00FF;
    pCmd[idx++] = wAddr & 0x00FF;

    pCmd[idx++] = (wData>>8)& 0x00FF;
    pCmd[idx++] = wData & 0x00FF;

    wCrc = CRC16(pCmd, 6);

    pCmd[idx++] = wCrc & 0x00FF;
    pCmd[idx++] = (wCrc>>8)& 0x00FF;

}

static void CmdSingleRead(BYTE* pCmd, BYTE bSlaveID,WORD wAddr)
{
    BYTE idx = 0;
    WORD wCrc;

    pCmd[idx++] = bSlaveID;
    pCmd[idx++] = RD_INPUT_CMD;

    pCmd[idx++] = (wAddr>>8)& 0x00FF;
    pCmd[idx++] = wAddr & 0x00FF;

    pCmd[idx++] = 0x00;
    pCmd[idx++] = 0x01;

    wCrc = CRC16(pCmd, idx);

    pCmd[idx++] = wCrc & 0x00FF;
    pCmd[idx++] = (wCrc>>8)& 0x00FF;
}

#endif
