#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 14536
//���� �-�� �������
#define TOTAL_OBJ 128
#define REGISTER_FOR_OBJ 18

int privateOLANBigGetReg1(int adrReg);
int privateOLANBigGetReg2(int adrReg);

int getOLANBigModbusRegister(int);//�������� ���������� ��������
int getOLANBigModbusBit(int);//�������� ���������� ����
int setOLANBigModbusRegister(int, int);//�������� ���������� ��������
int setOLANBigModbusBit(int, int);//�������� ���������� ����

void setOLANBigCountObject(void);//�������� �-�� �������
void preOLANBigReadAction(void);//action �� ������
void postOLANBigReadAction(void);//action ����� ������
void preOLANBigWriteAction(void);//action �� ������
int postOLANBigWriteAction(void);//action ����� ������
void repairEditArrayOLAN(int countRegister, __settings_for_NETWORK_OUTPUT_BLOCK *arr, __settings_for_NETWORK_OUTPUT_BLOCK *arr1);
void config_and_settingsOLANBig(void);//action ���������

COMPONENT_OBJ *olanbigcomponent;

/**************************************/
//���������� ���������� Output LAN
/**************************************/
void constructorOLANBigComponent(COMPONENT_OBJ *olanbigcomp)
{
  olanbigcomponent = olanbigcomp;

  olanbigcomponent->countObject = 0;//�-�� �������

  olanbigcomponent->getModbusRegister = getOLANBigModbusRegister;//�������� ���������� ��������
  olanbigcomponent->getModbusBit      = getOLANBigModbusBit;//�������� ���������� ����
  olanbigcomponent->setModbusRegister = setOLANBigModbusRegister;//�������� ���������� ��������
  olanbigcomponent->setModbusBit      = setOLANBigModbusBit;//�������� ���������� ����

  olanbigcomponent->preReadAction   = preOLANBigReadAction;//action �� ������
  olanbigcomponent->postReadAction  = postOLANBigReadAction;//action ����� ������
  olanbigcomponent->preWriteAction  = preOLANBigWriteAction;//action �� ������
  olanbigcomponent->postWriteAction = postOLANBigWriteAction;//action ����� ������
  olanbigcomponent->config_and_settings = config_and_settingsOLANBig;//action ���������

  olanbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

int getOLANBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
extern int pointInterface;//����� ���������� 0-USB 1-RS485
  if(privateOLANBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(olanbigcomponent->isActiveActualData) setOLANBigCountObject(); //�-�� �������
  olanbigcomponent->isActiveActualData = 0;
  if(privateOLANBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  superSetOperativMarker(olanbigcomponent, adrReg);

  int offset = adrReg-BEGIN_ADR_REGISTER;
  int idxSubObj = offset/REGISTER_FOR_OBJ;//������ ����������
  int idxParam = (offset/2)%9;//������ param
  __settings_for_NETWORK_OUTPUT_BLOCK *arr =  ((config_settings_modified & MASKA_FOR_BIT(BIT_USB_LOCKS)) == 0 ) ? &(((__LN_NETWORK_OUTPUT_BLOCK*)(spca_of_p_prt[ID_FB_NETWORK_OUTPUT_BLOCK - _ID_FB_FIRST_VAR])) + idxSubObj)->settings : (((__settings_for_NETWORK_OUTPUT_BLOCK*)(sca_of_p[ID_FB_NETWORK_OUTPUT_BLOCK - _ID_FB_FIRST_VAR])) + idxSubObj);
  if(pointInterface)//����� ���������� 0-USB 1-RS485
                        arr =  ((config_settings_modified & MASKA_FOR_BIT(BIT_RS485_LOCKS)) == 0 ) ? &(((__LN_NETWORK_OUTPUT_BLOCK*)(spca_of_p_prt[ID_FB_NETWORK_OUTPUT_BLOCK - _ID_FB_FIRST_VAR])) + idxSubObj)->settings : (((__settings_for_NETWORK_OUTPUT_BLOCK*)(sca_of_p[ID_FB_NETWORK_OUTPUT_BLOCK - _ID_FB_FIRST_VAR])) + idxSubObj);

  switch(offset%2) {//������ �������� 
   case 0:
        return  arr->param[idxParam] & 0xffff;//
   case 1:
        return  (arr->param[idxParam] >> 16) & 0x7fff;//
  }//switch
  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int getOLANBigModbusBit(int x)
{
  //�������� ���������� ��������
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setOLANBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateOLANBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(olanbigcomponent->isActiveActualData) setOLANBigCountObject(); //�-�� �������
  olanbigcomponent->isActiveActualData = 0;
  if(privateOLANBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  superSetOperativMarker(olanbigcomponent, adrReg);
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
   case 4:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
   break; 
   case 5:
    //�������� ���������� ������������
    if(superControlParam(dataReg)) return MARKER_ERRORDIAPAZON;
   break; 
   case 6:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
   break; 
   case 7:
    //�������� ���������� ������������
    if(superControlParam(dataReg)) return MARKER_ERRORDIAPAZON;
   break; 
   case 8:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
   break; 
   case 9:
    //�������� ���������� ������������
    if(superControlParam(dataReg)) return MARKER_ERRORDIAPAZON;
   break; 
   case 10:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
   break; 
   case 11:
    //�������� ���������� ������������
    if(superControlParam(dataReg)) return MARKER_ERRORDIAPAZON;
   break; 
   case 12:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
   break; 
   case 13:
    //�������� ���������� ������������
    if(superControlParam(dataReg)) return MARKER_ERRORDIAPAZON;
   break; 
   case 14:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
   break; 
   case 15:
    //�������� ���������� ������������
    if(superControlParam(dataReg)) return MARKER_ERRORDIAPAZON;
   break; 
   case 16:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
   break; 
   case 17:
    //�������� ���������� ������������
    if(superControlParam(dataReg)) return MARKER_ERRORDIAPAZON;
   break; 
  default: return MARKER_OUTPERIMETR;
  }//switch
  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int setOLANBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void setOLANBigCountObject(void) {
//�������� �-�� �������
  int cntObj = current_config.n_network_output_block;    //ʳ������ �������� "���"
  if(cntObj<0) return;
  if(cntObj>TOTAL_OBJ) return;
  olanbigcomponent->countObject = cntObj;
}//
void preOLANBigReadAction(void) {
//action �� ������
  olanbigcomponent->operativMarker[0] = -1;
  olanbigcomponent->operativMarker[1] = -1;//����������� ������
  olanbigcomponent->isActiveActualData = 1;
}//
void postOLANBigReadAction(void) {
//action ����� ������
  if(olanbigcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preOLANBigWriteAction(void) {
//action �� ������
  olanbigcomponent->operativMarker[0] = -1;
  olanbigcomponent->operativMarker[1] = -1;//����������� ������
  olanbigcomponent->isActiveActualData = 1;
}//
int postOLANBigWriteAction(void) {
extern int pointInterface;//����� ���������� 0-USB 1-RS485
extern int upravlconfig_and_settings;//���� ��������� ����������
//action ����� ������
  if(olanbigcomponent->operativMarker[0]<0) return 0;//�� ���� ������
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
  int countRegister = olanbigcomponent->operativMarker[1]-olanbigcomponent->operativMarker[0]+1;
  if(olanbigcomponent->operativMarker[1]<0) countRegister = 1;

   __settings_for_NETWORK_OUTPUT_BLOCK *arr  = (__settings_for_NETWORK_OUTPUT_BLOCK*)(sca_of_p[ID_FB_NETWORK_OUTPUT_BLOCK - _ID_FB_FIRST_VAR]);
   __settings_for_NETWORK_OUTPUT_BLOCK *arr1 = (__settings_for_NETWORK_OUTPUT_BLOCK*)(sca_of_p_edit[ID_FB_NETWORK_OUTPUT_BLOCK - _ID_FB_FIRST_VAR]);
//�������� edit ������
  for(int i=0; i<countRegister; i++) {
  int offset = i+olanbigcomponent->operativMarker[0]-BEGIN_ADR_REGISTER;
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
  int offset = i+olanbigcomponent->operativMarker[0]-BEGIN_ADR_REGISTER;
  int idxSubObj = offset/REGISTER_FOR_OBJ;//������ ����������
  int idx_SIGNALS_IN = (offset%REGISTER_FOR_OBJ)/2;//������ ����� ����������

  switch(offset%2) {//������ �������� �����
  case 0:
  case 1:
        if(superValidParam(arr1[idxSubObj].param[idx_SIGNALS_IN])) 
                {//�������� ����������
                repairEditArrayOLAN(countRegister, arr, arr1);//������������ edit ������
                return 2;//����
        }//if
  break;
 }//switch
  }//for

//�������� ������� - ��������������
  for(int i=0; i<countRegister; i++) {
  int offset = i+olanbigcomponent->operativMarker[0]-BEGIN_ADR_REGISTER;
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

void repairEditArrayOLAN(int countRegister, __settings_for_NETWORK_OUTPUT_BLOCK *arr, __settings_for_NETWORK_OUTPUT_BLOCK *arr1) {
  //������������ edit ������
  for(int i=0; i<countRegister; i++) {
  int offset = i+olanbigcomponent->operativMarker[0]-BEGIN_ADR_REGISTER;
  int idxSubObj = offset/REGISTER_FOR_OBJ;//������ ����������
  int idx_SIGNALS_IN = (offset%REGISTER_FOR_OBJ)/2;//������ ����� ����������

  switch(offset%2) {//������ �������� �����
  case 0:
  case 1:
        arr1[idxSubObj].param[idx_SIGNALS_IN] = arr[idxSubObj].param[idx_SIGNALS_IN];
  break;
 }//switch
  }//for
}//repairEditArray(int countRegister, __settings_for_OR *arr, __settings_for_OR *arr1) 

int privateOLANBigGetReg1(int adrReg)
{
  //��������� ���������� ��������
  int count_register = olanbigcomponent->countObject*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg1(int adrReg)
int privateOLANBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = TOTAL_OBJ*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)

void config_and_settingsOLANBig(void)
{
}

