#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 14024
//���� �-�� �������
#define TOTAL_OBJ 128
#define REGISTER_FOR_OBJ 4

int privateIMMSBigGetReg1(int adrReg);
int privateIMMSBigGetReg2(int adrReg);

int getIMMSBigModbusRegister(int);//�������� ���������� ��������
int getIMMSBigModbusBit(int);//�������� ���������� ����
int setIMMSBigModbusRegister(int, int);//�������� ���������� ��������
int setIMMSBigModbusBit(int, int);//�������� ���������� ����

void setIMMSBigCountObject(void);//�������� �-�� �������
void preIMMSBigReadAction(void);//action �� ������
void postIMMSBigReadAction(void);//action ����� ������
void preIMMSBigWriteAction(void);//action �� ������
int postIMMSBigWriteAction(void);//action ����� ������
void repairEditArrayIMMS(int countRegister, __settings_for_INPUT_MMS_BLOCK *arr, __settings_for_INPUT_MMS_BLOCK *arr1);
void config_and_settingsIMMSBig(void);//action ���������

COMPONENT_OBJ *immsbigcomponent;

/**************************************/
//���������� ���������� IMMS
/**************************************/
void constructorIMMSBigComponent(COMPONENT_OBJ *immsbigcomp)
{
  immsbigcomponent = immsbigcomp;

  immsbigcomponent->countObject = 0;//�-�� �������

  immsbigcomponent->getModbusRegister = getIMMSBigModbusRegister;//�������� ���������� ��������
  immsbigcomponent->getModbusBit      = getIMMSBigModbusBit;//�������� ���������� ����
  immsbigcomponent->setModbusRegister = setIMMSBigModbusRegister;//�������� ���������� ��������
  immsbigcomponent->setModbusBit      = setIMMSBigModbusBit;//�������� ���������� ����

  immsbigcomponent->preReadAction   = preIMMSBigReadAction;//action �� ������
  immsbigcomponent->postReadAction  = postIMMSBigReadAction;//action ����� ������
  immsbigcomponent->preWriteAction  = preIMMSBigWriteAction;//action �� ������
  immsbigcomponent->postWriteAction = postIMMSBigWriteAction;//action ����� ������
  immsbigcomponent->config_and_settings = config_and_settingsIMMSBig;//action ���������

  immsbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

int getIMMSBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
extern int pointInterface;//����� ���������� 0-USB 1-RS485
  if(privateIMMSBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(immsbigcomponent->isActiveActualData) setIMMSBigCountObject(); //�-�� �������
  immsbigcomponent->isActiveActualData = 0;
  if(privateIMMSBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  superSetOperativMarker(immsbigcomponent, adrReg);

  int offset = adrReg-BEGIN_ADR_REGISTER;
  int idxSubObj = offset/REGISTER_FOR_OBJ;//������ ����������
  int idxParam = (offset/2)% 2;//������ param
  __settings_for_INPUT_MMS_BLOCK *arr =  ((config_settings_modified & MASKA_FOR_BIT(BIT_USB_LOCKS)) == 0 ) ? &(((__LN_INPUT_MMS_BLOCK*)(spca_of_p_prt[ID_FB_INPUT_MMS_BLOCK - _ID_FB_FIRST_VAR])) + idxSubObj)->settings : (((__settings_for_INPUT_MMS_BLOCK*)(sca_of_p[ID_FB_INPUT_MMS_BLOCK - _ID_FB_FIRST_VAR])) + idxSubObj);
  if(pointInterface)//����� ���������� 0-USB 1-RS485
                        arr =  ((config_settings_modified & MASKA_FOR_BIT(BIT_RS485_LOCKS)) == 0 ) ? &(((__LN_INPUT_MMS_BLOCK*)(spca_of_p_prt[ID_FB_INPUT_MMS_BLOCK - _ID_FB_FIRST_VAR])) + idxSubObj)->settings : (((__settings_for_INPUT_MMS_BLOCK*)(sca_of_p[ID_FB_INPUT_MMS_BLOCK - _ID_FB_FIRST_VAR])) + idxSubObj);

  switch(offset%2) {//������ �������� 
   case 0:
        return  arr->param[idxParam] & 0xffff;//
   case 1:
        return  (arr->param[idxParam] >> 16) & 0x7fff;//
  }//switch
  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int getIMMSBigModbusBit(int x)
{
  //�������� ���������� ��������
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setIMMSBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateIMMSBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(immsbigcomponent->isActiveActualData) setIMMSBigCountObject(); //�-�� �������
  immsbigcomponent->isActiveActualData = 0;
  if(privateIMMSBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  superSetOperativMarker(immsbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  switch((adrReg-BEGIN_ADR_REGISTER)%REGISTER_FOR_OBJ) {
   case 0:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
   break; 
   case 1:
    //�������� ���������� ������������
    if(superControlParam(dataReg)) return MARKER_ERRORDIAPAZON;
   break; 
   case 2:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
   break; 
   case 3:
    //�������� ���������� ������������
    if(superControlParam(dataReg)) return MARKER_ERRORDIAPAZON;
   break; 
  default: return MARKER_OUTPERIMETR;
  }//switch
  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int setIMMSBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� bit
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void setIMMSBigCountObject(void) {
//�������� �-�� �������
  int cntObj = current_config.n_input_MMS_block;  //ʳ������ �������� IMMS
  if(cntObj<0) return;
  if(cntObj>TOTAL_OBJ) return;
  immsbigcomponent->countObject = cntObj;
}//
void preIMMSBigReadAction(void) {
//action �� ������
  immsbigcomponent->operativMarker[0] = -1;
  immsbigcomponent->operativMarker[1] = -1;//����������� ������
  immsbigcomponent->isActiveActualData = 1;
}//
void postIMMSBigReadAction(void) {
//action ����� ������
  if(immsbigcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preIMMSBigWriteAction(void) {
//action �� ������
  immsbigcomponent->operativMarker[0] = -1;
  immsbigcomponent->operativMarker[1] = -1;//����������� ������
  immsbigcomponent->isActiveActualData = 1;
}//
int postIMMSBigWriteAction(void) {
extern int pointInterface;//����� ���������� 0-USB 1-RS485
extern int upravlconfig_and_settings;//���� ��������� ����������
//action ����� ������
  if(immsbigcomponent->operativMarker[0]<0) return 0;//�� ���� ������
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
  int countRegister = immsbigcomponent->operativMarker[1]-immsbigcomponent->operativMarker[0]+1;
  if(immsbigcomponent->operativMarker[1]<0) countRegister = 1;

   __settings_for_INPUT_MMS_BLOCK *arr  = (__settings_for_INPUT_MMS_BLOCK*)(sca_of_p[ID_FB_INPUT_MMS_BLOCK - _ID_FB_FIRST_VAR]);
   __settings_for_INPUT_MMS_BLOCK *arr1 = (__settings_for_INPUT_MMS_BLOCK*)(sca_of_p_edit[ID_FB_INPUT_MMS_BLOCK - _ID_FB_FIRST_VAR]);
//�������� edit ������
  for(int i=0; i<countRegister; i++) {
  int offset = i+immsbigcomponent->operativMarker[0]-BEGIN_ADR_REGISTER;
  int idxSubObj = offset/REGISTER_FOR_OBJ;//������ ����������
  int idx_SIGNALS_IN = (offset%REGISTER_FOR_OBJ)/2;//������ ����� ����������

  switch(offset%2) {//������ �������� �����
  case 0:
        arr1[idxSubObj].param[idx_SIGNALS_IN]  &= (uint32_t)~0xffff;
        arr1[idxSubObj].param[idx_SIGNALS_IN]  |= (tempWriteArray[offsetTempWriteArray+i] & 0xffff);
  break;
  case 1:
        arr1[idxSubObj].param[idx_SIGNALS_IN]  &= (uint32_t)~(0x7fff<<16);
        arr1[idxSubObj].param[idx_SIGNALS_IN]  |= ((tempWriteArray[offsetTempWriteArray+i] & 0x7fff)<<16);//
  break;
 }//switch
  }//for

  //�������� ����������
  for(int i=0; i<countRegister; i++) {
  int offset = i+immsbigcomponent->operativMarker[0]-BEGIN_ADR_REGISTER;
  int idxSubObj = offset/REGISTER_FOR_OBJ;//������ ����������
  int idx_SIGNALS_IN = (offset%REGISTER_FOR_OBJ)/2;//������ ����� ����������

  switch(offset%2) {//������ �������� �����
  case 0:
  case 1:
        if(superValidParam(arr1[idxSubObj].param[idx_SIGNALS_IN])) 
                {//�������� ����������
                repairEditArrayIMMS(countRegister, arr, arr1);//������������ edit ������
                return 2;//����
        }//if
  break;
 }//switch
  }//for

//�������� ������� - ��������������
  for(int i=0; i<countRegister; i++) {
  int offset = i+immsbigcomponent->operativMarker[0]-BEGIN_ADR_REGISTER;
  int idxSubObj = offset/REGISTER_FOR_OBJ;//������ ����������
  int idx_SIGNALS_IN = (offset%REGISTER_FOR_OBJ)/2;//������ ����� ����������

  switch(offset%2) {//������ �������� �����
  case 0:
        arr1[idxSubObj].param[idx_SIGNALS_IN] = arr[idxSubObj].param[idx_SIGNALS_IN] &= (uint32_t)~0xffff;
        arr1[idxSubObj].param[idx_SIGNALS_IN] = arr[idxSubObj].param[idx_SIGNALS_IN] |= (tempWriteArray[offsetTempWriteArray+i] & 0xffff);
  break;
  case 1:
        arr1[idxSubObj].param[idx_SIGNALS_IN] = arr[idxSubObj].param[idx_SIGNALS_IN] &= (uint32_t)~(0x7fff<<16);
        arr1[idxSubObj].param[idx_SIGNALS_IN] = arr[idxSubObj].param[idx_SIGNALS_IN] |= ((tempWriteArray[offsetTempWriteArray+i] & 0x7fff)<<16);//
  break;
 }//switch
  }//for

  upravlconfig_and_settings=1;//���� ��������� ����������

  config_settings_modified |= MASKA_FOR_BIT(BIT_CHANGED_SCHEMATIC);
  if(pointInterface)//����� ���������� 0-USB 1-RS485
     config_settings_modified |= MASKA_FOR_BIT(BIT_RS485_LOCKS);
  else 
     config_settings_modified |= MASKA_FOR_BIT(BIT_USB_LOCKS);
  restart_timeout_idle_new_settings = true;
 return 0;
}//

void repairEditArrayIMMS(int countRegister, __settings_for_INPUT_MMS_BLOCK *arr, __settings_for_INPUT_MMS_BLOCK *arr1) {
  //������������ edit ������
  for(int i=0; i<countRegister; i++) {
  int offset = i+immsbigcomponent->operativMarker[0]-BEGIN_ADR_REGISTER;
  int idxSubObj = offset/REGISTER_FOR_OBJ;//������ ����������
  int idx_SIGNALS_IN = (offset%REGISTER_FOR_OBJ)/2;//������ ����� ����������

  switch(offset%2) {//������ �������� �����
  case 0:
  case 1:
        arr1[idxSubObj].param[idx_SIGNALS_IN] = arr[idxSubObj].param[idx_SIGNALS_IN];
  break;
 }//switch
  }//for
}//repairEditArray(int countRegister, __settings_for_XOR *arr, __settings_for_XOR *arr1) 

int privateIMMSBigGetReg1(int adrReg)
{
  //��������� ���������� ��������
  int count_register = immsbigcomponent->countObject*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg1(int adrReg)
int privateIMMSBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = TOTAL_OBJ*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)

void config_and_settingsIMMSBig(void)
{
}

