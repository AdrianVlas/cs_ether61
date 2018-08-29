#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 13768
//���� �-�� �������
#define TOTAL_OBJ 128
#define REGISTER_FOR_OBJ 2

int privateIGOOSEBigGetReg1(int adrReg);
int privateIGOOSEBigGetReg2(int adrReg);

int getIGOOSEBigModbusRegister(int);//�������� ���������� ��������
int getIGOOSEBigModbusBit(int);//�������� ���������� ����
int setIGOOSEBigModbusRegister(int, int);//�������� ���������� ��������
int setIGOOSEBigModbusBit(int, int);//�������� ���������� ����

void setIGOOSEBigCountObject(void);//�������� �-�� �������
void preIGOOSEBigReadAction(void);//action �� ������
void postIGOOSEBigReadAction(void);//action ����� ������
void preIGOOSEBigWriteAction(void);//action �� ������
int postIGOOSEBigWriteAction(void);//action ����� ������
void loadIGOOSEBigActualData(void);
void repairEditArrayIGOOSE(int countRegister, __settings_for_INPUT_GOOSE_BLOCK *arr, __settings_for_INPUT_GOOSE_BLOCK *arr1);
void config_and_settingsIGOOSEBig(void);//action ���������

COMPONENT_OBJ *igoosebigcomponent;

/**************************************/
//���������� ���������� input GOOSE
/**************************************/
void constructorIGOOSEBigComponent(COMPONENT_OBJ *igoosebigcomp)
{
  igoosebigcomponent = igoosebigcomp;

  igoosebigcomponent->countObject = 0;//�-�� �������

  igoosebigcomponent->getModbusRegister = getIGOOSEBigModbusRegister;//�������� ���������� ��������
  igoosebigcomponent->getModbusBit      = getIGOOSEBigModbusBit;//�������� ���������� ����
  igoosebigcomponent->setModbusRegister = setIGOOSEBigModbusRegister;//�������� ���������� ��������
  igoosebigcomponent->setModbusBit      = setIGOOSEBigModbusBit;//�������� ���������� ����

  igoosebigcomponent->preReadAction   = preIGOOSEBigReadAction;//action �� ������
  igoosebigcomponent->postReadAction  = postIGOOSEBigReadAction;//action ����� ������
  igoosebigcomponent->preWriteAction  = preIGOOSEBigWriteAction;//action �� ������
  igoosebigcomponent->postWriteAction = postIGOOSEBigWriteAction;//action ����� ������
  igoosebigcomponent->config_and_settings = config_and_settingsIGOOSEBig;//action ���������

  igoosebigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

int getIGOOSEBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
extern int pointInterface;  
  if(privateIGOOSEBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(igoosebigcomponent->isActiveActualData) setIGOOSEBigCountObject(); //�-�� �������
  igoosebigcomponent->isActiveActualData = 0;
  if(privateIGOOSEBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  superSetOperativMarker(igoosebigcomponent, adrReg);

  int offset = adrReg-BEGIN_ADR_REGISTER;
  int idxSubObj = offset/REGISTER_FOR_OBJ;//������ ����������
  __settings_for_INPUT_GOOSE_BLOCK *arr =  ((config_settings_modified & MASKA_FOR_BIT(BIT_USB_LOCKS)) == 0 ) ? &(((__LN_INPUT_GOOSE_BLOCK*)(spca_of_p_prt[ID_FB_INPUT_GOOSE_BLOCK - _ID_FB_FIRST_VAR])) + idxSubObj)->settings : (((__settings_for_INPUT_GOOSE_BLOCK*)(sca_of_p[ID_FB_INPUT_GOOSE_BLOCK - _ID_FB_FIRST_VAR])) + idxSubObj);
  if(pointInterface)//����� ���������� 0-USB 1-RS485
                        arr =  ((config_settings_modified & MASKA_FOR_BIT(BIT_RS485_LOCKS)) == 0 ) ? &(((__LN_INPUT_GOOSE_BLOCK*)(spca_of_p_prt[ID_FB_INPUT_GOOSE_BLOCK - _ID_FB_FIRST_VAR])) + idxSubObj)->settings : (((__settings_for_INPUT_GOOSE_BLOCK*)(sca_of_p[ID_FB_INPUT_GOOSE_BLOCK - _ID_FB_FIRST_VAR])) + idxSubObj);

  switch(offset%REGISTER_FOR_OBJ) {//������ �������� 
   case 0:
        return arr->param[0] & 0xffff;//
   case 1:
        return (arr->param[0] >> 16) & 0x7fff;//
  }//switch
  return 0;
}//getIGOOSEBigModbusRegister(int adrReg)
int getIGOOSEBigModbusBit(int x)
{
  //�������� ���������� ��������
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getIGOOSEBigModbusRegister(int adrReg)
int setIGOOSEBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateIGOOSEBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(igoosebigcomponent->isActiveActualData) setIGOOSEBigCountObject(); //�-�� �������
  igoosebigcomponent->isActiveActualData = 0;
  if(privateIGOOSEBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  superSetOperativMarker(igoosebigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  switch((adrReg-BEGIN_ADR_REGISTER)%REGISTER_FOR_OBJ) {
   case 0:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
   break; 
   case 1:
    //�������� ���������� ������������
    if(superControlParam(dataReg)) return MARKER_ERRORDIAPAZON;
   break; 
  default: return MARKER_OUTPERIMETR;
  }//switch
  return 0;
}//getIGOOSEBigModbusRegister(int adrReg)
int setIGOOSEBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void setIGOOSEBigCountObject(void) {
//�������� �-�� �������
  int cntObj = current_config.n_input_GOOSE_block;   //ʳ������ �������� IGOOSE
  if(cntObj<0) return;
  if(cntObj>TOTAL_OBJ) return;
  igoosebigcomponent->countObject = cntObj;
}//
void preIGOOSEBigReadAction(void) {
//action �� ������
  igoosebigcomponent->operativMarker[0] = -1;
  igoosebigcomponent->operativMarker[1] = -1;//����������� ������
  igoosebigcomponent->isActiveActualData = 1;
}//
void postIGOOSEBigReadAction(void) {
//action ����� ������
  if(igoosebigcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preIGOOSEBigWriteAction(void) {
//action �� ������
  igoosebigcomponent->operativMarker[0] = -1;
  igoosebigcomponent->operativMarker[1] = -1;//����������� ������
  igoosebigcomponent->isActiveActualData = 1;
}//
int postIGOOSEBigWriteAction(void) {
//action ����� ������
extern int pointInterface;//����� ���������� 0-USB 1-RS485
  if(igoosebigcomponent->operativMarker[0]<0) return 0;//�� ���� ������
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
  int countRegister = igoosebigcomponent->operativMarker[1]-igoosebigcomponent->operativMarker[0]+1;
  if(igoosebigcomponent->operativMarker[1]<0) countRegister = 1;

   __settings_for_INPUT_GOOSE_BLOCK *arr  = (__settings_for_INPUT_GOOSE_BLOCK*)(sca_of_p[ID_FB_INPUT_GOOSE_BLOCK - _ID_FB_FIRST_VAR]);
   __settings_for_INPUT_GOOSE_BLOCK *arr1 = (__settings_for_INPUT_GOOSE_BLOCK*)(sca_of_p_edit[ID_FB_INPUT_GOOSE_BLOCK - _ID_FB_FIRST_VAR]);
//�������� edit ������
  for(int i=0; i<countRegister; i++) {
  int offset = i+igoosebigcomponent->operativMarker[0]-BEGIN_ADR_REGISTER;
  int idxSubObj = offset/REGISTER_FOR_OBJ;//������ ����������
  switch(offset%2) {//������ �������� �����
  case 0:

        arr1[idxSubObj].param[0]  &= (uint32_t)~0xffff;
        arr1[idxSubObj].param[0]  |= (tempWriteArray[offsetTempWriteArray+i] & 0xffff);
  break;
  case 1:

        arr1[idxSubObj].param[0]  &= (uint32_t)~(0x7fff<<16);
        arr1[idxSubObj].param[0]  |= ((tempWriteArray[offsetTempWriteArray+i] & 0x7fff)<<16);//
  break;
  }//switch
  }//for

  //�������� ����������
  for(int i=0; i<countRegister; i++) {
  int offset = i+igoosebigcomponent->operativMarker[0]-BEGIN_ADR_REGISTER;
  int idxSubObj = offset/REGISTER_FOR_OBJ;//������ ����������

  switch(offset%2) {//������ �������� �����
  case 0:
  case 1:
        if(superValidParam(arr1[idxSubObj].param[0])) 
                {//�������� ����������
                repairEditArrayIGOOSE(countRegister, arr, arr1);//������������ edit ������
                return 2;//����
        }//if
  break;
 }//switch
  }//for

//�������� ������� - ��������������
  for(int i=0; i<countRegister; i++) {
  int offset = i+igoosebigcomponent->operativMarker[0]-BEGIN_ADR_REGISTER;
  int idxSubObj = offset/REGISTER_FOR_OBJ;//������ ����������
  switch(offset%2) {//������ �������� �����
  case 0:

        arr1[idxSubObj].param[0] = arr[idxSubObj].param[0] &= (uint32_t)~0xffff;
        arr1[idxSubObj].param[0] = arr[idxSubObj].param[0] |= (tempWriteArray[offsetTempWriteArray+i] & 0xffff);
  break;
  case 1:

        arr1[idxSubObj].param[0] = arr[idxSubObj].param[0] &= (uint32_t)~(0x7fff<<16);
        arr1[idxSubObj].param[0] = arr[idxSubObj].param[0] |= ((tempWriteArray[offsetTempWriteArray+i] & 0x7fff)<<16);//
  break;
  }//switch
  }//for

  config_settings_modified |= MASKA_FOR_BIT(BIT_CHANGED_SCHEMATIC);
  if(pointInterface)//����� ���������� 0-USB 1-RS485
     config_settings_modified |= MASKA_FOR_BIT(BIT_RS485_LOCKS);
  else 
     config_settings_modified |= MASKA_FOR_BIT(BIT_USB_LOCKS);
  restart_timeout_idle_new_settings = true;
 return 0;
}//

void repairEditArrayIGOOSE(int countRegister, __settings_for_INPUT_GOOSE_BLOCK *arr, __settings_for_INPUT_GOOSE_BLOCK *arr1) {
  //������������ edit ������
  for(int i=0; i<countRegister; i++) {
  int offset = i+igoosebigcomponent->operativMarker[0]-BEGIN_ADR_REGISTER;
  int idxSubObj = offset/REGISTER_FOR_OBJ;//������ ����������

  switch(offset%2) {//������ �������� �����
  case 0:
  case 1:
        arr1[idxSubObj].param[0] = arr[idxSubObj].param[0];
  break;
 }//switch
  }//for
}//repairEditArray(int countRegister, __settings_for_INPUT_GOOSE_BLOCK *arr, __settings_for_INPUT_GOOSE_BLOCK *arr1) 

int privateIGOOSEBigGetReg1(int adrReg)
{
  //��������� ���������� ��������
  int count_register = igoosebigcomponent->countObject*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg1(int adrReg)
int privateIGOOSEBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = TOTAL_OBJ*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)

void config_and_settingsIGOOSEBig(void)
{
//action ���������
}

