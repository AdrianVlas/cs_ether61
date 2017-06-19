#include "variables_external_m.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 9299
//���� �-�� �������
#define TOTAL_OBJ 128
#define REGISTER_FOR_OBJ 16

int privateORBigGetReg1(int adrReg);
int privateORBigGetReg2(int adrReg);

int getORBigModbusRegister(int);//�������� ���������� ��������
int getORBigModbusBit(int);//�������� ���������� ����
int setORBigModbusRegister(int, int);//�������� ���������� ��������
int setORBigModbusBit(int, int);//�������� ���������� ����

void setORBigCountObject(int);//�������� �-�� �������
void preORBigReadAction();//action �� ������
void postORBigReadAction();//action ����� ������
void preORBigWriteAction();//action �� ������
void postORBigWriteAction();//action ����� ������
void loadORBigActualData();

COMPONENT_OBJ *orbigcomponent;

/**************************************/
//���������� ���������� OR
/**************************************/
void constructorORBigComponent(COMPONENT_OBJ *orbigcomp)
{
  orbigcomponent = orbigcomp;

  orbigcomponent->countObject = 0;//�-�� �������

  orbigcomponent->getModbusRegister = getORBigModbusRegister;//�������� ���������� ��������
  orbigcomponent->getModbusBit      = getORBigModbusBit;//�������� ���������� ����
  orbigcomponent->setModbusRegister = setORBigModbusRegister;//�������� ���������� ��������
  orbigcomponent->setModbusBit      = setORBigModbusBit;//�������� ���������� ����

  orbigcomponent->setCountObject  = setORBigCountObject;//�������� �-�� �������
  orbigcomponent->preReadAction   = preORBigReadAction;//action �� ������
  orbigcomponent->postReadAction  = postORBigReadAction;//action ����� ������
  orbigcomponent->preWriteAction  = preORBigWriteAction;//action �� ������
  orbigcomponent->postWriteAction = postORBigWriteAction;//action ����� ������

  orbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadORBigActualData() {
  //ActualData
  for(int i=0; i<100; i++) tempReadArray[i] = i;
}//loadActualData() 

int getORBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateORBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(privateORBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  if(orbigcomponent->isActiveActualData) loadORBigActualData(); //ActualData
  orbigcomponent->isActiveActualData = 0;

  superSetOperativMarker(orbigcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDOUTBigModbusRegister(int adrReg)
int getORBigModbusBit(int adrReg)
{
  //�������� ���������� ��������
  superSetOperativMarker(orbigcomponent, adrReg);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setORBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateORBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(privateORBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  superSetOperativMarker(orbigcomponent, adrReg);
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
}//getDOUTBigModbusRegister(int adrReg)
int setORBigModbusBit(int adrBit, int )
{
  //�������� ���������� ��������
  superSetOperativMarker(orbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void setORBigCountObject(int cntObj) {
//�������� �-�� �������
  if(cntObj<0) return;
  if(cntObj>=TOTAL_OBJ) return;
  orbigcomponent->countObject = cntObj;
}//
void preORBigReadAction() {
//action �� ������
  orbigcomponent->operativMarker[0] = -1;
  orbigcomponent->operativMarker[1] = -1;//����������� ������
  orbigcomponent->isActiveActualData = 1;
}//
void postORBigReadAction() {
//action ����� ������
  if(orbigcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preORBigWriteAction() {
//action �� ������
  orbigcomponent->operativMarker[0] = -1;
  orbigcomponent->operativMarker[1] = -1;//����������� ������
  orbigcomponent->isActiveActualData = 1;
}//
void postORBigWriteAction() {
//action ����� ������
  if(orbigcomponent->operativMarker[0]<0) return;//�� ���� ������
  int offset = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
  int countRegister = orbigcomponent->operativMarker[1]-orbigcomponent->operativMarker[0]+1;
  if(orbigcomponent->operativMarker[1]<0) countRegister = 1;
}//

int privateORBigGetReg1(int adrReg)
{
  //��������� ���������� ��������
  int count_register = orbigcomponent->countObject*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg1(int adrReg)
int privateORBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = TOTAL_OBJ*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)
