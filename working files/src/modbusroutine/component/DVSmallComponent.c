#include "variables_external_m.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 300
//��������� bit � ����� ������
#define BEGIN_ADR_BIT 19000
#define BIT_FOR_OBJ 1
//���� �-�� �������
#define TOTAL_OBJ 128
int privateDVSmallGetReg1(int adrReg);
int privateDVSmallGetReg2(int adrReg);
int privateDVSmallGetBit1(int adrBit);
int privateDVSmallGetBit2(int adrBit);

int getDVSmallModbusRegister(int);//�������� ���������� ��������
int getDVSmallModbusBit(int);//�������� ���������� ����
int setDVSmallModbusRegister(int, int);//�������� ���������� ��������
int setDVSmallModbusBit(int, int);//�������� ���������� ����

void setDVSmallCountObject(int);//�������� �-�� �������
void preDVSmallReadAction();//action �� ������
void postDVSmallReadAction();//action ����� ������
void preDVSmallWriteAction();//action �� ������
void postDVSmallWriteAction();//action ����� ������
void loadDVSmallActualData();

COMPONENT_OBJ *dvsmallcomponent;

/**************************************/
//���������� ���������� ��
/**************************************/
void constructorDVSmallComponent(COMPONENT_OBJ *dvsmallcomp)
{
  dvsmallcomponent = dvsmallcomp;

  dvsmallcomponent->countObject = 0;//�-�� �������

  dvsmallcomponent->getModbusRegister = getDVSmallModbusRegister;//�������� ���������� ��������
  dvsmallcomponent->getModbusBit      = getDVSmallModbusBit;//�������� ���������� ����
  dvsmallcomponent->setModbusRegister = setDVSmallModbusRegister;//�������� ���������� ��������
  dvsmallcomponent->setModbusBit      = setDVSmallModbusBit;//�������� ���������� ����

  dvsmallcomponent->setCountObject  = setDVSmallCountObject;//�������� �-�� �������
  dvsmallcomponent->preReadAction   = preDVSmallReadAction;//action �� ������
  dvsmallcomponent->postReadAction  = postDVSmallReadAction;//action ����� ������
  dvsmallcomponent->preWriteAction  = preDVSmallWriteAction;//action �� ������
  dvsmallcomponent->postWriteAction = postDVSmallWriteAction;//action ����� ������

  dvsmallcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadDVSmallActualData() {
  //ActualData
  for(int i=0; i<100; i++) tempReadArray[i] = i;
}//loadActualData() 

int getDVSmallModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateDVSmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(privateDVSmallGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  if(dvsmallcomponent->isActiveActualData) loadDVSmallActualData(); //ActualData
  dvsmallcomponent->isActiveActualData = 0;

  superSetOperativMarker(dvsmallcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDVModbusRegister(int adrReg)
int getDVSmallModbusBit(int adrBit)
{
  //�������� ���������� bit
  if(privateDVSmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(privateDVSmallGetBit1(adrBit)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  if(dvsmallcomponent->isActiveActualData) loadDVSmallActualData();
  dvsmallcomponent->isActiveActualData = 0;

  superSetOperativMarker(dvsmallcomponent, adrBit);

  short tmp   = tempReadArray[(adrBit-BEGIN_ADR_BIT)/16];
  short maska = 1<<((adrBit-BEGIN_ADR_BIT)%16);
  if(tmp&maska) return 1;
  return 0;
}//getDVModbusBit(int adrReg)
int setDVSmallModbusRegister(int adrReg, int )
{
  //�������� ���������� ��������
  superSetOperativMarker(dvsmallcomponent, adrReg);
  return MARKER_OUTPERIMETR;
}//getDVModbusRegister(int adrReg)
int setDVSmallModbusBit(int adrBit, int )
{
  //�������� ���������� ��������
  superSetOperativMarker(dvsmallcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDVModbusRegister(int adrReg)

void setDVSmallCountObject(int cntObj) {
//�������� �-�� �������
  if(cntObj<0) return;
  if(cntObj>=TOTAL_OBJ) return;
  dvsmallcomponent->countObject = cntObj;
}//
void preDVSmallReadAction() {
//action �� ������
  dvsmallcomponent->operativMarker[0] = -1;
  dvsmallcomponent->operativMarker[1] = -1;//����������� ������
  dvsmallcomponent->isActiveActualData = 1;
}//
void postDVSmallReadAction() {
//action ����� ������
  if(dvsmallcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preDVSmallWriteAction() {
//action �� ������
  dvsmallcomponent->operativMarker[0] = -1;
  dvsmallcomponent->operativMarker[1] = -1;//����������� ������
  dvsmallcomponent->isActiveActualData = 1;
}//
void postDVSmallWriteAction() {
//action ����� ������
}//

int privateDVSmallGetReg1(int adrReg)
{
  //��������� ���������� ��������
  int count_register = dvsmallcomponent->countObject/16;
  if(dvsmallcomponent->countObject%16) count_register++;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg1(int adrReg)

int privateDVSmallGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = TOTAL_OBJ/16;
  if(TOTAL_OBJ%16) count_register++;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)

int privateDVSmallGetBit1(int adrBit)
{
  //��������� ���������� ��������
  int count_bit = BIT_FOR_OBJ*dvsmallcomponent->countObject;
  if(adrBit>=BEGIN_ADR_BIT && adrBit<(BEGIN_ADR_BIT+count_bit)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg1(int adrReg)

int privateDVSmallGetBit2(int adrBit)
{
  //��������� ������� ��������
  int count_bit = BIT_FOR_OBJ*TOTAL_OBJ;
  if(adrBit>=BEGIN_ADR_BIT && adrBit<(BEGIN_ADR_BIT+count_bit)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)
