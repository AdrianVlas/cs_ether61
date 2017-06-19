#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 408
//��������� bit � ����� ������
#define BEGIN_ADR_BIT 20128
#define REGISTER_FOR_OBJ 1
#define BIT_FOR_OBJ 2

int privateSDI2ColorSmallGetReg2(int adrReg);
int privateSDI2ColorSmallGetBit2(int adrReg);

int getSDI2ColorSmallModbusRegister(int);//�������� ���������� ��������
int getSDI2ColorSmallModbusBit(int);//�������� ���������� ����
int setSDI2ColorSmallModbusRegister(int, int);//�������� ���������� ��������
int setSDI2ColorSmallModbusBit(int, int);//�������� ���������� ����

void setSDI2ColorSmallCountObject(int);//�������� �-�� �������
void preSDI2ColorSmallReadAction(void);//action �� ������
void postSDI2ColorSmallReadAction(void);//action ����� ������
void preSDI2ColorSmallWriteAction(void);//action �� ������
void postSDI2ColorSmallWriteAction(void);//action ����� ������
void loadSDI2ColorSmallActualData(void);

COMPONENT_OBJ *sdi2colorsmallcomponent;

/**************************************/
//���������� ���������� ���������������� 2 �����
/**************************************/
void constructorSDI2ColorSmallComponent(COMPONENT_OBJ *sdi2colorsmallcomp)
{
  sdi2colorsmallcomponent = sdi2colorsmallcomp;

  sdi2colorsmallcomponent->countObject = 3;//�-�� �������

  sdi2colorsmallcomponent->getModbusRegister = getSDI2ColorSmallModbusRegister;//�������� ���������� ��������
  sdi2colorsmallcomponent->getModbusBit      = getSDI2ColorSmallModbusBit;//�������� ���������� ����
  sdi2colorsmallcomponent->setModbusRegister = setSDI2ColorSmallModbusRegister;//�������� ���������� ��������
  sdi2colorsmallcomponent->setModbusBit      = setSDI2ColorSmallModbusBit;//�������� ���������� ����

  sdi2colorsmallcomponent->setCountObject  = setSDI2ColorSmallCountObject;//�������� �-�� �������
  sdi2colorsmallcomponent->preReadAction   = preSDI2ColorSmallReadAction;//action �� ������
  sdi2colorsmallcomponent->postReadAction  = postSDI2ColorSmallReadAction;//action ����� ������
  sdi2colorsmallcomponent->preWriteAction  = preSDI2ColorSmallWriteAction;//action �� ������
  sdi2colorsmallcomponent->postWriteAction = postSDI2ColorSmallWriteAction;//action ����� ������

  sdi2colorsmallcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadSDI2ColorSmallActualData(void) {
  //ActualData
  for(int i=0; i<100; i++) tempReadArray[i] = i;
}//loadActualData() 

int getSDI2ColorSmallModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateSDI2ColorSmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  if(sdi2colorsmallcomponent->isActiveActualData) loadSDI2ColorSmallActualData(); //ActualData
  sdi2colorsmallcomponent->isActiveActualData = 0;

  superSetOperativMarker(sdi2colorsmallcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDVModbusRegister(int adrReg)
int getSDI2ColorSmallModbusBit(int adrBit)
{
  //�������� ���������� bit
  if(privateSDI2ColorSmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  if(sdi2colorsmallcomponent->isActiveActualData) loadSDI2ColorSmallActualData();
  sdi2colorsmallcomponent->isActiveActualData = 0;

  superSetOperativMarker(sdi2colorsmallcomponent, adrBit);

  short tmp   = tempReadArray[(adrBit-BEGIN_ADR_BIT)/16];
  short maska = 1<<((adrBit-BEGIN_ADR_BIT)%16);
  if(tmp&maska) return 1;
  return 0;
}//getDVModbusRegister(int adrReg)
int setSDI2ColorSmallModbusRegister(int adrReg, int x)
{
  UNUSED(x);
  //�������� ���������� ��������
  superSetOperativMarker(sdi2colorsmallcomponent, adrReg);
  return MARKER_OUTPERIMETR;
}//getDVModbusRegister(int adrReg)
int setSDI2ColorSmallModbusBit(int adrBit, int x)
{
  UNUSED(x);
  //�������� ���������� ��������
  superSetOperativMarker(sdi2colorsmallcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDVModbusRegister(int adrReg)

void setSDI2ColorSmallCountObject(int x) {
  UNUSED(x);
//�������� �-�� �������
}//
void preSDI2ColorSmallReadAction(void) {
//action �� ������
  sdi2colorsmallcomponent->operativMarker[0] = -1;
  sdi2colorsmallcomponent->operativMarker[1] = -1;//����������� ������
  sdi2colorsmallcomponent->isActiveActualData = 1;
}//
void postSDI2ColorSmallReadAction(void) {
//action ����� ������
  if(sdi2colorsmallcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preSDI2ColorSmallWriteAction(void) {
//action �� ������
  sdi2colorsmallcomponent->operativMarker[0] = -1;
  sdi2colorsmallcomponent->operativMarker[1] = -1;//����������� ������
  sdi2colorsmallcomponent->isActiveActualData = 1;
}//
void postSDI2ColorSmallWriteAction(void) {
//action ����� ������
}//

int privateSDI2ColorSmallGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = REGISTER_FOR_OBJ*sdi2colorsmallcomponent->countObject;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateSDI2ColorSmallGetReg2(int adrReg)
int privateSDI2ColorSmallGetBit2(int adrBit)
{
  //��������� ������� ��������
  int count_bit = BIT_FOR_OBJ*sdi2colorsmallcomponent->countObject;
  if(adrBit>=BEGIN_ADR_BIT && adrBit<(BEGIN_ADR_BIT+count_bit)) return 0;
  return MARKER_OUTPERIMETR;
}//privateSDI2ColorSmallGetBit2(int adrReg)
