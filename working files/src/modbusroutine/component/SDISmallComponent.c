#include "variables_external_m.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 400
//��������� bit � ����� ������
#define BEGIN_ADR_BIT 20000
#define BIT_FOR_OBJ 1
//���� �-�� �������
#define TOTAL_OBJ 128
int privateSDISmallGetReg1(int adrReg);
int privateSDISmallGetReg2(int adrReg);
int privateSDISmallGetBit1(int adrBit);
int privateSDISmallGetBit2(int adrBit);

int getSDISmallModbusRegister(int);//�������� ���������� ��������
int getSDISmallModbusBit(int);//�������� ���������� ����
int setSDISmallModbusRegister(int, int);//�������� ���������� ��������
int setSDISmallModbusBit(int, int);//�������� ���������� ����

void setSDISmallCountObject(int);//�������� �-�� �������
void preSDISmallReadAction();//action �� ������
void postSDISmallReadAction();//action ����� ������
void preSDISmallWriteAction();//action �� ������
void postSDISmallWriteAction();//action ����� ������
void loadSDISmallActualData();

COMPONENT_OBJ *sdismallcomponent;

/**************************************/
//���������� ���������� ����������������
/**************************************/
void constructorSDISmallComponent(COMPONENT_OBJ *sdismallcomp)
{
  sdismallcomponent = sdismallcomp;

  sdismallcomponent->countObject = 0;//�-�� �������

  sdismallcomponent->getModbusRegister = getSDISmallModbusRegister;//�������� ���������� ��������
  sdismallcomponent->getModbusBit      = getSDISmallModbusBit;//�������� ���������� ����
  sdismallcomponent->setModbusRegister = setSDISmallModbusRegister;//�������� ���������� ��������
  sdismallcomponent->setModbusBit      = setSDISmallModbusBit;//�������� ���������� ����

  sdismallcomponent->setCountObject  = setSDISmallCountObject;//�������� �-�� �������
  sdismallcomponent->preReadAction   = preSDISmallReadAction;//action �� ������
  sdismallcomponent->postReadAction  = postSDISmallReadAction;//action ����� ������
  sdismallcomponent->preWriteAction  = preSDISmallWriteAction;//action �� ������
  sdismallcomponent->postWriteAction = postSDISmallWriteAction;//action ����� ������

  sdismallcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadSDISmallActualData() {
  //ActualData
  for(int i=0; i<100; i++) tempReadArray[i] = i;
}//loadActualData() 

int getSDISmallModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateSDISmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(privateSDISmallGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  if(sdismallcomponent->isActiveActualData) loadSDISmallActualData(); //ActualData
  sdismallcomponent->isActiveActualData = 0;

  superSetOperativMarker(sdismallcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDVModbusRegister(int adrReg)
int getSDISmallModbusBit(int adrBit)
{
  //�������� ���������� ��������
  if(privateSDISmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(privateSDISmallGetBit1(adrBit)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  if(sdismallcomponent->isActiveActualData) loadSDISmallActualData();
  sdismallcomponent->isActiveActualData = 0;

  superSetOperativMarker(sdismallcomponent, adrBit);

  short tmp   = tempReadArray[(adrBit-BEGIN_ADR_BIT)/16];
  short maska = 1<<((adrBit-BEGIN_ADR_BIT)%16);
  if(tmp&maska) return 1;
  return 0;
}//getDVModbusBit(int adrReg)
int setSDISmallModbusRegister(int adrReg, int)
{
  //�������� ���������� ��������
  superSetOperativMarker(sdismallcomponent, adrReg);
  return MARKER_OUTPERIMETR;
}//getDVModbusRegister(int adrReg)
int setSDISmallModbusBit(int adrBit, int )
{
  //�������� ���������� ��������
  superSetOperativMarker(sdismallcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDVModbusRegister(int adrReg)

void setSDISmallCountObject(int cntObj) {
//�������� �-�� �������
  if(cntObj<0) return;
  if(cntObj>=TOTAL_OBJ) return;
  sdismallcomponent->countObject = cntObj;
}//
void preSDISmallReadAction() {
//action �� ������
  sdismallcomponent->operativMarker[0] = -1;
  sdismallcomponent->operativMarker[1] = -1;//����������� ������
  sdismallcomponent->isActiveActualData = 1;
}//
void postSDISmallReadAction() {
//action ����� ������
  if(sdismallcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preSDISmallWriteAction() {
//action �� ������
  sdismallcomponent->operativMarker[0] = -1;
  sdismallcomponent->operativMarker[1] = -1;//����������� ������
  sdismallcomponent->isActiveActualData = 1;
}//
void postSDISmallWriteAction() {
//action ����� ������
}//

int privateSDISmallGetReg1(int adrReg)
{
  //��������� ���������� ��������
  int count_register = sdismallcomponent->countObject/16;
  if(sdismallcomponent->countObject%16) count_register++;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg1(int adrReg)

int privateSDISmallGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = TOTAL_OBJ/16;
  if(TOTAL_OBJ%16) count_register++;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)

int privateSDISmallGetBit1(int adrBit)
{
  //��������� ���������� ��������
  int count_bit = BIT_FOR_OBJ*sdismallcomponent->countObject;
  if(adrBit>=BEGIN_ADR_BIT && adrBit<(BEGIN_ADR_BIT+count_bit)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg1(int adrReg)

int privateSDISmallGetBit2(int adrBit)
{
  //��������� ������� ��������
  int count_bit = BIT_FOR_OBJ*TOTAL_OBJ;
  if(adrBit>=BEGIN_ADR_BIT && adrBit<(BEGIN_ADR_BIT+count_bit)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)
