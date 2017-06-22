#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 6734
//���� �-�� �������
#define TOTAL_OBJ 128
#define REGISTER_FOR_OBJ 8

int privateDTRBigGetReg1(int adrReg);
int privateDTRBigGetReg2(int adrReg);

int getDTRBigModbusRegister(int);//�������� ���������� ��������
int getDTRBigModbusBit(int);//�������� ���������� ����
int setDTRBigModbusRegister(int, int);// ��������
int setDTRBigModbusBit(int, int);// ����

void setDTRBigCountObject(void);//�������� �-�� �������
void preDTRBigReadAction(void);//action �� ������
void postDTRBigReadAction(void);//action ����� ������
void preDTRBigWriteAction(void);//action �� ������
void postDTRBigWriteAction(void);//action ����� ������
void loadDTRBigActualData(void);

COMPONENT_OBJ *dtrbigcomponent;

/**************************************/
//���������� ���������� D-�������
/**************************************/
void constructorDTRBigComponent(COMPONENT_OBJ *dtrbigcomp)
{
  dtrbigcomponent = dtrbigcomp;

  dtrbigcomponent->countObject = 128;//�-�� �������

  dtrbigcomponent->getModbusRegister = getDTRBigModbusRegister;//�������� ���������� ��������
  dtrbigcomponent->getModbusBit      = getDTRBigModbusBit;//�������� ���������� ����
  dtrbigcomponent->setModbusRegister = setDTRBigModbusRegister;// ��������
  dtrbigcomponent->setModbusBit      = setDTRBigModbusBit;// ����

  dtrbigcomponent->preReadAction   = preDTRBigReadAction;//action �� ������
  dtrbigcomponent->postReadAction  = postDTRBigReadAction;//action ����� ������
  dtrbigcomponent->preWriteAction  = preDTRBigWriteAction;//action �� ������
  dtrbigcomponent->postWriteAction = postDTRBigWriteAction;//action ����� ������

  dtrbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadDTRBigActualData(void) {
  //ActualData
  for(int i=0; i<100; i++) tempReadArray[i] = i;
}//loadActualData() 

int getDTRBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateDTRBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(privateDTRBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  if(dtrbigcomponent->isActiveActualData) loadDTRBigActualData(); //ActualData
  dtrbigcomponent->isActiveActualData = 0;

  superSetOperativMarker(dtrbigcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDOUTBigModbusRegister(int adrReg)
int getDTRBigModbusBit(int adrBit)
{
  //�������� ���������� ��������
  superSetOperativMarker(dtrbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setDTRBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateDTRBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(privateDTRBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  superSetOperativMarker(dtrbigcomponent, adrReg);
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
  }//switch
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setDTRBigModbusBit(int adrBit, int x)
{
  UNUSED(x);
  //�������� ���������� ��������
  superSetOperativMarker(dtrbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void setDTRBigCountObject(void) {
//�������� �-�� �������
}//
void preDTRBigReadAction(void) {
//action �� ������
  dtrbigcomponent->operativMarker[0] = -1;
  dtrbigcomponent->operativMarker[1] = -1;//����������� ������
  dtrbigcomponent->isActiveActualData = 1;
}//
void postDTRBigReadAction(void) {
//action ����� ������
  if(dtrbigcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preDTRBigWriteAction(void) {
//action �� ������
  dtrbigcomponent->operativMarker[0] = -1;
  dtrbigcomponent->operativMarker[1] = -1;//����������� ������
  dtrbigcomponent->isActiveActualData = 1;
}//
void postDTRBigWriteAction(void) {
//action ����� ������
  if(dtrbigcomponent->operativMarker[0]<0) return;//�� ���� ������
//  int offset = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
//  int countRegister = dtrbigcomponent->operativMarker[1]-dtrbigcomponent->operativMarker[0]+1;
//  if(dtrbigcomponent->operativMarker[1]<0) countRegister = 1;
}//

int privateDTRBigGetReg1(int adrReg)
{
  //��������� ���������� ��������
  int count_register = dtrbigcomponent->countObject*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg1(int adrReg)
int privateDTRBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = TOTAL_OBJ*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)
