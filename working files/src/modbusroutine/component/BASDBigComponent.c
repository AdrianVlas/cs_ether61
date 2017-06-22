#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 4413
#define REGISTER_FOR_OBJ 6

int getBASDBigModbusRegister(int);//�������� ���������� ��������
int getBASDBigModbusBit(int);//�������� ���������� ����
int setBASDBigModbusRegister(int, int);//��������
int setBASDBigModbusBit(int, int);// ����
int privateBASDBigGetReg2(int adrReg);

void setBASDBigCountObject(void);
void preBASDBigReadAction(void);//action �� ������
void postBASDBigReadAction(void);//action ����� ������
void preBASDBigWriteAction(void);//action �� ������
void postBASDBigWriteAction(void);//action ����� ������
void loadBASDBigActualData(void);

COMPONENT_OBJ *basdbigcomponent;
/**************************************/
//���������� ���������� ��������� ���������������� block\alarm
/**************************************/
void constructorBASDBigComponent(COMPONENT_OBJ *basdbigcomp)
{
  basdbigcomponent = basdbigcomp;

  basdbigcomponent->countObject = 1;//�-�� �������

  basdbigcomponent->getModbusRegister = getBASDBigModbusRegister;//�������� ���������� ��������
  basdbigcomponent->getModbusBit      = getBASDBigModbusBit;//�������� ���������� ����
  basdbigcomponent->setModbusRegister = setBASDBigModbusRegister;// ��������
  basdbigcomponent->setModbusBit      = setBASDBigModbusBit;//����

  basdbigcomponent->preReadAction   = preBASDBigReadAction;//action �� ������
  basdbigcomponent->postReadAction  = postBASDBigReadAction;//action ����� ������
  basdbigcomponent->preWriteAction  = preBASDBigWriteAction;//action �� ������
  basdbigcomponent->postWriteAction = postBASDBigWriteAction;//action ����� ������

  basdbigcomponent->isActiveActualData = 0;

}//prepareDVinConfig

void loadBASDBigActualData(void) {
  //ActualData
  for(int i=0; i<100; i++) tempReadArray[i] = i;
}//loadActualData() 

int getBASDBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateBASDBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  if(basdbigcomponent->isActiveActualData) loadBASDBigActualData(); //ActualData
  basdbigcomponent->isActiveActualData = 0;

  superSetOperativMarker(basdbigcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDVModbusRegister(int adrReg)
int getBASDBigModbusBit(int adrBit)
{
  //�������� ���������� ��������
  superSetOperativMarker(basdbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getBASDBigModbusBit(int adrReg)
int setBASDBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateBASDBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  superSetOperativMarker(basdbigcomponent, adrReg);
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
  }//switch
  return MARKER_OUTPERIMETR;
}//setDVModbusRegister(int adrReg)
int setBASDBigModbusBit(int adrBit, int x)
{
  UNUSED(x);
  //�������� ���������� ��������
  superSetOperativMarker(basdbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getBASDBigModbusBit(int adrReg)

void setBASDBigCountObject(void) {
//�������� �-�� �������
}//

void preBASDBigReadAction(void) {
//action �� ������
  basdbigcomponent->operativMarker[0] = -1;
  basdbigcomponent->operativMarker[1] = -1;//����������� ������
  basdbigcomponent->isActiveActualData = 1;
}//
void postBASDBigReadAction(void) {
//action ����� ������
  if(basdbigcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preBASDBigWriteAction(void) {
//action �� ������
  basdbigcomponent->operativMarker[0] = -1;
  basdbigcomponent->operativMarker[1] = -1;//����������� ������
  basdbigcomponent->isActiveActualData = 1;
}//
void postBASDBigWriteAction(void) {
//action ����� ������
  if(basdbigcomponent->operativMarker[0]<0) return;//�� ���� ������
//  int offset = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
//  int countRegister = basdbigcomponent->operativMarker[1]-basdbigcomponent->operativMarker[0]+1;
//  if(basdbigcomponent->operativMarker[1]<0) countRegister = 1;
}//

int privateBASDBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)
