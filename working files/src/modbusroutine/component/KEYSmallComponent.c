#include "variables_external_m.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 308
//��������� bit � ����� ������
#define BEGIN_ADR_BIT 19128
//���� �-�� �������
#define REGISTER_FOR_OBJ 1
#define BIT_FOR_OBJ 3

int privateKEYSmallGetReg2(int adrReg);
int privateKEYSmallGetBit2(int adrReg);

int getKEYSmallModbusRegister(int);//�������� ���������� ��������
int getKEYSmallModbusBit(int);//�������� ���������� ����
int setKEYSmallModbusRegister(int, int);//�������� ���������� ��������
int setKEYSmallModbusBit(int, int);//�������� ���������� ����

void setKEYSmallCountObject(int);//�������� �-�� �������
void preKEYSmallReadAction();//action �� ������
void postKEYSmallReadAction();//action ����� ������
void preKEYSmallWriteAction();//action �� ������
void postKEYSmallWriteAction();//action ����� ������
void loadKEYSmallActualData();

COMPONENT_OBJ *keysmallcomponent;

/**************************************/
//���������� ���������� ������������ ���������������� ���������
/**************************************/
void constructorKEYSmallComponent(COMPONENT_OBJ *keysmallcomp)
{
  keysmallcomponent = keysmallcomp;

  keysmallcomponent->countObject = 1;//�-�� �������

  keysmallcomponent->getModbusRegister = getKEYSmallModbusRegister;//�������� ���������� ��������
  keysmallcomponent->getModbusBit      = getKEYSmallModbusBit;//�������� ���������� ����
  keysmallcomponent->setModbusRegister = setKEYSmallModbusRegister;//�������� ���������� ��������
  keysmallcomponent->setModbusBit      = setKEYSmallModbusBit;//�������� ���������� ����

  keysmallcomponent->setCountObject  = setKEYSmallCountObject;//�������� �-�� �������
  keysmallcomponent->preReadAction   = preKEYSmallReadAction;//action �� ������
  keysmallcomponent->postReadAction  = postKEYSmallReadAction;//action ����� ������
  keysmallcomponent->preWriteAction  = preKEYSmallWriteAction;//action �� ������
  keysmallcomponent->postWriteAction = postKEYSmallWriteAction;//action ����� ������

  keysmallcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadKEYSmallActualData() {
  //ActualData
  for(int i=0; i<100; i++) tempReadArray[i] = i;
}//loadActualData() 

int getKEYSmallModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateKEYSmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  if(keysmallcomponent->isActiveActualData) loadKEYSmallActualData(); //ActualData
  keysmallcomponent->isActiveActualData = 0;

  superSetOperativMarker(keysmallcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDOUTModbusRegister(int adrReg)
int getKEYSmallModbusBit(int adrBit)
{
  //�������� ���������� bit
  if(privateKEYSmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  if(keysmallcomponent->isActiveActualData) loadKEYSmallActualData();
  keysmallcomponent->isActiveActualData = 0;

  superSetOperativMarker(keysmallcomponent, adrBit);

  short tmp   = tempReadArray[(adrBit-BEGIN_ADR_BIT)/16];
  short maska = 1<<((adrBit-BEGIN_ADR_BIT)%16);
  if(tmp&maska) return 1;
  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int setKEYSmallModbusRegister(int adrReg, int)
{
  //�������� ���������� ��������
  superSetOperativMarker(keysmallcomponent, adrReg);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setKEYSmallModbusBit(int adrBit, int )
{
  //�������� ���������� ��������
  superSetOperativMarker(keysmallcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void setKEYSmallCountObject(int) {
//�������� �-�� �������
}//
void preKEYSmallReadAction() {
//action �� ������
  keysmallcomponent->operativMarker[0] = -1;
  keysmallcomponent->operativMarker[1] = -1;//����������� ������
  keysmallcomponent->isActiveActualData = 1;
}//
void postKEYSmallReadAction() {
//action ����� ������
  if(keysmallcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preKEYSmallWriteAction() {
//action �� ������
  keysmallcomponent->operativMarker[0] = -1;
  keysmallcomponent->operativMarker[1] = -1;//����������� ������
  keysmallcomponent->isActiveActualData = 1;
}//
void postKEYSmallWriteAction() {
//action ����� ������
}//

int privateKEYSmallGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateKEYSmallGetReg2(int adrReg)

int privateKEYSmallGetBit2(int adrBit)
{
  //��������� ������� ��������
  int count_bit = BIT_FOR_OBJ;
  if(adrBit>=BEGIN_ADR_BIT && adrBit<(BEGIN_ADR_BIT+count_bit)) return 0;
  return MARKER_OUTPERIMETR;
}//privateKEYSmallGetReg2(int adrReg)
