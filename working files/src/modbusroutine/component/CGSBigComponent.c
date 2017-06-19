#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 5954
//���� �-�� �������
#define REGISTER_FOR_OBJ 3

int privateCGSBigGetReg2(int adrReg);

int getCGSBigModbusRegister(int);//�������� ���������� ��������
int getCGSBigModbusBit(int);//�������� ���������� ����
int setCGSBigModbusRegister(int, int);// ��������
int setCGSBigModbusBit(int, int);// ����

void setCGSBigCountObject(int);//�������� �-�� �������
void preCGSBigReadAction(void);//action �� ������
void postCGSBigReadAction(void);//action ����� ������
void preCGSBigWriteAction(void);//action �� ������
void postCGSBigWriteAction(void);//action ����� ������
void loadCGSBigActualData(void);

COMPONENT_OBJ *cgsbigcomponent;
/**************************************/
//���������� ���������� ���
/**************************************/
void constructorCGSBigComponent(COMPONENT_OBJ *cgsbigcomp)
{
  cgsbigcomponent = cgsbigcomp;

  cgsbigcomponent->countObject = 4;//�-�� �������

  cgsbigcomponent->getModbusRegister = getCGSBigModbusRegister;//�������� ���������� ��������
  cgsbigcomponent->getModbusBit      = getCGSBigModbusBit;//�������� ���������� ����
  cgsbigcomponent->setModbusRegister = setCGSBigModbusRegister;//��������
  cgsbigcomponent->setModbusBit      = setCGSBigModbusBit;// ����

  cgsbigcomponent->setCountObject  = setCGSBigCountObject;//�������� �-�� �������
  cgsbigcomponent->preReadAction   = preCGSBigReadAction;//action �� ������
  cgsbigcomponent->postReadAction  = postCGSBigReadAction;//action ����� ������
  cgsbigcomponent->preWriteAction  = preCGSBigWriteAction;//action �� ������
  cgsbigcomponent->postWriteAction = postCGSBigWriteAction;//action ����� ������

  cgsbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadCGSBigActualData(void) {
  //ActualData
  for(int i=0; i<100; i++) tempReadArray[i] = i;
}//loadActualData() 

int getCGSBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateCGSBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  if(cgsbigcomponent->isActiveActualData) loadCGSBigActualData(); //ActualData
  cgsbigcomponent->isActiveActualData = 0;

  superSetOperativMarker(cgsbigcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDOUTBigModbusRegister(int adrReg)
int getCGSBigModbusBit(int adrBit)
{
  //�������� ���������� ��������
  superSetOperativMarker(cgsbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setCGSBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateCGSBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  superSetOperativMarker(cgsbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  switch((adrReg-BEGIN_ADR_REGISTER)%REGISTER_FOR_OBJ) {
   case 0:
    return dataReg;
   case 1:
    if(dataReg>500) return MARKER_ERRORDIAPAZON;
    if(dataReg<5)   return MARKER_ERRORDIAPAZON;
    return dataReg;
   case 2:
    if(dataReg>320) return MARKER_ERRORDIAPAZON;
   return dataReg;
  }//switch
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setCGSBigModbusBit(int adrBit, int x)
{
  UNUSED(x);
  //�������� ���������� ��������
  superSetOperativMarker(cgsbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void setCGSBigCountObject(int x) {
  UNUSED(x);
//�������� �-�� �������
}//
void preCGSBigReadAction(void) {
//action �� ������
  cgsbigcomponent->operativMarker[0] = -1;
  cgsbigcomponent->operativMarker[1] = -1;//����������� ������
  cgsbigcomponent->isActiveActualData = 1;
}//
void postCGSBigReadAction(void) {
//action ����� ������
  if(cgsbigcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preCGSBigWriteAction(void) {
//action �� ������
  cgsbigcomponent->operativMarker[0] = -1;
  cgsbigcomponent->operativMarker[1] = -1;//����������� ������
  cgsbigcomponent->isActiveActualData = 1;
}//
void postCGSBigWriteAction(void) {
//action ����� ������
  if(cgsbigcomponent->operativMarker[0]<0) return;//�� ���� ������
  int offset = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
  int countRegister = cgsbigcomponent->operativMarker[1]-cgsbigcomponent->operativMarker[0]+1;
  if(cgsbigcomponent->operativMarker[1]<0) countRegister = 1;
}//

int privateCGSBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = cgsbigcomponent->countObject*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)
