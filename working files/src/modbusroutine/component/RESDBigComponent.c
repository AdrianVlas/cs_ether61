#include "variables_external_m.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 4408
#define REGISTER_FOR_OBJ 6

int getRESDBigModbusRegister(int);//�������� ���������� ��������
int getRESDBigModbusBit(int);//�������� ���������� ����
int setRESDBigModbusRegister(int, int);//�������� ���������� ��������
int setRESDBigModbusBit(int, int);//�������� ���������� ����

void setRESDBigCountObject(int);//�������� �-�� �������
void preRESDBigReadAction();//action �� ������
void postRESDBigReadAction();//action ����� ������
void preRESDBigWriteAction();//action �� ������
void postRESDBigWriteAction();//action ����� ������
void loadRESDBigActualData();

int privateRESDBigGetReg2(int adrReg);

COMPONENT_OBJ *resdbigcomponent;

/**************************************/
//���������� ���������� ��������� ���������������� run\error
/**************************************/
void constructorRESDBigComponent(COMPONENT_OBJ *resdbigcomp)
{
  resdbigcomponent = resdbigcomp;

  resdbigcomponent->countObject = 1;//�-�� �������

  resdbigcomponent->getModbusRegister = getRESDBigModbusRegister;//�������� ���������� ��������
  resdbigcomponent->getModbusBit      = getRESDBigModbusBit;//�������� ���������� ����
  resdbigcomponent->setModbusRegister = setRESDBigModbusRegister;//�������� ���������� ��������
  resdbigcomponent->setModbusBit      = setRESDBigModbusBit;//�������� ���������� ����

  resdbigcomponent->setCountObject  = setRESDBigCountObject;//�������� �-�� �������
  resdbigcomponent->preReadAction   = preRESDBigReadAction;//action �� ������
  resdbigcomponent->postReadAction  = postRESDBigReadAction;//action ����� ������
  resdbigcomponent->preWriteAction  = preRESDBigWriteAction;//action �� ������
  resdbigcomponent->postWriteAction = postRESDBigWriteAction;//action ����� ������

  resdbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadRESDBigActualData() {
  //ActualData
  for(int i=0; i<100; i++) tempReadArray[i] = i;
}//loadActualData() 

int getRESDBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateRESDBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  if(resdbigcomponent->isActiveActualData) loadRESDBigActualData(); //ActualData
  resdbigcomponent->isActiveActualData = 0;

  superSetOperativMarker(resdbigcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDVModbusRegister(int adrReg)
int getRESDBigModbusBit(int adrBit)
{
  //�������� ���������� ��������
  superSetOperativMarker(resdbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDVModbusRegister(int adrReg)
int setRESDBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateRESDBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  superSetOperativMarker(resdbigcomponent, adrReg);
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
  }//switch
  return MARKER_OUTPERIMETR;
}//getDVModbusRegister(int adrReg)
int setRESDBigModbusBit(int adrBit, int )
{
  //�������� ���������� ��������
  superSetOperativMarker(resdbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDVModbusRegister(int adrReg)

void setRESDBigCountObject(int) {
//�������� �-�� �������
}//
void preRESDBigReadAction() {
//action �� ������
  resdbigcomponent->operativMarker[0] = -1;
  resdbigcomponent->operativMarker[1] = -1;//����������� ������
  resdbigcomponent->isActiveActualData = 1;
}//
void postRESDBigReadAction() {
//action ����� ������
  if(resdbigcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preRESDBigWriteAction() {
//action �� ������
  resdbigcomponent->operativMarker[0] = -1;
  resdbigcomponent->operativMarker[1] = -1;//����������� ������
  resdbigcomponent->isActiveActualData = 1;
}//
void postRESDBigWriteAction() {
//action ����� ������
  if(resdbigcomponent->operativMarker[0]<0) return;//�� ���� ������
  int offset = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
  int countRegister = resdbigcomponent->operativMarker[1]-resdbigcomponent->operativMarker[0]+1;
  if(resdbigcomponent->operativMarker[1]<0) countRegister = 1;
}//

int privateRESDBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)
