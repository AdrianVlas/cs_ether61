#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 1156
//���� �-�� �������
#define TOTAL_OBJ 128
#define REGISTER_FOR_OBJ 11
int privateDOUTBigGetReg1(int adrReg);
int privateDOUTBigGetReg2(int adrReg);

int getDOUTBigModbusRegister(int);//�������� ���������� ��������
int getDOUTBigModbusBit(int);//�������� ���������� ����
int setDOUTBigModbusRegister(int, int);// ��������
int setDOUTBigModbusBit(int, int);// ����

void setDOUTBigCountObject(int);//�������� �-�� �������
void preDOUTBigReadAction(void);//action �� ������
void postDOUTBigReadAction(void);//action ����� ������
void preDOUTBigWriteAction(void);//action �� ������
void postDOUTBigWriteAction(void);//action ����� ������
void loadDOUTBigActualData(void);

COMPONENT_OBJ *doutbigcomponent;

/**************************************/
//���������� ���������� ��
/**************************************/
void constructorDOUTBigComponent(COMPONENT_OBJ *doutcomp)
{
  doutbigcomponent = doutcomp;
  doutbigcomponent->countObject = 0;//�-�� �������

  doutbigcomponent->getModbusRegister = getDOUTBigModbusRegister;//�������� ���������� ��������
  doutbigcomponent->getModbusBit      = getDOUTBigModbusBit;//�������� ���������� ����
  doutbigcomponent->setModbusRegister = setDOUTBigModbusRegister;// ��������
  doutbigcomponent->setModbusBit      = setDOUTBigModbusBit;// ����

  doutbigcomponent->setCountObject  = setDOUTBigCountObject;//�������� �-�� �������
  doutbigcomponent->preReadAction   = preDOUTBigReadAction;//action �� ������
  doutbigcomponent->postReadAction  = postDOUTBigReadAction;//action ����� ������
  doutbigcomponent->preWriteAction  = preDOUTBigWriteAction;//action �� ������
  doutbigcomponent->postWriteAction = postDOUTBigWriteAction;//action ����� ������

  doutbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadDOUTBigActualData(void) {
  //ActualData
  for(int i=0; i<100; i++) tempReadArray[i] = i;
  /*
  ...
  
  �� ����� "��������� C1" � "��������� C2" ����� ���������� ����� ����������, ����� id ������ ���������� ID_FB_MEANDER, n �� ���� ������������ ������� ���������� � ������������, out ������ ���������� 1, �� � ���������� � ������ �����
  */
}//loadActualData() 

int getDOUTBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateDOUTBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(privateDOUTBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  if(doutbigcomponent->isActiveActualData) loadDOUTBigActualData(); //ActualData
  doutbigcomponent->isActiveActualData = 0;

  superSetOperativMarker(doutbigcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDOUTBigModbusRegister(int adrReg)
int getDOUTBigModbusBit(int adrBit)
{
  //�������� ���������� ��������
  superSetOperativMarker(doutbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setDOUTBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateDOUTBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(privateDOUTBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  superSetOperativMarker(doutbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  switch((adrReg-BEGIN_ADR_REGISTER)%REGISTER_FOR_OBJ) {
   case 0:
   return dataReg;
   case 1:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
    return dataReg;
   case 2:
   return dataReg;
   case 3:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
    return dataReg;
   case 4:
   return dataReg;
   case 5:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
    return dataReg;
   case 6:
   return dataReg;
   case 7:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
    return dataReg;
   case 8:
   return dataReg;
   case 9:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
    return dataReg;
   case 10:
   return dataReg;
  }//switch
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setDOUTBigModbusBit(int adrReg, int x)
{
  UNUSED(x);
  //�������� ���������� ��������
  superSetOperativMarker(doutbigcomponent, adrReg);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void setDOUTBigCountObject(int cntObj) {
//�������� �-�� �������
  if(cntObj<0) return;
  if(cntObj>=TOTAL_OBJ) return;
  doutbigcomponent->countObject = cntObj;
}//
void preDOUTBigReadAction(void) {
//action �� ������
  doutbigcomponent->operativMarker[0] = -1;
  doutbigcomponent->operativMarker[1] = -1;//����������� ������
  doutbigcomponent->isActiveActualData = 1;
}//
void postDOUTBigReadAction(void) {
//action ����� ������
  if(doutbigcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preDOUTBigWriteAction(void) {
//action �� ������
  doutbigcomponent->operativMarker[0] = -1;
  doutbigcomponent->operativMarker[1] = -1;//����������� ������
  doutbigcomponent->isActiveActualData = 1;
}//
void postDOUTBigWriteAction(void) {
//action ����� ������
  if(doutbigcomponent->operativMarker[0]<0) return;//�� ���� ������
  int offset = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
  int countRegister = doutbigcomponent->operativMarker[1]-doutbigcomponent->operativMarker[0]+1;
  if(doutbigcomponent->operativMarker[1]<0) countRegister = 1;
}//

int privateDOUTBigGetReg1(int adrReg)
{
  //��������� ���������� ��������
  int count_register = doutbigcomponent->countObject*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg1(int adrReg)
int privateDOUTBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = TOTAL_OBJ*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)
