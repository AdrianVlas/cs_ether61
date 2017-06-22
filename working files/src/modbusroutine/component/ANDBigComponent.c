#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 8267
//���� �-�� �������
#define TOTAL_OBJ 128
#define REGISTER_FOR_OBJ 16

int privateANDBigGetReg1(int adrReg);
int privateANDBigGetReg2(int adrReg);

int getANDBigModbusRegister(int);//�������� ���������� ��������
int getANDBigModbusBit(int);//�������� ���������� ����
int setANDBigModbusRegister(int, int);// �������
int setANDBigModbusBit(int, int);// ���

void setANDBigCountObject(void);
void preANDBigReadAction(void);//action �� ������
void postANDBigReadAction(void);//action ����� ������
void preANDBigWriteAction(void);//action �� ������
void postANDBigWriteAction(void);//action ����� ������
void loadANDBigActualData(void);

COMPONENT_OBJ *andbigcomponent;

/**************************************/
//���������� ���������� AND
/**************************************/
void constructorANDBigComponent(COMPONENT_OBJ *andbigcomp)
{
  andbigcomponent = andbigcomp;

  andbigcomponent->countObject = 0;//�-�� �������

  andbigcomponent->getModbusRegister = getANDBigModbusRegister;//�������� ���������� ��������
  andbigcomponent->getModbusBit      = getANDBigModbusBit;//�������� ���������� ����
  andbigcomponent->setModbusRegister = setANDBigModbusRegister;//�������� �������
  andbigcomponent->setModbusBit      = setANDBigModbusBit;//�������� ���

  andbigcomponent->preReadAction   = preANDBigReadAction;//action �� ������
  andbigcomponent->postReadAction  = postANDBigReadAction;//action ����� ������
  andbigcomponent->preWriteAction  = preANDBigWriteAction;//action �� ������
  andbigcomponent->postWriteAction = postANDBigWriteAction;//action ����� ������

  andbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadANDBigActualData(void) {
  //ActualData
  for(int i=0; i<100; i++) tempReadArray[i] = i;
  /*
  ��� ��� ���������, �� � �� ���� ���������� ���� � ������� (��. ����� � ������ ����) ���� ������ ����� ����������� ��� 0-� ���� �����, � �����(id;n; out) ����� � ������� ���������
  */
}//loadActualData() 

int getANDBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateANDBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(privateANDBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  if(andbigcomponent->isActiveActualData) loadANDBigActualData(); //ActualData
  andbigcomponent->isActiveActualData = 0;

  superSetOperativMarker(andbigcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDOUTBigModbusRegister(int adrReg)
int getANDBigModbusBit(int adrBit)
{
  //�������� ���������� ����
  superSetOperativMarker(andbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setANDBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateANDBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(privateANDBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  superSetOperativMarker(andbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  switch((adrReg-BEGIN_ADR_REGISTER)%REGISTER_FOR_OBJ) {
   case 0:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
    return dataReg;
   case 1:
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
   case 6:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
    return dataReg;
   case 7:
   return dataReg;
   case 8:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
    return dataReg;
   case 9:
   return dataReg;
   case 10:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
    return dataReg;
   case 11:
   return dataReg;
   case 12:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
    return dataReg;
   case 13:
   return dataReg;
   case 14:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
    return dataReg;
   case 15:
   return dataReg;
  }//switch
  return MARKER_OUTPERIMETR;
}//setDOUTBigModbusRegister(int adrReg)
int setANDBigModbusBit(int adrBit, int x)
{
  UNUSED(x);
  //�������� ���������� ����
  superSetOperativMarker(andbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//setDOUTBigModbusRegister(int adrReg)

void setANDBigCountObject(void) {
//�������� �-�� �������
}//

void preANDBigReadAction(void) {
//action �� ������
  andbigcomponent->operativMarker[0] = -1;
  andbigcomponent->operativMarker[1] = -1;//����������� ������
  andbigcomponent->isActiveActualData = 1;
}//preANDBigReadAction() 

void postANDBigReadAction(void) {
//action ����� ������
  if(andbigcomponent->operativMarker[0]<0) return;//�� ���� ������
}//postANDBigReadAction() 

void preANDBigWriteAction(void) {
//action �� ������
  andbigcomponent->operativMarker[0] = -1;
  andbigcomponent->operativMarker[1] = -1;//����������� ������
  andbigcomponent->isActiveActualData = 1;
}//preANDBigWriteAction() 

void postANDBigWriteAction(void) {
//action ����� ������
  if(andbigcomponent->operativMarker[0]<0) return;//�� ���� ������
//  int offset = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
  //int countRegister = andbigcomponent->operativMarker[1]-andbigcomponent->operativMarker[0]+1;
//  if(andbigcomponent->operativMarker[1]<0) countRegister = 1;
}//postANDBigWriteAction() 

int privateANDBigGetReg1(int adrReg)
{
  //��������� ���������� ��������
  int count_register = andbigcomponent->countObject*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg1(int adrReg)
int privateANDBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = TOTAL_OBJ*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)
