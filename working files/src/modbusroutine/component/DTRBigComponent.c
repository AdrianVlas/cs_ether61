#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 6875
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

  dtrbigcomponent->countObject = 0;//�-�� �������

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
 setDTRBigCountObject(); //�������� �-�� �������
  //ActualData
   __LN_TRIGGER *arr = (__LN_TRIGGER*)(spca_of_p_prt[ID_FB_INPUT - _ID_FB_FIRST_VAR]);
   for(int item=0; item<dtrbigcomponent->countObject; item++) {

   //Set D-T 0  item
   int value = arr[item].settings.param[0];
   tempReadArray[item*REGISTER_FOR_OBJ+0] = value;
   //Set D-T 1  item
   value = arr[item].settings.param[1];
   tempReadArray[item*REGISTER_FOR_OBJ+1] = value;

   //CLR D-T 0  item
   value = arr[item].settings.param[1];
   tempReadArray[item*REGISTER_FOR_OBJ+2] = value;
   //CLR D-T 1  item
   value = arr[item].settings.param[1];
   tempReadArray[item*REGISTER_FOR_OBJ+3] = value;

   //D D-T 0  item
   value = arr[item].settings.param[1];
   tempReadArray[item*REGISTER_FOR_OBJ+4] = value;
   //D D-T 1  item
   value = arr[item].settings.param[1];
   tempReadArray[item*REGISTER_FOR_OBJ+5] = value;

   //C D-T 0  item
   value = arr[item].settings.param[1];
   tempReadArray[item*REGISTER_FOR_OBJ+6] = value;
   //C D-T 1  item
   value = arr[item].settings.param[1];
   tempReadArray[item*REGISTER_FOR_OBJ+7] = value;

   }//for
}//loadActualData() 

int getDTRBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateDTRBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(dtrbigcomponent->isActiveActualData) loadDTRBigActualData(); //ActualData
  dtrbigcomponent->isActiveActualData = 0;
  if(privateDTRBigGetReg1(adrReg)==MARKER_OUTPERIMETR) return MARKER_ERRORPERIMETR;

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
   default: return MARKER_OUTPERIMETR;
  }//switch
  return 0;
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
  int cntObj = current_config.n_trigger;    //ʳ������ ������
  if(cntObj<0) return;
  if(cntObj>TOTAL_OBJ) return;
  dtrbigcomponent->countObject = cntObj;
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
