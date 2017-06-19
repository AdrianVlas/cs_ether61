
#include "variables_external_m.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 200
//��������� bit � ����� ������
#define BEGIN_ADR_BIT 18000
#define BIT_FOR_OBJ 1
//���� �-�� �������
#define TOTAL_OBJ 128
int privateDOUTSmallGetReg1(int adrReg);
int privateDOUTSmallGetReg2(int adrReg);
int privateDOUTSmallGetBit1(int adrBit);
int privateDOUTSmallGetBit2(int adrBit);

int getDOUTSmallModbusRegister(int);//�������� ���������� ��������
int getDOUTSmallModbusBit(int);//�������� ���������� ����
int setDOUTSmallModbusRegister(int, int);//�������� �������
int setDOUTSmallModbusBit(int, int);//�������� ���

void setDOUTSmallCountObject(int);//�������� �-�� �������
void preDOUTSmallReadAction();//action �� ������
void postDOUTSmallReadAction();//action ����� ������
void preDOUTSmallWriteAction();//action �� ������
void postDOUTSmallWriteAction();//action ����� ������
void loadDOUTSmallActualData();

COMPONENT_OBJ *doutsmallcomponent;

/**************************************/
//���������� ���������� ��
/**************************************/
void constructorDOUTSmallComponent(COMPONENT_OBJ *doutcomp)
{
  doutsmallcomponent = doutcomp;
  doutsmallcomponent->countObject = 0;//�-�� �������

  doutsmallcomponent->getModbusRegister = getDOUTSmallModbusRegister;//�������� ���������� ��������
  doutsmallcomponent->getModbusBit      = getDOUTSmallModbusBit;//�������� ���������� ����
  doutsmallcomponent->setModbusRegister = setDOUTSmallModbusRegister;// ��������
  doutsmallcomponent->setModbusBit      = setDOUTSmallModbusBit;// ����

  doutsmallcomponent->setCountObject  = setDOUTSmallCountObject;//�������� �-�� �������
  doutsmallcomponent->preReadAction   = preDOUTSmallReadAction;//action �� ������
  doutsmallcomponent->postReadAction  = postDOUTSmallReadAction;//action ����� ������
  doutsmallcomponent->preWriteAction  = preDOUTSmallWriteAction;//action �� ������
  doutsmallcomponent->postWriteAction = postDOUTSmallWriteAction;//action ����� ������
 
  doutsmallcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadDOUTSmallActualData() {
  //ActualData
  for(int i=0; i<100; i++) tempReadArray[i] = i;
  tempReadArray[0] = 55;//0xf2f2;
  tempReadArray[1] = 0;//xffff;
}//loadActualData() 

int getDOUTSmallModbusRegister(int adrReg) {
  //�������� ���������� ��������
  if(privateDOUTSmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(privateDOUTSmallGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  if(doutsmallcomponent->isActiveActualData) loadDOUTSmallActualData(); //ActualData
  doutsmallcomponent->isActiveActualData = 0;

  superSetOperativMarker(doutsmallcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDOUTModbusRegister(int adrReg)
int getDOUTSmallModbusBit(int adrBit) {
  //�������� ���������� bit
  if(privateDOUTSmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(privateDOUTSmallGetBit1(adrBit)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  if(doutsmallcomponent->isActiveActualData) loadDOUTSmallActualData();
  doutsmallcomponent->isActiveActualData = 0;

  superSetOperativMarker(doutsmallcomponent, adrBit);

  short tmp   = tempReadArray[(adrBit-BEGIN_ADR_BIT)/16];
  short maska = 1<<((adrBit-BEGIN_ADR_BIT)%16);
  if(tmp&maska) return 1;
  return 0;
}//getDOUTModbusBit(int adrReg)
int setDOUTSmallModbusRegister(int adrReg, int) {
  //�������� ���������� ��������
  superSetOperativMarker(doutsmallcomponent, adrReg);
  return MARKER_OUTPERIMETR;
}//getDOUTModbusRegister(int adrReg)
int setDOUTSmallModbusBit(int adrBit, int ) {
  //�������� ���������� ��������
  superSetOperativMarker(doutsmallcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTModbusRegister(int adrReg)

void setDOUTSmallCountObject(int cntObj) {
//�������� �-�� �������
  if(cntObj<0) return;
  if(cntObj>=TOTAL_OBJ) return;
  doutsmallcomponent->countObject = cntObj;
}//
void preDOUTSmallReadAction() {
//action �� ������
  doutsmallcomponent->operativMarker[0] = -1;
  doutsmallcomponent->operativMarker[1] = -1;//����������� ������
  doutsmallcomponent->isActiveActualData = 1;
}//
void postDOUTSmallReadAction() {
//action ����� ������
  if(doutsmallcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preDOUTSmallWriteAction() {
//action �� ������
  doutsmallcomponent->operativMarker[0] = -1;
  doutsmallcomponent->operativMarker[1] = -1;//����������� ������
  doutsmallcomponent->isActiveActualData = 1;
}//
void postDOUTSmallWriteAction() {
//action ����� ������
}//

int privateDOUTSmallGetReg1(int adrReg)
{
  //��������� ���������� ��������
  int count_register = doutsmallcomponent->countObject/16;
  if(doutsmallcomponent->countObject%16) count_register++;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg1(int adrReg)

int privateDOUTSmallGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = TOTAL_OBJ/16;
  if(TOTAL_OBJ%16) count_register++;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)

int privateDOUTSmallGetBit1(int adrBit)
{
  //��������� ���������� ��������
  int count_bit = BIT_FOR_OBJ*doutsmallcomponent->countObject;
  if(adrBit>=BEGIN_ADR_BIT && adrBit<(BEGIN_ADR_BIT+count_bit)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg1(int adrReg)

int privateDOUTSmallGetBit2(int adrBit)
{
  //��������� ������� ��������
  int count_bit = BIT_FOR_OBJ*TOTAL_OBJ;
  if(adrBit>=BEGIN_ADR_BIT && adrBit<(BEGIN_ADR_BIT+count_bit)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)
