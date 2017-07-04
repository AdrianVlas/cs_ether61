#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 4670
#define REGISTER_FOR_OBJ 6

int getBASDBigModbusRegister(int);//�������� ���������� ��������
int getBASDBigModbusBit(int);//�������� ���������� ����
int setBASDBigModbusRegister(int, int);//��������
int setBASDBigModbusBit(int, int);// ����
int privateBASDBigGetReg2(int adrReg);

void setBASDBigCountObject(void);
void preBASDBigReadAction(void);//action �� ������
void postBASDBigReadAction(void);//action ����� ������
void preBASDBigWriteAction(void);//action �� ������
void postBASDBigWriteAction(void);//action ����� ������
void loadBASDBigActualData(void);

COMPONENT_OBJ *basdbigcomponent;
/**************************************/
//���������� ���������� ��������� ���������������� block\alarm
/**************************************/
void constructorBASDBigComponent(COMPONENT_OBJ *basdbigcomp)
{
  basdbigcomponent = basdbigcomp;

  basdbigcomponent->countObject = 1;//�-�� �������

  basdbigcomponent->getModbusRegister = getBASDBigModbusRegister;//�������� ���������� ��������
  basdbigcomponent->getModbusBit      = getBASDBigModbusBit;//�������� ���������� ����
  basdbigcomponent->setModbusRegister = setBASDBigModbusRegister;// ��������
  basdbigcomponent->setModbusBit      = setBASDBigModbusBit;//����

  basdbigcomponent->preReadAction   = preBASDBigReadAction;//action �� ������
  basdbigcomponent->postReadAction  = postBASDBigReadAction;//action ����� ������
  basdbigcomponent->preWriteAction  = preBASDBigWriteAction;//action �� ������
  basdbigcomponent->postWriteAction = postBASDBigWriteAction;//action ����� ������

  basdbigcomponent->isActiveActualData = 0;

}//prepareDVinConfig

void loadBASDBigActualData(void) {
  //ActualData
  __SETTINGS_FIX *arr = &settings_fix_prt;

  int value;
  value = arr->param[FIX_BLOCK_BLOCK] & 0xffff;//
  tempReadArray[0] = value;
  value = (arr->param[FIX_BLOCK_BLOCK] >> 16) & 0x7fff;//
  tempReadArray[1] = value;

  value = arr->param[FIX_BLOCK_ALARM] & 0xffff;//
  tempReadArray[2] = value;
  value = (arr->param[FIX_BLOCK_ALARM] >> 16) & 0x7fff;//
  tempReadArray[3] = value;

  value = arr->param[FIX_BLOCK_MUTE] & 0xffff;//
  tempReadArray[4] = value;
  value = (arr->param[FIX_BLOCK_MUTE] >> 16) & 0x7fff;//
  tempReadArray[5] = value;
}//loadActualData() 

int getBASDBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateBASDBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  if(basdbigcomponent->isActiveActualData) loadBASDBigActualData(); //ActualData
  basdbigcomponent->isActiveActualData = 0;

  superSetOperativMarker(basdbigcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDVModbusRegister(int adrReg)
int getBASDBigModbusBit(int adrBit)
{
  //�������� ���������� ��������
  superSetOperativMarker(basdbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getBASDBigModbusBit(int adrReg)
int setBASDBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateBASDBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  superSetOperativMarker(basdbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  switch((adrReg-BEGIN_ADR_REGISTER)%REGISTER_FOR_OBJ) {
   case 0:
   break; 
   case 1:
   break; 
   case 2:
   break; 
   case 3:
   break; 
   case 4:
   break; 
   case 5:
   break; 
   default: return MARKER_OUTPERIMETR;
  }//switch
  return 0;
}//setDVModbusRegister(int adrReg)
int setBASDBigModbusBit(int adrBit, int x)
{
  UNUSED(x);
  //�������� ���������� ��������
  superSetOperativMarker(basdbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getBASDBigModbusBit(int adrReg)

void setBASDBigCountObject(void) {
//�������� �-�� �������
}//

void preBASDBigReadAction(void) {
//action �� ������
  basdbigcomponent->operativMarker[0] = -1;
  basdbigcomponent->operativMarker[1] = -1;//����������� ������
  basdbigcomponent->isActiveActualData = 1;
}//
void postBASDBigReadAction(void) {
//action ����� ������
  if(basdbigcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preBASDBigWriteAction(void) {
//action �� ������
  basdbigcomponent->operativMarker[0] = -1;
  basdbigcomponent->operativMarker[1] = -1;//����������� ������
  basdbigcomponent->isActiveActualData = 1;
}//
void postBASDBigWriteAction(void) {
//action ����� ������
  if(basdbigcomponent->operativMarker[0]<0) return;//�� ���� ������
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
  int countRegister = basdbigcomponent->operativMarker[1]-basdbigcomponent->operativMarker[0]+1;
  if(basdbigcomponent->operativMarker[1]<0) countRegister = 1;

  __SETTINGS_FIX *arr = &settings_fix, *arr1 = &settings_fix_edit;
  for(int i=0; i<countRegister; i++) {
  int offset = i+basdbigcomponent->operativMarker[0]-BEGIN_ADR_REGISTER;
  switch(offset) {//������ �������� 
   case 0:
        arr1->param[FIX_BLOCK_BLOCK] = arr->param[FIX_BLOCK_BLOCK] &= (uint32_t)~0xffff;
        arr->param[FIX_BLOCK_BLOCK] |= (tempWriteArray[offsetTempWriteArray+i] & 0xffff);
   break;
   case 1:
        arr->param[FIX_BLOCK_BLOCK] &= (uint32_t)~(0x7fff<<16);
        arr->param[FIX_BLOCK_BLOCK] |= ((tempWriteArray[offsetTempWriteArray+i] & 0x7fff)<<16);//
   break; 

   case 2:
        arr->param[FIX_BLOCK_ALARM] &= (uint32_t)~0xffff;
        arr->param[FIX_BLOCK_ALARM] |= (tempWriteArray[offsetTempWriteArray+i] & 0xffff);
   break;
   case 3:
        arr->param[FIX_BLOCK_ALARM] &= (uint32_t)~(0x7fff<<16);
        arr->param[FIX_BLOCK_ALARM] |= ((tempWriteArray[offsetTempWriteArray+i] & 0x7fff)<<16);//
   break; 

   case 4:
        arr->param[FIX_BLOCK_MUTE] &= (uint32_t)~0xffff;
        arr->param[FIX_BLOCK_MUTE] |= (tempWriteArray[offsetTempWriteArray+i] & 0xffff);
   break;
   case 5:
        arr->param[FIX_BLOCK_MUTE] &= (uint32_t)~(0x7fff<<16);
        arr->param[FIX_BLOCK_MUTE] |= ((tempWriteArray[offsetTempWriteArray+i] & 0x7fff)<<16);//
   break; 
 }//switch
  }//for

}//

int privateBASDBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)
