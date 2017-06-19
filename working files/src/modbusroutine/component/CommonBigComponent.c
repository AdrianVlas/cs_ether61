#include "variables_external_m.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 10718
//���� �-�� �������
#define REGISTER_FOR_OBJ 10

int privateCommonBigGetReg2(int adrReg);

int getCommonBigModbusRegister(int);//�������� ���������� ��������
int getCommonBigModbusBit(int);//�������� ���������� ����
int setCommonBigModbusRegister(int, int);// ��������
int setCommonBigModbusBit(int, int);// ����

void setCommonBigCountObject(int);//�������� �-�� �������
void preCommonBigReadAction();//action �� ������
void postCommonBigReadAction();//action ����� ������
void preCommonBigWriteAction();//action �� ������
void postCommonBigWriteAction();//action ����� ������
void loadCommonBigActualData();

COMPONENT_OBJ *commonbigcomponent;

/**************************************/
//���������� ���������� �����
/**************************************/
void constructorCommonBigComponent(COMPONENT_OBJ *commonbigcomp)
{
  commonbigcomponent = commonbigcomp;

  commonbigcomponent->countObject = 1;//�-�� �������

  commonbigcomponent->getModbusRegister = getCommonBigModbusRegister;//�������� ���������� ��������
  commonbigcomponent->getModbusBit      = getCommonBigModbusBit;//�������� ���������� ����
  commonbigcomponent->setModbusRegister = setCommonBigModbusRegister;//��������
  commonbigcomponent->setModbusBit      = setCommonBigModbusBit;// ����

  commonbigcomponent->setCountObject  = setCommonBigCountObject;//�������� �-�� �������
  commonbigcomponent->preReadAction   = preCommonBigReadAction;//action �� ������
  commonbigcomponent->postReadAction  = postCommonBigReadAction;//action ����� ������
  commonbigcomponent->preWriteAction  = preCommonBigWriteAction;//action �� ������
  commonbigcomponent->postWriteAction = postCommonBigWriteAction;//action ����� ������

  commonbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadCommonBigActualData() {
  //ActualData
  for(int i=0; i<100; i++) tempReadArray[i] = i;
}//loadActualData() 

int getCommonBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateCommonBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  if(commonbigcomponent->isActiveActualData) loadCommonBigActualData(); //ActualData
  commonbigcomponent->isActiveActualData = 0;

  superSetOperativMarker(commonbigcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDOUTBigModbusRegister(int adrReg)
int getCommonBigModbusBit(int adrBit)
{
  //�������� ���������� bit
  superSetOperativMarker(commonbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setCommonBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateCommonBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  superSetOperativMarker(commonbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  switch((adrReg-BEGIN_ADR_REGISTER)%REGISTER_FOR_OBJ) {
   case 0:
   return dataReg;
   case 1:
   return dataReg;
   case 2:
   return dataReg;
   case 3:
   return dataReg;
   case 4:
   return dataReg;
   case 5:
   return dataReg;
   case 6:
   return dataReg;
   case 7:
   return dataReg;
   case 8:
   return dataReg;
   case 9:
   return dataReg;
  }//switch
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setCommonBigModbusBit(int adrBit, int )
{
  //�������� ���������� ��������
  superSetOperativMarker(commonbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void setCommonBigCountObject(int) {
//�������� �-�� �������
}//
void preCommonBigReadAction() {
//action �� ������
  commonbigcomponent->operativMarker[0] = -1;
  commonbigcomponent->operativMarker[1] = -1;//����������� ������
  commonbigcomponent->isActiveActualData = 1;
}//
void postCommonBigReadAction() {
//action ����� ������
  if(commonbigcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preCommonBigWriteAction() {
//action �� ������
  commonbigcomponent->operativMarker[0] = -1;
  commonbigcomponent->operativMarker[1] = -1;//����������� ������
  commonbigcomponent->isActiveActualData = 1;
}//
void postCommonBigWriteAction() {
//action ����� ������
  if(commonbigcomponent->operativMarker[0]<0) return;//�� ���� ������
  int offset = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
  int countRegister = commonbigcomponent->operativMarker[1]-commonbigcomponent->operativMarker[0]+1;
  if(commonbigcomponent->operativMarker[1]<0) countRegister = 1;
}//

int privateCommonBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)
