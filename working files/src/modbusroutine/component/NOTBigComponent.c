#include "variables_external_m.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 10589
//���� �-�� �������
#define TOTAL_OBJ 128
#define REGISTER_FOR_OBJ 2

int privateNOTBigGetReg1(int adrReg);
int privateNOTBigGetReg2(int adrReg);

int getNOTBigModbusRegister(int);//�������� ���������� ��������
int getNOTBigModbusBit(int);//�������� ���������� ����
int setNOTBigModbusRegister(int, int);//�������� ���������� ��������
int setNOTBigModbusBit(int, int);//�������� ���������� ����

void setNOTBigCountObject(int);//�������� �-�� �������
void preNOTBigReadAction();//action �� ������
void postNOTBigReadAction();//action ����� ������
void preNOTBigWriteAction();//action �� ������
void postNOTBigWriteAction();//action ����� ������
void loadNOTBigActualData();

COMPONENT_OBJ *notbigcomponent;

/**************************************/
//���������� ���������� NOT
/**************************************/
void constructorNOTBigComponent(COMPONENT_OBJ *notbigcomp)
{
  notbigcomponent = notbigcomp;

  notbigcomponent->countObject = 0;//�-�� �������

  notbigcomponent->getModbusRegister = getNOTBigModbusRegister;//�������� ���������� ��������
  notbigcomponent->getModbusBit      = getNOTBigModbusBit;//�������� ���������� ����
  notbigcomponent->setModbusRegister = setNOTBigModbusRegister;//�������� ���������� ��������
  notbigcomponent->setModbusBit      = setNOTBigModbusBit;//�������� ���������� ����

  notbigcomponent->setCountObject  = setNOTBigCountObject;//�������� �-�� �������
  notbigcomponent->preReadAction   = preNOTBigReadAction;//action �� ������
  notbigcomponent->postReadAction  = postNOTBigReadAction;//action ����� ������
  notbigcomponent->preWriteAction  = preNOTBigWriteAction;//action �� ������
  notbigcomponent->postWriteAction = postNOTBigWriteAction;//action ����� ������

  notbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadNOTBigActualData() {
  //ActualData
  for(int i=0; i<100; i++) tempReadArray[i] = i;
}//loadActualData() 

int getNOTBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateNOTBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(privateNOTBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  if(notbigcomponent->isActiveActualData) loadNOTBigActualData(); //ActualData
  notbigcomponent->isActiveActualData = 0;

  superSetOperativMarker(notbigcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDOUTBigModbusRegister(int adrReg)
int getNOTBigModbusBit(int adrBit)
{
  //�������� ���������� ��������
  superSetOperativMarker(notbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setNOTBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateNOTBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(privateNOTBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  superSetOperativMarker(notbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  switch((adrReg-BEGIN_ADR_REGISTER)%REGISTER_FOR_OBJ) {
   case 0:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
    return dataReg;
   case 1:
   return dataReg;
  }//switch
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setNOTBigModbusBit(int adrBit, int )
{
  //�������� ���������� ��������
  superSetOperativMarker(notbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void setNOTBigCountObject(int cntObj) {
//�������� �-�� �������
  if(cntObj<0) return;
  if(cntObj>=TOTAL_OBJ) return;
  notbigcomponent->countObject = cntObj;
}//
void preNOTBigReadAction() {
//action �� ������
  notbigcomponent->operativMarker[0] = -1;
  notbigcomponent->operativMarker[1] = -1;//����������� ������
  notbigcomponent->isActiveActualData = 1;
}//
void postNOTBigReadAction() {
//action ����� ������
  if(notbigcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preNOTBigWriteAction() {
//action �� ������
  notbigcomponent->operativMarker[0] = -1;
  notbigcomponent->operativMarker[1] = -1;//����������� ������
  notbigcomponent->isActiveActualData = 1;
}//
void postNOTBigWriteAction() {
//action ����� ������
  if(notbigcomponent->operativMarker[0]<0) return;//�� ���� ������
  int offset = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
  int countRegister = notbigcomponent->operativMarker[1]-notbigcomponent->operativMarker[0]+1;
  if(notbigcomponent->operativMarker[1]<0) countRegister = 1;
}//

int privateNOTBigGetReg1(int adrReg)
{
  //��������� ���������� ��������
  int count_register = notbigcomponent->countObject*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg1(int adrReg)
int privateNOTBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = TOTAL_OBJ*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)
