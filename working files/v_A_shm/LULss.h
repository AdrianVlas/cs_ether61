#pragma once
#include "lubase.h"
#include "prtTmr.hpp"

#include "LssAuxDsc.h"
typedef struct LssCfgSuit_tag{
char chSel;
long lTCs;
}LssCfgSuit;
const short shCLULss_x_y_AmtOut = 1;
const short shCLULss_x_y_AmtIn  = 0;

class CLULss :
public CLUBase {
public:
    CLULss(void);
    ~CLULss(void);
    CLULss(char chM,char chI);
    void CalcLssSchematic(void);
    void CalcLssSchematicOpt(void);
    void CalcLssSchematicOpt3(void);
    void CalcLssSchematicOptManual(void);
    void CalcLssSchematicOptManual_v1(void);
    char* arrPchIn[TOTAL_LSS_IN];
    char  arrOut  [TOTAL_LSS_OUTPUT];//Trash field
    LssCfgSuit m_LssCfgSuit;
    TmrNode m_Node1_1Ms;
    TmrNode m_Node1_2Ms;
    TmrNode m_Node1_3Ms;
    TmrNode m_Node4_2Ms;
    TmrNode m_NodeCs;
    char m_chLinkedTimers; //Field bit
    char m_chStateT1_1Ms;//T1 on 1 ms
    char m_chStateT1_2Ms;//T2 on 1 ms
    char m_chStateT1_3Ms;//T3 on 1 ms
    char m_chStateT4_2Ms;//Timer 4 on 2 ms
    char m_chStateTCs;
    char m_chQTrg29;
    char m_chQTrg11;
    char m_chErrorQTrg29;
    char m_chInC29;
    char m_chInC11;
    char m_chErrorQTrg11;
    
    //.char LSSIN_Imp_1Ms, LSSIN_Imp_2Ms, LSS_D_Imp_2Ms, RESET_Imp_1Ms;
    
    char  LSSIN_I_1ms_Prev;                 // For Timer Emulation 1MS      LSSIN_I_1ms,
    char  LSSIN_I_2ms_Prev, LSSIN_I_2ms_Val;// For Timer Emulation 2MS      LSSIN_I_2ms,
    char  LSS_D_2ms_Prev, LSS_D_2ms_Val;      // For Timer Emulation 2MS    LSS_D_2ms,  
    char  RESET_I_1ms_Prev;                 // For Timer Emulation !MS      RESET_I_1ms,
    
    
    
    short m_shAmountProcessedRec;
    short m_shStartRecord;
    unsigned long last_state_wrp;
    unsigned long last_state_1Ms_lTmrVal;
    unsigned long last_state_1Ms_2TmrVal;
    unsigned long last_state_3Ms_lTmrVal;
    unsigned long last_state_Cs_lTmrVal; 
    
     LedShcemasDscRecord** m_pArLssShcemasDscRecords;//PConst
    long T1_1Ms(long lActivKey);
    long T1_2Ms(long lActivKey);
    long T1_3Ms(long lActivKey);
    long T4_2Ms(long lActivKey);
    long TCs(long lActivKey);
//	void inline SetStateTrg(long lIdTrg, void* pvState);
//	void inline ClrStateTrg(long lIdTrg, void* pvState);
    inline void SetStateVarchQTrg(long lIdTrg, char&chQTrg);
    inline void SetStateVarchInC (long lIdTrg, char&chInC);
    inline void SetStateVarchErrorQTrg(long lIdTrg, char&chErrorQTrg);
    inline long GetStateVarchQTrg(long lIdTrg);
    inline long GetStateVarchErrorQTrg(long lIdTrg);
    inline long GetStateVarchInC (long lIdTrg);
    inline void SetTrg(long lIdTrg);
    inline void ClrTrg(long lIdTrg);
     void ClearTrgTmrMemberVar(void);
    void UpdateCLss(void);
    long LinkLssTimers(void);
    friend void LssOp	(void *pObj);
};

extern void LssOp    (void *pObj);

extern const LedShcemasDscRecord* const arPLssShcemasSimpleModeDscRecords[];
extern const short shPLssShcemasSimpleModeDscRecords;
extern const LedShcemasDscRecord* const arPLssShcemasTriggerModeDscRecords[];
extern const short shPLssShcemasTriggerModeDscRecords;

#ifdef DEBUG_MODE


typedef struct CsDBGRecord_tag{
long luIterLU;

char ch_LSS_LSSIN1;//
char ch_LSS_MUTE_I;//

char ch_T_IMP_27_bt0;
char ch_T_IMP_27_bt7;
char ch_T_IMP_30_bt0;
char ch_T_IMP_30_bt7;


char ch_T_T_0_29_bt0;
char ch_T_T_0_29_bt1;
char ch_T_0_T_add1ms_29_bt7;

char ch_Or_18__3_1_bt0;
char ch_DT_15__4_2_clr;
char ch_DT_15__4_2_in ;
char ch_DT_15__4_2_Q  ;

char ch_Or_11__3_1_bt0;
char ch_Or_11__3_1_bt1;
char ch_Or_11__3_1_bt2;
char ch_Or_11__3_1_bt7;
char ch_Or_12__3_1_bt0;
char ch_Or_12__3_1_bt1;
char ch_Or_12__3_1_bt2;
char ch_Or_12__3_1_bt7;
//char ch_DT_13__4_2
//char ch_DT_13__4_2
//char ch_DT_13__4_2
char ch_DT_13__4_2_Q  ;

stt_LULss_Or_25_bt7;
stt_LULss_Or_26_bt7;





}CSSigDbgRecord;
#define AMOIUNT_DBG_REC 100//2700
//#define DEBUG_MODE 1

extern CSSigDbgRecord  ArrCSSigDbgRec[AMOIUNT_DBG_REC];
extern unsigned int uiIdxArrCSSigDbgRec ;
extern void* pvCCSSigDbg;


#endif

