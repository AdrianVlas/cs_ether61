#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 11000
//���� �-�� �������
#define REGISTER_FOR_OBJ 132

int privateMEBigGetReg2(int adrReg);

int getMEBigModbusRegister(int);//�������� ���������� ��������
int getMEBigModbusBit(int);//�������� ���������� ����
int setMEBigModbusRegister(int, int);//�������� ���������� ��������
int setMEBigModbusBit(int, int);//�������� ���������� ����

void setMEBigCountObject(int);//�������� �-�� �������
void preMEBigReadAction(void);//action �� ������
void postMEBigReadAction(void);//action ����� ������
void preMEBigWriteAction(void);//action �� ������
void postMEBigWriteAction(void);//action ����� ������
void loadMEBigActualData(void);

COMPONENT_OBJ *mebigcomponent;

/**************************************/
//���������� ���������� ������ �������
/**************************************/
void constructorMEBigComponent(COMPONENT_OBJ *mebigcomp)
{
  mebigcomponent = mebigcomp;

  mebigcomponent->countObject = 1;//�-�� �������

  mebigcomponent->getModbusRegister = getMEBigModbusRegister;//�������� ���������� ��������
  mebigcomponent->getModbusBit      = getMEBigModbusBit;//�������� ���������� ��������
  mebigcomponent->setModbusRegister = setMEBigModbusRegister;//�������� ���������� ��������
  mebigcomponent->setModbusBit      = setMEBigModbusBit;//�������� ���������� ��������

  mebigcomponent->setCountObject  = setMEBigCountObject;//�������� �-�� �������
  mebigcomponent->preReadAction   = preMEBigReadAction;//action �� ������
  mebigcomponent->postReadAction  = postMEBigReadAction;//action ����� ������
  mebigcomponent->preWriteAction  = preMEBigWriteAction;//action �� ������
  mebigcomponent->postWriteAction = postMEBigWriteAction;//action ����� ������

  mebigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadMEBigActualData(void) {
  //ActualData
  for(int i=0; i<100; i++) tempReadArray[i] = i;
}//loadActualData() 

int getMEBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateMEBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  if(mebigcomponent->isActiveActualData) loadMEBigActualData(); //ActualData
  mebigcomponent->isActiveActualData = 0;

  superSetOperativMarker(mebigcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDOUTBigModbusRegister(int adrReg)
int getMEBigModbusBit(int adrBit)
{
  //�������� ���������� ��������
  superSetOperativMarker(mebigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setMEBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateMEBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  superSetOperativMarker(mebigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  return dataReg;
}//getDOUTBigModbusRegister(int adrReg)
int setMEBigModbusBit(int adrBit, int x)
{
  UNUSED(x);
  //�������� ���������� ��������
  superSetOperativMarker(mebigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void setMEBigCountObject(int x) {
  UNUSED(x);
//�������� �-�� �������
}//
void preMEBigReadAction(void) {
//action �� ������
  mebigcomponent->operativMarker[0] = -1;
  mebigcomponent->operativMarker[1] = -1;//����������� ������
  mebigcomponent->isActiveActualData = 1;
}//
void postMEBigReadAction(void) {
//action ����� ������
  if(mebigcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preMEBigWriteAction(void) {
//action �� ������
  mebigcomponent->operativMarker[0] = -1;
  mebigcomponent->operativMarker[1] = -1;//����������� ������
  mebigcomponent->isActiveActualData = 1;
}//
void postMEBigWriteAction(void) {
//action ����� ������
  if(mebigcomponent->operativMarker[0]<0) return;//�� ���� ������
  int offset = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
  int countRegister = mebigcomponent->operativMarker[1]-mebigcomponent->operativMarker[0]+1;
  if(mebigcomponent->operativMarker[1]<0) countRegister = 1;
}//

int privateMEBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)
