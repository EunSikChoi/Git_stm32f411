/*
 * RS_Para.h
 *
 *  Created on: 2021. 3. 29.
 *      Author: 82109
 */

#ifndef SRC_COMMON_HW_INCLUDE_RS_PARA_H_
#define SRC_COMMON_HW_INCLUDE_RS_PARA_H_

#include "hw_def.h"

#ifdef _USE_HW_W5500

// #define TEST_MODE

/* defind dispatch function type */
typedef void (*pFUNCParaI) (WORD *pwData);
typedef void (*pFUNCParaF) (float *pwData);
typedef void (*pFUNCParaRWI) (WORD wAttr, WORD *pwData);
typedef void (*pFUNCParaRWF) (WORD wAttr, float *pwData);


#define MB_READ     0x5555
#define MB_WRITE    0xAAAA



enum {
	PARA_NO_ERROR,			// Parameter R/W succesfully
	PARA_COMMAND_ERROR,		// ��ɾ Ʋ������
	PARA_ADDRRANGE_ERROR,	// Parameter address range fail
	PARA_DATARANGE_ERROR,	// ����Ÿ ������ Ʋ������
	PARA_WRITEMODE_ERROR,	// ���� ���� �Ķ���Ϳ� ������ �Ҷ�
	PARA_FRAME_SIZE_ERROR,
	PARA_FRAME_GEN_ERROR
};


//BYTE GetParameter (WORD wAddr, WORD* wpParam);
BYTE SetParameter (WORD wAddr, WORD* wpParam, BOOL);


typedef struct
{
    WORD wInit;
    WORD wTMaxNum;
    WORD wTMax;
    WORD wTMinNum;
    WORD wTMin;
    WORD wVMaxNum;
    WORD wVMax;
    WORD wVMinNum;
    WORD wVMin;

    float fSOC;
    float fLmtChrg;
    float fLmtDisChrg;
    float fVoltage;
    float fCurrent;

} BAT_RACKS;

typedef struct
{
    WORD wTMaxRack;
    WORD wTMaxNum;
    WORD wTMinRack;
    WORD wTMinNum;
    WORD wVMaxRack;
    WORD wVMaxNum;
    WORD wVMinRack;
    WORD wVMinNum;
} BAT_T_V_INFO;



//------------0x04-----------------------------
 void paraReserved1(WORD *pwData);


//04_00//
void paraSoftware_ver(WORD *pwData);

//04_01//
void paraParameter_ver(WORD *pwData);

//04_02//
void paraSourceInfo(WORD *pwData);

//04_03//
void paraSysCapaInfo(WORD *pwData);




/////////////////////////////////////////////
//////////// RESERVED 02 - 09 ///////////////
/////////////////////////////////////////////

//04_10//
void paraInfoDoStatus(WORD *pwData);

//04_11//
void paraInfoDiStatus(WORD *pwData);

//04_12//
void paraProtDigitalErrStatus1(WORD *pwData);

//04_13//
void paraProtDigitalErrStatus2(WORD *pwData);

//04_14//
void paraProtAnalogErrStatus1(WORD *pwData);

//04_15//
void paraProtAnalogErrStatus2(WORD *pwData);

//04_16//
void paraProtHwErrStatus(WORD *pwData);

//04_17//
void paraProtExtErrStatus(WORD *pwData);

//04_18//
void parawProtWarnStatus(WORD *pwData);

//04_19//
void	paraOperation_State(WORD *pwData);

//04_20//
void	paraCharge_State(WORD *pwData);

//04_21//
void	paraControl_Mode_State(WORD *pwData);

//04_22//
 void paraGrid_Uan(WORD *pwData);

//04_23//
 void paraGrid_Ubn(WORD *pwData);

//04_24//
 void paraGrid_Ucn(WORD *pwData);

//04_25//
 void paraInv_Ian(SWORD *pwData);

//04_26//
 void paraInv_Ibn(SWORD *pwData);

//04_27//
 void paraInv_Icn(SWORD *pwData);

//04_28//
 void	paraSysWe(WORD *pwData);

//04_29//
 void pararInv_Kw_rms(SWORD *pwData);

//04_30//
 void pararInv_Kva_rms(SWORD *pwData);

//04_31//
 void	parapowerFactor(SWORD *pwData);

//04_32//
 void	pararVdcCheck(WORD *pwData);

//04_33//
 void	paraDcVTG(SWORD *pwData);

//04_34//
 void pararIbatt_rms(SWORD *pwData);

//04_35//
 void	pararVdc_Kw_rms(SWORD *pwData);

//04_36//
 void	paraPCS_Temper_1(SWORD *pwData);

//04_37//
 void	paraPCS_Temper_2(SWORD *pwData);

//04_38//
 void	paraPCS_Temper_3(SWORD *pwData);

//04_39//
 void	paraPCS_Temper_4(SWORD *pwData);

//04_40//
 void	paraPCS_Temper_5(SWORD *pwData);

//04_41//
 void	paraPCS_Temper_6(SWORD *pwData);

//04_42//
 void	paraPCS_Temper_7(SWORD *pwData);

//04_43//
 void	paraAcChaPwrKwh(SWORD *pwData);

//04_44//
 void	paraAcChaPwrMwh(SWORD *pwData);

//04_45//
 void	paraAcChaPwrGwh(SWORD *pwData);

//04_46//
void	paraAcDisChaPwrKwh(SWORD *pwData);

//04_47//
 void	paraAcDisChaPwrMwh(SWORD *pwData);

//04_48//
 void	paraAcDisChaPwrGwh(SWORD *pwData);

//04_49//
 void	paraDcChaPwrKwh(SWORD *pwData);

//04_50//
 void	paraDcChaPwrMwh(SWORD *pwData);

//04_51//
 void	paraDcChaPwrGwh(SWORD *pwData);

//04_52//
 void	paraDcDisChaPwrKwh(SWORD *pwData);

//04_53//
 void	paraDcDisChaPwrMwh(SWORD *pwData);

//04_54//
 void	paraDcDisChaPwrGwh(SWORD *pwData);

//04_55//
 void	paraErrAllStatus(WORD *pwData);

//04_56//
 void	paraDailyChaPwrKwh(SWORD *pwData);

//04_57//
 void	paraDailyDisCharPwrKwh(SWORD *pwData);

//04_58//
void	paraTwoagoChaPwrKwh(SWORD *pwData);

//04_59//
void	paraTwoagoDisChaPwrKwh(SWORD *pwData);

//04_60//
void	paraDccbState(WORD *pwData);

//04_61//
void	paraHour0708Kwh(WORD *pwData);

//04_62//
void	paraHour0809Kwh(WORD *pwData);

//04_63//
void	paraHour0910Kwh(WORD *pwData);

//04_64//
void	paraHour1011Kwh(WORD *pwData);

//04_65//
void	paraHour1112Kwh(WORD *pwData);

//04_66//
void	paraHour1213Kwh(WORD *pwData);

//04_67//
void	paraHour1314Kwh(WORD *pwData);

//04_68//
void	paraHour1415Kwh(WORD *pwData);

//04_69//
void	paraHour1516Kwh(WORD *pwData);

//04_70//
void	paraHour1617Kwh(WORD *pwData);

//04_71//
void	paraHour1718Kwh(WORD *pwData);

//04_72//
void	paraHour1819Kwh(WORD *pwData);



///////////////////////////////////////////////
//////////////// RESERVED 55-59 ///////////////
///////////////////////////////////////////////

 void paraOVGR_Fault(WORD wAttr, WORD *pwData);
 void paraOVGR_Fault_1(WORD wAttr, WORD *pwData);



//--------End 0x04--------------------------------



//------------0x010-----------------------------


 void paraReserved(WORD wAttr, WORD *pwData);

//10_1000//
 void paraKwh_Reset(WORD wAttr, WORD *pwData);

//10_1001//
 void paraINV_errReset(WORD wAttr, WORD *pwData);

//10_1002//
 void paraRemote_Sel(WORD wAttr, WORD *pwData);

//10_1003//
 void paraEmergencyStop(WORD wAttr, WORD *pwData);

//10_1004//
 void parawAutoCVMode(WORD wAttr, WORD *pwData);

//10_1005//
 void paraRefModeVoltage(WORD wAttr, WORD *pwData);

//10_1006//
 void paraCP_Step_Flag(WORD wAttr, WORD *pwData);

//10_1007//
 void paraTarget_Gradient(WORD wAttr, WORD *pwData);

//10_1008//
 void paraRunCmd(WORD wAttr, WORD *pwData);

//10_1009//
 void parafStateControl(WORD wAttr, WORD *pwData);

//10_1010//
 void paraiActiveCmd(SWORD wAttr, SWORD *pwData);

//10_1011//
 void paraiCVCmd(WORD wAttr, WORD *pwData);

//10_1012//
 void paraiCCCmd(SWORD wAttr, SWORD *pwData);

//10_1013//
 void paraiReActiveCmd (WORD wAttr, WORD *pwData);

//10_1014//
 void paraiUpsVoltCmd (WORD wAttr, WORD *pwData);

//10_1015///
void paraIdseRefCmd(WORD wAttr, WORD *pwData);

//10_1016//
 void parawCheckComm(WORD wAttr, WORD *pwData);

//10_1017//
 void paraTotalComAlarm(WORD wAttr, WORD *pwData);

//10_1018//
 void paraInfoExternal_Status(WORD wAttr, WORD *pwData);

//10_1019//
void paraPVAutoRunEnable(WORD wAttr, WORD *pwData);

//10_1020//
void paraSetRunHourTime(WORD wAttr, WORD *pwData);

//10_1021//
void paraSetRunMinTime(WORD wAttr, WORD *pwData);

//10_1022//
void paraSetStopHourTime(WORD wAttr, WORD *pwData);

//10_1023//
void paraSetStopMinTime(WORD wAttr, WORD *pwData);

//10_1024//
void paraPVdc_Start(WORD wAttr, WORD *pwData);

//10_1025//
void paraPVidc_Standby(WORD wAttr, WORD *pwData);

//10_1026//
void paraPVDelayCount(WORD wAttr, WORD *pwData);

/////////////////////////////////////////////
//////////// RESERVED 24 - 29 ///////////////
/////////////////////////////////////////////

//10_1030///
 void paraFrtEnable(WORD wAttr, WORD *pwData);

//10_1031///
 void paraHvrt01_Lvl(WORD wAttr, WORD *pwData);

//10_1032///
 void paraHvrt02_Lvl(WORD wAttr, WORD *pwData);

//10_1033///
 void paraHvrt03_Lvl(WORD wAttr, WORD *pwData);

//10_1034///
 void paraHvrt01_PERIOD(WORD wAttr, WORD *pwData);

//10_1035///
 void paraHvrt02_PERIOD(WORD wAttr, WORD *pwData);

//10_1036///
 void paraHvrt03_PERIOD(WORD wAttr, WORD *pwData);

//10_1037///
 void paraLvrt01_Lvl(WORD wAttr, WORD *pwData);

//10_1038///
 void paraLvrt02_Lvl(WORD wAttr, WORD *pwData);

//10_1039///
 void paraLvrt03_Lvl(WORD wAttr, WORD *pwData);

//10_1040///
 void paraLvrt01_PERIOD(WORD wAttr, WORD *pwData);

//10_1041///
 void paraLvrt02_PERIOD(WORD wAttr, WORD *pwData);

//10_1042///
 void paraLvrt03_PERIOD(WORD wAttr, WORD *pwData);

//10_1043///
 void paraHfrt01_Lvl(WORD wAttr, WORD *pwData);

//10_1044///
 void paraHfrt02_Lvl(WORD wAttr, WORD *pwData);

//10_1045///
 void paraHfrt03_Lvl(WORD wAttr, WORD *pwData);

//10_1046///
 void paraHfrt01_PERIOD(WORD wAttr, WORD *pwData);

//10_1047///
 void paraHfrt02_PERIOD(WORD wAttr, WORD *pwData);

//10_1048///
 void paraHfrt03_PERIOD(WORD wAttr, WORD *pwData);

//10_1049///
 void paraLfrt01_Lvl(WORD wAttr, WORD *pwData);

//10_1050///
 void paraLfrt02_Lvl(WORD wAttr, WORD *pwData);

//10_1051///
 void paraLfrt03_Lvl(WORD wAttr, WORD *pwData);

//10_1052///
 void paraLfrt01_PERIOD(WORD wAttr, WORD *pwData);

//10_1053///
 void paraLfrt02_PERIOD(WORD wAttr, WORD *pwData);

//10_1054///
 void paraLfrt03_PERIOD(WORD wAttr, WORD *pwData);

/////////////////////////////////////////////
//////////// RESERVED 55 - 59 ///////////////
/////////////////////////////////////////////

//10_1060//
 void paraDC_OV_cmd(WORD wAttr, WORD *pwData);

//10_1061//
 void paraDC_UV_cmd(WORD wAttr, WORD *pwData);

//10_1062//
 void paraDC_OC_cmd(WORD wAttr, WORD *pwData);

//10_1063//
 void paraGrid_OV_cmd(WORD wAttr, WORD *pwData);

//10_1064//
 void paraGrid_UV_cmd(WORD wAttr, WORD *pwData);

//10_1065//
 void paraGrid_OC_cmd(WORD wAttr, WORD *pwData);

//10_1066//
 void paraGrid_OF_cmd(WORD wAttr, WORD *pwData);

//10_1067//
 void paraGrid_UF_cmd(WORD wAttr, WORD *pwData);

/////////////////////////////////////////////
//////////// RESERVED 68 - 69 ///////////////
/////////////////////////////////////////////

//10_1070//
void paraSocUpperLimitSet(WORD wAttr, WORD *pwData);

//10_1071//
void paraSocLowerLimitSet(WORD wAttr, WORD *pwData);

//10_1072//
void paraBankSOC(WORD wAttr, WORD *pwData);

//10_1073//
void paraBankSOH(WORD wAttr, WORD *pwData);

//10_1074//
void paraBankMinVolt(WORD wAttr, WORD *pwData);

//10_1075//
void paraBankMaxVolt(WORD wAttr, WORD *pwData);

//10_1076//
void paraBankMaxTemp(WORD wAttr, WORD *pwData);

//10_1077//
 void paraBankStatus(WORD wAttr, WORD *pwData);

//10_1078//
 void paraBankWarning(WORD wAttr, WORD *pwData);

//10_1079//
 void paraBankFault(WORD wAttr, WORD *pwData);

//10_1080//
 void paraPVWarning(WORD wAttr, WORD *pwData);

//10_1081//
 void paraPVFault(WORD wAttr, WORD *pwData);


/////////////////////////////////////////////
//////////// RESERVED 80 - 89 ///////////////
/////////////////////////////////////////////

//10_1090///
void paraVdcPerc(WORD wAttr, WORD *pwData);

//10_1091///
void paraVbattPerc(WORD wAttr, WORD *pwData);

//10_1092///
void paraIbattCharPerc(WORD wAttr, WORD *pwData);

//10_1093///
void paraIbattPerc(WORD wAttr, WORD *pwData);

//10_1094///
void paraVsaPerc(WORD wAttr, WORD *pwData);

//10_1095///
void paraVsbPerc(WORD wAttr, WORD *pwData);

//10_1096///
void paraVscPerc(WORD wAttr, WORD *pwData);

//10_1097///
void paraIoutCharPerc(WORD wAttr, WORD *pwData);

//10_1098///
void paraIoutPerc (WORD wAttr, WORD *pwData);

//10_1099///
void paraIoutR2Perc(WORD wAttr, WORD *pwData);

//10_1103///
void paraIoutR3Perc(WORD wAttr, WORD *pwData);

//10_1101///
void paraInvKwCharPerc(WORD wAttr, WORD *pwData);

//10_1102///
void paraInvKwPerc(WORD wAttr, WORD *pwData);

//10_1103///
void paraInvKvaPerc(WORD wAttr, WORD *pwData);

///////////////////////////////////////////////
////////////// RESERVED 104-109 ///////////////
///////////////////////////////////////////////

//10_1110///
void paraKpVgdse(WORD wAttr, WORD *pwData);

//10_1111///
void paraKiVgdse(WORD wAttr, WORD *pwData);

//10_1112///
void paraKpVgqse(WORD wAttr, WORD *pwData);

//10_1113///
void paraKiVgqse(WORD wAttr, WORD *pwData);

//10_1114///
void paraVgdseLimit (WORD wAttr, WORD *pwData);

//10_1115///
void paraVgqseLimit(WORD wAttr, WORD *pwData);

//10_1116///
void paraKpReactive(WORD wAttr, WORD *pwData);

//10_1117///
void paraKiReactive(WORD wAttr, WORD *pwData);

//10_1118///
void paraReactiveLimit(WORD wAttr, WORD *pwData);

//10_1119///
void paraKpActive(WORD wAttr, WORD *pwData);

//10_1120///
void paraKiActive(WORD wAttr, WORD *pwData);

//10_1121///
void paraActiveLimit(WORD wAttr, WORD *pwData);

//10_1122///
void paraKpIdc(WORD wAttr, WORD *pwData);

//10_1123///
void paraKiIdc(WORD wAttr, WORD *pwData);

//10_1124///
void paraIdcLimit(WORD wAttr, WORD *pwData);

//10_1125///
void paraKpIdse(WORD wAttr, WORD *pwData);

//10_1126///
void paraKiIdse(WORD wAttr, WORD *pwData);

//10_1127///
void paraKpIqse(WORD wAttr, WORD *pwData);

//10_1128///
void paraKiIqse(WORD wAttr, WORD *pwData);

//10_1129///
void paraIdseLimit(WORD wAttr, WORD *pwData);

//10_1130///
void paraIqseLimit(WORD wAttr, WORD *pwData);

//10_1131///
void paraKpVdc(WORD wAttr, WORD *pwData);

//10_1132///
void paraKiVdc(WORD wAttr, WORD *pwData);

//10_1133///
void paraVdcLimit(WORD wAttr, WORD *pwData);

//10_1134///
void paraKpAngle(WORD wAttr, WORD *pwData);

//10_1135///
void paraKiAngle(WORD wAttr, WORD *pwData);

//10_1136///
void paraAngleLimit(WORD wAttr, WORD *pwData);



//////////////////////////////////////////////////
//////////////////RESERVED 137-149 ///////////////
//////////////////////////////////////////////////

//10_1150///
void paraFacWriteEnable(WORD wAttr, WORD *pwData);

//10_1151///
void paraVariInitEnable(WORD wAttr, WORD *pwData);

//10_1152///
void paraFactoryMode(WORD wAttr, WORD *pwData);

//10_1153///
void paraSelctDoNum(WORD wAttr, WORD *pwData);

//10_1154///
void paraDcRelayEnable(WORD wAttr, WORD *pwData);

//10_1155///
void paraFactoryVoltCmd(WORD wAttr, WORD *pwData);

//10_1156///
void paraFactoryFanDuty(WORD wAttr, WORD *pwData);

//10_1157///
void paraDigitalFLT1_Enable(WORD wAttr, WORD *pwData);

//10_1158///
void paraDigitalFLT2_Enable(WORD wAttr, WORD *pwData);

//10_1159///
void paraAnalogFLT1_Enable(WORD wAttr, WORD *pwData);

//10_1160///
void paraAnalogFLT2_Enable(WORD wAttr, WORD *pwData);

//10_1161///
void paraExternalFLT1_Enable(WORD wAttr, WORD *pwData);

//10_1162///
void paraWarning_Enable(WORD wAttr, WORD *pwData);

//10_1163///
void paraRunningTime(WORD wAttr, WORD *pwData);

//10_1164///
void paraRTC_Enable(WORD wAttr, WORD *pwData);

//10_1165///
void paraRTC_1sec(WORD wAttr, WORD *pwData);

//10_1166///
void paraRTC_10sec(WORD wAttr, WORD *pwData);

//10_1167///
void paraRTC_1min(WORD wAttr, WORD *pwData);

//10_1168///
void paraRTC_10min(WORD wAttr, WORD *pwData);

//10_1169///
void paraRTC_1hour(WORD wAttr, WORD *pwData);

//10_1170///
void paraRTC_10hour(WORD wAttr, WORD *pwData);

//10_1171///
void paraRTC_1day(WORD wAttr, WORD *pwData);

//10_1172///
void paraRTC_10day(WORD wAttr, WORD *pwData);

//10_1173///
void paraRTC_1month(WORD wAttr, WORD *pwData);

//10_1174///
void paraRTC_10month(WORD wAttr, WORD *pwData);

//10_1175///
void paraRTC_1year(WORD wAttr, WORD *pwData);

//10_1176///
void paraRTC_10year(WORD wAttr, WORD *pwData);

//10_1177///
void paraRTC_week(WORD wAttr, WORD *pwData);

//10_1178///
void paraTimeSyncSet(WORD wAttr, WORD *pwData);

//10_1179///
void paraAngleTest(WORD wAttr, WORD *pwData);

//10_1180///
void paraPLLSet(WORD wAttr, WORD *pwData);

//10_1181///
void paraDbCompSet(WORD wAttr, WORD *pwData);

//10_1182///
void paraAutoResetNum(WORD wAttr, WORD *pwData);

//10_1183///
void paraTimeSyncEnable(WORD wAttr, WORD *pwData);

//10_1184///
void paraTimeSync_1sec(WORD wAttr, WORD *pwData);

//10_1185///
void paraTimeSync_10sec(WORD wAttr, WORD *pwData);

//10_1186///
void paraTimeSync_1min(WORD wAttr, WORD *pwData);

//10_1187///
void paraTimeSync_10min(WORD wAttr, WORD *pwData);

//10_1188///
void paraTimeSync_1hour(WORD wAttr, WORD *pwData);

//10_1189///
void paraTimeSync_10hour(WORD wAttr, WORD *pwData);

//10_1190///
void paraTimeSync_1day(WORD wAttr, WORD *pwData);

//10_1191///
void paraTimeSync_10day(WORD wAttr, WORD *pwData);

//10_1192///
void paraTimeSync_1month(WORD wAttr, WORD *pwData);

//10_1193///
void paraTimeSync_10month(WORD wAttr, WORD *pwData);

//10_1194///
void paraFacReserved1(WORD wAttr, WORD *pwData);

//10_1195///
void paraFacReserved2(WORD wAttr, WORD *pwData);

//10_1196///
void paraFacReserved3(WORD wAttr, WORD *pwData);

//10_1197///
void paraFacReserved4(WORD wAttr, WORD *pwData);

//10_1198///
void paraFacReserved5(WORD wAttr, WORD *pwData);

//10_1199///
void paraFacReserved6(WORD wAttr, WORD *pwData);



//--------End 0x10--------------------------------
#endif

#endif /* SRC_COMMON_HW_INCLUDE_RS_PARA_H_ */
