#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 5966
//���� �-�� �������
#define TOTAL_OBJ 128
#define REGISTER_FOR_OBJ 6

int privateMFTBigGetReg1(int adrReg);
int privateMFTBigGetReg2(int adrReg);

int getMFTBigModbusRegister(int);//�������� ���������� ��������
int getMFTBigModbusBit(int);//�������� ���������� ����
int setMFTBigModbusRegister(int, int);//�������� ���������� ��������
int setMFTBigModbusBit(int, int);//�������� ���������� ����

void setMFTBigCountObject(int);//�������� �-�� �������
void preMFTBigReadAction(void);//action �� ������
void postMFTBigReadAction(void);//action ����� ������
void preMFTBigWriteAction(void);//action �� ������
void postMFTBigWriteAction(void);//action ����� ������
void loadMFTBigActualData(void);

COMPONENT_OBJ *mftbigcomponent;

/**************************************/
//���������� ���������� ���
/**************************************/
void constructorMFTBigComponent(COMPONENT_OBJ *mftbigcomp)
{
  mftbigcomponent = mftbigcomp;

  mftbigcomponent->countObject = 128;//�-�� �������

  mftbigcomponent->getModbusRegister = getMFTBigModbusRegister;//�������� ���������� ��������
  mftbigcomponent->getModbusBit      = getMFTBigModbusBit;//�������� ���������� ����
  mftbigcomponent->setModbusRegister = setMFTBigModbusRegister;//�������� ���������� ��������
  mftbigcomponent->setModbusBit      = setMFTBigModbusBit;//�������� ���������� ����

  mftbigcomponent->setCountObject  = setMFTBigCountObject;//�������� �-�� �������
  mftbigcomponent->preReadAction   = preMFTBigReadAction;//action �� ������
  mftbigcomponent->postReadAction  = postMFTBigReadAction;//action ����� ������
  mftbigcomponent->preWriteAction  = preMFTBigWriteAction;//action �� ������
  mftbigcomponent->postWriteAction = postMFTBigWriteAction;//action ����� ������

  mftbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadMFTBigActualData(void) {
  //ActualData
  for(int i=0; i<100; i++) tempReadArray[i] = i;
}//loadActualData() 

int getMFTBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateMFTBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(privateMFTBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  if(mftbigcomponent->isActiveActualData) loadMFTBigActualData(); //ActualData
  mftbigcomponent->isActiveActualData = 0;

  superSetOperativMarker(mftbigcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDOUTBigModbusRegister(int adrReg)
int getMFTBigModbusBit(int adrBit)
{
  //�������� ���������� ��������
  superSetOperativMarker(mftbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setMFTBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateMFTBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(privateMFTBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  superSetOperativMarker(mftbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  switch((adrReg-BEGIN_ADR_REGISTER)%REGISTER_FOR_OBJ) {
   case 0:
    if(dataReg>60000) return MARKER_ERRORDIAPAZON;
    return dataReg;
   case 1:
    if(dataReg>60000) return MARKER_ERRORDIAPAZON;
    return dataReg;
   case 2:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
    return dataReg;
   case 3:
   return dataReg;
   case 4:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
    return dataReg;
   case 5:
   return dataReg;
  }//switch
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setMFTBigModbusBit(int adrBit, int x)
{
  UNUSED(x);
  //�������� ���������� ��������
  superSetOperativMarker(mftbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void setMFTBigCountObject(int cntObj) {
//�������� �-�� �������
  if(cntObj<0) return;
  if(cntObj>=TOTAL_OBJ) return;
  mftbigcomponent->countObject = cntObj;
}//
void preMFTBigReadAction(void) {
//action �� ������
  mftbigcomponent->operativMarker[0] = -1;
  mftbigcomponent->operativMarker[1] = -1;//����������� ������
  mftbigcomponent->isActiveActualData = 1;
}//
void postMFTBigReadAction(void) {
//action ����� ������
  if(mftbigcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preMFTBigWriteAction(void) {
//action �� ������
  mftbigcomponent->operativMarker[0] = -1;
  mftbigcomponent->operativMarker[1] = -1;//����������� ������
  mftbigcomponent->isActiveActualData = 1;
}//
void postMFTBigWriteAction(void) {
//action ����� ������
  if(mftbigcomponent->operativMarker[0]<0) return;//�� ���� ������
  int offset = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
  int countRegister = mftbigcomponent->operativMarker[1]-mftbigcomponent->operativMarker[0]+1;
  if(mftbigcomponent->operativMarker[1]<0) countRegister = 1;
}//

int privateMFTBigGetReg1(int adrReg)
{
  //��������� ���������� ��������
  int count_register = mftbigcomponent->countObject*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg1(int adrReg)
int privateMFTBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = TOTAL_OBJ*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)
