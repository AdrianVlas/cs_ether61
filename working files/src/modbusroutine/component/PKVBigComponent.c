#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 20200
//���� �-�� �������
#define REGISTER_FOR_OBJ 43

int privatePKVBigGetReg2(int adrReg);

int getPKVBigModbusRegister(int);//�������� ���������� ��������
int getPKVBigModbusBit(int);//�������� ���������� ����
int setPKVBigModbusRegister(int, int);//�������� ���������� ��������
int setPKVBigModbusBit(int, int);//�������� ���������� ����

void prePKVBigReadAction(void);//action �� ������
void postPKVBigReadAction(void);//action ����� ������
void prePKVBigWriteAction(void);//action �� ������
int postPKVBigWriteAction(void);//action ����� ������
void loadPKVBigActualData(void);
void config_and_settingsPKVBig(void);//action ���������

COMPONENT_OBJ *pkvbigcomponent;

/**************************************/
//���������� ���������� ������ ������������ �����
/**************************************/
void constructorPKVBigComponent(COMPONENT_OBJ *pkvbigcomp)
{
  pkvbigcomponent = pkvbigcomp;

  pkvbigcomponent->countObject = 1;//�-�� �������

  pkvbigcomponent->getModbusRegister = getPKVBigModbusRegister;//�������� ���������� ��������
  pkvbigcomponent->getModbusBit      = getPKVBigModbusBit;//�������� ���������� ����
  pkvbigcomponent->setModbusRegister = setPKVBigModbusRegister;//�������� ���������� ��������
  pkvbigcomponent->setModbusBit      = setPKVBigModbusBit;//�������� ���������� ����

  pkvbigcomponent->preReadAction   = prePKVBigReadAction;//action �� ������
  pkvbigcomponent->postReadAction  = postPKVBigReadAction;//action ����� ������
  pkvbigcomponent->preWriteAction  = prePKVBigWriteAction;//action �� ������
  pkvbigcomponent->postWriteAction = postPKVBigWriteAction;//action ����� ������
  pkvbigcomponent->config_and_settings = config_and_settingsPKVBig;//action ���������

  pkvbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

void loadPKVBigActualData(void) {
  //ActualData
    unsigned char *label_to_time_array;
    if (copying_time == 2) label_to_time_array = time_copy;
    else label_to_time_array = time;
    
  for(int i=0; i<REGISTER_FOR_OBJ; i++) {
   switch(i) {
    case 0://����� ��������� ������ ����� �������
    tempReadArray[i] = settings_fix.timeout_deactivation_password_interface_USB;
    break;
    case 1://��������/��������� ������
    tempReadArray[i] = 0;
    break;
    case 2://����-��� ���������� ���������
    tempReadArray[i] = settings_fix.timeout_idle_new_settings;
    break;
    case 3://���� ����������������� ����������
    tempReadArray[i] = settings_fix.language;
    break;
    case 4://
    tempReadArray[i] = 0;
    break;
    case 5://�������� ����� �����
    tempReadArray[i] = settings_fix.baud_RS485;
    break;
    case 6://���������� ����-���
    tempReadArray[i] = 1+settings_fix.number_stop_bit_RS485;
    break;
    case 7://�������
    tempReadArray[i] = settings_fix.pare_bit_RS485;
    break;
    case 8://�������� �����
    tempReadArray[i] = settings_fix.time_out_1_RS485;
    break;
//    case 9://�������� ����� ����� 2
//    tempReadArray[i] = settings_fix.baud_RS485;
//    break;
//    case 10://���������� ����-��� 2
//    tempReadArray[i] = settings_fix.number_stop_bit_RS485;
//    break;
//    case 11://������� 2
//    tempReadArray[i] = settings_fix.pare_bit_RS485;
//    break;
//    case 12://�������� ����� 2
//    tempReadArray[i] = settings_fix.time_out_1_RS485;
    case 9://
    case 10://
    case 11://
    case 12://
     tempReadArray[i] = 0;
    break;

    case 13://����� ���������� � ����
    tempReadArray[i] = settings_fix.address;
    break;
    case 14://������� ����� ������
     tempReadArray[i] = 0;
    break;
    case 15://���� TCP
    tempReadArray[i] = 0;
    break;

    case 16://IP ����� ����������
    tempReadArray[i] = settings_fix.IPv4[0]&0xff;
    tempReadArray[i] |= ((settings_fix.IPv4[1]&0xff)<<8)&0xFF00;
    break;
    case 17://IP ����� ����������
    tempReadArray[i] = settings_fix.IPv4[2]&0xff;
    tempReadArray[i] |= ((settings_fix.IPv4[3]&0xff)<<8)&0xFF00;
    break;
    case 18://����� �������
    tempReadArray[i] = settings_fix.mask&0xff;
    break;
    case 19://���� ��-���������
    tempReadArray[i] = settings_fix.gateway[0]&0xff;
    tempReadArray[i] |= ((settings_fix.gateway[1]&0xff)<<8)&0xFF00;
    break;
    case 20://���� ��-���������
    tempReadArray[i] = settings_fix.gateway[2]&0xff;
    tempReadArray[i] |= ((settings_fix.gateway[3]&0xff)<<8)&0xFF00;
    break;
    case 21://������������ DHCP (0=���, 1=��)
    tempReadArray[i] = 0;
    break;
    case 22://
    tempReadArray[i] = 0;
    break;

    case 23://������ 1 � 2
    tempReadArray[i] = settings_fix.name_of_cell[0] + ((settings_fix.name_of_cell[1]<<8)&0xFF00);
    break;
    case 24://������ 3 � 4
    tempReadArray[i] = settings_fix.name_of_cell[2] + ((settings_fix.name_of_cell[3]<<8)&0xFF00);
    break;
    case 25://������ 5 � 6
    tempReadArray[i] = settings_fix.name_of_cell[4] + ((settings_fix.name_of_cell[5]<<8)&0xFF00);
    break;
    case 26://������ 7 � 8
    tempReadArray[i] = settings_fix.name_of_cell[6] + ((settings_fix.name_of_cell[7]<<8)&0xFF00);
    break;
    case 27://������ 9 � 10
    tempReadArray[i] = settings_fix.name_of_cell[8] + ((settings_fix.name_of_cell[9]<<8)&0xFF00);
    break;
    case 28://������ 11 � 12
    tempReadArray[i] = settings_fix.name_of_cell[10] + ((settings_fix.name_of_cell[11]<<8)&0xFF00);
    break;
    case 29://������ 13 � 14
    tempReadArray[i] = settings_fix.name_of_cell[12] + ((settings_fix.name_of_cell[13]<<8)&0xFF00);
    break;
    case 30://������ 15 � 16
    tempReadArray[i] = settings_fix.name_of_cell[14] + ((settings_fix.name_of_cell[15]<<8)&0xFF00);
    break;

    case 31://���
    tempReadArray[i] = *(label_to_time_array + 6);
    break;
    case 32://�����
    tempReadArray[i] = *(label_to_time_array + 5);
    break;
    case 33://����
    tempReadArray[i] = *(label_to_time_array + 4);
    break;
    case 34://���
    tempReadArray[i] = *(label_to_time_array + 3);
    break;
    case 35://������
    tempReadArray[i] = *(label_to_time_array + 2);
    break;
    case 36://�������
    tempReadArray[i] = *(label_to_time_array + 1);
    break;
    case 37://����� �������
    tempReadArray[i] = *(label_to_time_array + 0);
    break;

    case 38://������� ����
    case 39://������� �� ������/������ �����
    case 40://����� �������� �� ������ �����
    case 41://������ ������ �������� �� ������ �����
    case 42://���� ������ �������� �� ������ �����
    case 43://��� ������ �������� �� ������ �����
    case 44://����� �������� �� ������ �����
    case 45://������ ������ �������� �� ������ �����
    case 46://���� ������ �������� �� ������ �����
    case 47://��� ������ �������� �� ������ �����
    case 48://������������� �������
    case 49://����� ������ �������������
    tempReadArray[i] = 0;
    break;
   }//switch
  }//for
  /*
  �������/����� ��������� � ���� (�������� ����� � ���� type_definition.h ��� ��������� ���� __CONFIG)
  settings_fix_prt/settings_fix/settings_fix_edit
  
  14200 - timeout_deactivation_password_interface_USB/timeout_deactivation_password_interface_RS485
  14201 - password_interface_USB/password_interface_RS485 (����� �����. ���. ���� � �� ����)
  14202 - timeout_idle_new_settings (�������, �� � ���� ���'��)
  6 - �� (0x0-0x99)
  
   ��� ����� �� ����� 0 (���������) � �������������� ����� ���� � ����. �������� � �������� ������ ��� ������
  
  ��� ������ �������� ��������� �������� � time_edit � �������� ����� ���
            if (check_data_for_data_time_menu() == 1)
            {
              //��� ��������
              //����������� ����������� ������ ���� � RTC
              //��� ����� ����������� �� ���������� ��������� ������� ��������, ��� ����������������� � ������� ����������� �������
              _SET_BIT(control_i2c_taskes, TASK_START_WRITE_RTC_BIT);
              _SET_BIT(control_i2c_taskes, TASK_BLK_OPERATION_BIT);
            }
            else error = ERROR_ILLEGAL_DATA_VALUE;  
  ------------
  */
}//loadActualData() 

int getPKVBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privatePKVBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if((adrReg-BEGIN_ADR_REGISTER)==1) return MARKER_ERRORPERIMETR;

  if(pkvbigcomponent->isActiveActualData) loadPKVBigActualData(); //ActualData
  pkvbigcomponent->isActiveActualData = 0;

  superSetOperativMarker(pkvbigcomponent, adrReg);

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDOUTBigModbusRegister(int adrReg)
int getPKVBigModbusBit(int x)
{
  //�������� ���������� ��������
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setPKVBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privatePKVBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  superSetOperativMarker(pkvbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  switch(adrReg-BEGIN_ADR_REGISTER) {
   case 0: //14000
    if(dataReg>3600) return MARKER_ERRORDIAPAZON;
    if(dataReg<30) return MARKER_ERRORDIAPAZON;
   break; 
   case 1: //14001
   break; 
   case 2: //14002
    if(dataReg>3600) return MARKER_ERRORDIAPAZON;
    if(dataReg<60) return MARKER_ERRORDIAPAZON;
   break; 
   case 3: //14003
    if(dataReg>5) return MARKER_ERRORDIAPAZON;
    if(dataReg<1) return MARKER_ERRORDIAPAZON;
   break; 
   case 4: //14004
   break; 
   case 5: //14005
    if(dataReg>6) return MARKER_ERRORDIAPAZON;
   break; 
   case 6: //14006
    if(dataReg>2) return MARKER_ERRORDIAPAZON;
    if(dataReg<1) return MARKER_ERRORDIAPAZON;
   break; 
   case 7: //14007
    if(dataReg>2) return MARKER_ERRORDIAPAZON;
   break; 
   case 8: //14008
    if(dataReg>1500) return MARKER_ERRORDIAPAZON;
    if(dataReg<15) return MARKER_ERRORDIAPAZON;
   break; 
   case 9: //14009
    if(dataReg>6) return MARKER_ERRORDIAPAZON;
   break; 
   case 10: //14010
    if(dataReg>2) return MARKER_ERRORDIAPAZON;
    if(dataReg<1) return MARKER_ERRORDIAPAZON;
   break; 
   case 11: //14011
    if(dataReg>2) return MARKER_ERRORDIAPAZON;
   break; 
   case 12: //14012
    if(dataReg>1500) return MARKER_ERRORDIAPAZON;
    if(dataReg<15) return MARKER_ERRORDIAPAZON;
   break; 
   case 13: //14013
    if(dataReg>247) return MARKER_ERRORDIAPAZON;
    if(dataReg<1) return MARKER_ERRORDIAPAZON;
   break; 
   case 14: //14014
    if(dataReg<3) return MARKER_ERRORDIAPAZON;
   break; 
   case 15: //14015
   case 16: //14015
   case 17: //14015
   case 18: //14015
    if(dataReg>32) return MARKER_ERRORDIAPAZON;
    if(dataReg<0) return MARKER_ERRORDIAPAZON;
    break;
   case 19: //14015
   case 20: //14015
   case 21: //14015
   case 22: //14015
   case 23: //14015
   case 24: //14015
   case 25: //14015
   case 26: //14015
   case 27: //14015
   case 28: //14015
   case 29: //14015
   case 30: //14015
   break; 
   case 31: //14231
    if(dataReg&0xff00) return MARKER_ERRORDIAPAZON;
    if(dataReg>0x99) return MARKER_ERRORDIAPAZON;
    if((dataReg&0xf)>9) return MARKER_ERRORDIAPAZON;
   break; 
   case 32: //14232
    if(dataReg&0xff00) return MARKER_ERRORDIAPAZON;
    if(dataReg==0) return MARKER_ERRORDIAPAZON;
    if(dataReg>0x12) return MARKER_ERRORDIAPAZON;
    if((dataReg&0xf)>9) return MARKER_ERRORDIAPAZON;
//    if(dataReg==0xA || dataReg==0xB || dataReg==0xC || dataReg==0xD || dataReg==0xE || dataReg==0xF) return MARKER_ERRORDIAPAZON;
   break; 
   case 33: //14233
    if(dataReg&0xff00) return MARKER_ERRORDIAPAZON;
    if(dataReg==0) return MARKER_ERRORDIAPAZON;
    if(dataReg>0x31) return MARKER_ERRORDIAPAZON;
    if((dataReg&0xf)>9) return MARKER_ERRORDIAPAZON;
   break; 
   case 34: //14234
    if(dataReg&0xff00) return MARKER_ERRORDIAPAZON;
    if(dataReg>0x23) return MARKER_ERRORDIAPAZON;
    if((dataReg&0xf)>9) return MARKER_ERRORDIAPAZON;
   break; 
   case 35: //14235
   case 36: //14236
    if(dataReg&0xff00) return MARKER_ERRORDIAPAZON;
    if(dataReg>0x59) return MARKER_ERRORDIAPAZON;
    if((dataReg&0xf)>9) return MARKER_ERRORDIAPAZON;
   break; 
   case 37: //14237
    if(dataReg&0xff00) return MARKER_ERRORDIAPAZON;
    if(dataReg>0x99) return MARKER_ERRORDIAPAZON;
    if((dataReg&0xf)>9) return MARKER_ERRORDIAPAZON;
   break; 
   case 38: //14038
//    if(dataReg>32767) dataReg -=32768;
    if(dataReg>720) return MARKER_ERRORDIAPAZON;
    if(dataReg<-720) return MARKER_ERRORDIAPAZON;
   break; 
   case 39: //14039
    if(dataReg&0xff00) return MARKER_ERRORDIAPAZON;
    if(dataReg>1) return MARKER_ERRORDIAPAZON;
   break; 
   case 40: //14040
    if(dataReg>12) return MARKER_ERRORDIAPAZON;
    if(dataReg<1) return MARKER_ERRORDIAPAZON;
   break; 
   case 41: //14041
    if(dataReg>5) return MARKER_ERRORDIAPAZON;
    if(dataReg<1) return MARKER_ERRORDIAPAZON;
   break; 
   case 42: //14042
    if(dataReg>7) return MARKER_ERRORDIAPAZON;
    if(dataReg<1) return MARKER_ERRORDIAPAZON;
   break; 
   case 43: //14043
    if(dataReg>23) return MARKER_ERRORDIAPAZON;
   break; 
   case 44: //14044
    if(dataReg>12) return MARKER_ERRORDIAPAZON;
    if(dataReg<1) return MARKER_ERRORDIAPAZON;
   break; 
   case 45: //14045
    if(dataReg>5) return MARKER_ERRORDIAPAZON;
    if(dataReg<1) return MARKER_ERRORDIAPAZON;
   break; 
   case 46: //14046
    if(dataReg>7) return MARKER_ERRORDIAPAZON;
    if(dataReg<1) return MARKER_ERRORDIAPAZON;
   break; 
   case 47: //14047
    if(dataReg>23) return MARKER_ERRORDIAPAZON;
   break; 
   case 48: //14048
    if(dataReg>2) return MARKER_ERRORDIAPAZON;
   break; 
   case 49: //14049
   break; 
  default: return MARKER_OUTPERIMETR;
  }//switch

  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int setPKVBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void prePKVBigReadAction(void) {
//action �� ������
  pkvbigcomponent->operativMarker[0] = -1;
  pkvbigcomponent->operativMarker[1] = -1;//����������� ������
  pkvbigcomponent->isActiveActualData = 1;
}//
void postPKVBigReadAction(void) {
//action ����� ������
  if(pkvbigcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void prePKVBigWriteAction(void) {
//action �� ������
  pkvbigcomponent->operativMarker[0] = -1;
  pkvbigcomponent->operativMarker[1] = -1;//����������� ������
  pkvbigcomponent->isActiveActualData = 1;
}//
int postPKVBigWriteAction(void) {
//action ����� ������
int flag_time_array = 0;
  if(pkvbigcomponent->operativMarker[0]<0) return 0;//�� ���� ������
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
  int countRegister = pkvbigcomponent->operativMarker[1]-pkvbigcomponent->operativMarker[0]+1;
  if(pkvbigcomponent->operativMarker[1]<0) countRegister = 1;

    unsigned char *label_to_time_array = time_edit;
    for(int i=0; i<7; i++) time_edit[i] = time[i];
   // if (copying_time == 0) label_to_time_array = time;
  //  else label_to_time_array = time_copy;
  
  __SETTINGS_FIX *arr = &settings_fix, *arr1 = &settings_fix_edit;
  for(int i=0; i<countRegister; i++) {
  int offset = i+pkvbigcomponent->operativMarker[0]-BEGIN_ADR_REGISTER;
  switch(offset) {//������ �������� 
    case 0://����� ��������� ������ ����� �������
    arr1->timeout_deactivation_password_interface_USB = arr->timeout_deactivation_password_interface_USB = (tempWriteArray[offsetTempWriteArray+i]);
    config_settings_modified |= MASKA_FOR_BIT(BIT_CHANGED_SETTINGS);
    restart_timeout_idle_new_settings = true;
    break;
    case 1://��������/��������� ������
    //tempReadArray[i] = 0;
    config_settings_modified |= MASKA_FOR_BIT(BIT_CHANGED_SETTINGS);
    restart_timeout_idle_new_settings = true;
    break;
    case 2://����-��� ���������� ���������
    arr1->timeout_idle_new_settings = arr->timeout_idle_new_settings = (tempWriteArray[offsetTempWriteArray+i]);
    config_settings_modified |= MASKA_FOR_BIT(BIT_CHANGED_SETTINGS);
    restart_timeout_idle_new_settings = true;
    break;
    case 3://���� ����������������� ����������
    arr1->language = arr->language = (tempWriteArray[offsetTempWriteArray+i]);
    config_settings_modified |= MASKA_FOR_BIT(BIT_CHANGED_SETTINGS);
    restart_timeout_idle_new_settings = true;
    break;
    case 4://
    //tempReadArray[i] = 0;
//    config_settings_modified |= MASKA_FOR_BIT(BIT_CHANGED_SETTINGS);
//    restart_timeout_idle_new_settings = true;
    break;
    case 5://�������� ����� �����
    arr1->baud_RS485 = arr->baud_RS485 = (tempWriteArray[offsetTempWriteArray+i]);
    config_settings_modified |= MASKA_FOR_BIT(BIT_CHANGED_SETTINGS);
    restart_timeout_idle_new_settings = true;
    break;
    case 6://���������� ����-���
    arr1->number_stop_bit_RS485 = arr->number_stop_bit_RS485 = (tempWriteArray[offsetTempWriteArray+i])-1;
    config_settings_modified |= MASKA_FOR_BIT(BIT_CHANGED_SETTINGS);
    restart_timeout_idle_new_settings = true;
    break;
    case 7://�������
    arr1->pare_bit_RS485 = arr->pare_bit_RS485 = (tempWriteArray[offsetTempWriteArray+i]);
    config_settings_modified |= MASKA_FOR_BIT(BIT_CHANGED_SETTINGS);
    restart_timeout_idle_new_settings = true;
    break;
    case 8://�������� �����
//    tempReadArray[i] = settings_fix.time_out_1_RS485;
    arr1->time_out_1_RS485 = arr->time_out_1_RS485 = (tempWriteArray[offsetTempWriteArray+i]);
    config_settings_modified |= MASKA_FOR_BIT(BIT_CHANGED_SETTINGS);
    restart_timeout_idle_new_settings = true;
    break;
//    case 9://�������� ����� ����� 2
//    tempReadArray[i] = settings_fix.baud_RS485;
//    break;
//    case 10://���������� ����-��� 2
//    tempReadArray[i] = settings_fix.number_stop_bit_RS485;
//    break;
//    case 11://������� 2
//    tempReadArray[i] = settings_fix.pare_bit_RS485;
//    break;
//    case 12://�������� ����� 2
//    tempReadArray[i] = settings_fix.time_out_1_RS485;
    case 9://
    case 10://
    case 11://
    case 12://
    // tempReadArray[i] = 0;
//    config_settings_modified |= MASKA_FOR_BIT(BIT_CHANGED_SETTINGS);
//    restart_timeout_idle_new_settings = true;
    break;

    case 13://����� ���������� � ����
    arr1->address = arr->address = (tempWriteArray[offsetTempWriteArray+i]);
    config_settings_modified |= MASKA_FOR_BIT(BIT_CHANGED_SETTINGS);
    restart_timeout_idle_new_settings = true;
    break;
    case 14://������� ����� ������
    // tempReadArray[i] = 0;
//    config_settings_modified |= MASKA_FOR_BIT(BIT_CHANGED_SETTINGS);
//    restart_timeout_idle_new_settings = true;
    break;
    case 15://���� TCP
    break;
    case 16://IP ����� ����������
     arr1->IPv4[0] = tempWriteArray[offsetTempWriteArray+i] & 0xff;
     arr1->IPv4[1] = (tempWriteArray[offsetTempWriteArray+i]>>8) & 0xff;
     config_settings_modified |= MASKA_FOR_BIT(BIT_CHANGED_SETTINGS);
     restart_timeout_idle_new_settings = true;
    break;
    case 17://IP ����� ����������
     arr1->IPv4[2] = tempWriteArray[offsetTempWriteArray+i] & 0xff;
     arr1->IPv4[3] = (tempWriteArray[offsetTempWriteArray+i]>>8) & 0xff;
     config_settings_modified |= MASKA_FOR_BIT(BIT_CHANGED_SETTINGS);
     restart_timeout_idle_new_settings = true;
    break;

    case 18://����� �������
     arr1->mask = tempWriteArray[offsetTempWriteArray+i] & 0xff;
     config_settings_modified |= MASKA_FOR_BIT(BIT_CHANGED_SETTINGS);
     restart_timeout_idle_new_settings = true;
    break;

    case 19://���� ��-���������
     arr1->gateway[0] = tempWriteArray[offsetTempWriteArray+i] & 0xff;
     arr1->gateway[1] = (tempWriteArray[offsetTempWriteArray+i]>>8) & 0xff;
     config_settings_modified |= MASKA_FOR_BIT(BIT_CHANGED_SETTINGS);
     restart_timeout_idle_new_settings = true;
    break;
    case 20://���� ��-���������
     arr1->gateway[2] = tempWriteArray[offsetTempWriteArray+i] & 0xff;
     arr1->gateway[3] = (tempWriteArray[offsetTempWriteArray+i]>>8) & 0xff;
     config_settings_modified |= MASKA_FOR_BIT(BIT_CHANGED_SETTINGS);
     restart_timeout_idle_new_settings = true;
    break;

    case 23://������ 1 � 2
     arr1->name_of_cell[0] = arr->name_of_cell[0] = (tempWriteArray[offsetTempWriteArray+i]);
     arr1->name_of_cell[1] = arr->name_of_cell[1] = (tempWriteArray[offsetTempWriteArray+i])>>8;
    config_settings_modified |= MASKA_FOR_BIT(BIT_CHANGED_SETTINGS);
    restart_timeout_idle_new_settings = true;
    break;
    case 24://������ 3 � 4
     arr1->name_of_cell[2] = arr->name_of_cell[2] = (tempWriteArray[offsetTempWriteArray+i]);
     arr1->name_of_cell[3] = arr->name_of_cell[3] = (tempWriteArray[offsetTempWriteArray+i])>>8;
    config_settings_modified |= MASKA_FOR_BIT(BIT_CHANGED_SETTINGS);
    restart_timeout_idle_new_settings = true;
    break;
    case 25://������ 5 � 6
     arr1->name_of_cell[4] = arr->name_of_cell[4] = (tempWriteArray[offsetTempWriteArray+i]);
     arr1->name_of_cell[5] = arr->name_of_cell[5] = (tempWriteArray[offsetTempWriteArray+i])>>8;
    config_settings_modified |= MASKA_FOR_BIT(BIT_CHANGED_SETTINGS);
    restart_timeout_idle_new_settings = true;
    break;
    case 26://������ 7 � 8
     arr1->name_of_cell[6] = arr->name_of_cell[6] = (tempWriteArray[offsetTempWriteArray+i]);
     arr1->name_of_cell[7] = arr->name_of_cell[7] = (tempWriteArray[offsetTempWriteArray+i])>>8;
    config_settings_modified |= MASKA_FOR_BIT(BIT_CHANGED_SETTINGS);
    restart_timeout_idle_new_settings = true;
    break;
    case 27://������ 9 � 10
     arr1->name_of_cell[8] = arr->name_of_cell[8] = (tempWriteArray[offsetTempWriteArray+i]);
     arr1->name_of_cell[9] = arr->name_of_cell[9] = (tempWriteArray[offsetTempWriteArray+i])>>8;
    config_settings_modified |= MASKA_FOR_BIT(BIT_CHANGED_SETTINGS);
    restart_timeout_idle_new_settings = true;
    break;
    case 28://������ 11 � 12
     arr1->name_of_cell[10] = arr->name_of_cell[10] = (tempWriteArray[offsetTempWriteArray+i]);
     arr1->name_of_cell[11] = arr->name_of_cell[11] = (tempWriteArray[offsetTempWriteArray+i])>>8;
    config_settings_modified |= MASKA_FOR_BIT(BIT_CHANGED_SETTINGS);
    restart_timeout_idle_new_settings = true;
    break;
    case 29://������ 13 � 14
     arr1->name_of_cell[12] = arr->name_of_cell[12] = (tempWriteArray[offsetTempWriteArray+i]);
     arr1->name_of_cell[13] = arr->name_of_cell[13] = (tempWriteArray[offsetTempWriteArray+i])>>8;
    config_settings_modified |= MASKA_FOR_BIT(BIT_CHANGED_SETTINGS);
    restart_timeout_idle_new_settings = true;
    break;
    case 30://������ 15 � 16
     arr1->name_of_cell[14] = arr->name_of_cell[14] = (tempWriteArray[offsetTempWriteArray+i]);
     arr1->name_of_cell[15] = arr->name_of_cell[15] = (tempWriteArray[offsetTempWriteArray+i])>>8;
    config_settings_modified |= MASKA_FOR_BIT(BIT_CHANGED_SETTINGS);
    restart_timeout_idle_new_settings = true;
    break;

    case 31://���
     *(label_to_time_array + 6) = (tempWriteArray[offsetTempWriteArray+i]);
     flag_time_array = 1;
    break;
    case 32://�����
     *(label_to_time_array + 5) = (tempWriteArray[offsetTempWriteArray+i]);
     flag_time_array = 1;
    break;
    case 33://����
     *(label_to_time_array + 4) = (tempWriteArray[offsetTempWriteArray+i]);
     flag_time_array = 1;
    break;
    case 34://���
     *(label_to_time_array + 3) = (tempWriteArray[offsetTempWriteArray+i]);
     flag_time_array = 1;
    break;
    case 35://������
     *(label_to_time_array + 2) = (tempWriteArray[offsetTempWriteArray+i]);
     flag_time_array = 1;
    break;
    case 36://�������
     *(label_to_time_array + 1) = (tempWriteArray[offsetTempWriteArray+i]);
     flag_time_array = 1;
    break;
    case 37://����� �������
     *(label_to_time_array + 0) = (tempWriteArray[offsetTempWriteArray+i]);
     flag_time_array = 1;
    break;

    case 38://������� ����
    case 39://������� �� ������/������ �����
    case 40://����� �������� �� ������ �����
    case 41://������ ������ �������� �� ������ �����
    case 42://���� ������ �������� �� ������ �����
    case 43://��� ������ �������� �� ������ �����
    case 44://����� �������� �� ������ �����
    case 45://������ ������ �������� �� ������ �����
    case 46://���� ������ �������� �� ������ �����
    case 47://��� ������ �������� �� ������ �����
    case 48://������������� �������
    case 49://����� ������ �������������
    //tempReadArray[i] = 0;
//    config_settings_modified |= MASKA_FOR_BIT(BIT_CHANGED_SETTINGS);
//    restart_timeout_idle_new_settings = true;
    break;
 }//switch
  }//for
            if(flag_time_array)
            if (check_data_for_data_time_menu() == 1)
            {
              //��� ��������
    _SET_BIT(control_i2c_taskes, TASK_START_WRITE_RTC_BIT);
    _SET_BIT(control_i2c_taskes, TASK_BLK_OPERATION_BIT);
           }//if
           else return 2;
 return 0;
}//

int privatePKVBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  int count_register = REGISTER_FOR_OBJ;
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)

void config_and_settingsPKVBig(void)
{
//action ���������
}

