#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 3000
//���� �-�� �������
#define TOTAL_OBJ 128
#define REGISTER_FOR_OBJ 11

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

  sdibigcomponent->countObject = 14;//�-�� �������

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
  //ActualData
  for(int i=0; i<100; i++) tempReadArray[i] = i;
}//loadActualData() 

int getSDIBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateSDIBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
//  if(privateSDIBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  if(sdibigcomponent->isActiveActualData) loadSDIBigActualData(); //ActualData
  sdibigcomponent->isActiveActualData = 0;

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
   return dataReg;
   case 1:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
    return dataReg;
   case 2:
   return dataReg;
   case 3:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
    return dataReg;
   case 4:
   return dataReg;
   case 5:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
    return dataReg;
   case 6:
   return dataReg;
   case 7:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
    return dataReg;
   case 8:
   return dataReg;
   case 9:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
    return dataReg;
   case 10:
   return dataReg;
  }//switch
  return MARKER_OUTPERIMETR;
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
//  if(cntObj<0) return;
//  if(cntObj>TOTAL_OBJ) return;
//  sdibigcomponent->countObject = cntObj;
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

int privateSDIBigGetReg2(int adrReg)
{
  //��������� ���������� ��������
  int count_register = sdibigcomponent->countObject*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg1(int adrReg)
//int privateSDIBigGetReg2(int adrReg)
//{
  //��������� ������� ��������
//  int count_register = TOTAL_OBJ*REGISTER_FOR_OBJ;
//  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
//  return MARKER_OUTPERIMETR;
//}//privateGetReg2(int adrReg)
