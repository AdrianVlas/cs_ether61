#include "variables_external_m.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 7753
//���� �-�� �������
#define TOTAL_OBJ 128
#define REGISTER_FOR_OBJ 4

int privateTSBigGetReg1(int adrReg);
int privateTSBigGetReg2(int adrReg);

int getTSBigModbusRegister(int);//�������� ���������� ��������
int getTSBigModbusBit(int);//�������� ���������� ����
int setTSBigModbusRegister(int, int);//�������� ���������� ��������
int setTSBigModbusBit(int, int);//�������� ���������� ����

void setTSBigCountObject(int);//�������� �-�� �������
void preTSBigReadAction();//action �� ������
void postTSBigReadAction();//action ����� ������
void preTSBigWriteAction();//action �� ������
void postTSBigWriteAction();//action ����� ������
void loadTSBigActualData();

COMPONENT_OBJ *tsbigcomponent;

/**************************************/
//���������� ���������� ����������������
/**************************************/
void constructorTSBigComponent(COMPONENT_OBJ *tsbigcomp)
{
  tsbigcomponent = tsbigcomp;

  tsbigcomponent->countObject = 0;//�-�� �������

  tsbigcomponent->getModbusRegister = getTSBigModbusRegister;//�������� ���������� ��������
  tsbigcomponent->getModbusBit      = getTSBigModbusBit;//�������� ���������� ����
  tsbigcomponent->setModbusRegister = setTSBigModbusRegister;//�������� ���������� ��������
  tsbigcomponent->setModbusBit      = setTSBigModbusBit;//�������� ���������� ����

  tsbigcomponent->setCountObject  = setTSBigCountObject;//�������� �-�� �������
  tsbigcomponent->preReadAction   = preTSBigReadAction;//action �� ������
  tsbigcomponent->postReadAction  = postTSBigReadAction;//action ����� ������
  tsbigcomponent->preWriteAction  = preTSBigWriteAction;//action �� ������
  tsbigcomponent->postWriteAction = postTSBigWriteAction;//action ����� ������

  tsbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadTSBigActualData() {
  //ActualData
  for(int i=0; i<100; i++) tempReadArray[i] = i;
}//loadActualData() 

int getTSBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateTSBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(privateTSBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  if(tsbigcomponent->isActiveActualData) loadTSBigActualData(); //ActualData
  tsbigcomponent->isActiveActualData = 0;

  superSetOperativMarker(tsbigcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDOUTBigModbusRegister(int adrReg)
int getTSBigModbusBit(int adrBit)
{
  //�������� ���������� ��������
  superSetOperativMarker(tsbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setTSBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateTSBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(privateTSBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  superSetOperativMarker(tsbigcomponent, adrReg);
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
  }//switch
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setTSBigModbusBit(int adrBit, int )
{
  //�������� ���������� ��������
  superSetOperativMarker(tsbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void setTSBigCountObject(int cntObj) {
//�������� �-�� �������
  if(cntObj<0) return;
  if(cntObj>=TOTAL_OBJ) return;
  tsbigcomponent->countObject = cntObj;
}//
void preTSBigReadAction() {
//action �� ������
  tsbigcomponent->operativMarker[0] = -1;
  tsbigcomponent->operativMarker[1] = -1;//����������� ������
  tsbigcomponent->isActiveActualData = 1;
}//
void postTSBigReadAction() {
//action ����� ������
  if(tsbigcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preTSBigWriteAction() {
//action �� ������
  tsbigcomponent->operativMarker[0] = -1;
  tsbigcomponent->operativMarker[1] = -1;//����������� ������
  tsbigcomponent->isActiveActualData = 1;
}//
void postTSBigWriteAction() {
//action ����� ������
  if(tsbigcomponent->operativMarker[0]<0) return;//�� ���� ������
  int offset = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
  int countRegister = tsbigcomponent->operativMarker[1]-tsbigcomponent->operativMarker[0]+1;
  if(tsbigcomponent->operativMarker[1]<0) countRegister = 1;
}//

int privateTSBigGetReg1(int adrReg)
{
  //��������� ���������� ��������
  int count_register = tsbigcomponent->countObject*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg1(int adrReg)
int privateTSBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = TOTAL_OBJ*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)
