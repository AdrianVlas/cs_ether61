#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 3000
//���� �-�� �������
#define TOTAL_OBJ 128
#define REGISTER_FOR_OBJ 13

int privateSDIBigGetReg1(int adrReg);
int privateSDIBigGetReg2(int adrReg);

int getSDIBigModbusRegister(int);//�������� ���������� ��������
int getSDIBigModbusBit(int);//�������� ���������� ����
int setSDIBigModbusRegister(int, int);//�������� ���������� ��������
int setSDIBigModbusBit(int, int);//�������� ���������� ����

void setSDIBigCountObject(void);//�������� �-�� �������
void preSDIBigReadAction(void);//action �� ������
void postSDIBigReadAction(void);//action ����� ������
void preSDIBigWriteAction(void);//action �� ������
void postSDIBigWriteAction(void);//action ����� ������
void loadSDIBigActualData(void);

COMPONENT_OBJ *sdibigcomponent;

/**************************************/
//���������� ���������� ����������������
/**************************************/
void constructorSDIBigComponent(COMPONENT_OBJ *sdibigcomp)
{
  sdibigcomponent = sdibigcomp;

  sdibigcomponent->countObject = 0;//�-�� �������

  sdibigcomponent->getModbusRegister = getSDIBigModbusRegister;//�������� ���������� ��������
  sdibigcomponent->getModbusBit      = getSDIBigModbusBit;//�������� ���������� ����
  sdibigcomponent->setModbusRegister = setSDIBigModbusRegister;//�������� ���������� ��������
  sdibigcomponent->setModbusBit      = setSDIBigModbusBit;//�������� ���������� ����

  sdibigcomponent->preReadAction   = preSDIBigReadAction;//action �� ������
  sdibigcomponent->postReadAction  = postSDIBigReadAction;//action ����� ������
  sdibigcomponent->preWriteAction  = preSDIBigWriteAction;//action �� ������
  sdibigcomponent->postWriteAction = postSDIBigWriteAction;//action ����� ������

  sdibigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadSDIBigActualData(void) {
 setSDIBigCountObject(); //�������� �-�� �������

  //ActualData
  __LN_OUTPUT_LED *arr = (__LN_OUTPUT_LED*)(spca_of_p_prt[ID_FB_LED - _ID_FB_FIRST_VAR]);
   for(int item=0; item<sdibigcomponent->countObject; item++) {
   int value = arr[item].settings.control;//��������� �� item
   tempReadArray[item*REGISTER_FOR_OBJ+0] = value;

   value = arr[item].settings.param[OUTPUT_LED_LOGIC_INPUT] & 0xffff;//LEDIN 0 �� item
   tempReadArray[item*REGISTER_FOR_OBJ+1] = value;
   value = (arr[item].settings.param[OUTPUT_LED_LOGIC_INPUT] >> 16) & 0x7fff;//LEDIN 1 �� item
   tempReadArray[item*REGISTER_FOR_OBJ+2] = value;

   value = arr[item].settings.param[OUTPUT_LED_RESET] & 0xffff;//Reset 0 �� item
   tempReadArray[item*REGISTER_FOR_OBJ+3] = value;
   value = (arr[item].settings.param[OUTPUT_LED_RESET] >> 16) & 0x7fff;//Reset 1 �� item
   tempReadArray[item*REGISTER_FOR_OBJ+4] = value;

   value = arr[item].settings.param[OUTPUT_LED_BL_IMP] & 0xffff;//BL-IMP 0 �� item
   tempReadArray[item*REGISTER_FOR_OBJ+5] = value;
   value = (arr[item].settings.param[OUTPUT_LED_BL_IMP] >> 16) & 0x7fff;//BL-IMP 1 �� item
   tempReadArray[item*REGISTER_FOR_OBJ+6] = value;

   value = arr[item].settings.param[OUTPUT_LED_MEANDER1_MEANDER2] & 0xffff;//C1/C2 0 �� item
   tempReadArray[item*REGISTER_FOR_OBJ+7] = value;
   value = (arr[item].settings.param[OUTPUT_LED_MEANDER1_MEANDER2] >> 16) & 0x7fff;//C1/C2 1 �� item
   tempReadArray[item*REGISTER_FOR_OBJ+8] = value;

   value = arr[item].settings.param[OUTPUT_LED_MEANDER1] & 0xffff;//��������� �1 ���.0 �� item
   tempReadArray[item*REGISTER_FOR_OBJ+9] = value;
   value = (arr[item].settings.param[OUTPUT_LED_MEANDER1] >> 16) & 0x7fff;//��������� �1 ���.1 �� item
   tempReadArray[item*REGISTER_FOR_OBJ+10] = value;

   value = arr[item].settings.param[OUTPUT_LED_MEANDER2] & 0xffff;//��������� �2 ���.0 �� item
   tempReadArray[item*REGISTER_FOR_OBJ+11] = value;
   value = (arr[item].settings.param[OUTPUT_LED_MEANDER2] >> 16) & 0x7fff;//��������� �2 ���. 1 �� item
   tempReadArray[item*REGISTER_FOR_OBJ+12] = value;
  }//for
}//loadActualData() 

int getSDIBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateSDIBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(sdibigcomponent->isActiveActualData) loadSDIBigActualData(); //ActualData
  sdibigcomponent->isActiveActualData = 0;
  if(privateSDIBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;//MARKER_ERRORPERIMETR;

  superSetOperativMarker(sdibigcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getSDIBigModbusRegister(int adrReg)
int getSDIBigModbusBit(int adrBit)
{
  //�������� ���������� ��������
  superSetOperativMarker(sdibigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getSDIBigModbusBit(int )
int setSDIBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateSDIBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
//  if(privateSDIBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  superSetOperativMarker(sdibigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  switch((adrReg-BEGIN_ADR_REGISTER)%REGISTER_FOR_OBJ) {
   case 0:
   break; 
   case 1:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
   break; 
   case 2:
   break; 
   case 3:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
   break; 
   case 4:
   break; 
   case 5:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
   break; 
   case 6:
   break; 
   case 7:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
   break; 
   case 8:
   break; 
   case 9:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
   break; 
   case 10:
   break; 
  default: return MARKER_OUTPERIMETR;
  }//switch
  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int setSDIBigModbusBit(int adrBit, int x)
{
  UNUSED(x);
  //�������� ���������� ��������
  superSetOperativMarker(sdibigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void setSDIBigCountObject(void) {
//�������� �-�� �������
  int cntObj = current_config.n_led; //ʳ������ ���������� ��������������
  if(cntObj<0) return;
  if(cntObj>TOTAL_OBJ) return;
  sdibigcomponent->countObject = cntObj;
}//
void preSDIBigReadAction(void) {
//action �� ������
  sdibigcomponent->operativMarker[0] = -1;
  sdibigcomponent->operativMarker[1] = -1;//����������� ������
  sdibigcomponent->isActiveActualData = 1;
}//
void postSDIBigReadAction(void) {
//action ����� ������
  if(sdibigcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preSDIBigWriteAction(void) {
//action �� ������
  sdibigcomponent->operativMarker[0] = -1;
  sdibigcomponent->operativMarker[1] = -1;//����������� ������
  sdibigcomponent->isActiveActualData = 1;
}//
void postSDIBigWriteAction(void) {
//action ����� ������
  if(sdibigcomponent->operativMarker[0]<0) return;//�� ���� ������
//  int offset = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
//  int countRegister = sdibigcomponent->operativMarker[1]-sdibigcomponent->operativMarker[0]+1;
//  if(sdibigcomponent->operativMarker[1]<0) countRegister = 1;
}//

int privateSDIBigGetReg1(int adrReg)
{
  //��������� ���������� ��������
  int count_register = sdibigcomponent->countObject*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg1(int adrReg)
int privateSDIBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = TOTAL_OBJ*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)
