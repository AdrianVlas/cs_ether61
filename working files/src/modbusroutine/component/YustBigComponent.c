#include "variables_external_m.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 61968
//���� �-�� �������
#define REGISTER_FOR_OBJ 33

int privateYustBigGetReg2(int adrReg);

int getYustBigModbusRegister(int);//�������� ���������� ��������
int getYustBigModbusBit(int);//�������� ���������� ����
int setYustBigModbusRegister(int, int);//�������� ���������� ��������
int setYustBigModbusBit(int, int);//�������� ���������� ����

void setYustBigCountObject(int);//�������� �-�� �������
void preYustBigReadAction();//action �� ������
void postYustBigReadAction();//action ����� ������
void preYustBigWriteAction();//action �� ������
void postYustBigWriteAction();//action ����� ������
void loadYustBigActualData();

COMPONENT_OBJ *yustbigcomponent;

/**************************************/
//���������� ���������� ���������
/**************************************/
void constructorYustBigComponent(COMPONENT_OBJ *yustbigcomp)
{
  yustbigcomponent = yustbigcomp;

  yustbigcomponent->countObject = 1;//�-�� �������

  yustbigcomponent->getModbusRegister = getYustBigModbusRegister;//�������� ���������� ��������
  yustbigcomponent->getModbusBit      = getYustBigModbusBit;//�������� ���������� ����
  yustbigcomponent->setModbusRegister = setYustBigModbusRegister;//�������� ���������� ��������
  yustbigcomponent->setModbusBit      = setYustBigModbusBit;//�������� ���������� ����

  yustbigcomponent->setCountObject  = setYustBigCountObject;//�������� �-�� �������
  yustbigcomponent->preReadAction   = preYustBigReadAction;//action �� ������
  yustbigcomponent->postReadAction  = postYustBigReadAction;//action ����� ������
  yustbigcomponent->preWriteAction  = preYustBigWriteAction;//action �� ������
  yustbigcomponent->postWriteAction = postYustBigWriteAction;//action ����� ������

  yustbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadYustBigActualData() {
  //ActualData
  for(int i=0; i<100; i++) tempReadArray[i] = i;
}//loadActualData() 

int getYustBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateYustBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  if(yustbigcomponent->isActiveActualData) loadYustBigActualData(); //ActualData
  yustbigcomponent->isActiveActualData = 0;

  superSetOperativMarker(yustbigcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDOUTBigModbusRegister(int adrReg)
int getYustBigModbusBit(int adrBit)
{
  //�������� ���������� ��������
  superSetOperativMarker(yustbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setYustBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateYustBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  superSetOperativMarker(yustbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  return dataReg;
}//getDOUTBigModbusRegister(int adrReg)
int setYustBigModbusBit(int adrBit, int )
{
  //�������� ���������� bit
  superSetOperativMarker(yustbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void setYustBigCountObject(int ) {
//�������� �-�� �������
}//
void preYustBigReadAction() {
//action �� ������
  yustbigcomponent->operativMarker[0] = -1;
  yustbigcomponent->operativMarker[1] = -1;//����������� ������
  yustbigcomponent->isActiveActualData = 1;
}//
void postYustBigReadAction() {
//action ����� ������
  if(yustbigcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preYustBigWriteAction() {
//action �� ������
  yustbigcomponent->operativMarker[0] = -1;
  yustbigcomponent->operativMarker[1] = -1;//����������� ������
  yustbigcomponent->isActiveActualData = 1;
}//
void postYustBigWriteAction() {
//action ����� ������
  if(yustbigcomponent->operativMarker[0]<0) return;//�� ���� ������
  int offset = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
  int countRegister = yustbigcomponent->operativMarker[1]-yustbigcomponent->operativMarker[0]+1;
  if(yustbigcomponent->operativMarker[1]<0) countRegister = 1;
}//

int privateYustBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)
