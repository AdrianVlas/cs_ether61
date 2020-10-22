#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 2291
//��������� bit � ����� ������
#define BEGIN_ADR_BIT 50256
#define BIT_FOR_OBJ 1

int privateTXORSmallGetReg2(int adrReg);
int privateTXORSmallGetBit2(int adrBit);

int getTXORSmallModbusRegister(int);//�������� ���������� ��������
int getTXORSmallModbusBit(int);//�������� ���������� ����
int setTXORSmallModbusRegister(int, int);//�������� ���������� ��������
int setTXORSmallModbusBit(int, int);//�������� ���������� ����

void setTXORSmallCountObject(void);//�������� �-�� �������
void preTXORSmallReadAction(void);//action �� ������
void postTXORSmallReadAction(void);//action ����� ������
void preTXORSmallWriteAction(void);//action �� ������
int  postTXORSmallWriteAction(void);//action ����� ������
void loadTXORSmallActualData(void);
void config_and_settingsTXORSmall(void);//action ���������

COMPONENT_OBJ *txorsmallcomponent;

/**************************************/
//���������� ���������� ��������������� XOR
/**************************************/
void constructorTXORSmallComponent(COMPONENT_OBJ *txorsmallcomp)
{
  txorsmallcomponent = txorsmallcomp;

  txorsmallcomponent->countObject = 128;//�-�� �������

  txorsmallcomponent->getModbusRegister = getTXORSmallModbusRegister;//�������� ���������� ��������
  txorsmallcomponent->getModbusBit      = getTXORSmallModbusBit;//�������� ���������� ����
  txorsmallcomponent->setModbusRegister = setTXORSmallModbusRegister;//�������� ���������� ��������
  txorsmallcomponent->setModbusBit      = setTXORSmallModbusBit;//�������� ���������� ����

  txorsmallcomponent->preReadAction   = preTXORSmallReadAction;//action �� ������
  txorsmallcomponent->postReadAction  = postTXORSmallReadAction;//action ����� ������
  txorsmallcomponent->preWriteAction  = preTXORSmallWriteAction;//action �� ������
  txorsmallcomponent->postWriteAction = postTXORSmallWriteAction;//action ����� ������
  txorsmallcomponent->config_and_settings = config_and_settingsTXORSmall;//action ���������

  txorsmallcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadTXORSmallActualData(void) {
  //ActualData
extern int pointInterface;//����� ���������� 0-USB 1-RS485
   int cnt_treg = txorsmallcomponent->countObject/16;
   if(txorsmallcomponent->countObject%16) cnt_treg++;
   for(int ii=0; ii<cnt_treg; ii++) tempReadArray[ii] = 0;

   __LN_XOR *arr = (__LN_XOR*)(spca_of_p_prt[ID_FB_XOR - _ID_FB_FIRST_VAR]);
   for(int item=0; item<txorsmallcomponent->countObject; item++) {
   int ireg = item/16;
   int value = arr[item].active_state[0];
   int tdata = 0;
   if(value) tdata=1;
   tempReadArray[ireg] |= tdata<<(item%16);
  }//for

}//loadTXORSmallActualData() 

int getTXORSmallModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateTXORSmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(txorsmallcomponent->isActiveActualData) loadTXORSmallActualData(); //ActualData
  txorsmallcomponent->isActiveActualData = 0;

  superSetOperativMarker(txorsmallcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getTXORSmallModbusRegister(int adrReg)
int getTXORSmallModbusBit(int adrBit)
{
  //�������� ���������� bit
  if(privateTXORSmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(txorsmallcomponent->isActiveActualData) loadTXORSmallActualData();
  txorsmallcomponent->isActiveActualData = 0;

  superSetOperativMarker(txorsmallcomponent, adrBit);

  short tmp   = tempReadArray[(adrBit-BEGIN_ADR_BIT)/16];
  short maska = 1<<((adrBit-BEGIN_ADR_BIT)%16);
  if(tmp&maska) return 1;
  return 0;
}//getTORSmallModbusBit(int adrReg)
int setTXORSmallModbusRegister(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//setTXORSmallModbusRegister(int x, int y)
int setTXORSmallModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//setTXORSmallModbusBit(int x, int y)

void preTXORSmallReadAction(void) {
//action �� ������
  txorsmallcomponent->operativMarker[0] = -1;
  txorsmallcomponent->operativMarker[1] = -1;//����������� ������
  txorsmallcomponent->isActiveActualData = 1;
}//
void postTXORSmallReadAction(void) {
//action ����� ������
  if(txorsmallcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preTXORSmallWriteAction(void) {
//action �� ������
  txorsmallcomponent->operativMarker[0] = -1;
  txorsmallcomponent->operativMarker[1] = -1;//����������� ������
  txorsmallcomponent->isActiveActualData = 1;
}//
int postTXORSmallWriteAction(void) {
//action ����� ������
 return 0;
}//

int privateTXORSmallGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = TOTAL_OBJ/16;
  if(TOTAL_OBJ%16) count_register++;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)

int privateTXORSmallGetBit2(int adrBit)
{
  //��������� ������� ��������
  int count_bit = BIT_FOR_OBJ*TOTAL_OBJ;
  if(adrBit>=BEGIN_ADR_BIT && adrBit<(BEGIN_ADR_BIT+count_bit)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)

void config_and_settingsTXORSmall(void)
{
//action ���������
}

