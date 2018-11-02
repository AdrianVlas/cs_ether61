#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 50000
//��������� bit � ����� ������
#define BEGIN_ADR_BIT 51000
#define BIT_FOR_OBJ 1

int privateT1SmallGetReg2(int adrReg);
int privateT1SmallGetBit2(int adrBit);

int getT1SmallModbusRegister(int);//�������� ���������� ��������
int getT1SmallModbusBit(int);//�������� ���������� ����
int setT1SmallModbusRegister(int, int);//�������� ���������� ��������
int setT1SmallModbusBit(int, int);//�������� ���������� ����

void setT1SmallCountObject(void);//�������� �-�� �������
void preT1SmallReadAction(void);//action �� ������
void postT1SmallReadAction(void);//action ����� ������
void preT1SmallWriteAction(void);//action �� ������
int  postT1SmallWriteAction(void);//action ����� ������
void loadT1SmallActualData(void);
void config_and_settingsT1Small(void);//action ���������

COMPONENT_OBJ *t1smallcomponent;

/**************************************/
//���������� ���������� ��������������� AND
/**************************************/
void constructorT1SmallComponent(COMPONENT_OBJ *t1smallcomp)
{
  t1smallcomponent = t1smallcomp;

//  t1smallcomponent->countObject = 128;//�-�� �������

  t1smallcomponent->getModbusRegister = getT1SmallModbusRegister;//�������� ���������� ��������
  t1smallcomponent->getModbusBit      = getT1SmallModbusBit;//�������� ���������� ����
  t1smallcomponent->setModbusRegister = setT1SmallModbusRegister;//�������� ���������� ��������
  t1smallcomponent->setModbusBit      = setT1SmallModbusBit;//�������� ���������� ����

  t1smallcomponent->preReadAction   = preT1SmallReadAction;//action �� ������
  t1smallcomponent->postReadAction  = postT1SmallReadAction;//action ����� ������
  t1smallcomponent->preWriteAction  = preT1SmallWriteAction;//action �� ������
  t1smallcomponent->postWriteAction = postT1SmallWriteAction;//action ����� ������
  t1smallcomponent->config_and_settings = config_and_settingsT1Small;//action ���������

  t1smallcomponent->isActiveActualData = 0;
}//constructorT1SmallComponent(COMPONENT_OBJ *t1smallcomp)

void loadT1SmallActualData(void) {
  //ActualData
extern int pointInterface;//����� ���������� 0-USB 1-RS485
//   int cnt_treg = t1smallcomponent->countObject/16;
//   if(t1smallcomponent->countObject%16) cnt_treg++;
   for(int ii=0; ii<8*4; ii++) tempReadArray[ii] = 0;

   int ireg_offset =-8;
   {
   __LN_AND *arr = (__LN_AND*)(spca_of_p_prt[ID_FB_AND - _ID_FB_FIRST_VAR]);
   ireg_offset +=8;
   for(int item=0; item<128; item++) {
   int ireg = ireg_offset+item/16;
   int value = arr[item].active_state[0];
   int tdata = 0;
   if(value) tdata=1;
   tempReadArray[ireg] |= tdata<<(item%16);
  }//for
  }
  {
   __LN_OR *arr = (__LN_OR*)(spca_of_p_prt[ID_FB_OR - _ID_FB_FIRST_VAR]);
   ireg_offset +=8;
   for(int item=0; item<128; item++) {
   int ireg = ireg_offset+item/16;
   int value = arr[item].active_state[0];
   int tdata = 0;
   if(value) tdata=1;
   tempReadArray[ireg] |= tdata<<(item%16);
  }//for
  }
  {
   __LN_XOR *arr = (__LN_XOR*)(spca_of_p_prt[ID_FB_XOR - _ID_FB_FIRST_VAR]);
   ireg_offset +=8;
   for(int item=0; item<128; item++) {
   int ireg = ireg_offset+item/16;
   int value = arr[item].active_state[0];
   int tdata = 0;
   if(value) tdata=1;
   tempReadArray[ireg] |= tdata<<(item%16);
  }//for
  }
  {
   __LN_NOT *arr = (__LN_NOT*)(spca_of_p_prt[ID_FB_NOT - _ID_FB_FIRST_VAR]);
   ireg_offset +=8;
   for(int item=0; item<128; item++) {
   int ireg = ireg_offset+item/16;
   int value = arr[item].active_state[0];
   int tdata = 0;
   if(value) tdata=1;
   tempReadArray[ireg] |= tdata<<(item%16);
  }//for
  }
  {
   __LN_TRIGGER *arr = (__LN_TRIGGER*)(spca_of_p_prt[ID_FB_TRIGGER - _ID_FB_FIRST_VAR]);
   ireg_offset +=8;
   for(int item=0; item<128; item++) {
   int ireg = ireg_offset+item/16;
   int value = arr[item].active_state[0];
   int tdata = 0;
   if(value) tdata=1;
   tempReadArray[ireg] |= tdata<<(item%16);
  }//for
  }

}//loadTANDSmallActualData() 

int getT1SmallModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateT1SmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(t1smallcomponent->isActiveActualData) loadT1SmallActualData(); //ActualData
  t1smallcomponent->isActiveActualData = 0;

  superSetOperativMarker(t1smallcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getT1SmallModbusRegister(int adrReg)
int getT1SmallModbusBit(int adrBit)
{
  //�������� ���������� bit
  if(privateT1SmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(t1smallcomponent->isActiveActualData) loadT1SmallActualData();
  t1smallcomponent->isActiveActualData = 0;

  superSetOperativMarker(t1smallcomponent, adrBit);

  short tmp   = tempReadArray[(adrBit-BEGIN_ADR_BIT)/16];
  short maska = 1<<((adrBit-BEGIN_ADR_BIT)%16);
  if(tmp&maska) return 1;
  return 0;
}//getT1SmallModbusBit(int adrBit)
int setT1SmallModbusRegister(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//setT1SmallModbusRegister(int x, int y)
int setT1SmallModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//setT1SmallModbusBit(int x, int y)

void preT1SmallReadAction(void) {
//action �� ������
  t1smallcomponent->operativMarker[0] = -1;
  t1smallcomponent->operativMarker[1] = -1;//����������� ������
  t1smallcomponent->isActiveActualData = 1;
}//
void postT1SmallReadAction(void) {
//action ����� ������
  if(t1smallcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preT1SmallWriteAction(void) {
//action �� ������
  t1smallcomponent->operativMarker[0] = -1;
  t1smallcomponent->operativMarker[1] = -1;//����������� ������
  t1smallcomponent->isActiveActualData = 1;
}//
int postT1SmallWriteAction(void) {
//action ����� ������
 return 0;
}//

int privateT1SmallGetReg2(int adrReg)
{
  //��������� ������� ��������
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+8*5)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)

int privateT1SmallGetBit2(int adrBit)
{
  //��������� ������� ��������
  if(adrBit>=BEGIN_ADR_BIT && adrBit<(BEGIN_ADR_BIT+8*5*16)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)

void config_and_settingsT1Small(void)
{
//action ���������
}

