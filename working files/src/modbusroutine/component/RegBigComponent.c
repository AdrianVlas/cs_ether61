#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 12000
//���� �-�� �������
#define REGISTER_FOR_OBJ 135

int privateRegBigGetReg2(int adrReg);

int getRegBigModbusRegister(int);//�������� ���������� ��������
int getRegBigModbusBit(int);//�������� ���������� ����
int setRegBigModbusRegister(int, int);//�������� ���������� ��������
int setRegBigModbusBit(int, int);//�������� ���������� ����

void setRegBigCountObject(int);//�������� �-�� �������
void preRegBigReadAction(void);//action �� ������
void postRegBigReadAction(void);//action ����� ������
void preRegBigWriteAction(void);//action �� ������
void postRegBigWriteAction(void);//action ����� ������
void loadRegBigActualData(void);

COMPONENT_OBJ *regbigcomponent;

/**************************************/
//���������� ���������� �����������
/**************************************/
void constructorRegBigComponent(COMPONENT_OBJ *regbigcomp)
{
  regbigcomponent = regbigcomp;

  regbigcomponent->countObject = 1;//�-�� �������

  regbigcomponent->getModbusRegister = getRegBigModbusRegister;//�������� ���������� ��������
  regbigcomponent->getModbusBit      = getRegBigModbusBit;//�������� ���������� ����
  regbigcomponent->setModbusRegister = setRegBigModbusRegister;//�������� ���������� ��������
  regbigcomponent->setModbusBit      = setRegBigModbusBit;//�������� ���������� ����

  regbigcomponent->setCountObject  = setRegBigCountObject;//�������� �-�� �������
  regbigcomponent->preReadAction   = preRegBigReadAction;//action �� ������
  regbigcomponent->postReadAction  = postRegBigReadAction;//action ����� ������
  regbigcomponent->preWriteAction  = preRegBigWriteAction;//action �� ������
  regbigcomponent->postWriteAction = postRegBigWriteAction;//action ����� ������

  regbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadRegBigActualData(void) {
  //ActualData
  for(int i=0; i<100; i++) tempReadArray[i] = i;
}//loadActualData() 

int getRegBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateRegBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  if(regbigcomponent->isActiveActualData) loadRegBigActualData(); //ActualData
  regbigcomponent->isActiveActualData = 0;

  superSetOperativMarker(regbigcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDOUTBigModbusRegister(int adrReg)
int getRegBigModbusBit(int adrBit)
{
  //�������� ���������� ��������
  superSetOperativMarker(regbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setRegBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateRegBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  superSetOperativMarker(regbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  switch(adrReg-BEGIN_ADR_REGISTER) {
   case 0: //12000
    if(dataReg>250) return MARKER_ERRORDIAPAZON;
    if(dataReg<5) return MARKER_ERRORDIAPAZON;
    return dataReg;
   case 1: //12001
    if(dataReg>1250) return MARKER_ERRORDIAPAZON;
    if(dataReg<5) return MARKER_ERRORDIAPAZON;
    return dataReg;
    case 2://12002
    return MARKER_ERRORPERIMETR;
   case 3: //12003
    if(dataReg>99) return MARKER_ERRORDIAPAZON;
    return dataReg;
  }//switch

  return dataReg;
}//getDOUTBigModbusRegister(int adrReg)
int setRegBigModbusBit(int adrBit, int x)
{
  UNUSED(x);
  //�������� ���������� ��������
  superSetOperativMarker(regbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void setRegBigCountObject(int x) {
  UNUSED(x);
//�������� �-�� �������
}//
void preRegBigReadAction(void) {
//action �� ������
  regbigcomponent->operativMarker[0] = -1;
  regbigcomponent->operativMarker[1] = -1;//����������� ������
  regbigcomponent->isActiveActualData = 1;
}//
void postRegBigReadAction(void) {
//action ����� ������
  if(regbigcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preRegBigWriteAction(void) {
//action �� ������
  regbigcomponent->operativMarker[0] = -1;
  regbigcomponent->operativMarker[1] = -1;//����������� ������
  regbigcomponent->isActiveActualData = 1;
}//
void postRegBigWriteAction(void) {
//action ����� ������
  if(regbigcomponent->operativMarker[0]<0) return;//�� ���� ������
  int offset = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
  int countRegister = regbigcomponent->operativMarker[1]-regbigcomponent->operativMarker[0]+1;
  if(regbigcomponent->operativMarker[1]<0) countRegister = 1;
}//

int privateRegBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)
