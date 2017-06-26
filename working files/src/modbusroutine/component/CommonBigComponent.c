#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 13781
//���� �-�� �������
#define REGISTER_FOR_OBJ 10

int privateCommonBigGetReg2(int adrReg);

int getCommonBigModbusRegister(int);//�������� ���������� ��������
int getCommonBigModbusBit(int);//�������� ���������� ����
int setCommonBigModbusRegister(int, int);// ��������
int setCommonBigModbusBit(int, int);// ����

void preCommonBigReadAction(void);//action �� ������
void postCommonBigReadAction(void);//action ����� ������
void preCommonBigWriteAction(void);//action �� ������
void postCommonBigWriteAction(void);//action ����� ������
void loadCommonBigActualData(void);

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

  commonbigcomponent->preReadAction   = preCommonBigReadAction;//action �� ������
  commonbigcomponent->postReadAction  = postCommonBigReadAction;//action ����� ������
  commonbigcomponent->preWriteAction  = preCommonBigWriteAction;//action �� ������
  commonbigcomponent->postWriteAction = postCommonBigWriteAction;//action ����� ������

  commonbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadCommonBigActualData(void) {
  //ActualData
//   __LN_AND *arr = (__LN_AND*)(spca_of_p_prt[ID_FB_INPUT - _ID_FB_FIRST_VAR]);
   for(int item=0; item<commonbigcomponent->countObject; item++) {
   //������� 0
   int value = 0;//arr[item].settings.param[0];
   tempReadArray[item*REGISTER_FOR_OBJ+0] = value;
   //������� 1
   value = 0;//arr[item].settings.param[1];
   tempReadArray[item*REGISTER_FOR_OBJ+1] = value;

   //������ 0
   value = 0;//arr[item].settings.param[0];
   tempReadArray[item*REGISTER_FOR_OBJ+2] = value;
   //������ 1
   value = 0;//arr[item].settings.param[1];
   tempReadArray[item*REGISTER_FOR_OBJ+3] = value;

   //����. 0
   value = 0;//arr[item].settings.param[0];
   tempReadArray[item*REGISTER_FOR_OBJ+4] = value;
   //����. 1
   value = 0;//arr[item].settings.param[1];
   tempReadArray[item*REGISTER_FOR_OBJ+5] = value;

   //����.����. 0
   value = 0;//arr[item].settings.param[0];
   tempReadArray[item*REGISTER_FOR_OBJ+6] = value;
   //����.����. 1
   value = 0;//arr[item].settings.param[1];
   tempReadArray[item*REGISTER_FOR_OBJ+7] = value;

   //����.�����. 0
   value = 0;//arr[item].settings.param[0];
   tempReadArray[item*REGISTER_FOR_OBJ+8] = value;
   //����.�����. 1
   value = 0;//arr[item].settings.param[1];
   tempReadArray[item*REGISTER_FOR_OBJ+9] = value;
  }//for
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
   break; 
   case 1:
   break; 
   case 2:
   break; 
   case 3:
   break; 
   case 4:
   break; 
   case 5:
   break; 
   case 6:
   break; 
   case 7:
   break; 
   case 8:
   break; 
   case 9:
   break; 
   default: return MARKER_OUTPERIMETR;
  }//switch
  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int setCommonBigModbusBit(int adrBit, int x)
{
  UNUSED(x);
  //�������� ���������� ��������
  superSetOperativMarker(commonbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void preCommonBigReadAction(void) {
//action �� ������
  commonbigcomponent->operativMarker[0] = -1;
  commonbigcomponent->operativMarker[1] = -1;//����������� ������
  commonbigcomponent->isActiveActualData = 1;
}//
void postCommonBigReadAction(void) {
//action ����� ������
  if(commonbigcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preCommonBigWriteAction(void) {
//action �� ������
  commonbigcomponent->operativMarker[0] = -1;
  commonbigcomponent->operativMarker[1] = -1;//����������� ������
  commonbigcomponent->isActiveActualData = 1;
}//
void postCommonBigWriteAction(void) {
//action ����� ������
  if(commonbigcomponent->operativMarker[0]<0) return;//�� ���� ������
//  int offset = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
//  int countRegister = commonbigcomponent->operativMarker[1]-commonbigcomponent->operativMarker[0]+1;
//  if(commonbigcomponent->operativMarker[1]<0) countRegister = 1;
}//

int privateCommonBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)
