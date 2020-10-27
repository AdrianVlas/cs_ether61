#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 2275
//��������� bit � ����� ������
#define BEGIN_ADR_BIT 50000
#define BIT_FOR_OBJ 1

int privateTANDSmallGetReg2(int adrReg);
int privateTANDSmallGetBit2(int adrBit);

int getTANDSmallModbusRegister(int);//�������� ���������� ��������
int getTANDSmallModbusBit(int);//�������� ���������� ����
int setTANDSmallModbusRegister(int, int);//�������� ���������� ��������
int setTANDSmallModbusBit(int, int);//�������� ���������� ����

void setTANDSmallCountObject(void);//�������� �-�� �������
void preTANDSmallReadAction(void);//action �� ������
void postTANDSmallReadAction(void);//action ����� ������
void preTANDSmallWriteAction(void);//action �� ������
int  postTANDSmallWriteAction(void);//action ����� ������
void loadTANDSmallActualData(void);
void config_and_settingsTANDSmall(void);//action ���������

COMPONENT_OBJ *tandsmallcomponent;

/**************************************/
//���������� ���������� ��������������� AND
/**************************************/
void constructorTANDSmallComponent(COMPONENT_OBJ *tandsmallcomp)
{
  tandsmallcomponent = tandsmallcomp;

  tandsmallcomponent->countObject = 128;//�-�� �������

  tandsmallcomponent->getModbusRegister = getTANDSmallModbusRegister;//�������� ���������� ��������
  tandsmallcomponent->getModbusBit      = getTANDSmallModbusBit;//�������� ���������� ����
  tandsmallcomponent->setModbusRegister = setTANDSmallModbusRegister;//�������� ���������� ��������
  tandsmallcomponent->setModbusBit      = setTANDSmallModbusBit;//�������� ���������� ����

  tandsmallcomponent->preReadAction   = preTANDSmallReadAction;//action �� ������
  tandsmallcomponent->postReadAction  = postTANDSmallReadAction;//action ����� ������
  tandsmallcomponent->preWriteAction  = preTANDSmallWriteAction;//action �� ������
  tandsmallcomponent->postWriteAction = postTANDSmallWriteAction;//action ����� ������
  tandsmallcomponent->config_and_settings = config_and_settingsTANDSmall;//action ���������

  tandsmallcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadTANDSmallActualData(void) {
  //ActualData
extern int pointInterface;//����� ���������� 0-USB 1-RS485
   int cnt_treg = tandsmallcomponent->countObject/16;
   if(tandsmallcomponent->countObject%16) cnt_treg++;
   for(int ii=0; ii<cnt_treg; ii++) tempReadArray[ii] = 0;

   __LN_AND *arr = (__LN_AND*)(spca_of_p_prt[ID_FB_AND - _ID_FB_FIRST_VAR]);
   for(int item=0; item<tandsmallcomponent->countObject; item++) {
   int ireg = item/16;
   int value = arr[item].active_state[0];
   int tdata = 0;
   if(value) tdata=1;
   tempReadArray[ireg] |= tdata<<(item%16);
  }//for

}//loadTANDSmallActualData() 

int getTANDSmallModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateTANDSmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(tandsmallcomponent->isActiveActualData) loadTANDSmallActualData(); //ActualData
  tandsmallcomponent->isActiveActualData = 0;

  superSetOperativMarker(tandsmallcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getTANDSmallModbusRegister(int adrReg)
int getTANDSmallModbusBit(int adrBit)
{
  //�������� ���������� bit
  if(privateTANDSmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(tandsmallcomponent->isActiveActualData) loadTANDSmallActualData();
  tandsmallcomponent->isActiveActualData = 0;

  superSetOperativMarker(tandsmallcomponent, adrBit);

  short tmp   = tempReadArray[(adrBit-BEGIN_ADR_BIT)/16];
  short maska = 1<<((adrBit-BEGIN_ADR_BIT)%16);
  if(tmp&maska) return 1;
  return 0;
}//getTANDSmallModbusBit(int adrReg)
int setTANDSmallModbusRegister(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//setTANDSmallModbusRegister(int x, int y)
int setTANDSmallModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//setTANDSmallModbusBit(int x, int y)

void preTANDSmallReadAction(void) {
//action �� ������
  tandsmallcomponent->operativMarker[0] = -1;
  tandsmallcomponent->operativMarker[1] = -1;//����������� ������
  tandsmallcomponent->isActiveActualData = 1;
}//
void postTANDSmallReadAction(void) {
//action ����� ������
  if(tandsmallcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preTANDSmallWriteAction(void) {
//action �� ������
  tandsmallcomponent->operativMarker[0] = -1;
  tandsmallcomponent->operativMarker[1] = -1;//����������� ������
  tandsmallcomponent->isActiveActualData = 1;
}//
int postTANDSmallWriteAction(void) {
//action ����� ������
 return 0;
}//

int privateTANDSmallGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = TOTAL_OBJ/16;
  if(TOTAL_OBJ%16) count_register++;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)

int privateTANDSmallGetBit2(int adrBit)
{
  //��������� ������� ��������
  int count_bit = BIT_FOR_OBJ*TOTAL_OBJ;
  if(adrBit>=BEGIN_ADR_BIT && adrBit<(BEGIN_ADR_BIT+count_bit)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)

void config_and_settingsTANDSmall(void)
{
//action ���������
}

