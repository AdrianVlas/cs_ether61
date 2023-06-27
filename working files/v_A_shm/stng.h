#ifndef e_stng_h
#define e_stng_h

//Demo for Using Descriptors

#define NUM_STNG_IN   10
#define NUM_STNG_OUT  10
#define NUM_STNG_LED  10

enum ID_LU_STNG { 
STNG_BASE_POINT = 1,
STNG_POINT    ,      
STNG_EXTENDED ,   
STNG_LU_AND   ,  
STNG_LU_OR    ,  
STNG_LU_XOR   ,
STNG_LU_TIMERS,
STNG_LU_TRIGGERS,
STNG_LU_ALARMS,
STNG_LU_MEANDERS,
STNG_LU_INPUT ,
STNG_LU_OUTPUT ,
STNG_LU_LED     ,
STNG_LU_NOT    ,
STNG_LU_MFT,
STNG_LU_UPPER_BOUND
};
enum ORD_NUMSTNG {
ORD_INPUT_01 = 1,
ORD_INPUT_02,
ORD_INPUT_03,
ORD_INPUT_04,
ORD_INPUT_05,
ORD_INPUT_06,
ORD_INPUT_07,
ORD_INPUT_08,
ORD_INPUT_09,
ORD_INPUT_10,
ORD_OUTPUT_01,
ORD_OUTPUT_02,
ORD_OUTPUT_03,
ORD_OUTPUT_04,
ORD_OUTPUT_05,
ORD_OUTPUT_06,
ORD_OUTPUT_07,
ORD_OUTPUT_08,
ORD_OUTPUT_09,
ORD_OUTPUT_10,
ORD_LED_01,
ORD_LED_02,
ORD_LED_03,
ORD_LED_04,
ORD_LED_05,
ORD_LED_06,
ORD_LED_07,
ORD_LED_08,
ORD_LED_09,
ORD_LED_10,
ORD_AND_01,
ORD_AND_02,
ORD_AND_03,
ORD_AND_04,
ORD_AND_05,
ORD_AND_06,
ORD_AND_07,
ORD_AND_08,
ORD_AND_09,
ORD_AND_10,
ORD_OR_01,
ORD_OR_02,
ORD_OR_03,
ORD_OR_04,
ORD_OR_05,
ORD_OR_06,
ORD_OR_07,
ORD_OR_08,
ORD_OR_09,
ORD_OR_10,
ORD_XOR_01,
ORD_XOR_02,
ORD_XOR_03,
ORD_XOR_04,
ORD_XOR_05,
ORD_XOR_06,
ORD_XOR_07,
ORD_XOR_08,
ORD_XOR_09,
ORD_XOR_10,
ORD_NOT_01,
ORD_NOT_02,
ORD_NOT_03,
ORD_NOT_04,
ORD_NOT_05,
ORD_NOT_06,
ORD_NOT_07,
ORD_NOT_08,
ORD_NOT_09,
ORD_NOT_10,
ORD_TIMERS_01,
ORD_TIMERS_02,
ORD_TIMERS_03,
ORD_TIMERS_04,
ORD_TIMERS_05,
ORD_TIMERS_06,
ORD_TIMERS_07,
ORD_TIMERS_08,
ORD_TIMERS_09,
ORD_TIMERS_10
};


LUOutDsc ar_n_output_Dsc[NUM_STNG_OUT] = {
{STNG_LU_INPUT,ORD_INPUT_01,1},{STNG_LU_INPUT,ORD_INPUT_02,1},{STNG_LU_INPUT,ORD_INPUT_03,1},{STNG_LU_INPUT,ORD_INPUT_04,1},{STNG_LU_INPUT,ORD_INPUT_05,1},
{STNG_LU_INPUT,ORD_INPUT_01,1},{STNG_LU_INPUT,ORD_INPUT_01,1},{STNG_LU_INPUT,ORD_INPUT_01,1},{STNG_LU_INPUT,ORD_INPUT_01,1},{STNG_LU_INPUT,ORD_INPUT_01,1}
};

LUOutDsc ar_n_led_Dsc[NUM_STNG_LED] = { 
{STNG_LU_INPUT,ORD_INPUT_01,1},{STNG_LU_INPUT,ORD_INPUT_02,1},{STNG_LU_INPUT,ORD_INPUT_03,1},{STNG_LU_INPUT,ORD_INPUT_04,1},{STNG_LU_INPUT,ORD_INPUT_05,1},
{STNG_LU_INPUT,ORD_INPUT_01,1},{STNG_LU_INPUT,ORD_INPUT_01,1},{STNG_LU_INPUT,ORD_INPUT_01,1},{STNG_LU_INPUT,ORD_INPUT_01,1},{STNG_LU_INPUT,ORD_INPUT_01,1}

};


#define TOTAL_NUM_AND 2
LUOutDsc ar_n_and_Dsc[8*TOTAL_NUM_AND] = {
{STNG_LU_INPUT,ORD_INPUT_01,1},{STNG_LU_INPUT,ORD_INPUT_02,1},{STNG_LU_INPUT,ORD_INPUT_03,1},{STNG_LU_INPUT,ORD_INPUT_04,1},
{STNG_LU_INPUT,ORD_INPUT_01,1},{STNG_LU_INPUT,ORD_INPUT_01,1},{STNG_LU_INPUT,ORD_INPUT_01,1},{STNG_LU_INPUT,ORD_INPUT_01,1},

{STNG_LU_INPUT,ORD_INPUT_01,1},{STNG_LU_INPUT,ORD_INPUT_02,1},{STNG_LU_INPUT,ORD_INPUT_03,1},{STNG_LU_INPUT,ORD_INPUT_04,1},
{STNG_LU_INPUT,ORD_INPUT_01,1},{STNG_LU_INPUT,ORD_INPUT_01,1},{STNG_LU_INPUT,ORD_INPUT_01,1},{STNG_LU_INPUT,ORD_INPUT_01,1}


};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//,-LUCrossRefDsc ar_n_output_CRDsc[NUM_STNG_OUT] = {
//,-{STNG_LU_OUTPUT,ORD_OUTPUT_01,STNG_LU_INPUT,ORD_INPUT_01,1},{STNG_LU_OUTPUT,ORD_OUTPUT_02,STNG_LU_INPUT,ORD_INPUT_02,1},
//,-{STNG_LU_OUTPUT,ORD_OUTPUT_03,STNG_LU_INPUT,ORD_INPUT_03,1},{STNG_LU_OUTPUT,ORD_OUTPUT_04,STNG_LU_INPUT,ORD_INPUT_04,1},
//,-{STNG_LU_OUTPUT,ORD_OUTPUT_05,STNG_LU_INPUT,ORD_INPUT_05,1},{STNG_LU_OUTPUT,ORD_OUTPUT_06,STNG_LU_INPUT,ORD_INPUT_06,1},
//,-{STNG_LU_OUTPUT,ORD_OUTPUT_07,STNG_LU_INPUT,ORD_INPUT_07,1},{STNG_LU_OUTPUT,ORD_OUTPUT_08,STNG_LU_INPUT,ORD_INPUT_08,1},
//,-{STNG_LU_OUTPUT,ORD_OUTPUT_09,STNG_LU_INPUT,ORD_INPUT_09,1},{STNG_LU_OUTPUT,ORD_OUTPUT_10,STNG_LU_INPUT,ORD_INPUT_10,1}
//,-};
enum MAX_AMOUNT_LU_STNG { 
MAX_AMOUNT_BASE_POINT = 1,
MAX_AMOUNT_POINT      = 1,      
MAX_AMOUNT_EXTENDED   = 1,   
MAX_AMOUNT_LU_AND     = 64,  
MAX_AMOUNT_LU_OR      = 64,  
MAX_AMOUNT_LU_XOR     = 64,
MAX_AMOUNT_LU_NOT     = 64, 
MAX_AMOUNT_LU_TIMERS  = 64,
MAX_AMOUNT_LU_TRIGGERS = 65,
MAX_AMOUNT_LU_ALARMS   = 75,
MAX_AMOUNT_LU_MEANDERS = 65,
MAX_AMOUNT_LU_INPUT    = 19,
MAX_AMOUNT_LU_OUTPUT   = 7,
MAX_AMOUNT_LU_LED      = 22,
MAX_AMOUNT_LU_FKey     = 5,
MAX_AMOUNT_LU_LSS      = 72,
MAX_AMOUNT_LU_BGS      = 10,
MAX_AMOUNT_LU_TRIG     = 64,
MAX_AMOUNT_LU_ALT     = 60,
MAX_AMOUNT_LU_TU     = 64,
MAX_AMOUNT_LU_TS     = 64,
MAX_AMOUNT_LU_GOOSE     = 64,
MAX_AMOUNT_LU_MMS       = 64,
MAX_AMOUNT_LU_LAN       = 64,
MAX_AMOUNT_LU_LOG    = 1


};
LUCrossRefDsc ar_n_output_CRDsc[MAX_AMOUNT_LU_OUTPUT+MAX_AMOUNT_LU_LED+8*1] @ "NonZeroIniVars_RAM1" = {
//---Output 	
{STNG_LU_OUTPUT,ORD_OUTPUT_01,STNG_LU_INPUT,ORD_INPUT_01,1},{STNG_LU_OUTPUT,ORD_OUTPUT_02,STNG_LU_INPUT,ORD_INPUT_02,1},//
{STNG_LU_OUTPUT,ORD_OUTPUT_03,STNG_LU_INPUT,ORD_INPUT_03,1},{STNG_LU_OUTPUT,ORD_OUTPUT_04,STNG_LU_INPUT,ORD_INPUT_04,1},//
{STNG_LU_OUTPUT,ORD_OUTPUT_05,STNG_LU_INPUT,ORD_INPUT_05,1},{STNG_LU_OUTPUT,ORD_OUTPUT_06,STNG_LU_INPUT,ORD_INPUT_06,1},//
{STNG_LU_OUTPUT,ORD_OUTPUT_07,STNG_LU_INPUT,ORD_INPUT_07,1},{STNG_LU_OUTPUT,ORD_OUTPUT_08,STNG_LU_INPUT,ORD_INPUT_08,1},//
{STNG_LU_OUTPUT,ORD_OUTPUT_09,STNG_LU_INPUT,ORD_INPUT_09,1},{STNG_LU_OUTPUT,ORD_OUTPUT_10,STNG_LU_INPUT,ORD_INPUT_10,1},//
//---Led
{STNG_LU_LED,ORD_LED_01,STNG_LU_INPUT,ORD_INPUT_01,1},{STNG_LU_LED,ORD_LED_02,STNG_LU_INPUT,ORD_INPUT_02,1},//
{STNG_LU_LED,ORD_LED_03,STNG_LU_INPUT,ORD_INPUT_03,1},{STNG_LU_LED,ORD_LED_04,STNG_LU_INPUT,ORD_INPUT_04,1},//
{STNG_LU_LED,ORD_LED_05,STNG_LU_INPUT,ORD_INPUT_05,1},{STNG_LU_LED,ORD_LED_06,STNG_LU_INPUT,ORD_INPUT_06,1},//
{STNG_LU_LED,ORD_LED_07,STNG_LU_INPUT,ORD_INPUT_07,1},{STNG_LU_LED,ORD_LED_08,STNG_LU_INPUT,ORD_INPUT_08,1},//
{STNG_LU_LED,ORD_LED_09,STNG_LU_INPUT,ORD_INPUT_09,1},{STNG_LU_LED,ORD_LED_10,STNG_LU_INPUT,ORD_INPUT_10,1}, //
//---AND 1
{STNG_LU_AND,ORD_AND_01,STNG_LU_INPUT,ORD_INPUT_01,1},{STNG_LU_AND,ORD_AND_01,STNG_LU_INPUT,ORD_INPUT_02,1},//8-Dsc
{STNG_LU_AND,ORD_AND_01,STNG_LU_INPUT,ORD_INPUT_03,1},{STNG_LU_AND,ORD_AND_01,STNG_LU_INPUT,ORD_INPUT_04,1},//8-Dsc
{STNG_LU_AND,ORD_AND_01,STNG_LU_INPUT,ORD_INPUT_05,1},{STNG_LU_AND,ORD_AND_01,STNG_LU_INPUT,ORD_INPUT_06,1},//8-Dsc
{STNG_LU_AND,ORD_AND_01,STNG_LU_INPUT,ORD_INPUT_07,1},{STNG_LU_AND,ORD_AND_01,STNG_LU_INPUT,ORD_INPUT_08,1}//8-Dsc


//---OR1

//---XOR1

//---NOT1

//---
};
SBitFldCRefInfo arrSBitFldCRefInfo[MAX_AMOUNT_LU_OUTPUT+MAX_AMOUNT_LU_LED+
(MAX_AMOUNT_LU_AND*8) +(MAX_AMOUNT_LU_OR*8 ) +
(MAX_AMOUNT_LU_XOR*2) +(MAX_AMOUNT_LU_NOT*1) 
];

#include "outInfo.h"
#include "ledIstng.h"
#include "andstng.h"
#include "orstng.h"
#include "xorstng.h"
#include "notstng.h"

//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//``````````````````````````````````````````````````````````````````````````````````
//  For Init2  Context  Area 
//..................................................................................
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
typedef struct tag_Init2LcVarArea {
	char* pCh;
    char** arrPchIn;
    LUAreaListElem* arrLUAreaListElem;
    CLUBase *pCLUBase, *pCLURef;
    short shCounterInitCLUObj;//shIdxLUOutDsc
    short shIdxGlobalObjectMapPointers;
//Should be avoided used	
    void *pV;
    long lId, lsizeobj;
    short shAmtLU, shIdxLUOut; //
    short shAmtLUDsc, shIdx;
    short shIdxLUOutDsc, shIdxLU; //shCountCmd;
    char chVal, chErrCount, chBrk1, chBrk2;
}Init2LcVarArea;//
//~~~       ~~
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
typedef struct tag_LUCRefExchParam{

short shLU_ObjScanIndex;
char  chLU_OrdNumIn;
CLUBase* pCLUBase;


}LUCRefExchParam;

#endif
