#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 4811
//���� �-�� �������
#define TOTAL_OBJ 128
#define REGISTER_FOR_OBJ 10

int privateSZSBigGetReg1(int adrReg);
int privateSZSBigGetReg2(int adrReg);

int getSZSBigModbusRegister(int);//�������� ���������� ��������
int getSZSBigModbusBit(int);//�������� ���������� ����
int setSZSBigModbusRegister(int, int);//�������� ���������� ��������
int setSZSBigModbusBit(int, int);//�������� ���������� ����

void setSZSBigCountObject(void);//�������� �-�� �������
void preSZSBigReadAction(void);//action �� ������
void postSZSBigReadAction(void);//action ����� ������
void preSZSBigWriteAction(void);//action �� ������
void postSZSBigWriteAction(void);//action ����� ������
void loadSZSBigActualData(void);

COMPONENT_OBJ *szsbigcomponent;

/**************************************/
//���������� ���������� ���
/**************************************/
void constructorSZSBigComponent(COMPONENT_OBJ *szsbigcomp)
{
  szsbigcomponent = szsbigcomp;

  szsbigcomponent->countObject = 0;//�-�� �������

  szsbigcomponent->getModbusRegister = getSZSBigModbusRegister;//�������� ���������� ��������
  szsbigcomponent->getModbusBit      = getSZSBigModbusBit;//�������� ���������� ����
  szsbigcomponent->setModbusRegister = setSZSBigModbusRegister;//�������� ���������� ��������
  szsbigcomponent->setModbusBit      = setSZSBigModbusBit;//�������� ���������� ����

  szsbigcomponent->preReadAction   = preSZSBigReadAction;//action �� ������
  szsbigcomponent->postReadAction  = postSZSBigReadAction;//action ����� ������
  szsbigcomponent->preWriteAction  = preSZSBigWriteAction;//action �� ������
  szsbigcomponent->postWriteAction = postSZSBigWriteAction;//action ����� ������

  szsbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadSZSBigActualData(void) {
 setSZSBigCountObject(); //�������� �-�� �������
  //ActualData
   __LN_ALARM *arr = (__LN_ALARM*)(spca_of_p_prt[ID_FB_ALARM - _ID_FB_FIRST_VAR]);
   for(int item=0; item<szsbigcomponent->countObject; item++) {
   //����� ������ ��� item
   int value = arr[item].settings.control & 0x3;
   tempReadArray[item*REGISTER_FOR_OBJ+0] = value;

   //������ ���  item
   value = arr[item].settings.set_delay[ALARM_SET_DELAY_PERIOD];
   tempReadArray[item*REGISTER_FOR_OBJ+1] = value / 100;

   //LSSIN1 0 ���  item
   value = arr[item].settings.param[ALARM_LOGIC_INPUT] & 0xffff;//LEDIN 0 �� item
   tempReadArray[item*REGISTER_FOR_OBJ+2] = value;
   value = (arr[item].settings.param[ALARM_LOGIC_INPUT] >> 16) & 0x7fff;//LEDIN 1 �� item
   tempReadArray[item*REGISTER_FOR_OBJ+3] = value;

   //Mute-I 0 ���   item
   value = arr[item].settings.param[ALARM_IN_MUTE] & 0xffff;//LEDIN 0 �� item
   tempReadArray[item*REGISTER_FOR_OBJ+4] = value;
   value = (arr[item].settings.param[ALARM_IN_MUTE] >> 16) & 0x7fff;//LEDIN 1 �� item
   tempReadArray[item*REGISTER_FOR_OBJ+5] = value;

   //Block-I 0 ���    item
   value = arr[item].settings.param[ALARM_IN_BLOCK] & 0xffff;//LEDIN 0 �� item
   tempReadArray[item*REGISTER_FOR_OBJ+6] = value;
   value = (arr[item].settings.param[ALARM_IN_BLOCK] >> 16) & 0x7fff;//LEDIN 1 �� item
   tempReadArray[item*REGISTER_FOR_OBJ+7] = value;

   //Reset-I 0 ���   item
   value = arr[item].settings.param[ALARM_RESET] & 0xffff;//LEDIN 0 �� item
   tempReadArray[item*REGISTER_FOR_OBJ+8] = value;
   value = (arr[item].settings.param[ALARM_RESET] >> 16) & 0x7fff;//LEDIN 1 �� item
   tempReadArray[item*REGISTER_FOR_OBJ+9] = value;
   }//for
}//loadActualData() 

int getSZSBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateSZSBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  if(szsbigcomponent->isActiveActualData) loadSZSBigActualData(); //ActualData
  szsbigcomponent->isActiveActualData = 0;

  if(privateSZSBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;
  superSetOperativMarker(szsbigcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDOUTBigModbusRegister(int adrReg)
int getSZSBigModbusBit(int adrBit)
{
  //�������� ���������� ��������
  superSetOperativMarker(szsbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setSZSBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateSZSBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(privateSZSBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  superSetOperativMarker(szsbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  switch((adrReg-BEGIN_ADR_REGISTER)%REGISTER_FOR_OBJ) {
   case 0:
    if(dataReg>2) return MARKER_ERRORDIAPAZON;
   break; 
   case 1:
    if(dataReg>320) return MARKER_ERRORDIAPAZON;
    if(dataReg<5) return MARKER_ERRORDIAPAZON;
   break; 
   case 2:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
   break; 
   case 3:
   break; 
   case 4:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
   break; 
   case 5:
   break; 
   case 6:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
   break; 
   case 7:
   break; 
   case 8:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
   break; 
   case 9:
   break; 
  default: return MARKER_OUTPERIMETR;
  }//switch
  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int setSZSBigModbusBit(int adrBit, int x)
{
  UNUSED(x);
  //�������� ���������� ��������
  superSetOperativMarker(szsbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void setSZSBigCountObject(void) {
//�������� �-�� �������
  int cntObj = current_config.n_alarm;  //ʳ������ ���������� �������
  if(cntObj<0) return;
  if(cntObj>TOTAL_OBJ) return;
  szsbigcomponent->countObject = cntObj;
}//
void preSZSBigReadAction(void) {
//action �� ������
  szsbigcomponent->operativMarker[0] = -1;
  szsbigcomponent->operativMarker[1] = -1;//����������� ������
  szsbigcomponent->isActiveActualData = 1;
}//
void postSZSBigReadAction(void) {
//action ����� ������
  if(szsbigcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preSZSBigWriteAction(void) {
//action �� ������
  szsbigcomponent->operativMarker[0] = -1;
  szsbigcomponent->operativMarker[1] = -1;//����������� ������
  szsbigcomponent->isActiveActualData = 1;
}//
void postSZSBigWriteAction(void) {
//action ����� ������
  if(szsbigcomponent->operativMarker[0]<0) return;//�� ���� ������
//  int offset = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
//  int countRegister = szsbigcomponent->operativMarker[1]-szsbigcomponent->operativMarker[0]+1;
//  if(szsbigcomponent->operativMarker[1]<0) countRegister = 1;
}//

int privateSZSBigGetReg1(int adrReg)
{
  //��������� ���������� ��������
  int count_register = szsbigcomponent->countObject*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg1(int adrReg)
int privateSZSBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = TOTAL_OBJ*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)
