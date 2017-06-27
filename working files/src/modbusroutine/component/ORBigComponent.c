#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 10965
//���� �-�� �������
#define TOTAL_OBJ 128
#define REGISTER_FOR_OBJ 16

int privateORBigGetReg1(int adrReg);
int privateORBigGetReg2(int adrReg);

int getORBigModbusRegister(int);//�������� ���������� ��������
int getORBigModbusBit(int);//�������� ���������� ����
int setORBigModbusRegister(int, int);//�������� ���������� ��������
int setORBigModbusBit(int, int);//�������� ���������� ����

void setORBigCountObject(void);//�������� �-�� �������
void preORBigReadAction(void);//action �� ������
void postORBigReadAction(void);//action ����� ������
void preORBigWriteAction(void);//action �� ������
void postORBigWriteAction(void);//action ����� ������
void loadORBigActualData(void);

COMPONENT_OBJ *orbigcomponent;

/**************************************/
//���������� ���������� OR
/**************************************/
void constructorORBigComponent(COMPONENT_OBJ *orbigcomp)
{
  orbigcomponent = orbigcomp;

  orbigcomponent->countObject = 0;//�-�� �������

  orbigcomponent->getModbusRegister = getORBigModbusRegister;//�������� ���������� ��������
  orbigcomponent->getModbusBit      = getORBigModbusBit;//�������� ���������� ����
  orbigcomponent->setModbusRegister = setORBigModbusRegister;//�������� ���������� ��������
  orbigcomponent->setModbusBit      = setORBigModbusBit;//�������� ���������� ����

  orbigcomponent->preReadAction   = preORBigReadAction;//action �� ������
  orbigcomponent->postReadAction  = postORBigReadAction;//action ����� ������
  orbigcomponent->preWriteAction  = preORBigWriteAction;//action �� ������
  orbigcomponent->postWriteAction = postORBigWriteAction;//action ����� ������

  orbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadORBigActualData(void) {
 setORBigCountObject(); //�������� �-�� �������
  //ActualData
   __LN_OR *arr = (__LN_OR*)(spca_of_p_prt[ID_FB_OR - _ID_FB_FIRST_VAR]);
   for(int item=0; item<orbigcomponent->countObject; item++) {

     for (int i = 0; i < OR_SIGNALS_IN; i ++)
     {
        int value = arr[item].settings.param[i] & 0xffff;//LEDIN 0 �� item
        tempReadArray[item*REGISTER_FOR_OBJ+2*i+0] = value;
        value = (arr[item].settings.param[i] >> 16) & 0x7fff;//LEDIN 1 �� item
        tempReadArray[item*REGISTER_FOR_OBJ+2*i+1] = value;
     }
     
//     //OR item.1 0
//   int value = arr[item].settings.param[0];
//   tempReadArray[item*REGISTER_FOR_OBJ+0] = value;
//   //OR item.2 1
//   value = arr[item].settings.param[1];
//   tempReadArray[item*REGISTER_FOR_OBJ+1] = value;
//
//   //OR item.1 0
//   value = arr[item].settings.param[0];
//   tempReadArray[item*REGISTER_FOR_OBJ+2] = value;
//   //OR item.2 1
//   value = arr[item].settings.param[1];
//   tempReadArray[item*REGISTER_FOR_OBJ+3] = value;
//
//   //OR item.1 0
//   value = arr[item].settings.param[0];
//   tempReadArray[item*REGISTER_FOR_OBJ+4] = value;
//   //OR item.2 1
//   value = arr[item].settings.param[1];
//   tempReadArray[item*REGISTER_FOR_OBJ+5] = value;
//
//   //OR item.1 0
//   value = arr[item].settings.param[0];
//   tempReadArray[item*REGISTER_FOR_OBJ+6] = value;
//   //OR item.2 1
//   value = arr[item].settings.param[1];
//   tempReadArray[item*REGISTER_FOR_OBJ+7] = value;
//
//   //OR item.1 0
//   value = arr[item].settings.param[0];
//   tempReadArray[item*REGISTER_FOR_OBJ+8] = value;
//   //OR item.2 1
//   value = arr[item].settings.param[1];
//   tempReadArray[item*REGISTER_FOR_OBJ+9] = value;
//
//   //OR item.1 0
//   value = arr[item].settings.param[0];
//   tempReadArray[item*REGISTER_FOR_OBJ+10] = value;
//   //OR item.2 1
//   value = arr[item].settings.param[1];
//   tempReadArray[item*REGISTER_FOR_OBJ+11] = value;
//
//   //OR item.1 0
//   value = arr[item].settings.param[0];
//   tempReadArray[item*REGISTER_FOR_OBJ+12] = value;
//   //OR item.2 1
//   value = arr[item].settings.param[1];
//   tempReadArray[item*REGISTER_FOR_OBJ+13] = value;
//
//   //OR item.1 0
//   value = arr[item].settings.param[0];
//   tempReadArray[item*REGISTER_FOR_OBJ+14] = value;
//   //OR item.2 1
//   value = arr[item].settings.param[1];
//   tempReadArray[item*REGISTER_FOR_OBJ+15] = value;

   }//for
}//loadActualData() 

int getORBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateORBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(orbigcomponent->isActiveActualData) loadORBigActualData(); //ActualData
  orbigcomponent->isActiveActualData = 0;
  if(privateORBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  superSetOperativMarker(orbigcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDOUTBigModbusRegister(int adrReg)
int getORBigModbusBit(int adrReg)
{
  //�������� ���������� ��������
  superSetOperativMarker(orbigcomponent, adrReg);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setORBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateORBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(privateORBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

  superSetOperativMarker(orbigcomponent, adrReg);
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
   case 4:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
   break; 
   case 5:
   break; 
   case 6:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
   break; 
   case 7:
   break; 
   case 8:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
   break; 
   case 9:
   break; 
   case 10:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
   break; 
   case 11:
   break; 
   case 12:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
   break; 
   case 13:
   break; 
   case 14:
    if(dataReg>MAXIMUMI) return MARKER_ERRORDIAPAZON;
   break; 
   case 15:
   break; 
  default: return MARKER_OUTPERIMETR;
  }//switch
  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int setORBigModbusBit(int adrBit, int x)
{
  UNUSED(x);
  //�������� ���������� ��������
  superSetOperativMarker(orbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void setORBigCountObject(void) {
//�������� �-�� �������
  int cntObj = current_config.n_or;    //ʳ������ �������� "���"
  if(cntObj<0) return;
  if(cntObj>TOTAL_OBJ) return;
  orbigcomponent->countObject = cntObj;
}//
void preORBigReadAction(void) {
//action �� ������
  orbigcomponent->operativMarker[0] = -1;
  orbigcomponent->operativMarker[1] = -1;//����������� ������
  orbigcomponent->isActiveActualData = 1;
}//
void postORBigReadAction(void) {
//action ����� ������
  if(orbigcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preORBigWriteAction(void) {
//action �� ������
  orbigcomponent->operativMarker[0] = -1;
  orbigcomponent->operativMarker[1] = -1;//����������� ������
  orbigcomponent->isActiveActualData = 1;
}//
void postORBigWriteAction(void) {
//action ����� ������
  if(orbigcomponent->operativMarker[0]<0) return;//�� ���� ������
//  int offset = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
//  int countRegister = orbigcomponent->operativMarker[1]-orbigcomponent->operativMarker[0]+1;
//  if(orbigcomponent->operativMarker[1]<0) countRegister = 1;
}//

int privateORBigGetReg1(int adrReg)
{
  //��������� ���������� ��������
  int count_register = orbigcomponent->countObject*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg1(int adrReg)
int privateORBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = TOTAL_OBJ*REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)
