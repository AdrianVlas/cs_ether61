#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 600
//��������� bit � ����� ������
#define BEGIN_ADR_BIT  22000
#define REGISTER_FOR_OBJ 2
#define BIT_FOR_OBJ 18

int privateCommonSmallGetReg2(int adrReg);
int privateCommonSmallGetBit2(int adrBit);

int getCommonSmallModbusRegister(int);//�������� ���������� ��������
int getCommonSmallModbusBit(int);//�������� ���������� ����
int setCommonSmallModbusRegister(int, int);// ��������
int setCommonSmallModbusBit(int, int);// ����

void setCommonSmallCountObject(int);//�������� �-�� �������
void preCommonSmallReadAction(void);//action �� ������
void postCommonSmallReadAction(void);//action ����� ������
void preCommonSmallWriteAction(void);//action �� ������
void postCommonSmallWriteAction(void);//action ����� ������
void loadCommonSmallActualData(void);

COMPONENT_OBJ *commonsmallcomponent;

/**************************************/
//���������� ���������� �����
/**************************************/
void constructorCommonSmallComponent(COMPONENT_OBJ *commonsmallcomp)
{
  commonsmallcomponent = commonsmallcomp;

  commonsmallcomponent->countObject = 1;//�-�� �������

  commonsmallcomponent->getModbusRegister = getCommonSmallModbusRegister;//�������� ���������� ��������
  commonsmallcomponent->getModbusBit      = getCommonSmallModbusBit;//�������� ���������� ����
  commonsmallcomponent->setModbusRegister = setCommonSmallModbusRegister;// ��������
  commonsmallcomponent->setModbusBit      = setCommonSmallModbusBit;// ����

  commonsmallcomponent->setCountObject  = setCommonSmallCountObject;//�������� �-�� �������
  commonsmallcomponent->preReadAction   = preCommonSmallReadAction;//action �� ������
  commonsmallcomponent->postReadAction  = postCommonSmallReadAction;//action ����� ������
  commonsmallcomponent->preWriteAction  = preCommonSmallWriteAction;//action �� ������
  commonsmallcomponent->postWriteAction = postCommonSmallWriteAction;//action ����� ������

  commonsmallcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadCommonSmallActualData(void) {
  //ActualData
  for(int i=0; i<100; i++) tempReadArray[i] = i;
  /*
  1) ���� ����������� � ����� uint8_t fix_block_active_state[DIV_TO_HIGHER(FIX_BLOCK_SIGNALS_OUT, 8)];
  2) ������������ �� �������� ��� � �� ������� � � enum _FIX_BLOCK_output_signals
  */
}//loadActualData() 

int getCommonSmallModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateCommonSmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  if(commonsmallcomponent->isActiveActualData) loadCommonSmallActualData(); //ActualData
  commonsmallcomponent->isActiveActualData = 0;

  superSetOperativMarker(commonsmallcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDVModbusRegister(int adrReg)
int getCommonSmallModbusBit(int adrBit)
{
  //�������� ���������� bit
  if(privateCommonSmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  if(commonsmallcomponent->isActiveActualData) loadCommonSmallActualData();
  commonsmallcomponent->isActiveActualData = 0;

  superSetOperativMarker(commonsmallcomponent, adrBit);

  short tmp   = tempReadArray[(adrBit-BEGIN_ADR_BIT)/16];
  short maska = 1<<((adrBit-BEGIN_ADR_BIT)%16);
  if(tmp&maska) return 1;
  return 0;
}//getDVModbusRegister(int adrReg)
int setCommonSmallModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateCommonSmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  superSetOperativMarker(commonsmallcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  switch(adrReg-BEGIN_ADR_REGISTER) {
   case 0:
   return MARKER_ERRORPERIMETR;
   case 1:
   return dataReg;
  }//switch
  return MARKER_OUTPERIMETR;
}//getDVModbusRegister(int adrReg)
int setCommonSmallModbusBit(int adrBit, int dataBit)
{
  //�������� ���������� bit
  if(privateCommonSmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  superSetOperativMarker(commonsmallcomponent, adrBit);

  switch(adrBit-BEGIN_ADR_BIT) {
   case 16: 
  //    qDebug()<<"16="<<dataBit;
   return dataBit;
   case 17:
//      qDebug()<<"17="<<dataBit;
   return dataBit;
  }//switch
  return MARKER_ERRORPERIMETR;
}//setCommonSmallModbusBit(int adrBit, int adrData)

void setCommonSmallCountObject(int x) {
  UNUSED(x);
//�������� �-�� �������
}//
void preCommonSmallReadAction(void) {
//action �� ������
  commonsmallcomponent->operativMarker[0] = -1;
  commonsmallcomponent->operativMarker[1] = -1;
  commonsmallcomponent->isActiveActualData = 1;//����������� ������
}//
void postCommonSmallReadAction(void) {
//action ����� ������
  if(commonsmallcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preCommonSmallWriteAction(void) {
//action �� ������
  commonsmallcomponent->operativMarker[0] = -1;
  commonsmallcomponent->operativMarker[1] = -1;//����������� ������
  commonsmallcomponent->isActiveActualData = 1;
}//
void postCommonSmallWriteAction(void) {
//action ����� ������
}//

int privateCommonSmallGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)
int privateCommonSmallGetBit2(int adrBit)
{
  //��������� ������� ��������
  int count_bit = BIT_FOR_OBJ;
  if(adrBit>=BEGIN_ADR_BIT && adrBit<(BEGIN_ADR_BIT+count_bit)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)
