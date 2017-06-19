#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 500
//��������� bit � ����� ������
#define BEGIN_ADR_BIT 21000
#define BIT_FOR_OBJ 1
//���� �-�� �������
#define TOTAL_OBJ 128
int privateTSSmallGetReg1(int adrReg);
int privateTSSmallGetReg2(int adrReg);
int privateTSSmallGetBit1(int adrBit);
int privateTSSmallGetBit2(int adrBit);

int getTSSmallModbusRegister(int);//�������� ���������� ��������
int getTSSmallModbusBit(int);//�������� ���������� ����
int setTSSmallModbusRegister(int, int);//�������� ���������� ��������
int setTSSmallModbusBit(int, int);//�������� ���������� ����

void setTSSmallCountObject(int);//�������� �-�� �������
void preTSSmallReadAction(void);//action �� ������
void postTSSmallReadAction(void);//action ����� ������
void preTSSmallWriteAction(void);//action �� ������
void postTSSmallWriteAction(void);//action ����� ������
void loadTSSmallActualData(void);

COMPONENT_OBJ *tssmallcomponent;

/**************************************/
//���������� ���������� ����������������
/**************************************/
void constructorTSSmallComponent(COMPONENT_OBJ *tssmallcomp)
{
  tssmallcomponent = tssmallcomp;

  tssmallcomponent->countObject = 0;//�-�� �������

  tssmallcomponent->getModbusRegister = getTSSmallModbusRegister;//�������� ���������� ��������
  tssmallcomponent->getModbusBit      = getTSSmallModbusBit;//�������� ���������� ����
  tssmallcomponent->setModbusRegister = setTSSmallModbusRegister;//�������� ���������� ��������
  tssmallcomponent->setModbusBit      = setTSSmallModbusBit;//�������� ���������� ����

  tssmallcomponent->setCountObject  = setTSSmallCountObject;//�������� �-�� �������
  tssmallcomponent->preReadAction   = preTSSmallReadAction;//action �� ������
  tssmallcomponent->postReadAction  = postTSSmallReadAction;//action ����� ������
  tssmallcomponent->preWriteAction  = preTSSmallWriteAction;//action �� ������
  tssmallcomponent->postWriteAction = postTSSmallWriteAction;//action ����� ������

  tssmallcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadTSSmallActualData(void) {
  //ActualData
  for(int i=0; i<100; i++) tempReadArray[i] = i;
}//loadActualData() 

int getTSSmallModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateTSSmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(privateTSSmallGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  if(tssmallcomponent->isActiveActualData) loadTSSmallActualData(); //ActualData
  tssmallcomponent->isActiveActualData = 0;

  superSetOperativMarker(tssmallcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDVModbusRegister(int adrReg)
int getTSSmallModbusBit(int adrBit)
{
  //�������� ���������� bit
  if(privateTSSmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(privateTSSmallGetBit1(adrBit)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  if(tssmallcomponent->isActiveActualData) loadTSSmallActualData();
  tssmallcomponent->isActiveActualData = 0;

  superSetOperativMarker(tssmallcomponent, adrBit);

  short tmp   = tempReadArray[(adrBit-BEGIN_ADR_BIT)/16];
  short maska = 1<<((adrBit-BEGIN_ADR_BIT)%16);
  if(tmp&maska) return 1;
  return 0;
}//getDVModbusRegister(int adrReg)
int setTSSmallModbusRegister(int adrReg, int x)
{
  UNUSED(x);
  //�������� ���������� ��������
  superSetOperativMarker(tssmallcomponent, adrReg);
  return MARKER_OUTPERIMETR;
}//getDVModbusRegister(int adrReg)
int setTSSmallModbusBit(int adrBit, int x)
{
  UNUSED(x);
  //�������� ���������� bit
  superSetOperativMarker(tssmallcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDVModbusRegister(int adrReg)

void setTSSmallCountObject(int cntObj) {
//�������� �-�� �������
  if(cntObj<0) return;
  if(cntObj>=TOTAL_OBJ) return;
  tssmallcomponent->countObject = cntObj;
}//
void preTSSmallReadAction(void) {
//action �� ������
  tssmallcomponent->operativMarker[0] = -1;
  tssmallcomponent->operativMarker[1] = -1;//����������� ������
  tssmallcomponent->isActiveActualData = 1;
}//
void postTSSmallReadAction(void) {
//action ����� ������
  if(tssmallcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preTSSmallWriteAction(void) {
//action �� ������
  tssmallcomponent->operativMarker[0] = -1;
  tssmallcomponent->operativMarker[1] = -1;//����������� ������
  tssmallcomponent->isActiveActualData = 1;
}//
void postTSSmallWriteAction(void) {
//action ����� ������
}//

int privateTSSmallGetReg1(int adrReg)
{
  //��������� ���������� ��������
  int count_register = tssmallcomponent->countObject/16;
  if(tssmallcomponent->countObject%16) count_register++;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg1(int adrReg)

int privateTSSmallGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = TOTAL_OBJ/16;
  if(TOTAL_OBJ%16) count_register++;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)

int privateTSSmallGetBit1(int adrBit)
{
  //��������� ���������� ��������
  int count_bit = BIT_FOR_OBJ*tssmallcomponent->countObject;
  if(adrBit>=BEGIN_ADR_BIT && adrBit<(BEGIN_ADR_BIT+count_bit)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg1(int adrReg)

int privateTSSmallGetBit2(int adrBit)
{
  //��������� ������� ��������
  int count_bit = BIT_FOR_OBJ*TOTAL_OBJ;
  if(adrBit>=BEGIN_ADR_BIT && adrBit<(BEGIN_ADR_BIT+count_bit)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)
