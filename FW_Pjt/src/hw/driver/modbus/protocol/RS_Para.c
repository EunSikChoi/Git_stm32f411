/*
 * RS_Para.c
 *
 *  Created on: 2021. 3. 29.
 *      Author: 82109
 */


#include "modbus.h"
#include "ModProt.h"
#include "RS_Para.h"

#define ST_FAULT 0x0001
#define ST_OK    0x0000

#ifdef _USE_HW_W5500

#define LPMS_PCS_COM_ERR_TIME 5

// For Test 1000 ~ 1009 //
extern WORD wRefModeVoltage, wRefModeVoltage1, wRefModeVoltage2, wRefModeVoltage3, wRefModeVoltage4, wRefModeVoltage5;
extern WORD wRefModeVoltage6, wRefModeVoltage7, wRefModeVoltage8, wRefModeVoltage9, wRefModeVoltage10, wSelCmdMode;


///////////////////////////
// 0x04 : Input Register //
//     float : 4byte     //
///////////////////////////
WORD  wTestData[36];
float fTestData[20];


///////////////////////////
// 0x04 : Input Register //
//   Integer : 2byte     //
///////////////////////////
//--------------------------------------------------------------------------------------


#if 1
WORD reservedData1; // 0x04 �б� ���� //

void paraReserved1(WORD *pwData) {

			*pwData = 0xFFFF;


}
#endif

//04_00//
void paraParameter_ver(WORD *pwData)
{

			*pwData = 1;

}

//04_01//
void paraSoftware_ver(WORD *pwData)
{

			*pwData = 2;

}

//04_02//
void paraSourceInfo(WORD *pwData)
{

//		#ifdef PV_SOURCE
//     *pwData = 0; //PV //0
//    #else
//		 *pwData = 1; //ESS  //1
//    #endif

			*pwData = 	3;		//TEST// ���� ����//


}

//04_03//
void paraSysCapaInfo(WORD *pwData)
//100kW:0 250 :1 500:2 1000:3 1500:4 2000:5 2500:6 3000:7//
{


     *pwData = 4;

}

/////////////////////////////////////////////
//////////// RESERVED 02 - 09 ///////////////
/////////////////////////////////////////////

//04_10//
void paraInfoDoStatus(WORD *pwData)
{
			*pwData = 4;
}

//04_11//
void paraInfoDiStatus(WORD *pwData)
{

			*pwData = 5;

}

//04_12//
void paraProtDigitalErrStatus1(WORD *pwData)
{

			*pwData = 6;

}

//04_13//
void paraProtDigitalErrStatus2(WORD *pwData)
{

			*pwData = 7;

}

//04_14//
void paraProtAnalogErrStatus1(WORD *pwData)
{

			*pwData = 8;

}

//04_15//
void paraProtAnalogErrStatus2(WORD *pwData)
{

			*pwData = 9;

}

//04_16//
void paraProtHwErrStatus(WORD *pwData)
{
	*pwData = 1;
}

//04_17//
void paraProtExtErrStatus(WORD *pwData)
{

	*pwData = 2;

}

//04_18//
void parawProtWarnStatus(WORD *pwData)
{

	*pwData = 3;

}

//04_19//
void	paraOperation_State(WORD *pwData)
{
	*pwData = 4;
}

//04_20//
void	paraCharge_State(WORD *pwData)
{
	*pwData = 5;
}

//04_21//
void	paraControl_Mode_State(WORD *pwData)
{
	*pwData = 6; // ���� ���� // wModeSel // wInfoControl_Mode_State
}

//04_22//
void paraGrid_Uan(WORD *pwData)
{

	*pwData = 7;

}

//04_23//
void paraGrid_Ubn(WORD *pwData)
{

	*pwData = 8;

}

//04_23//
void paraGrid_Ucn(WORD *pwData)
{

	*pwData = 9;

}


//04_25//
void paraInv_Ian(SWORD *pwData)
{

	*pwData = 0;

}

//04_26//
void paraInv_Ibn(SWORD *pwData)
{

	*pwData = 1;

}

//04_27//
void paraInv_Icn(SWORD *pwData)
{

	*pwData = 2;

}

//04_28//
void	paraSysWe(WORD *pwData)
{

  *pwData = 3;

}

//04_29//
void pararInv_Kw_rms(SWORD *pwData)
{

	*pwData = 4 ;

}

//04_30//
void pararInv_Kva_rms(SWORD *pwData)
{

	*pwData = (SWORD)5 ;

}

//04_31//
void	parapowerFactor(SWORD *pwData)
{
	*pwData = (SWORD)6;
}

//04_32//
void	pararVdcCheck(WORD *pwData)
{

  *pwData = (WORD)7;

}

//04_33//
void	paraDcVTG(SWORD *pwData)
{

*pwData = (SWORD)8;

}

//04_34//
void pararIbatt_rms(SWORD *pwData)
{

	*pwData = (SWORD)9;

}

//04_35//
void	pararVdc_Kw_rms(SWORD *pwData)
{

*pwData = (SWORD)0;

}

//04_36//
void	paraPCS_Temper_1(SWORD *pwData)
{

*pwData = (SWORD)1;

}

//04_37//
void	paraPCS_Temper_2(SWORD *pwData)
{
*pwData = (SWORD)2;

}

//04_38//
void	paraPCS_Temper_3(SWORD *pwData)
{

*pwData = (SWORD)3;

}

//04_39//
void	paraPCS_Temper_4(SWORD *pwData)
{

*pwData = (SWORD)4;

}

//04_40//
void	paraPCS_Temper_5(SWORD *pwData)
{

*pwData = (SWORD)5;

}

//04_41//
void	paraPCS_Temper_6(SWORD *pwData)
{

*pwData = (SWORD)6;

}

//04_42//
void	paraPCS_Temper_7(SWORD *pwData)
{

*pwData = (SWORD)7;

}

//04_43//
void	paraAcChaPwrKwh(SWORD *pwData)
{

   *pwData = (SWORD)8;

}

//04_44//
void	paraAcChaPwrMwh(SWORD *pwData)
{

*pwData = (SWORD)9;

}

//04_45//
void	paraAcChaPwrGwh(SWORD *pwData)
{

*pwData = (SWORD)0;

}

//04_46//
void	paraAcDisChaPwrKwh(SWORD *pwData)
{

*pwData = (SWORD)1;

}

//04_47//
void	paraAcDisChaPwrMwh(SWORD *pwData)
{

*pwData = (SWORD)2;

}

//04_48//
void	paraAcDisChaPwrGwh(SWORD *pwData)
{

*pwData = (SWORD)3;

}

//04_49//
void	paraDcChaPwrKwh(SWORD *pwData)
{

*pwData = (SWORD)4;

}

//04_50//
void	paraDcChaPwrMwh(SWORD *pwData)
{

*pwData = (SWORD)5;

}

//04_51//
void	paraDcChaPwrGwh(SWORD *pwData)
{

*pwData = (SWORD)6;

}

//04_52//
void	paraDcDisChaPwrKwh(SWORD *pwData)
{

*pwData = (SWORD)7;

}


//04_53//
void	paraDcDisChaPwrMwh(SWORD *pwData)
{

*pwData = (SWORD)8;

}

//04_54//
void	paraDcDisChaPwrGwh(SWORD *pwData)
{

*pwData = (SWORD)9;

}

//04_55//
void	paraErrAllStatus(WORD *pwData)
{

*pwData = (WORD)0;

}

//04_56//
void	paraDailyChaPwrKwh(SWORD *pwData)
{

*pwData = (SWORD)1;

}

//04_57//
void	paraDailyDisCharPwrKwh(SWORD *pwData)
{

*pwData = (SWORD)2;

}

//04_58//
void	paraTwoagoChaPwrKwh(SWORD *pwData)
{

*pwData = (SWORD)3;

}

//04_59//
void	paraTwoagoDisChaPwrKwh(SWORD *pwData)
{

*pwData = (SWORD)3;

}

//04_60//
void	paraDccbState(WORD *pwData)
{

*pwData = (WORD)4;

}


//04_61//
void	paraHour0708Kwh(WORD *pwData)
{

*pwData = (WORD)5;

}

//04_62//
void	paraHour0809Kwh(WORD *pwData)
{

*pwData = (WORD)6;

}

//04_63//
void	paraHour0910Kwh(WORD *pwData)
{

*pwData = (WORD)7;

}

//04_64//
void	paraHour1011Kwh(WORD *pwData)
{

*pwData = (WORD)8;

}

//04_65//
void	paraHour1112Kwh(WORD *pwData)
{

*pwData = (WORD)9;

}

//04_66//
void	paraHour1213Kwh(WORD *pwData)
{

*pwData = (WORD)0;

}

//04_67//
void	paraHour1314Kwh(WORD *pwData)
{

*pwData = (WORD)1;

}

//04_68//
void	paraHour1415Kwh(WORD *pwData)
{

*pwData = (WORD)2;

}

//04_69//
void	paraHour1516Kwh(WORD *pwData)
{

*pwData = (WORD)3;

}

//04_70//
void	paraHour1617Kwh(WORD *pwData)
{

*pwData = (WORD)4;

}

//04_71//
void	paraHour1718Kwh(WORD *pwData)
{

*pwData = (WORD)5;

}

//04_72//
void	paraHour1819Kwh(WORD *pwData)
{

*pwData = (WORD)6;

}




/////////////////////////////////////////////
//////////// RESERVED 61 - 69 ///////////////
/////////////////////////////////////////////



/////////////////////////////////////
// 0x10 : Write Multiple Register  //
//        Integer : 2byte          //
/////////////////////////////////////



#if 1
WORD reservedData;//0x10 ���� Reserved //
void paraReserved(WORD wAttr, WORD *pwData) {
		if (MB_READ == wAttr) {
			/* Read */

			*pwData = 0xFFFF;

		} else {
			/* Write */
	       //	reservedData = 	(WORD)*pwData;
		}

}
#endif



//10_1000//
void paraKwh_Reset(WORD wAttr, WORD *pwData){

	if (MB_READ == wAttr) {
			/* Read */
			*pwData = (WORD)wRefModeVoltage ;

		} else {
			/* Write */


			wRefModeVoltage = (WORD)*pwData;



		}

}



//10_1001//

void paraINV_errReset(WORD wAttr, WORD *pwData){

      if(MB_READ == wAttr) {

    		*pwData = (WORD)wRefModeVoltage1 ;

    }
    else
    {

    	wRefModeVoltage1 = (WORD)*pwData;



    }

}


//10_1002//
void paraRemote_Sel(WORD wAttr, WORD *pwData){

     if(MB_READ == wAttr) {

        *pwData = (WORD)wSelCmdMode;


    }
    else {

    	wSelCmdMode = (WORD)*pwData;
    }





}

//10_1003//

void paraEmergencyStop(WORD wAttr, WORD *pwData){

	if (MB_READ == wAttr) {
			/* Read */
			*pwData = (WORD)wRefModeVoltage3 ;

		} else {
			/* Write */

			wRefModeVoltage3 = (WORD)*pwData;




		}



}

//10_1004//
void parawAutoCVMode(WORD wAttr, WORD *pwData){

	if (MB_READ == wAttr) {

			/* Read */

     *pwData = wRefModeVoltage4;


		} else {

			wRefModeVoltage4 = (WORD)*pwData;


		}



}


//10_1005//
void paraRefModeVoltage(WORD wAttr, WORD *pwData){

	if (MB_READ == wAttr) {
			/* Read */
			*pwData = wRefModeVoltage5;

		} else {
			/* Write */

	    	wRefModeVoltage5 = (WORD)*pwData;
		}



}

//10_1006//
void paraCP_Step_Flag(WORD wAttr, WORD *pwData){

	if (MB_READ == wAttr) {
			/* Read */
			*pwData = wRefModeVoltage6 ;

		} else {
			/* Write */

			wRefModeVoltage6 = (WORD)*pwData;



		}



}

//10_1007//
void paraTarget_Gradient(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = wRefModeVoltage7 ;

		} else {
			/* Write */

			wRefModeVoltage7 = (WORD)*pwData;

		}


}

//10_1008//
void paraRunCmd(WORD wAttr, WORD *pwData)
{

	if(MB_READ == wAttr) {
		/* Read Holding Registers */

		*pwData = wRefModeVoltage8 ;


	} else {
		/* Write Single Coil */
		wRefModeVoltage8 = (WORD)*pwData;


	}

}

//10_1009//
/**
 * @brief Operating mode
 *
 * @remarks
 * - Modes are defined in Vari.H as below.
 *      CC Mode: 0
 *      CV Mode: 1
 *      CP Mode: 2
 */

void parafStateControl(WORD wAttr, WORD *pwData){
	if (MB_READ == wAttr) {
  	/* Read */

		*pwData = wRefModeVoltage9 ;


	} else {
		/* Write */

		wRefModeVoltage9 = (WORD)*pwData;

	}

}



//10_1010//
void paraiActiveCmd(SWORD wAttr, SWORD *pwData)
{

	if (MB_READ == wAttr) {
		/* Read */
		*pwData = 1;

	} else {
		/* Write */

    		//	wRemote_Kw_Ref = (SWORD)*pwData;

	}

}


//10_1011//
void paraiCVCmd(WORD wAttr, WORD *pwData){ //wRemote_VdcRef

		if (MB_READ == wAttr) {
		/* Read */
		*pwData = 22;

	} else {
		/* Write */

    			//wRemote_VdcRef = (WORD)*pwData;

	}

}

//10_1012//
void paraiCCCmd(SWORD wAttr, SWORD *pwData){

		if (MB_READ == wAttr) {
		/* Read */
		*pwData = 34;

	} else {
		/* Write */

    			//wRemote_iIdcRef = (SWORD)*pwData;


	}

}

//10_1013//
void paraiReActiveCmd (WORD wAttr, WORD *pwData){

#if 1
	if (MB_READ == wAttr) {

		/* Read */
		*pwData = 12;

	} else {
		/* Write */

    			//wRemote_Kva_Ref = (SWORD)*pwData;

	}
#endif

}

//10_1014//

void paraiUpsVoltCmd  (WORD wAttr, WORD *pwData){

#if 1
	if (MB_READ == wAttr) {

		/* Read */
		*pwData = 232;

	} else {
		/* Write */

    			//iUPS_Volt_Ref = (SWORD)*pwData;

	}
#endif

}

//10_1015///
void paraIdseRefCmd(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 1    ;

		} else {
			/* Write */

	  //  iIdseRefCmd    = (SWORD)*pwData;

		}


}

//10_1016//

void parawCheckComm(WORD wAttr, WORD *pwData){ //wPlcCheckFlag //w485CheckFlag

			if (MB_READ == wAttr) {
		/* Read */
		*pwData = 11;

	} else {
		/* Write */


    		//	w485CheckFlag = (WORD)*pwData;


	}

}

//10_1017//
void paraTotalComAlarm(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 33 ; //0: ���� 1: PMS 2:BMS1 4:BMS2 Error Comm.

		} else {
			/* Write */

	  //  wTotalCommAlarm = (WORD)*pwData;

		}


}

//10_1018//
void paraInfoExternal_Status(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 12 ; //0: ���� 1: PMS 2:BMS1 4:BMS2 Error Comm.

		} else {
			/* Write */

	   // wInfoExternal_Status = (WORD)*pwData;

		}


}

//10_1019//
void paraPVAutoRunEnable(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 33 ; //0: Disable 1: Enable //

		} else {
			/* Write */

	   // wPVAutoRunEnable = (WORD)*pwData;

		}


}

//10_1020//
void paraSetRunHourTime(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 11 ;

		} else {
			/* Write */

	    //wSetRunHourTime = (WORD)*pwData;

		}


}

//10_1021//
void paraSetRunMinTime(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 3 ;

		} else {
			/* Write */

	   // wSetRunMinTime = (WORD)*pwData;

		}


}

//10_1022//
void paraSetStopHourTime(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 4 ;

		} else {
			/* Write */

	 //   wSetStopHourTime = (WORD)*pwData;

		}


}

//10_1023//
void paraSetStopMinTime(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 7 ;

		} else {
			/* Write */

	 //   wSetStopMinTime = (WORD)*pwData;

		}


}

//10_1024//
void paraPVdc_Start(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 1 ;

		} else {
			/* Write */

	   // wPVdc_Start = (WORD)*pwData;

		}


}

//10_1025//
void paraPVidc_Standby(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 2 ;

		} else {
			/* Write */

	  // wPVidc_Standby = (WORD)*pwData;

		}


}

//10_1026//
void paraPVDelayCount(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 3 ;

		} else {
			/* Write */

	 //   wPVDelayCount = (WORD)*pwData;

		}


}


////////////////////////////////////////////////////
///////////10_1024 ~ 10_29// RESERVED///////////////
////////////////////////////////////////////////////


//10_1030//
void paraFrtEnable(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 4 ; //0: None 1: LVRT 2:HFRT 4:LFRT

		} else {
			/* Write */



	        //  wFRT_ENB= (WORD)*pwData;



		}


}
//--------------------------------------------


//10_1031//
void paraHvrt01_Lvl(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 5  ; //0: None 1: Hvrt 2:HFRT 4:LFRT

		} else {
			/* Write */

	  //  wHVRT_01_Trip_Perc  = (WORD)*pwData;

		}


}

//10_1032//
void paraHvrt02_Lvl(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 6  ; //0: None 1: Hvrt 2:HFRT 4:LFRT

		} else {
			/* Write */

	  //  wHVRT_02_Trip_Perc  = (WORD)*pwData;

		}


}


//10_1033//
void paraHvrt03_Lvl(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 6  ; //0: None 1: Hvrt 2:HFRT 4:LFRT

		} else {
			/* Write */

	   // wHVRT_03_Trip_Perc  = (WORD)*pwData;

		}


}

//10_1034//
void paraHvrt01_PERIOD(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 1  ;

		} else {
			/* Write */

	    //wHVRT_01_PERIOD  = (WORD)*pwData;

		}


}

//10_1035//
void paraHvrt02_PERIOD(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 2  ;

		} else {
			/* Write */

	  //  wHVRT_02_PERIOD  = (WORD)*pwData;

		}


}

//10_1036//
void paraHvrt03_PERIOD(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 3   ;

		} else {
			/* Write */

	  //  wHVRT_03_PERIOD   = (WORD)*pwData;

		}


}



//----------------------------------------------

//10_1037//
void paraLvrt01_Lvl(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 4  ; //0: None 1: LVRT 2:HFRT 4:LFRT

		} else {
			/* Write */

	    //wLVRT_01_Trip_Perc  = (WORD)*pwData;

		}


}

//10_1038//
void paraLvrt02_Lvl(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 8  ; //0: None 1: LVRT 2:HFRT 4:LFRT

		} else {
			/* Write */

	 //   wLVRT_02_Trip_Perc  = (WORD)*pwData;

		}


}


//10_1039//
void paraLvrt03_Lvl(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 1  ; //0: None 1: LVRT 2:HFRT 4:LFRT

		} else {
			/* Write */

	 //   wLVRT_03_Trip_Perc  = (WORD)*pwData;

		}


}

//10_1040//
void paraLvrt01_PERIOD(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 3  ;

		} else {
			/* Write */

	   // wLVRT_01_PERIOD  = (WORD)*pwData;

		}


}

//10_1041//
void paraLvrt02_PERIOD(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 1  ;

		} else {
			/* Write */

	   // wLVRT_02_PERIOD  = (WORD)*pwData;

		}


}

//10_1042//
void paraLvrt03_PERIOD(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 8   ;

		} else {
			/* Write */

	 //   wLVRT_03_PERIOD   = (WORD)*pwData;

		}


}

//////////////////////////////////////////////////////////////////////////////////////////////////////

//10_1043//
void paraHfrt01_Lvl(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 1    ;

		} else {
			/* Write */

	 //   wHFRT_01_Trip_Perc   = (WORD)*pwData;

		}


}

//10_1044//
void paraHfrt02_Lvl(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 3   ;

		} else {
			/* Write */

	   // wHFRT_02_Trip_Perc   = (WORD)*pwData;

		}


}

//10_1045//
void paraHfrt03_Lvl(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 3   ;

		} else {
			/* Write */

	  //  wHFRT_03_Trip_Perc   = (WORD)*pwData;

		}


}

//10_1046//
void paraHfrt01_PERIOD(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 3    ;

		} else {
			/* Write */

	   //wHFRT_01_PERIOD    = (WORD)*pwData;

		}


}

//10_1047//
void paraHfrt02_PERIOD(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 4    ;

		} else {
			/* Write */

	  //  wHFRT_02_PERIOD    = (WORD)*pwData;

		}


}

//10_1048//
void paraHfrt03_PERIOD(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 1    ;

		} else {
			/* Write */

	//    wHFRT_03_PERIOD    = (WORD)*pwData;

		}


}

//10_1049//
void paraLfrt01_Lvl(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 2   ;

		} else {
			/* Write */

	   // wLFRT_01_Trip_Perc   = (WORD)*pwData;

		}


}

//10_1050//
void paraLfrt02_Lvl(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 3   ;

		} else {
			/* Write */

	   // wLFRT_02_Trip_Perc   = (WORD)*pwData;

		}


}

//10_1051//
void paraLfrt03_Lvl(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 3   ;

		} else {
			/* Write */

	  //  wLFRT_03_Trip_Perc   = (WORD)*pwData;

		}


}

//10_1052//
void paraLfrt01_PERIOD(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 6    ;

		} else {
			/* Write */

	   // wLFRT_01_PERIOD    = (WORD)*pwData;

		}


}

//10_1053//
void paraLfrt02_PERIOD(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 1    ;

		} else {
			/* Write */

	   // wLFRT_02_PERIOD    = (WORD)*pwData;

		}


}

//10_1054//
void paraLfrt03_PERIOD(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 2    ;

		} else {
			/* Write */

	    //wLFRT_03_PERIOD    = (WORD)*pwData;

		}


}

////////////////////////////////////////////////////
///////////10_1055 ~ 10_59// RESERVED///////////////
////////////////////////////////////////////////////

//10_1060//
void paraDC_OV_cmd(WORD wAttr, WORD *pwData){
#if 1
  	if (MB_READ == wAttr) {
		/* Read */
		*pwData = 4;

//#endif
	} else {
		/* Write */

			 // wRefBatOV = *pwData;

	}
#endif
}
//10_1061//
void paraDC_UV_cmd(WORD wAttr, WORD *pwData){
#if 1
    	if (MB_READ == wAttr) {
		/* Read */
		*pwData = 6;

	} else {
		/* Write */

			//wRefBatUV = *pwData;

	}
#endif
}

//10_1062//

void paraDC_OC_cmd(WORD wAttr, WORD *pwData){
#if 1
      	if (MB_READ == wAttr) {
		/* Read */
		*pwData = 2;

	} else {
		/* Write */

			//wRefBatOC = *pwData;


	}
#endif
}
//10_1063//
void paraGrid_OV_cmd(WORD wAttr, WORD *pwData){

#if 1
if (MB_READ == wAttr) {
		/* Read */
		*pwData = 9;

	} else {
		/* Write */

		//	wRefGridOV = *pwData;

	}
#endif

}
//10_1064//

void paraGrid_UV_cmd(WORD wAttr, WORD *pwData){

#if 1
  if (MB_READ == wAttr) {
		/* Read */
		*pwData = 1;

	} else {
		/* Write */

		//	wRefGridUV = *pwData;

	}
#endif
}

//10_1065//
void paraGrid_OC_cmd(WORD wAttr, WORD *pwData){
#if 1

  if (MB_READ == wAttr) {
		/* Read */
		*pwData = 2;

	} else {
		/* Write */

			//wRefGridOC = *pwData;


	}
#endif
}

//10_1066//
void paraGrid_OF_cmd(WORD wAttr, WORD *pwData){
#if 1
    if (MB_READ == wAttr) {
		/* Read */
		*pwData = 3;

	} else {
		/* Write */

		//	wRefGridOF = *pwData;


	}
#endif
}
//10_1067//

void paraGrid_UF_cmd(WORD wAttr, WORD *pwData){
#if 1

    if (MB_READ == wAttr) {
		/* Read */
		*pwData = 4;

	} else {
		/* Write */

			//wRefGridUF = *pwData;


	}
	#endif
}

////////////////////////////////////////////////////
///////////10_1068 ~ 10_69// RESERVED///////////////
////////////////////////////////////////////////////


//10_1070//
void paraSocUpperLimitSet(WORD wAttr, WORD *pwData){

		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 7 ;

		} else {
			/* Write */

	    			//wSocUpperLimitSet = (WORD)*pwData;
    }
}

//10_1071//
void paraSocLowerLimitSet(WORD wAttr, WORD *pwData)
{

		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 1 ;

		} else {
			/* Write */

	    	//wSocLowerLimitSet = (WORD)*pwData;
		}

}

//10_1072//
void paraBankSOC(WORD wAttr, WORD *pwData)
{

		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 2 ;

		} else {
			/* Write */

	    	//wKaco_Soc = (WORD)*pwData;
		}


}


//10_1073//
void paraBankSOH(WORD wAttr, WORD *pwData)
{

		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 4 ;

		} else {
			/* Write */

	    	//wKaco_SoH = (WORD)*pwData;
		}


}

//10_1074//
void paraBankMinVolt(WORD wAttr, WORD *pwData)
{

		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 2 ;

		} else {
			/* Write */

	    //	wKaco_BankMinVolt = (WORD)*pwData;
		}


}

//10_1075//
void paraBankMaxVolt(WORD wAttr, WORD *pwData)
{

		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 3 ;

		} else {
			/* Write */

	    	//wKaco_BankMaxVolt = (WORD)*pwData;
		}


}

//10_1076//
void paraBankMaxTemp(WORD wAttr, WORD *pwData)
{

		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 1 ;

		} else {
			/* Write */

	    //	wKaco_BankMaxTemp = (WORD)*pwData;
		}


}

//10_1077//
void paraBankStatus(WORD wAttr, WORD *pwData)
{

		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 2 ;

		} else {
			/* Write */

	    //	wKaco_Status = (WORD)*pwData;
		}


}
//10_1078//
void paraBankWarning(WORD wAttr, WORD *pwData){

	if (MB_READ == wAttr) {
			/* Read */
			*pwData = 4 ;

		} else {
			/* Write */

	    	//wKaco_Warning = (WORD)*pwData;
		}

}
//10_1079//
void paraBankFault(WORD wAttr, WORD *pwData){

	if (MB_READ == wAttr) {
			/* Read */
			*pwData = 1 ;

		} else {
			/* Write */

	    //	wKaco_Fault = (WORD)*pwData;
		}
}

//10_1080//
void paraPVWarning(WORD wAttr, WORD *pwData){

	if (MB_READ == wAttr) {
			/* Read */
			*pwData = 1 ;

		} else {
			/* Write */

	    	//wPV_Warning = (WORD)*pwData;
		}

}
//10_1081//
void paraPVFault(WORD wAttr, WORD *pwData){

	if (MB_READ == wAttr) {
			/* Read */
			*pwData = 3 ;

		} else {
			/* Write */

	    	//wPV_Fault = (WORD)*pwData;
		}
}

////////////////////////////////////////////////////
///////////10_1080 ~ 10_89// RESERVED///////////////
////////////////////////////////////////////////////

//10_1090///
void paraVdcPerc(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 2    ;

		} else {
			/* Write */

	  //  wVdcPerc    = (WORD)*pwData;



		}


}

//10_1091///
void paraVbattPerc(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 33;

		} else {
			/* Write */

	   // wVbattPerc    = (WORD)*pwData;

		}


}

//10_1092///
void paraIbattCharPerc(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 2    ;

		} else {
			/* Write */

	   // wIbatt_Char_Perc    = (WORD)*pwData;

		}


}

//10_1093///
void paraIbattPerc(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 3    ;

		} else {
			/* Write */

	    //wIbattPerc    = (WORD)*pwData;

		}


}

//10_1094///
void paraVsaPerc(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 1    ;

		} else {
			/* Write */

	   // wVsaPerc    = (WORD)*pwData;

		}


}

//10_1095///
void paraVsbPerc(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 2    ;

		} else {
			/* Write */

	  // wVsbPerc    = (WORD)*pwData;

		}


}

//10_1096///
void paraVscPerc(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 1    ;

		} else {
			/* Write */

	  // wVscPerc    = (WORD)*pwData;

		}


}

//10_1097///
void paraIoutCharPerc(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 2     ;

		} else {
			/* Write */

	    //wIout_Char_Perc     = (WORD)*pwData;

		}


}

//10_1098///
void paraIoutPerc(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 33     ;

		} else {
			/* Write */

	   // wIoutPerc     = (WORD)*pwData;

		}


}

//10_1099///
void paraIoutR2Perc(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 33    ;

		} else {
			/* Write */

	   // wIoutR2_Perc    = (WORD)*pwData;

		}


}

//10_1100///
void paraIoutR3Perc(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 2    ;

		} else {
			/* Write */

	    //wIoutR3_Perc    = (WORD)*pwData;

		}


}

//10_1101///
void paraInvKwCharPerc(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 3    ;

		} else {
			/* Write */

	    //wInvKw_CharPerc    = (WORD)*pwData;

		}


}

//10_1102///
void paraInvKwPerc(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 5    ;

		} else {
			/* Write */

	  //  wInvKwPerc    = (WORD)*pwData;

		}


}

//10_1103///
void paraInvKvaPerc(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 3    ;

		} else {
			/* Write */

	   // wInvKvaPerc    = (WORD)*pwData;

		}


}

///////////////////////////////////////////////
//////////////RESERVED 104-109 ////////////////
///////////////////////////////////////////////


//10_1110///
void paraKpVgdse(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 1    ;

		} else {
			/* Write */

	   // wKpVgdse    = (WORD)*pwData;

		}


}

//10_1111///
void paraKiVgdse(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 2    ;

		} else {
			/* Write */

	    //wKiVgdse    = (WORD)*pwData;

		}


}

//10_1112///
void paraKpVgqse(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 4    ;

		} else {
			/* Write */

	   // wKpVgqse    = (WORD)*pwData;

		}


}

//10_1113///
void paraKiVgqse(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 5    ;

		} else {
			/* Write */

	  //  wKiVgqse    = (WORD)*pwData;

		}


}

//10_1114///
void paraVgdseLimit (WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 3    ;

		} else {
			/* Write */

	   // wVgdse_Limit    = (WORD)*pwData;

		}


}

//10_1115///
void paraVgqseLimit(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 1            ;

		} else {
			/* Write */

	   // wVgqse_Limit            = (WORD)*pwData;

		}


}

//10_1116///
void paraKpReactive(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 11     ;

		} else {
			/* Write */

	  //  wKpReactive     = (WORD)*pwData;

		}


}

//10_1117///
void paraKiReactive(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 11     ;

		} else {
			/* Write */

	  //  wKiReactive     = (WORD)*pwData;

		}


}

//10_1118///
void paraReactiveLimit(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 3    ;

		} else {
			/* Write */

	  //  wReactive_Limit    = (WORD)*pwData;

		}


}

//10_1119///
void paraKpActive(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 2     ;

		} else {
			/* Write */

	  //  wKpActive     = (WORD)*pwData;

		}


}

//10_1120///
void paraKiActive(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 2      ;

		} else {
			/* Write */

	  //  wKiActive      = (WORD)*pwData;

		}


}

//10_1121///
void paraActiveLimit(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 4     ;

		} else {
			/* Write */

	  //  wActive_Limit     = (WORD)*pwData;

		}


}

//10_1122///
void paraKpIdc(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 11    ;

		} else {
			/* Write */

	   // wKpIdc    = (WORD)*pwData;

		}


}

//10_1123///
void paraKiIdc(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 1     ;

		} else {
			/* Write */

	  //  wKiIdc     = (WORD)*pwData;

		}


}

//10_1124///
void paraIdcLimit(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 22    ;

		} else {
			/* Write */

	   // wIdc_Limit    = (WORD)*pwData;

		}


}

//10_1125///
void paraKpIdse(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 3    ;

		} else {
			/* Write */

	//    wKpIdse    = (WORD)*pwData;

		}


}

//10_1126///
void paraKiIdse(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 3    ;

		} else {
			/* Write */

	 //   wKiIdse    = (WORD)*pwData;

		}


}

//10_1127///
void paraKpIqse(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 33    ;

		} else {
			/* Write */

	   // wKpIqse    = (WORD)*pwData;

		}


}

//10_1128///
void paraKiIqse(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 3    ;

		} else {
			/* Write */

	  //  wKiIqse    = (WORD)*pwData;

		}


}

//10_1129///
void paraIdseLimit(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 33    ;

		} else {
			/* Write */

	   // wIdse_Limit    = (WORD)*pwData;

		}


}

//10_1130///
void paraIqseLimit(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 3    ;

		} else {
			/* Write */

	   // wIqse_Limit    = (WORD)*pwData;

		}


}

//10_1131///
void paraKpVdc(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 3    ;

		} else {
			/* Write */

	   // wKpVdc    = (WORD)*pwData;

		}


}

//10_1132///
void paraKiVdc(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 3    ;

		} else {
			/* Write */

	 //   wKiVdc    = (WORD)*pwData;

		}


}

//10_1133///
void paraVdcLimit(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 3    ;

		} else {
			/* Write */

	  //  wVdc_Limit    = (WORD)*pwData;

		}


}

//10_1134///
void paraKpAngle(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 1    ;

		} else {
			/* Write */

	    //wKpAngle    = (WORD)*pwData;

		}


}

//10_1135///
void paraKiAngle(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 1    ;

		} else {
			/* Write */

	    //wKiAngle    = (WORD)*pwData;

		}


}

//10_1136///
void paraAngleLimit(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 1    ;

		} else {
			/* Write */

	   // wAngle_Limit    = (WORD)*pwData;

		}


}



/////////////////////////////////////////////////





///////////////////////////////////////////////
//////////////RESERVED 137-149 ////////////////
///////////////////////////////////////////////

//10_1150///
void paraFacWriteEnable(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 1    ;

		} else {
			/* Write */


	          wTestData[6] = *pwData;





	     }



}

//10_1151///
void paraVariInitEnable(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 1    ;

		} else {
			/* Write */

	          wTestData[7] = *pwData;


	       }


}

//10_1152///
void paraFactoryMode(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 12    ;

		} else {
			/* Write */
			//if( (wFacWriteEnable == 1) && (wInvBlockFlag == 1) ) // if 10_1130 Value Enable is 1 //
			//{

//	        wFactoryMode    = (WORD)*pwData;


	 //   }

		}


}

//10_1153///
void paraSelctDoNum(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 1    ;

		} else {
			/* Write */


	     //   wSelctDoNum    = (WORD)*pwData;



		}


}

//10_1154///
void paraDcRelayEnable(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 12    ;

		} else {
			/* Write */
			//  if(wFacWriteEnable == 1) // if 10_1130 Value Enable is 1 //
			 // {

	      //    wTestData[8] = (WORD)*pwData;


	     // }

		}


}

//10_1155///
void paraFactoryVoltCmd(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 12;

		} else {
			/* Write */
		 // if(wFacWriteEnable == 1) // if 10_1130 Value Enable is 1 //
		//	{

	       // iFactoryVoltCmd    = (WORD)*pwData;

	   // }

		}


}

//10_1156///
void paraFactoryFanDuty(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 3   ;

		} else {
			/* Write */

	      //  iFactoryFanDuty    = (WORD)*pwData;


		}


}

//10_1157///
void paraDigitalFLT1_Enable(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 3;

		} else {
			/* Write */

	      //  wDigitalFLT1_Enable    = (WORD)*pwData;


		}


}

//10_1158///
void paraDigitalFLT2_Enable(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 4 ;

		} else {
			/* Write */

	       // wDigitalFLT2_Enable    = (WORD)*pwData;


		}


}

//10_1159///
void paraAnalogFLT1_Enable(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 6   ;  //wAnalogFLT1_Enable

		} else {
			/* Write */

	      //  wAnalogFLT1_Enable    = (WORD)*pwData;


		}


}

//10_1160///
void paraAnalogFLT2_Enable(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 8   ;

		} else {
			/* Write */


	      //  wAnalogFLT2_Enable    = (WORD)*pwData;


		}


}

//10_1161///
void paraExternalFLT1_Enable(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 8   ;

		} else {
			/* Write */


	    //    wExternalFLT1_Enable    = (WORD)*pwData;


		}


}

//10_1162///
void paraWarning_Enable(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 5   ;

		} else {
			/* Write */

	    //    wWarning_Enable    = (WORD)*pwData;


		}


}


//10_1163///
void paraRunningTime(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 7;

		} else {
			/* Write */


	       // wRunTimePrd    = (WORD)*pwData;


		}


}

//10_1164///
void paraRTC_Enable(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 9;

		} else {
			/* Write */


	     //   wRtcWriteEnable    = (WORD)*pwData;


		}


}


//10_1165///
void paraRTC_1sec(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 9;

		} else {
			/* Write */

	      //  w1sec    = (WORD)*pwData;

		}


}

//10_1166///
void paraRTC_10sec(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 5;

		} else {
			/* Write */


	      //  w10sec    = (WORD)*pwData;


		}


}

//10_1167///
void paraRTC_1min(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 5;

		} else {
			/* Write */


	      //  w1min    = (WORD)*pwData;


		}


}

//10_1168///
void paraRTC_10min(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 5;

		} else {
			/* Write */


	       // w10min    = (WORD)*pwData;


		}


}

//10_1169///
void paraRTC_1hour(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 1;

		} else {
			/* Write */

	      //  w1hour    = (WORD)*pwData;


		}


}

//10_1170///
void paraRTC_10hour(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 1;

		} else {
			/* Write */


	       // w10hour    = (WORD)*pwData;

		}


}

//10_1171///
void paraRTC_1day(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 7;

		} else {
			/* Write */


	       // w1day    = (WORD)*pwData;


		}


}

//10_1172///
void paraRTC_10day(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 4;

		} else {
			/* Write */


	       // w10day    = (WORD)*pwData;


		}


}

//10_1173///
void paraRTC_1month(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 4;

		} else {
			/* Write */

	       // w1month    = (WORD)*pwData;


		}


}

//10_1174///
void paraRTC_10month(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 5;

		} else {
			/* Write */


	       // w10month    = (WORD)*pwData;


		}


}

//10_1175///
void paraRTC_1year(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 4;

		} else {
			/* Write */

	       // w1year    = (WORD)*pwData;


		}


}

//10_1176///
void paraRTC_10year(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 1;

		} else {
			/* Write */


	      //  w10year    = (WORD)*pwData;


		}


}

//10_1177///
void paraRTC_week(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 4;

		} else {
			/* Write */

	        //w1week    = (WORD)*pwData;

		}


}

//10_1178///
void paraTimeSyncSet(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */
			*pwData = 3;

		} else {
			/* Write */

	     //   wTimeSyncSet    = (WORD)*pwData; //0�̸� 24�ð����


		}


}

//10_1179///
void paraAngleTest(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */


			 *pwData = 0; //������


		} else {
			/* Write */

	     //       = (WORD)*pwData;

		}


}

//10_1180///
void paraPLLSet(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */

			 *pwData = 1;


		} else {
			/* Write */

	       //     = (WORD)*pwData;

		}


}

//10_1181///
void paraDbCompSet(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */

			 *pwData = 0; //������

		} else {
			/* Write */

	       //     = (WORD)*pwData;

		}


}

//10_1182///
void paraAutoResetNum(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */

			 *pwData = 5; // �����


		} else {
			/* Write */


	      //  wAutoResetNumber  = (WORD)*pwData;
		}


}

//10_1183///
void paraTimeSyncEnable(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */

			 *pwData = 6; // �����


		} else {
			/* Write */

	      // wTimeSyncEnable   = (WORD)*pwData;

		}


}

//10_1184///
void paraTimeSync_1sec(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */

			 *pwData = 5;


		} else {
			/* Write */

	      // wTimeSync_1sec   = (WORD)*pwData;

		}


}

//10_1185///
void paraTimeSync_10sec(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */

			 *pwData = 3;


		} else {
			/* Write */

	     //  wTimeSync_10sec   = (WORD)*pwData;

		}


}

//10_1186///
void paraTimeSync_1min(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */

			 *pwData = 1;


		} else {
			/* Write */

	     //  wTimeSync_1min   = (WORD)*pwData;

		}


}

//10_1187///
void paraTimeSync_10min(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */

			 *pwData = 5;


		} else {
			/* Write */

	       //wTimeSync_10min   = (WORD)*pwData;

		}


}

//10_1188///
void paraTimeSync_1hour(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */

			 *pwData = 5;


		} else {
			/* Write */

	       //wTimeSync_1hour   = (WORD)*pwData;

		}


}

//10_1189///
void paraTimeSync_10hour(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */

			 *pwData = 66;


		} else {
			/* Write */

	   //    wTimeSync_10hour   = (WORD)*pwData;

		}


}

//10_1190///
void paraTimeSync_1day(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */

			 *pwData = 55;


		} else {
			/* Write */

	    //   wTimeSync_1day   = (WORD)*pwData;

		}


}

//10_1191///
void paraTimeSync_10day(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */

			 *pwData = 3;


		} else {
			/* Write */

	     //  wTimeSync_10day   = (WORD)*pwData;

		}


}

//10_1192///
void paraTimeSync_1month(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */

			 *pwData = 9;


		} else {
			/* Write */

	      // wTimeSync_1month   = (WORD)*pwData;

		}


}

//10_1193///
void paraTimeSync_10month(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */

			 *pwData = 6;


		} else {
			/* Write */

	      // wTimeSync_10month   = (WORD)*pwData;

		}


}

//10_1194///
void paraFacReserved1(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */

			 *pwData = 7;


		} else {
			/* Write */

	      // wFacReserved1   = (WORD)*pwData;

		}


}

//10_1195///
void paraFacReserved2(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */

			 *pwData = 6;


		} else {
			/* Write */

	      // wFacReserved2   = (WORD)*pwData;

		}


}

//10_1196///
void paraFacReserved3(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */

			 *pwData = 67;


		} else {
			/* Write */

	   //    wFacReserved3   = (WORD)*pwData;

		}


}

//10_1197///
void paraFacReserved4(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */

			 *pwData = 5;


		} else {
			/* Write */

	       //wFacReserved4   = (WORD)*pwData;

		}


}

//10_1198///
void paraFacReserved5(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */

			 *pwData = 2;


		} else {
			/* Write */

	     //  wFacReserved5   = (WORD)*pwData;

		}


}

//10_1199///
void paraFacReserved6(WORD wAttr, WORD *pwData){


		if (MB_READ == wAttr) {
			/* Read */

			 *pwData = 2;  //wFacReserved6


		} else {
			/* Write */

	     //  wCompIGBTTemperMax   = (WORD)*pwData;

		}


}

//****************************************************************//
//                 For 0x03, 0x05, 0x06, 0x10
//****************************************************************//



void pararPMSCommStatus (WORD wAttr, WORD *pwData){
	if(MB_READ == wAttr) {


		if (MB_READ == wAttr) {
		/* Read */
		*pwData = 4;

	} else {
		/* Write */


    		//	wPMSCommStatus = (WORD)*pwData;


	}


	}
}





void paraBmsCmdFault(WORD wAttr, WORD *pwData)
{
	if (MB_READ == wAttr)
	{
		/* Read */
	  	*pwData = 3;
	}
	else
	{
		/* Write */
    	 // wBmsCmdFault_Flag = (WORD)*pwData;
	}
}

#endif


///////////////////////////////////////////////


