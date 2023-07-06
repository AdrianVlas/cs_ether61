#include "I_Shm.h"
#include "IStng.h"
#include "prtTmr.h"
#include "Tim9.c"
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//``````````````````````````````````````````````````````````````````````````````````
//==================================================================================
//--- 			 Interface function for CPP Schematic     -----------
//==================================================================================
//..................................................................................
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//~~~  
//~~~  
//~~~  
//~~~  
//~~~  
//~~~  
//~~~  
//~~~  
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
/*
#include "header.h"
#include "../inc/libraries.h"
extern __CONFIG current_config_prt; 
 __CONFIG ccfg;
*/


char chGlb_ActivatorWREeprom = 0;

char chGlbAmountRegisteredElem = 0;
long lGlbRegisteredElemField = 0;
const char chAmtSchematicElement = 15;//
fnPvrL arrChangeCfgFnptr[16] = {
(fnPvrL)0,//00
(fnPvrL)0,//01
(fnPvrL)0,//02
(fnPvrL)0,//03
(fnPvrL)0,//04
(fnPvrL)0,//05
(fnPvrL)0,//06
(fnPvrL)0,//07
(fnPvrL)0,//08
(fnPvrL)0,//09
(fnPvrL)0,//10
(fnPvrL)0,//11
(fnPvrL)0,//12
(fnPvrL)0,//13
(fnPvrL)0,//14
(fnPvrL)0//15
//16
};

long ChangeCfg(void* pv){
    register long i, lRes, lCtrElem, lVl;

  lVl = InitSchematic();
  if( lVl != 0)
    return lVl;
    lRes = i = 0;
    lCtrElem = chGlbAmountRegisteredElem;
    if (lCtrElem)
    {
        do
        {
            while ((lGlbRegisteredElemField & (1 << i) == 0) && (i < chGlbAmountRegisteredElem))
                i++;
            lRes = (arrChangeCfgFnptr[i])(pv);
        } while (--lCtrElem > 0 && lRes == 0);
    }
  TIM9Stop();
  TIM9_Init();
  
  TIM9Start();
  DoCalcWrp();
  TIM9Stop();
  i = TIM_GetCounter(TIM9);
  //lCtrElem = i*417/10000;
  lCtrElem = i*834/10000;
  //lCtrElem = i*660/10000;
  TIM_SetCounter(TIM9,0);
    TIM9Start();
  DoCalcStatInfoWrp();
  TIM9Stop();
  //Out Param in Terminal
  
    return lVl;
}

// long RegisterHundlerLUCfg(void* pv){
 const long TIM9_BASE_CPP = TIM9_BASE;

// return 0;

// }
/*
long ChangeCfgLULed(void* pv){
register void* pvO;
register long i;
volatile 
struct{
void *pVOrigin;
char *pCh;
short shRelativeIndexLU;
}sLv;
sLv.pVOrigin = pv;
pvO = (void*)((LUCfgInfo*)pv)->pvCfgSuit;
//Get Index Current LU
//sLv.shRelativeIndexLU = ((LUCfgInfo*)pv)->shRelativeIndexLU;
//((__LN_TIMER*)spca_of_p_prt[ID_FB_TIMER - _ID_FB_FIRST_VAR] + (number_logical_node - 1))->settings.param[index]
//p_param = &(((__LN_TIMER*)spca_of_p_prt[ID_FB_TIMER - _ID_FB_FIRST_VAR] + (number_logical_node - 1))->settings.param[index]);
//
//            unsigned int id_input   = (param >> SFIFT_PARAM_ID ) & MASKA_PARAM_ID ;//Typ Funktsionalnoho bloku
//            unsigned int n_input    = (param >> SFIFT_PARAM_N  ) & MASKA_PARAM_N  ;//Poryadkovyy nomer
//            unsigned int out_input  = (param >> SFIFT_PARAM_OUT) & MASKA_PARAM_OUT;//Nomer vykhodu //
sLv.pCh = (char*)( spca_of_p_prt[ID_FB_LED - _ID_FB_FIRST_VAR]);
sLv.pCh += sLv.shRelativeIndexLU * sizeof(__LN_OUTPUT_LED);


return 0;
}
*/
extern DICfgSuit sDiCfg;
long lIdxWr = 0;
unsigned char arChInRg[20000] @ "ZeroInivars_RAM1";
void CheckingDIRegs(void){
while(1){	
	//Read Data from Reg Then Save It
	;
	;
	RdHrdIn((void*)&DiHrdStateUI32Bit);
    UpdateStateDI(); 
	arChInRg[lIdxWr++] = sDiCfg.DiState.ul_val;
	if(lIdxWr >= 20000){
		lIdxWr = 0;
		asm volatile(
                "bkpt 1"
                );
	}
	
}

}
//#include "../inc/variables_external.h" zaraz poky shcho ne yde

//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//---extern "C"
//..................................................................................
  void save_trg_info_for_Taras_code(void );
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//~~~                             
//~~~                             
//``````````````````````````````````````````````````````````````````````````````````
//~~~   
//~~~   
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
///////////////////////////////////////////////////////////////////////////////////
//=================================================================================
//Body func                                                                
//=================================================================================
void save_trg_info_for_Taras_code(void )
{
//Триґерна інформація
    if (periodical_tasks_TEST_TRG_FUNC != 0)
    {
      //Стоїть у черзі активна задача зроботи резервні копії даних
      if (
          ((state_i2c_task & MASKA_FOR_BIT(STATE_TRG_FUNC_EEPROM_GOOD_BIT)) != 0) &&
          (
           !(
             (diagnostyka != NULL) &&
             (
              (_CHECK_SET_BIT(diagnostyka, ERROR_PRT_MEMORY_BIT) != 0)
              ||
              (
               (_CHECK_SET_BIT(diagnostyka, ERROR_NO_FREE_DYNAMIC_MEMORY_BIT) != 0) &&
               ((TIM2->CR1 & (TIM_CR1_CEN)) == 0) &&
               ((TIM3->CR1 & (TIM_CR1_CEN)) == 0)
              )
             )
            )
          )  
         )   
      {
        //Робимо копію тільки тоді, коли триґерна інформація успішно зчитана і сформована контрольна сума
        if (
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_START_WRITE_TRG_FUNC_EEPROM_BIT) == 0) &&
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_WRITING_TRG_FUNC_EEPROM_BIT    ) == 0) &&
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_START_READ_TRG_FUNC_EEPROM_BIT ) == 0) &&
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_READING_TRG_FUNC_EEPROM_BIT    ) == 0)
           ) 
        {
          //На даний моммент не іде читання-запис триґерної інформації, тому можна здійснити копіювання
//          TestFlag = 0xff;
          for (__id_fb block_tmp = _ID_FB_FIRST_ALL; block_tmp < _ID_FB_LAST_ALL; block_tmp++)
          {
            switch (block_tmp)
            {
            case ID_FB_OUTPUT:
            case ID_FB_LED:
              {
                size_t n_max_block = ((block_tmp == ID_FB_OUTPUT) ? current_config_prt.n_output : current_config_prt.n_led);
                size_t n_max = DIV_TO_HIGHER(TRIGGER_D_TRIGGER_TOTAL, 8);
                for (size_t n_block = 0; n_block < n_max_block; n_block++)
                {
                  uint8_t *array_source = (((__LN_OUTPUT_LED*)spca_of_p_prt[block_tmp - _ID_FB_FIRST_VAR]) + n_block)->d_trigger_state_tmp;
                  uint8_t *array_target = (((__LN_OUTPUT_LED*)spca_of_p_prt[block_tmp - _ID_FB_FIRST_VAR]) + n_block)->d_trigger_state;
                  for (size_t n = 0; n < n_max; n++) array_source[n] = array_target[n];
                }
                break;
              }
            case ID_FB_ALARM:
              {
                size_t n_max_block = current_config_prt.n_alarm;
                size_t n_max = DIV_TO_HIGHER(ALARM_D_TRIGGER_TOTAL, 8);
                for (size_t n_block = 0; n_block < n_max_block; n_block++)
                {
                  uint8_t *array_source = (((__LN_ALARM*)spca_of_p_prt[ID_FB_ALARM - _ID_FB_FIRST_VAR]) + n_block)->d_trigger_state_tmp;
                  uint8_t *array_target = (((__LN_ALARM*)spca_of_p_prt[ID_FB_ALARM - _ID_FB_FIRST_VAR]) + n_block)->d_trigger_state;
                  for (size_t n = 0; n < n_max; n++) array_source[n] = array_target[n];
                }
                break;
              }
            case ID_FB_TRIGGER:
              {
                size_t n_max_block = current_config_prt.n_trigger;
                size_t n_max = DIV_TO_HIGHER(TRIGGER_D_TRIGGER_TOTAL, 8);
                for (size_t n_block = 0; n_block < n_max_block; n_block++)
                {
                  uint8_t *array_source = (((__LN_TRIGGER*)spca_of_p_prt[ID_FB_TRIGGER - _ID_FB_FIRST_VAR]) + n_block)->d_trigger_state_tmp;
                  uint8_t *array_target = (((__LN_TRIGGER*)spca_of_p_prt[ID_FB_TRIGGER - _ID_FB_FIRST_VAR]) + n_block)->d_trigger_state;
                  for (size_t n = 0; n < n_max; n++) array_source[n] = array_target[n];
                }
                break;
              }
            }
          }
          crc_trg_func_ctrl = crc_trg_func;
//          TestFlag = 0x0;

          //Скидаємо активну задачу формування резервної копії 
          periodical_tasks_TEST_TRG_FUNC = false;
          //Виставляємо активну задачу контролю достовірності по резервній копії 
          periodical_tasks_TEST_TRG_FUNC_LOCK = true;
        }
      }
      else
      {
        //Скидаємо активну задачу формування резервної копії 
        periodical_tasks_TEST_TRG_FUNC = false;
      }
    }

  
}
//----------------------------------------------------------------------------------




//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//---
//extern char GetCmdStateVcs(void *p_in_param);
//..................................................................................
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//~~~                             
//~~~                             
//``````````````````````````````````````````````````````````````````````````````````
//~~~   
//~~~   
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
///////////////////////////////////////////////////////////////////////////////////
//=================================================================================
//Body func                                                                
//=================================================================================
//char GetCmdStateVcs(void *p_in_param)
//{
//    register I32 i; 
//	
//}
//----------------------------------------------------------------------------------
