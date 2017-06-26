#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 13013
//���� �-�� �������
#define TOTAL_OBJ 128
#define REGISTER_FOR_OBJ 4

int privateXORBigGetReg1(int adrReg);
int privateXORBigGetReg2(int adrReg);

int getXORBigModbusRegister(int);//�������� ���������� ��������
int getXORBigModbusBit(int);//�������� ���������� ����
int setXORBigModbusRegister(int, int);//�������� ���������� ��������
int setXORBigModbusBit(int, int);//�������� ���������� ����

void setXORBigCountObject(void);//�������� �-�� �������
void preXORBigReadAction(void);//action �� ������
void postXORBigReadAction(void);//action ����� ������
void preXORBigWriteAction(void);//action �� ������
void postXORBigWriteAction(void);//action ����� ������
void loadXORBigActualData(void);

COMPONENT_OBJ *xorbigcomponent;

/**************************************/
//���������� ���������� XOR
/**************************************/
void constructorXORBigComponent(COMPONENT_OBJ *xorbigcomp)
{
  xorbigcomponent = xorbigcomp;

  xorbigcomponent->countObject = 0;//�-�� �������

  xorbigcomponent->getModbusRegister = getXORBigModbusRegister;//�������� ���������� ��������
  xorbigcomponent->getModbusBit      = getXORBigModbusBit;//�������� ���������� ����
  xorbigcomponent->setModbusRegister = setXORBigModbusRegister;//�������� ���������� ��������
  xorbigcomponent->setModbusBit      = setXORBigModbusBit;//�������� ���������� ����

  xorbigcomponent->preReadAction   = preXORBigReadAction;//action �� ������
  xorbigcomponent->postReadAction  = postXORBigReadAction;//action ����� ������
  xorbigcomponent->preWriteAction  = preXORBigWriteAction;//action �� ������
  xorbigcomponent->postWriteAction = postXORBigWriteAction;//action ����� ������

  xorbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadXORBigActualData(void) {
 setXORBigCountObject(); //�������� �-�� �������
  //ActualData
   __LN_XOR *arr = (__LN_XOR*)(spca_of_p_prt[ID_FB_INPUT - _ID_FB_FIRST_VAR]);
   for(int item=0; item<xorbigcomponent->countObject; item++) {
   //XOR item.1 0
   int value = arr[item].settings.param[0];
   tempReadArray[item*REGISTER_FOR_OBJ+0] = value;
   //XOR item.1 1
   value = arr[item].settings.param[1];
   tempReadArray[item*REGISTER_FOR_OBJ+1] = value;

   //XOR item.2 0
   value = arr[item].settings.param[0];
   tempReadArray[item*REGISTER_FOR_OBJ+2] = value;
   //XOR item.2 1
   value = arr[item].settings.param[1];
   tempReadArray[item*REGISTER_FOR_OBJ+3] = value;

   }//for
}//loadActualData() 

int getXORBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateXORBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(xorbigcomponent->isActiveActualData) loadXORBigActualData(); //ActualData
  xorbigcomponent->isActiveActualData = 0;
  if(privateXORBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  superSetOperativMarker(xorbigcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDOUTBigModbusRegister(int adrReg)
int getXORBigModbusBit(int adrBit)
{
  //�������� ���������� ��������
  superSetOperativMarker(xorbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setXORBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateXORBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(privateXORBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  superSetOperativMarker(xorbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  switch((adrReg-BEGIN_ADR_REGISTER)%REGISTER_FOR_OBJ) {
   case 0:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
   break; 
   case 1:
   break; 
   case 2:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
   break; 
   case 3:
   break; 
  default: return MARKER_OUTPERIMETR;
  }//switch
  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int setXORBigModbusBit(int adrBit, int x)
{
  UNUSED(x);
  //�������� ���������� bit
  superSetOperativMarker(xorbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void setXORBigCountObject(void) {
//�������� �-�� �������
  int cntObj = current_config.n_xor;  //ʳ������ �������� "����.���"
  if(cntObj<0) return;
  if(cntObj>TOTAL_OBJ) return;
  xorbigcomponent->countObject = cntObj;
}//
void preXORBigReadAction(void) {
//action �� ������
  xorbigcomponent->operativMarker[0] = -1;
  xorbigcomponent->operativMarker[1] = -1;//����������� ������
  xorbigcomponent->isActiveActualData = 1;
}//
void postXORBigReadAction(void) {
//action ����� ������
  if(xorbigcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preXORBigWriteAction(void) {
//action �� ������
  xorbigcomponent->operativMarker[0] = -1;
  xorbigcomponent->operativMarker[1] = -1;//����������� ������
  xorbigcomponent->isActiveActualData = 1;
}//
void postXORBigWriteAction(void) {
//action ����� ������
  if(xorbigcomponent->operativMarker[0]<0) return;//�� ���� ������
//  int offset = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
//  int countRegister = xorbigcomponent->operativMarker[1]-xorbigcomponent->operativMarker[0]+1;
//  if(xorbigcomponent->operativMarker[1]<0) countRegister = 1;
}//

int privateXORBigGetReg1(int adrReg)
{
  //��������� ���������� ��������
  int count_register = xorbigcomponent->countObject*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg1(int adrReg)
int privateXORBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = TOTAL_OBJ*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)
