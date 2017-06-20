#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 0
#define REGISTER_FOR_OBJ 16

int getIUModbusRegister(int);//�������� ���������� ��������
int getIUModbusBit(int);//�������� ���������� ����
int setIUModbusRegister(int, int);//�������� ���������� ��������
int setIUModbusBit(int, int);//�������� ���������� ����

void setIUSmallCountObject(int);//�������� �-�� �������
void preIUSmallReadAction(void);//action �� ������
void postIUSmallReadAction(void);//action ����� ������
void preIUSmallWriteAction(void);//action �� ������
void postIUSmallWriteAction(void);//action ����� ������
void loadIUSmallActualData(void);

int privateIUGetReg2(int adrReg);

COMPONENT_OBJ *iucomponent;

/**************************************/
//���������� ���������� ������������� ����������
/**************************************/
void constructorIUSmallComponent(COMPONENT_OBJ *iucomp)
{
  iucomponent = iucomp;

  iucomponent->countObject = 1;//�-�� �������

  iucomponent->getModbusRegister = getIUModbusRegister;//�������� ���������� ��������
  iucomponent->getModbusBit      = getIUModbusBit;//�������� ���������� ����
  iucomponent->setModbusRegister = setIUModbusRegister;//�������� ���������� ��������
  iucomponent->setModbusBit      = setIUModbusBit;//�������� ���������� ����

  iucomponent->setCountObject  = setIUSmallCountObject;//�������� �-�� �������
  iucomponent->preReadAction   = preIUSmallReadAction;//action �� ������
  iucomponent->postReadAction  = postIUSmallReadAction;//action ����� ������
  iucomponent->preWriteAction  = preIUSmallWriteAction;//action �� ������
  iucomponent->postWriteAction = postIUSmallWriteAction;//action ����� ������

  iucomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadIUSmallActualData(void) {
  //ActualData
  for(int i=0; i<100; i++) tempReadArray[i] = i+22;
  /*
  if(address_data <= M_ADDRESS_LAST_RO_BLOCK_1)
  {
    //������������� �������
    switch (address_data)
    {
    case MA_SERIAL_NUMBER:
      {
        temp_value = serial_number_dev;
        break;
      }
    case MA_NAME_CHAR_1_2:
      {
        temp_value = ('i' << 8) + 'K';
        break;
      }
    case MA_NAME_CHAR_3_4:
      {
        temp_value = ('v' << 8) + 'e';
        break;
      }
    case MA_NAME_CHAR_5_6:
      {
        temp_value = ('r' << 8) + 'p';
        break;
      }
    case MA_NAME_CHAR_7_8:
      {
        temp_value = ('b' << 8) + 'i';
        break;
      }
    case MA_NAME_CHAR_9_10:
      {
        temp_value = ('r' << 8) + 'o';
        break;
      }
    case MA_NAME_CHAR_11_12:
      {
        temp_value = ('R' << 8) + 'M';
        break;
      }
    case MA_NAME_CHAR_13_14:
      {
        temp_value = ('S' << 8) + 'Z';
        break;
      }
    case MA_NAME_CHAR_15_16:
      {
        temp_value = ('5' << 8) + '0';
        break;
      }
    case MA_NAME_CHAR_17_18:
      {
        temp_value = ('_' << 8) + 'L';
        break;
      }
    case MA_VERSION_SW:
      {
        temp_value = (VERSIA_PZ << 8) + MODYFIKACIA_VERSII_PZ;
        break;
      }
    case MA_DATA_COMP_1:
      {
        temp_value = (YEAR_VER << 8) + MONTH_VER;
        break;
      }
    case MA_DATA_COMP_2:
      {
        temp_value = (DAY_VER << 8);
        break;
      }
    case MA_TIME_COMP:
      {
        temp_value = (HOUR_VER << 8) + MINUTE_VER;
        break;
      }
    case MA_VERSION_GMM:
      {
        temp_value = (VERSIA_GMM << 8) + MODYFIKACIA_VERSII_GMM;
        break;
      }
    case MA_ZBIRKA_SW:
      {
        temp_value = ZBIRKA_VERSII_PZ;
        break;
      }
    default:
      {
        //�� ���� �� � ���� ������������ ���� ��������� ������
        temp_value = 0;
        break;
      }
    }
  }
  */
//  tempReadArray[0] = 22;
}//loadActualData() 

int getIUModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateIUGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  if(iucomponent->isActiveActualData) loadIUSmallActualData(); //ActualData
  iucomponent->isActiveActualData = 0;

  superSetOperativMarker(iucomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDVModbusRegister(int adrReg)
int getIUModbusBit(int adrBit) {
  //�������� ���������� bit
  superSetOperativMarker(iucomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//getIUModbusBit(int )
int setIUModbusRegister(int adrReg, int x) {
  UNUSED(x);
  //�������� ���������� ��������
  superSetOperativMarker(iucomponent, adrReg);
  return MARKER_OUTPERIMETR;
}//setIUModbusRegister(int, int)
int setIUModbusBit(int adrBit, int x) {
  UNUSED(x);
  //�������� ���������� bit
  superSetOperativMarker(iucomponent, adrBit);
  return MARKER_OUTPERIMETR;
}//setIUModbusBit(int, int )

void setIUSmallCountObject(int x) {
  UNUSED(x);
//�������� �-�� �������
}//
void preIUSmallReadAction(void) {
//action �� ������
  iucomponent->operativMarker[0] = -1;
  iucomponent->operativMarker[1] = -1;//����������� ������
  iucomponent->isActiveActualData = 1;
}//
void postIUSmallReadAction(void) {
//action ����� ������
  if(iucomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preIUSmallWriteAction(void) {
//action �� ������
  iucomponent->operativMarker[0] = -1;
  iucomponent->operativMarker[1] = -1;//����������� ������
  iucomponent->isActiveActualData = 1;
}//
void postIUSmallWriteAction(void) {
//action ����� ������
}//

int privateIUGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)

