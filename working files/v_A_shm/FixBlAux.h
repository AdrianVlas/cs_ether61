#ifndef LU_FIX_BLOCK_DSC_h
#define LU_FIX_BLOCK_DSC_h

#include "LuGenOp.h"

enum FIX_BLOCK_INPUT_NAMES{
IN_HOLDER_00_LED_ALARM,        
IN_HOLDER_01_LED_MUTE ,
IN_HOLDER_02_LED_BLOCK,
IN_HOLDER_03_TEST_K   ,
IN_HOLDER_04_RESET_I  ,
TOTAL_FIX_BLOCK_IN

};
 
enum FIX_BLOCK_OUTPUT_NAMES{
ERROR_C_CMD,
ERROR_NC_CMD,
RUN_CMD,
CHANGE_SETTING_CMD,
TRIGER_FAULT_CMD,
SHCEMATIC_FAULT_CMD,

TOTAL_FIX_BLOCK_CMD
};





#endif

