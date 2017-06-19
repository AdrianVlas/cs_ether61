#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 0
#define REGISTER_FOR_OBJ 16

int getIUModbusRegister(int);//�������� ���������� ��������
int getIUModbusBit(int);//�������� ���������� ����
int setIUModbusRegister(int, int);//�������� ���������� ��������
int setIUModbusBit(int, int);//�������� ���������� ����

void setIUSmallCountObject(int);//�������� �-�� �������
void preIUSmallReadAction(void);//action �� ������
void postIUSmallReadAction(void);//action ����� ������
void preIUSmallWriteAction(void);//action �� ������
void postIUSmallWriteAction(void);//action ����� ������
void loadIUSmallActualData(void);

int privateIUGetReg2(int adrReg);

COMPONENT_OBJ *iucomponent;

/**************************************/
//���������� ���������� ������������� ����������
/**************************************/
void constructorIUSmallComponent(COMPONENT_OBJ *iucomp)
{
  iucomponent = iucomp;

  iucomponent->countObject = 1;//�-�� �������

  iucomponent->getModbusRegister = getIUModbusRegister;//�������� ���������� ��������
  iucomponent->getModbusBit      = getIUModbusBit;//�������� ���������� ����
  iucomponent->setModbusRegister = setIUModbusRegister;//�������� ���������� ��������
  iucomponent->setModbusBit      = setIUModbusBit;//�������� ���������� ����

  iucomponent->setCountObject  = setIUSmallCountObject;//�������� �-�� �������
  iucomponent->preReadAction   = preIUSmallReadAction;//action �� ������
  iucomponent->postReadAction  = postIUSmallReadAction;//action ����� ������
  iucomponent->preWriteAction  = preIUSmallWriteAction;//action �� ������
  iucomponent->postWriteAction = postIUSmallWriteAction;//action ����� ������

  iucomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadIUSmallActualData(void) {
  //ActualData
  for(int i=0; i<100; i++) tempReadArray[i] = i+22;
//  tempReadArray[0] = 22;
}//loadActualData() 

int getIUModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateIUGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  if(iucomponent->isActiveActualData) loadIUSmallActualData(); //ActualData
  iucomponent->isActiveActualData = 0;

  superSetOperativMarker(iucomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDVModbusRegister(int adrReg)
int getIUModbusBit(int adrBit) {
  //�������� ���������� bit
  superSetOperativMarker(iucomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getIUModbusBit(int )
int setIUModbusRegister(int adrReg, int x) {
  UNUSED(x);
  //�������� ���������� ��������
  superSetOperativMarker(iucomponent, adrReg);
  return MARKER_OUTPERIMETR;
}//setIUModbusRegister(int, int)
int setIUModbusBit(int adrBit, int x) {
  UNUSED(x);
  //�������� ���������� bit
  superSetOperativMarker(iucomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//setIUModbusBit(int, int )

void setIUSmallCountObject(int x) {
  UNUSED(x);
//�������� �-�� �������
}//
void preIUSmallReadAction(void) {
//action �� ������
  iucomponent->operativMarker[0] = -1;
  iucomponent->operativMarker[1] = -1;//����������� ������
  iucomponent->isActiveActualData = 1;
}//
void postIUSmallReadAction(void) {
//action ����� ������
  if(iucomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preIUSmallWriteAction(void) {
//action �� ������
  iucomponent->operativMarker[0] = -1;
  iucomponent->operativMarker[1] = -1;//����������� ������
  iucomponent->isActiveActualData = 1;
}//
void postIUSmallWriteAction(void) {
//action ����� ������
}//

int privateIUGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)

