#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 61440

int privateT2SmallGetReg2(int adrReg);
int privateT2SmallGetBit2(int adrBit);

int getT2SmallModbusRegister(int);//�������� ���������� ��������
int getT2SmallModbusBit(int);//�������� ���������� ����
int setT2SmallModbusRegister(int, int);//�������� ���������� ��������
int setT2SmallModbusBit(int, int);//�������� ���������� ����

void setT2SmallCountObject(void);//�������� �-�� �������
void preT2SmallReadAction(void);//action �� ������
void postT2SmallReadAction(void);//action ����� ������
void preT2SmallWriteAction(void);//action �� ������
int  postT2SmallWriteAction(void);//action ����� ������
void config_and_settingsT2Small(void);//action ���������

COMPONENT_OBJ *t2smallcomponent;

/**************************************/
//���������� ���������� ��������������� AND
/**************************************/
void constructorT2SmallComponent(COMPONENT_OBJ *t2smallcomp)
{
  t2smallcomponent = t2smallcomp;

//  t1smallcomponent->countObject = 128;//�-�� �������

  t2smallcomponent->getModbusRegister = getT2SmallModbusRegister;//�������� ���������� ��������
  t2smallcomponent->getModbusBit      = getT2SmallModbusBit;//�������� ���������� ����
  t2smallcomponent->setModbusRegister = setT2SmallModbusRegister;//�������� ���������� ��������
  t2smallcomponent->setModbusBit      = setT2SmallModbusBit;//�������� ���������� ����

  t2smallcomponent->preReadAction   = preT2SmallReadAction;//action �� ������
  t2smallcomponent->postReadAction  = postT2SmallReadAction;//action ����� ������
  t2smallcomponent->preWriteAction  = preT2SmallWriteAction;//action �� ������
  t2smallcomponent->postWriteAction = postT2SmallWriteAction;//action ����� ������
  t2smallcomponent->config_and_settings = config_and_settingsT2Small;//action ���������

  t2smallcomponent->isActiveActualData = 0;
}//constructorT2SmallComponent(COMPONENT_OBJ *t1smallcomp)

int getT2SmallModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateT2SmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  return MARKER_ERRORPERIMETR;
}//getT2SmallModbusRegister(int adrReg)
int getT2SmallModbusBit(int x)
{
  //�������� ���������� bit
  UNUSED(x);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//getT2SmallModbusBit(int adrBit)

int setT2SmallModbusRegister(int adrReg, int dataReg)
{
  if(privateT2SmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  //�������� ���������� ��������
  superSetOperativMarker(t2smallcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  return 0;//MARKER_OUTPERIMETR;
}//setT2SmallModbusRegister(int x, int y)
int setT2SmallModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//setT2SmallModbusBit(int x, int y)

void preT2SmallReadAction(void) {
//action �� ������
  t2smallcomponent->operativMarker[0] = -1;
  t2smallcomponent->operativMarker[1] = -1;//����������� ������
  t2smallcomponent->isActiveActualData = 1;
}//
void postT2SmallReadAction(void) {
//action ����� ������
  if(t2smallcomponent->operativMarker[0]<0) return;//�� ���� ������
}//
void preT2SmallWriteAction(void) {
//action �� ������
  t2smallcomponent->operativMarker[0] = -1;
  t2smallcomponent->operativMarker[1] = -1;//����������� ������
  t2smallcomponent->isActiveActualData = 1;
}//
int postT2SmallWriteAction(void) {
//action ����� ������
extern int upravlSetting;//���� Setting
extern int upravlSchematic;//���� Shematic
extern int pointInterface;//����� ���������� 0-USB 1-RS485
//action ����� ������
  if(t2smallcomponent->operativMarker[0]<0) return 0;//�� ���� ������
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
  int countRegister = t2smallcomponent->operativMarker[1]-t2smallcomponent->operativMarker[0]+1;
  if(t2smallcomponent->operativMarker[1]<0) countRegister = 1;






//   __LN_BUTTON *arr = (__LN_BUTTON*)(spca_of_p_prt[ID_FB_BUTTON - _ID_FB_FIRST_VAR]);

//�������� edit ������
  int ID_tmp=-1;
  int N_tmp =-1;
  for(int i=0; i<countRegister; i++) {
   int offset = i+ t2smallcomponent->operativMarker[0]-BEGIN_ADR_REGISTER;
   int param = tempWriteArray[offsetTempWriteArray+i];
   if(param==0) return 2;//����

  switch(offset%2) {//������ �������� �����
    case 0://(ID,N)
      if(superControlParam(param)) return 2;//����
      ID_tmp  = (param>>8)&0xff;//id �����
      N_tmp   = param&0xff;//����� �����
    break;
    case 1://(n, link)
    {
      if(ID_tmp<0) return 2;//����
      if(N_tmp<0)  return 2;//����
       int n_tmp    = (param>>8)&0xff;//n
       int link_tmp = param&0xff;//link

  switch(ID_tmp)
    {
//    case _ID_FB_FIRST_ALL:
//      break;
    case ID_FB_INPUT:
    {
      __settings_for_INPUT *INPUT_arr  = (__settings_for_INPUT*)(sca_of_p[ID_FB_INPUT - _ID_FB_FIRST_VAR]);
      __settings_for_INPUT *INPUT_arr1 = (__settings_for_INPUT*)(sca_of_p_edit[ID_FB_INPUT - _ID_FB_FIRST_VAR]);
      INPUT_arr1[N_tmp]._n    = INPUT_arr[N_tmp]._n    = (int32_t)n_tmp;
      INPUT_arr1[N_tmp]._link = INPUT_arr[N_tmp]._link = (int32_t)link_tmp;
     } break;
    case ID_FB_OUTPUT:
     {
      __settings_for_OUTPUT_LED *OUTPUT_arr  = (__settings_for_OUTPUT_LED*)(sca_of_p[ID_FB_OUTPUT - _ID_FB_FIRST_VAR]);
      __settings_for_OUTPUT_LED *OUTPUT_arr1 = (__settings_for_OUTPUT_LED*)(sca_of_p_edit[ID_FB_OUTPUT - _ID_FB_FIRST_VAR]);
      OUTPUT_arr1[N_tmp]._n    = OUTPUT_arr[N_tmp]._n    = (int32_t)n_tmp;
      OUTPUT_arr1[N_tmp]._link = OUTPUT_arr[N_tmp]._link = (int32_t)link_tmp;
     } break;
    case ID_FB_LED:
      {
       __settings_for_OUTPUT_LED *LED_arr  = (__settings_for_OUTPUT_LED*)(sca_of_p[ID_FB_LED - _ID_FB_FIRST_VAR]);
       __settings_for_OUTPUT_LED *LED_arr1 = (__settings_for_OUTPUT_LED*)(sca_of_p_edit[ID_FB_LED - _ID_FB_FIRST_VAR]);
      LED_arr1[N_tmp]._n    = LED_arr[N_tmp]._n    = (int32_t)n_tmp;
      LED_arr1[N_tmp]._link = LED_arr[N_tmp]._link = (int32_t)link_tmp;
      } break;

//    case ID_FB_BUTTON:
//      {
//       __LN_BUTTON *BUTTON_arr  = (__LN_BUTTON*)(spca_of_p[ID_FB_BUTTON - _ID_FB_FIRST_VAR]);
//       __LN_BUTTON *BUTTON_arr1 = (__LN_BUTTON*)(spca_of_p_edit[ID_FB_BUTTON - _ID_FB_FIRST_VAR]);
//      BUTTON_arr1[N_tmp]._n    = BUTTON_arr[N_tmp]._n    = (int32_t)n_tmp;
//      BUTTON_arr1[N_tmp]._link = BUTTON_arr[N_tmp]._link = (int32_t)link_tmp;
//      } break;

    case ID_FB_ALARM:
      {
      __settings_for_ALARM *ALARM_arr  = (__settings_for_ALARM*)(sca_of_p[ID_FB_ALARM - _ID_FB_FIRST_VAR]);
      __settings_for_ALARM *ALARM_arr1 = (__settings_for_ALARM*)(sca_of_p_edit[ID_FB_ALARM - _ID_FB_FIRST_VAR]);
      ALARM_arr1[N_tmp]._n    = ALARM_arr[N_tmp]._n    = (int32_t)n_tmp;
      ALARM_arr1[N_tmp]._link = ALARM_arr[N_tmp]._link = (int32_t)link_tmp;
      } break;
    case ID_FB_GROUP_ALARM:
     {
      __settings_for_GROUP_ALARM *GROUP_ALARM_arr  = (__settings_for_GROUP_ALARM*)(sca_of_p[ID_FB_GROUP_ALARM - _ID_FB_FIRST_VAR]);
      __settings_for_GROUP_ALARM *GROUP_ALARM_arr1 = (__settings_for_GROUP_ALARM*)(sca_of_p_edit[ID_FB_GROUP_ALARM - _ID_FB_FIRST_VAR]);
      GROUP_ALARM_arr1[N_tmp]._n    = GROUP_ALARM_arr[N_tmp]._n    = (int32_t)n_tmp;
      GROUP_ALARM_arr1[N_tmp]._link = GROUP_ALARM_arr[N_tmp]._link = (int32_t)link_tmp;
     } break;
    case ID_FB_AND:
     {
      __settings_for_AND *AND_arr = (__settings_for_AND*)(sca_of_p[ID_FB_AND - _ID_FB_FIRST_VAR]);
      __settings_for_AND *AND_arr1 = (__settings_for_AND*)(sca_of_p_edit[ID_FB_AND - _ID_FB_FIRST_VAR]);
      AND_arr1[N_tmp]._n    = AND_arr[N_tmp]._n    = (int32_t)n_tmp;
      AND_arr1[N_tmp]._link = AND_arr[N_tmp]._link = (int32_t)link_tmp;
     } break;
    case ID_FB_OR:
     {
      __settings_for_OR *OR_arr  = (__settings_for_OR*)(sca_of_p[ID_FB_OR - _ID_FB_FIRST_VAR]);
      __settings_for_OR *OR_arr1 = (__settings_for_OR*)(sca_of_p_edit[ID_FB_OR - _ID_FB_FIRST_VAR]);
      OR_arr1[N_tmp]._n    = OR_arr[N_tmp]._n    = (int32_t)n_tmp;
      OR_arr1[N_tmp]._link = OR_arr[N_tmp]._link = (int32_t)link_tmp;
     } break;
    case ID_FB_XOR:
     {
      __settings_for_XOR *XOR_arr  = (__settings_for_XOR*)(sca_of_p[ID_FB_XOR - _ID_FB_FIRST_VAR]);
      __settings_for_XOR *XOR_arr1 = (__settings_for_XOR*)(sca_of_p_edit[ID_FB_XOR - _ID_FB_FIRST_VAR]);
      XOR_arr1[N_tmp]._n    = XOR_arr[N_tmp]._n    = (int32_t)n_tmp;
      XOR_arr1[N_tmp]._link = XOR_arr[N_tmp]._link = (int32_t)link_tmp;
     } break;
    case ID_FB_NOT:
     {
     __settings_for_NOT *NOT_arr  = (__settings_for_NOT*)(sca_of_p[ID_FB_NOT - _ID_FB_FIRST_VAR]);
     __settings_for_NOT *NOT_arr1 = (__settings_for_NOT*)(sca_of_p_edit[ID_FB_NOT - _ID_FB_FIRST_VAR]);
      NOT_arr1[N_tmp]._n    = NOT_arr[N_tmp]._n    = (int32_t)n_tmp;
      NOT_arr1[N_tmp]._link = NOT_arr[N_tmp]._link = (int32_t)link_tmp;
     } break;

    case ID_FB_TIMER:
     {
      __settings_for_TIMER *TIMER_arr  = (__settings_for_TIMER*)(sca_of_p[ID_FB_TIMER - _ID_FB_FIRST_VAR]);
      __settings_for_TIMER *TIMER_arr1 = (__settings_for_TIMER*)(sca_of_p_edit[ID_FB_TIMER - _ID_FB_FIRST_VAR]);
      TIMER_arr1[N_tmp]._n    = TIMER_arr[N_tmp]._n    = (int32_t)n_tmp;
      TIMER_arr1[N_tmp]._link = TIMER_arr[N_tmp]._link = (int32_t)link_tmp;
     } break;
    case ID_FB_TRIGGER:
     {
      __settings_for_TRIGGER *TRIGGER_arr  = (__settings_for_TRIGGER*)(sca_of_p[ID_FB_TRIGGER - _ID_FB_FIRST_VAR]);
      __settings_for_TRIGGER *TRIGGER_arr1 = (__settings_for_TRIGGER*)(sca_of_p_edit[ID_FB_TRIGGER - _ID_FB_FIRST_VAR]);
      TRIGGER_arr1[N_tmp]._n    = TRIGGER_arr[N_tmp]._n    = (int32_t)n_tmp;
      TRIGGER_arr1[N_tmp]._link = TRIGGER_arr[N_tmp]._link = (int32_t)link_tmp;
     } break;

    case ID_FB_MEANDER:
     {
      __settings_for_MEANDER *MEANDER_arr  = (__settings_for_MEANDER*)(sca_of_p[ID_FB_MEANDER - _ID_FB_FIRST_VAR]);
      __settings_for_MEANDER *MEANDER_arr1 = (__settings_for_MEANDER*)(sca_of_p_edit[ID_FB_MEANDER - _ID_FB_FIRST_VAR]);
      MEANDER_arr1[N_tmp]._n    = MEANDER_arr[N_tmp]._n    = (int32_t)n_tmp;
      MEANDER_arr1[N_tmp]._link = MEANDER_arr[N_tmp]._link = (int32_t)link_tmp;
     } break;
    case ID_FB_TU:
     {
      __settings_for_TU *TU_arr  = (__settings_for_TU*)(sca_of_p[ID_FB_TU - _ID_FB_FIRST_VAR]);
      __settings_for_TU *TU_arr1 = (__settings_for_TU*)(sca_of_p_edit[ID_FB_TU - _ID_FB_FIRST_VAR]);
      TU_arr1[N_tmp]._n    = TU_arr[N_tmp]._n    = (int32_t)n_tmp;
      TU_arr1[N_tmp]._link = TU_arr[N_tmp]._link = (int32_t)link_tmp;
     } break;
    case ID_FB_TS:
     {
      __settings_for_TS *TS_arr  = (__settings_for_TS*)(sca_of_p[ID_FB_TS - _ID_FB_FIRST_VAR]);
      __settings_for_TS *TS_arr1 = (__settings_for_TS*)(sca_of_p_edit[ID_FB_TS - _ID_FB_FIRST_VAR]);
      TS_arr1[N_tmp]._n    = TS_arr[N_tmp]._n    = (int32_t)n_tmp;
      TS_arr1[N_tmp]._link = TS_arr[N_tmp]._link = (int32_t)link_tmp;
     } break;

    //case ID_FB_EVENT_LOG:
//     {
//       __LOG_INPUT *LOG_arr  = (__LOG_INPUT*)(sca_of_p[ID_FB_EVENT_LOG - _ID_FB_FIRST_VAR]);
       //__LOG_INPUT *LOG_arr1 = (__LOG_INPUT*)(sca_of_p_edit[ID_FB_EVENT_LOG - _ID_FB_FIRST_VAR]);
//      LOG_arr1[N_tmp]._n    = LOG_arr[N_tmp]._n    = (int32_t)n_tmp;
//      LOG_arr1[N_tmp]._link = LOG_arr[N_tmp]._link = (int32_t)link_tmp;
//     } break;
    default:
      return 2;//����
    }//switch id

      ID_tmp=-1;
      N_tmp =-1;
    } break;
 }//switch

  }//for

 return 0;
}//

int privateT2SmallGetReg2(int adrReg)
{
  //��������� ������� ��������
  if(adrReg>=BEGIN_ADR_REGISTER && adrReg<(BEGIN_ADR_REGISTER+124)) return 0;
  return MARKER_OUTPERIMETR;
}//privateGetReg2(int adrReg)

void config_and_settingsT2Small(void)
{
//action ���������
}

