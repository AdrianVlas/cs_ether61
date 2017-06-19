#include "variables_external_m.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 4418
//���� �-�� �������
#define TOTAL_OBJ 128
#define REGISTER_FOR_OBJ 2

int privateGIBigGetReg1(int adrReg);
int privateGIBigGetReg2(int adrReg);

int getGIBigModbusRegister(int);//�������� ���������� ��������
int getGIBigModbusBit(int);//�������� ���������� ����
int setGIBigModbusRegister(int, int);//�������� ���������� ��������
int setGIBigModbusBit(int, int);//�������� ���������� ����

void setGIBigCountObject(int);//�������� �-�� �������
void preGIBigReadAction();//action �� ������
void postGIBigReadAction();//action ����� ������
void preGIBigWriteAction();//action �� ������
void postGIBigWriteAction();//action ����� ������
void loadGIBigActualData();

COMPONENT_OBJ *gibigcomponent;

/**************************************/
//���������� ���������� ����������� ���������
/**************************************/
void constructorGIBigComponent(COMPONENT_OBJ *gibigcomp)
{
  gibigcomponent = gibigcomp;

  gibigcomponent->countObject = 0;//�-�� �������

  gibigcomponent->getModbusRegister = getGIBigModbusRegister;//�������� ���������� ��������
  gibigcomponent->getModbusBit      = getGIBigModbusBit;//�������� ���������� ����
  gibigcomponent->setModbusRegister = setGIBigModbusRegister;//�������� ���������� ��������
  gibigcomponent->setModbusBit      = setGIBigModbusBit;//�������� ���������� ����

  gibigcomponent->setCountObject  = setGIBigCountObject;//�������� �-�� �������
  gibigcomponent->preReadAction   = preGIBigReadAction;//action �� ������
  gibigcomponent->postReadAction  = postGIBigReadAction;//action ����� ������
  gibigcomponent->preWriteAction  = preGIBigWriteAction;//action �� ������
  gibigcomponent->postWriteAction = postGIBigWriteAction;//action ����� ������

  gibigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadGIBigActualData() {
  //ActualData
  for(int i=0; i<100; i++) tempReadArray[i] = i;
}//loadActualData() 

int getGIBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateGIBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(privateGIBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  if(gibigcomponent->isActiveActualData) loadGIBigActualData(); //ActualData
  gibigcomponent->isActiveActualData = 0;

  superSetOperativMarker(gibigcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDOUTBigModbusRegister(int adrReg)
int getGIBigModbusBit(int adrBit)
{
  //�������� ���������� ��������
  superSetOperativMarker(gibigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setGIBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateGIBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(privateGIBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  superSetOperativMarker(gibigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  switch((adrReg-BEGIN_ADR_REGISTER)%REGISTER_FOR_OBJ) {
   case 0:
   return dataReg;
   case 1:
    if(dataReg>100) return MARKER_ERRORDIAPAZON;
    if(dataReg<5) return MARKER_ERRORDIAPAZON;
    return dataReg;
  }//switch
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setGIBigModbusBit(int adrBit, int )
{
  //�������� ���������� ��������
  superSetOperativMarker(gibigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void setGIBigCountObject(int cntObj) {
//�������� �-�� �������
  if(cntObj<0) return;
  if(cntObj>=TOTAL_OBJ) return;
  gibigcomponent->countObject = cntObj;
}//
void preGIBigReadAction() {
//action �� ������
  gibigcomponent->operativMarker[0] = -1;
  gibigcomponent->operativMarker[1] = -1;//����������� ������
  gibigcomponent->isActiveActualData = 1;
}//
void postGIBigReadAction() {
//action ����� ������
  if(gibigcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preGIBigWriteAction() {
//action �� ������
  gibigcomponent->operativMarker[0] = -1;
  gibigcomponent->operativMarker[1] = -1;//����������� ������
  gibigcomponent->isActiveActualData = 1;
}//
void postGIBigWriteAction() {
//action ����� ������
  if(gibigcomponent->operativMarker[0]<0) return;//�� ���� ������
  int offset = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
  int countRegister = gibigcomponent->operativMarker[1]-gibigcomponent->operativMarker[0]+1;
  if(gibigcomponent->operativMarker[1]<0) countRegister = 1;
}//

int privateGIBigGetReg1(int adrReg)
{
  //��������� ���������� ��������
  int count_register = gibigcomponent->countObject*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg1(int adrReg)
int privateGIBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = TOTAL_OBJ*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)
