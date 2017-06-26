#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 7894
//���� �-�� �������
#define TOTAL_OBJ 128
#define REGISTER_FOR_OBJ 5

int privateTSBigGetReg1(int adrReg);
int privateTSBigGetReg2(int adrReg);

int getTSBigModbusRegister(int);//�������� ���������� ��������
int getTSBigModbusBit(int);//�������� ���������� ����
int setTSBigModbusRegister(int, int);//�������� ���������� ��������
int setTSBigModbusBit(int, int);//�������� ���������� ����

void setTSBigCountObject(void);//�������� �-�� �������
void preTSBigReadAction(void);//action �� ������
void postTSBigReadAction(void);//action ����� ������
void preTSBigWriteAction(void);//action �� ������
void postTSBigWriteAction(void);//action ����� ������
void loadTSBigActualData(void);

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

  tsbigcomponent->preReadAction   = preTSBigReadAction;//action �� ������
  tsbigcomponent->postReadAction  = postTSBigReadAction;//action ����� ������
  tsbigcomponent->preWriteAction  = preTSBigWriteAction;//action �� ������
  tsbigcomponent->postWriteAction = postTSBigWriteAction;//action ����� ������

  tsbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadTSBigActualData(void) {
 setTSBigCountObject(); //�������� �-�� �������

  //ActualData
   __LN_TS *arr = (__LN_TS*)(spca_of_p_prt[ID_FB_TS - _ID_FB_FIRST_VAR]);
   for(int item=0; item<tsbigcomponent->countObject; item++) {

   //In TC 0  item
   int value = arr[item].settings.param[0];
   tempReadArray[item*REGISTER_FOR_OBJ+0] = value;
   //In TC 0  item
   value = arr[item].settings.param[0];
   tempReadArray[item*REGISTER_FOR_OBJ+1] = value;

   //Block TC 0  item
   value = arr[item].settings.param[0];
   tempReadArray[item*REGISTER_FOR_OBJ+2] = value;
   //Block TC 1  item
   value = arr[item].settings.param[0];
   tempReadArray[item*REGISTER_FOR_OBJ+3] = value;

   //����� �� 0  item
   value = arr[item].settings.param[0];
   tempReadArray[item*REGISTER_FOR_OBJ+4] = value;

   }//for
}//loadActualData() 

int getTSBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateTSBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(tsbigcomponent->isActiveActualData) loadTSBigActualData(); //ActualData
  tsbigcomponent->isActiveActualData = 0;
  if(privateTSBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

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
int setTSBigModbusBit(int adrBit, int x)
{
  UNUSED(x);
  //�������� ���������� ��������
  superSetOperativMarker(tsbigcomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void setTSBigCountObject(void) {
//�������� �-�� �������
  int cntObj = current_config.n_ts;    //ʳ������ ����� �����������
  if(cntObj<0) return;
  if(cntObj>TOTAL_OBJ) return;
  tsbigcomponent->countObject = cntObj;
}//
void preTSBigReadAction(void) {
//action �� ������
  tsbigcomponent->operativMarker[0] = -1;
  tsbigcomponent->operativMarker[1] = -1;//����������� ������
  tsbigcomponent->isActiveActualData = 1;
}//
void postTSBigReadAction(void) {
//action ����� ������
  if(tsbigcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preTSBigWriteAction(void) {
//action �� ������
  tsbigcomponent->operativMarker[0] = -1;
  tsbigcomponent->operativMarker[1] = -1;//����������� ������
  tsbigcomponent->isActiveActualData = 1;
}//
void postTSBigWriteAction(void) {
//action ����� ������
  if(tsbigcomponent->operativMarker[0]<0) return;//�� ���� ������
//  int offset = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
//  int countRegister = tsbigcomponent->operativMarker[1]-tsbigcomponent->operativMarker[0]+1;
//  if(tsbigcomponent->operativMarker[1]<0) countRegister = 1;
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
