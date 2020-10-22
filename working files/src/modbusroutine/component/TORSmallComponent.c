#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 2282
//��������� bit � ����� ������
#define BEGIN_ADR_BIT 50128
#define BIT_FOR_OBJ 1

int privateTORSmallGetReg2(int adrReg);
int privateTORSmallGetBit2(int adrBit);

int getTORSmallModbusRegister(int);//�������� ���������� ��������
int getTORSmallModbusBit(int);//�������� ���������� ����
int setTORSmallModbusRegister(int, int);//�������� ���������� ��������
int setTORSmallModbusBit(int, int);//�������� ���������� ����

void setTORSmallCountObject(void);//�������� �-�� �������
void preTORSmallReadAction(void);//action �� ������
void postTORSmallReadAction(void);//action ����� ������
void preTORSmallWriteAction(void);//action �� ������
int  postTORSmallWriteAction(void);//action ����� ������
void loadTORSmallActualData(void);
void config_and_settingsTORSmall(void);//action ���������

COMPONENT_OBJ *torsmallcomponent;

/**************************************/
//���������� ���������� ��������������� OR
/**************************************/
void constructorTORSmallComponent(COMPONENT_OBJ *torsmallcomp)
{
  torsmallcomponent = torsmallcomp;

  torsmallcomponent->countObject = 128;//�-�� �������

  torsmallcomponent->getModbusRegister = getTORSmallModbusRegister;//�������� ���������� ��������
  torsmallcomponent->getModbusBit      = getTORSmallModbusBit;//�������� ���������� ����
  torsmallcomponent->setModbusRegister = setTORSmallModbusRegister;//�������� ���������� ��������
  torsmallcomponent->setModbusBit      = setTORSmallModbusBit;//�������� ���������� ����

  torsmallcomponent->preReadAction   = preTORSmallReadAction;//action �� ������
  torsmallcomponent->postReadAction  = postTORSmallReadAction;//action ����� ������
  torsmallcomponent->preWriteAction  = preTORSmallWriteAction;//action �� ������
  torsmallcomponent->postWriteAction = postTORSmallWriteAction;//action ����� ������
  torsmallcomponent->config_and_settings = config_and_settingsTORSmall;//action ���������

  torsmallcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadTORSmallActualData(void) {
  //ActualData
extern int pointInterface;//����� ���������� 0-USB 1-RS485
   int cnt_treg = torsmallcomponent->countObject/16;
   if(torsmallcomponent->countObject%16) cnt_treg++;
   for(int ii=0; ii<cnt_treg; ii++) tempReadArray[ii] = 0;

   __LN_OR *arr = (__LN_OR*)(spca_of_p_prt[ID_FB_OR - _ID_FB_FIRST_VAR]);
   for(int item=0; item<torsmallcomponent->countObject; item++) {
   int ireg = item/16;
   int value = arr[item].active_state[0];
   int tdata = 0;
   if(value) tdata=1;
   tempReadArray[ireg] |= tdata<<(item%16);
  }//for

}//loadTORSmallActualData() 

int getTORSmallModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateTORSmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(torsmallcomponent->isActiveActualData) loadTORSmallActualData(); //ActualData
  torsmallcomponent->isActiveActualData = 0;

  superSetOperativMarker(torsmallcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getTORSmallModbusRegister(int adrReg)
int getTORSmallModbusBit(int adrBit)
{
  //�������� ���������� bit
  if(privateTORSmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(torsmallcomponent->isActiveActualData) loadTORSmallActualData();
  torsmallcomponent->isActiveActualData = 0;

  superSetOperativMarker(torsmallcomponent, adrBit);

  short tmp   = tempReadArray[(adrBit-BEGIN_ADR_BIT)/16];
  short maska = 1<<((adrBit-BEGIN_ADR_BIT)%16);
  if(tmp&maska) return 1;
  return 0;
}//getTORSmallModbusBit(int adrReg)
int setTORSmallModbusRegister(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//setTORSmallModbusRegister(int x, int y)
int setTORSmallModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//setTORSmallModbusBit(int x, int y)

void preTORSmallReadAction(void) {
//action �� ������
  torsmallcomponent->operativMarker[0] = -1;
  torsmallcomponent->operativMarker[1] = -1;//����������� ������
  torsmallcomponent->isActiveActualData = 1;
}//
void postTORSmallReadAction(void) {
//action ����� ������
  if(torsmallcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preTORSmallWriteAction(void) {
//action �� ������
  torsmallcomponent->operativMarker[0] = -1;
  torsmallcomponent->operativMarker[1] = -1;//����������� ������
  torsmallcomponent->isActiveActualData = 1;
}//
int postTORSmallWriteAction(void) {
//action ����� ������
 return 0;
}//

int privateTORSmallGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = TOTAL_OBJ/16;
  if(TOTAL_OBJ%16) count_register++;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)

int privateTORSmallGetBit2(int adrBit)
{
  //��������� ������� ��������
  int count_bit = BIT_FOR_OBJ*TOTAL_OBJ;
  if(adrBit>=BEGIN_ADR_BIT && adrBit<(BEGIN_ADR_BIT+count_bit)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)

void config_and_settingsTORSmall(void)
{
//action ���������
}

