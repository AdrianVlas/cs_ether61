#include "header.h"

/*****************************************************/
//ĳ��������� ���
/*****************************************************/
inline void diagnostyca_adc_execution(void)
{
  if (gnd_adc >0x51) _SET_BIT(set_diagnostyka, ERROR_GND_ADC_TEST_BIT);
  else _SET_BIT(clear_diagnostyka, ERROR_GND_ADC_TEST_BIT);

  if ((vref_adc <0x709) || (vref_adc > 0x8f5)) _SET_BIT(set_diagnostyka, ERROR_VREF_ADC_TEST_BIT);
  else _SET_BIT(clear_diagnostyka,ERROR_VREF_ADC_TEST_BIT);

  if ((vdd_adc <0x8F9) || (vdd_adc > 0xC24)) _SET_BIT(set_diagnostyka, ERROR_VDD_ADC_TEST_BIT);
  else _SET_BIT(clear_diagnostyka, ERROR_VDD_ADC_TEST_BIT);
}
/*****************************************************/

/*****************************************************/
//����� ���������� �����
/*****************************************************/
inline unsigned int norma_value(unsigned long long y)
{
  unsigned long long temp=y;
  unsigned int rezultat =0;
  if (temp == 0) return 0;

  while(temp !=0)
  {
    temp = temp>>1;
    rezultat++;
  }
  return rezultat-1;
}
/*****************************************************/

/*****************************************************/
//����� ���������� ����� ���� int
/*****************************************************/
inline unsigned int get_order(int value)
{
  unsigned int i = 0;

  if (value == 0) return 1;  
  if (value < 0) value =-value;

  while ((value >> (++i)) != 0); 

  return i;    
}
/*****************************************************/

/*****************************************************/
//���������� ������ ����������� ������� ����������� ������  � �������������� ���������� �����
/*****************************************************/
unsigned int sqrt_64(unsigned long long y)
{
   unsigned int b;
   unsigned int a;
   unsigned int c;
   unsigned int norma_rez;
   unsigned long long temp;
   
   norma_rez = norma_value(y)>>1;
   
   a = (1<<norma_rez) - 1;
   b = (1<<(norma_rez+1));
 
   do 
    {
     c = (a + b)>>1;
     temp = (unsigned long long)c*(unsigned long long)c; 
     if (temp != y)
       {
        if ( temp > y) b= c; else a= c;
       } 
     else return c;
    }
   while ((b-a)>1);

   c = (a + b)>>1;

   return c ;
}
/*****************************************************/

/*****************************************************/
//���������� ������ ����������� ������� ����������� ������  � �������������� ���������� �����
/*****************************************************/
inline unsigned int sqrt_32(unsigned int y)
{
   unsigned int b;
   unsigned int a;
   unsigned int c;
   unsigned int norma_rez;
   unsigned int temp;
   
   norma_rez = norma_value(y)>>1;
   
   a = (1<<norma_rez) - 1;
   b = (1<<(norma_rez+1));
 
   do 
    {
     c = (a + b)>>1;
     temp = c*c; 
     if (temp != y)
       {
        if ( temp > y) b= c; else a= c;
       } 
     else return c;
    }
   while ((b-a)>1);

   c = (a + b)>>1;

   return c ;
}
/*****************************************************/

/*****************************************************/
//���������� ���������
/*****************************************************/
inline void calc_measurement(void)
{
  unsigned long long sum_sqr_data_local[NUMBER_ANALOG_CANALES];
  
  //������� ����� �������� � ������� ����
  /*
  ������� ��� ������������� ���������� ���� �������� � ����� ������� �� �����, 
  �� ��� ����������� ������� ��������� ������ �� ������� �������, �� �� �� ������ ����� � ������ ��������
  */
  unsigned int bank_sum_sqr_data_tmp = (bank_sum_sqr_data + 1) & 0x1;
  for(uint32_t i =0; i < NUMBER_ANALOG_CANALES; i++ )
  {
    /***/
    //����������� ���� �������� 3I0 �� ����������� �������
    /***/
    /*�������� ���������� �����*/
    sum_sqr_data_local[i] = sqrt_64(sum_sqr_data[i][bank_sum_sqr_data_tmp]);
  
    /*��� ���������� ����� �������� � �� ����� ��������� �� ��� ����������*/
    /*�� ������ ����� � ��������� � 16 ���, ���� ���� ���� �� 16*/

    /*
    �� ���� �� �������� ����� ������� �� ����� � NUMBER_POINT = 32 = 16*2
    ����� �� ���� ������� �� 4*sqrt(2)
    4 �� ������� �� 2
    ((MNOGNYK_I_D * X )>> VAGA_DILENNJA_I_D)/sqrt(2) �������
   (MNOGNYK_I_DIJUCHE_D * X )>> VAGA_DILENNJA_I_DIJUCHE_D 
  
    ���� ������ ����� ���� (MNOGNYK_I_DIJUCHE_D;VAGA_DILENNJA_I_DIJUCHE_D) � (MNOGNYK_I_D;VAGA_DILENNJA_I_D)
    �� ������ ������� ��� ���������� ����� ������� (�� ���� ���� ��� ������� ������ ��� ����������. � �� �� ����).
    ���� ����� ��� �� ��� ��������� ������
    */
    float value_i_float = (unsigned int)(MNOGNYK_I_DIJUCHE_FLOAT*((float)sum_sqr_data_local[i])/(64.0f)); /*64 = 4*16. 16 - �� ��������� ������ "Analog Input"; 4 - �� sqrt(16), � 16 �������� � ����, �� 32 = 16*2 */
    measurement[i] = (unsigned int)value_i_float; 
    /***/
  }
}
/*****************************************************/

/*****************************************************/
//������ ������� ���������� �����
/*****************************************************/
inline void input_scan(void)
{
  unsigned int state_inputs_into_pin, temp_state_inputs_into_pin; //���� � ��� ������� �������� ����� ������� �� ����� ��������� (���)
  static unsigned int state_inputs_into_pin_trigger; //� ��� ����� ���������� ��������� ���� ���, � ������� ���� �� ���������� .�� ���������� ��� ���� �� ���� ���������� ������� �������

  /***************************/
  //� ��� ������ ������ ����� ����� ��������� (���), �� ���������� �� �������� �����
  /*
  -----------------------------
  �������� ��������� � �������� ��� ����� state_inputs_into_pin
    "�     ������ " - ������� ������������� ��� (1)
    "���� �������" - ������� ���������     ��� (0)
  -----------------------------
  */
  unsigned int temp_state_inputs_into_pin_1 = _DEVICE_REGISTER(Bank1_SRAM2_ADDR, OFFSET_INPUTS_1) & ((1 << NUMBER_INPUTS_1) - 1);
  unsigned int temp_state_inputs_into_pin_2 = _DEVICE_REGISTER(Bank1_SRAM2_ADDR, OFFSET_INPUTS_2) & ((1 << NUMBER_INPUTS_2) - 1);
  temp_state_inputs_into_pin = temp_state_inputs_into_pin_1 | (temp_state_inputs_into_pin_2 << NUMBER_INPUTS_1);
  
  //������� �������, ��� �������� ����� �������� ��������� �� ������
  state_inputs_into_pin = 0;
  for (unsigned int index = 0; index < NUMBER_INPUTS; index++)
  {
    if ((temp_state_inputs_into_pin & (1 << index)) != 0) 
    {
      if (index < NUMBER_INPUTS_1)
        state_inputs_into_pin |= 1 << (NUMBER_INPUTS_1 - index - 1);
      else
        state_inputs_into_pin |= 1 << index;
    }
  }
  /***************************/
  
  /***************************/
  //������� ������� ������� ���������� �����
  /***************************/
  for(unsigned int i = 0; i < NUMBER_INPUTS; i++)
  {
    unsigned int maska = 1<<i;
    int max_value_timer = current_settings_prt.dopusk_dv[i];

    if (global_timers[INDEX_TIMER_INPUT_START + i] < 0)
    {
      //������� �� ���� ����� ����������� ����� ����������� ����� ���, ���� �������� ������� � ��'�����
      //�� ������ �� ������ �� ��������� � ��������� ������
      
      //���������� �� ��������� ���� ����� ������� ������������ �����
      if ((state_inputs_into_pin & maska) != (state_inputs & maska))
      {
        //���� ���� ����� �������, �� �������� 䳿 �� ��� �����

        //1-��: �����'������� ������� ���� �����
        state_inputs_into_pin_trigger &= ~maska;
        state_inputs_into_pin_trigger |= (state_inputs_into_pin & maska);

        //2-��: ��������� ������ ������� ����������� �����
        global_timers[INDEX_TIMER_INPUT_START + i] = 0;
      }
    }
    else
    {
      //���� ������ ��������� �������� 䳿 � ��������
      if (global_timers[INDEX_TIMER_INPUT_START + i] < max_value_timer)
      {
        //���� ������ �� �� ����� �� ����� ���������, �� ������ �������� ���� ��������
        global_timers[INDEX_TIMER_INPUT_START + i] += DELTA_TIME_FOR_TIMERS;
        
        //� �������, ���� ��� ������� �� ���� �������� ������
        //� �� ���������� �� �� ������� ������� "� ������"->"���� �������"
        //�� ����� ������� �� ��� ���������� ������� ������� ������, �� ������ �� ���� � - 
        //� �� ������, �� ����� �������� n�����, �� �������� "� ������"->"���� �������" �� ������ ���������� ������� �� ����������� 
        if ((current_settings_prt.type_of_input_signal & maska) != 0)
        {
           if ((state_inputs_into_pin_trigger & maska) == 0)
           {
             if ((state_inputs_into_pin & maska) != 0)
               global_timers[INDEX_TIMER_INPUT_START + i] = -1;
           }
        }
      }
    }
    
    //���� �������� ������� ������� ����������� � ����� ������������� �������, �� ����������, �� �������� ������� ����� �����
    if (global_timers[INDEX_TIMER_INPUT_START + i] >= max_value_timer)
    {
      //����������, �� ���� ��� ������� � �������� �� ������, ���� ��� �� ������ ������� ������� ������� ����������� �����
      unsigned int state_1, state_2;
        
      state_1 = state_inputs_into_pin_trigger & maska;
      state_2 = state_inputs_into_pin  & maska;
        
      if (state_1 == state_2)
      {
        //���� ��� ����� ����������, �� �� �������, �� �������� ��� ����� ����������� ����� � ������� ����� ���� �����
        //��� ����� ��������� �� ��� �������    ����� 1 - �� �������� ����, � 0 - �� �������� ����
        //                        ��� ���������� ����� 0 - �� �������� ����, � 1 - �� �������� ����
          state_inputs &=  ~maska;
          state_inputs |=   state_2;
      }
        
      //� ����-����� ������, �� ������� ���� �����, �� �, � ������� ������ ������� ���������� �� ����, �� ������������� � �������� ��'���� ��������
      global_timers[INDEX_TIMER_INPUT_START + i] = -1;
    }
  }
  /***************************/
}
/*****************************************************/

/*****************************************************/
//������� ������� �������
/*****************************************************/
inline void clocking_global_timers(void)
{
  //����������� �������� �����
  input_scan();
  
  //����������� �� ����� ������� �����
  for (unsigned int i = INDEX_TIMER_TEMP; i < MAX_NUMBER_GLOBAL_TIMERS; i++)
  {
    if (global_timers[i] >= 0)
    {
      //������ ������ ����, �� ������ ����� ��������� � ��� ����, �� �������� ������� �� ��'����

      //���������� �� ����� ���������� �������� �������, ���� �� �� �� �������� ����� ���������
      if (global_timers[i] <= (0x7fffffff - DELTA_TIME_FOR_TIMERS)) global_timers[i] += DELTA_TIME_FOR_TIMERS;
    }
  }
  
  if (++timer_meander >= PERIOD_SIGNAL_MEANDER)
  {
    timer_meander = 0;
    output_timer_meander ^= true;
  }
}
/*****************************************************/

/*****************************************************/
//�������� �� ����������� ���������� ������ �����������/����������� ����������
/*****************************************************/
inline unsigned int stop_regisrator(volatile unsigned int* carrent_active_functions, unsigned int* ranguvannja_registrator)
{
  unsigned int stop = 0;

  {
    if (
        ((carrent_active_functions[0] & ranguvannja_registrator[0]) == 0) &&
        ((carrent_active_functions[1] & ranguvannja_registrator[1]) == 0) &&
        ((carrent_active_functions[2] & ranguvannja_registrator[2]) == 0) &&
        ((carrent_active_functions[3] & ranguvannja_registrator[3]) == 0) &&
        ((carrent_active_functions[4] & ranguvannja_registrator[4]) == 0) &&
        ((carrent_active_functions[5] & ranguvannja_registrator[5]) == 0) &&
        ((carrent_active_functions[6] & ranguvannja_registrator[6]) == 0)
      )
    {
      //�����������, �� � ���� ������� ��������� ���������� ����� �� �������
      
      //�������, �� ��������� ���� ���� ���������
      stop = 0xff;
    }
  }
  
  return stop;
}
/*****************************************************/

/*****************************************************/
//����������� ����������� ������� ������ ����������� ����������
/*****************************************************/
void fix_undefined_error_dr(volatile unsigned int* carrent_active_functions)
{
  //����������� ������� � ������� � ��������� ���������
  _SET_BIT(set_diagnostyka, ERROR_DR_UNDEFINED_BIT);
  _SET_BIT(carrent_active_functions, RANG_DEFECT);
  //���������� ����� ������ � ����������� � ���� "�� ����� ������ ���� �� � ���������� ����������� �� ����������" 
  state_dr_record = STATE_DR_NO_RECORD;
  //������� ������ ������ ����������� ����������
  _CLEAR_BIT(carrent_active_functions, RANG_WORK_D_REJESTRATOR);
}
/*****************************************************/

/*****************************************************/
//������ ������� ������ ����� � DataFalsh
/*****************************************************/
inline void command_start_saving_record_dr_into_dataflash(void)
{
  //� �������� �� ���������� ����� ���������� ����������� �����������, �� ������� ����� � �� �� ���������
  _SET_BIT(control_i2c_taskes, TASK_START_WRITE_INFO_REJESTRATOR_DR_EEPROM_BIT);
  info_rejestrator_dr.saving_execution = 1;
        
  //����������� ����� ������� ������
  part_writing_dr_into_dataflash = 0;
  //����������� ������� ������ � ��������� DataFlash
  control_tasks_dataflash |= TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_DR;
}
/*****************************************************/

/*****************************************************/
//������� ������� ����� ����������� ������ ����������� ����������
/*****************************************************/
inline void routine_for_queue_dr(void)
{
  if(
     (state_dr_record == STATE_DR_MAKE_RECORD)            || /*����� ����� ��������� ��� �������� �� ����� � DataFlsh ��� ���������� �������  ������ ������, �� ����� ���������� �����    ���������, � �� ������ ����� ����������� ���������� ������ ������������ ������*/
     (state_dr_record == STATE_DR_CUT_RECORD)             || /*����� ����� ��������� ��� �������� �� ����� � DataFlsh   � ���������  �������� ������ ������, �� ����� ���������� ����� �� ���������, �    ������ ����� ����������� ���������� ������ ������������ ������*/
     (number_records_dr_waiting_for_saving_operation != 0)  
    )
  {
    if ((control_tasks_dataflash & TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_DR) == 0)
    {
      //�� ����� ������ ������ � Datafalsh �� �����������
      unsigned char *buffer_source, *buffer_target;
        
      if (number_records_dr_waiting_for_saving_operation == 0)
      {
        buffer_target = buffer_for_save_dr_record_level_2;
        buffer_source = buffer_for_save_dr_record;
        for(unsigned int i = 0; i < SIZE_BUFFER_FOR_DR_RECORD; i++)
        {
          *(buffer_target + i) = *(buffer_source + i);
          *(buffer_source + i) = 0xff;
        }
        number_records_dr_waiting_for_saving_operation = 0;
      }
      else if (number_records_dr_waiting_for_saving_operation == 1)
      {
        buffer_target = buffer_for_save_dr_record_level_2;
        buffer_source = buffer_for_save_dr_record_level_1;
        for(unsigned int i = 0; i < SIZE_BUFFER_FOR_DR_RECORD; i++) *(buffer_target +i ) = *(buffer_source + i);
        
        if((state_dr_record == STATE_DR_MAKE_RECORD) || (state_dr_record == STATE_DR_CUT_RECORD))
        {
          buffer_target = buffer_for_save_dr_record_level_1;
          buffer_source = buffer_for_save_dr_record;
          for(unsigned int i = 0; i < SIZE_BUFFER_FOR_DR_RECORD; i++) 
          {
            *(buffer_target + i) = *(buffer_source + i);
            *(buffer_source + i) = 0xff;
          }
          number_records_dr_waiting_for_saving_operation = 1;
        }
        else number_records_dr_waiting_for_saving_operation = 0;
      }
      else
      {
        buffer_target = buffer_for_save_dr_record_level_2;
        buffer_source = buffer_for_save_dr_record_level_1;
        for(unsigned int i = 0; i < SIZE_BUFFER_FOR_DR_RECORD; i++) *(buffer_target + i) = *(buffer_source + i);
        buffer_target = buffer_for_save_dr_record_level_1;
        buffer_source = buffer_for_save_dr_record;
        for(unsigned int i = 0; i < SIZE_BUFFER_FOR_DR_RECORD; i++) 
        {
          *(buffer_target + i) = *(buffer_source + i);
          *(buffer_source + i) = 0xff;
        }
        number_records_dr_waiting_for_saving_operation = 1;
      }
        
      //������ ������� ������ ����� � DataFalsh
      command_start_saving_record_dr_into_dataflash();
    }
    else
    {
      //�� ����� ������ ������ � Datafalsh �����������
      //����� ��� �������� ����� ��������� � �����
      if (number_records_dr_waiting_for_saving_operation == 0)
      {
        //�� ����� ������ ��� ����, ������ - � ����� ��� ������������ ����� � DataFalsh, ����� ����� ������ � ���� ����
        //��������� ������� ����� � ����� buffer_for_save_dr_record_level_1
        for(unsigned int i = 0; i < SIZE_BUFFER_FOR_DR_RECORD; i++) 
        {
          buffer_for_save_dr_record_level_1[i] = buffer_for_save_dr_record[i];
          buffer_for_save_dr_record[i] = 0xff;
        }
    
        //�������, �� ���� ����� ����� ������� ���� �� ������������ ����� � dataFalsh
        number_records_dr_waiting_for_saving_operation = 1;
      }
      else if (number_records_dr_waiting_for_saving_operation >= 1)
      {
         if((state_dr_record == STATE_DR_MAKE_RECORD) || (state_dr_record == STATE_DR_CUT_RECORD))
         {
          //�� ����� ������ ��� ����, ������ - � ����� ��� ������������ ����� � DataFalsh, �� ���� ����������� ����� ����� � ����� buffer_for_save_dr_record_level_1
          /*
          ���� ����� ����� �������� � ����� ������ ����� buffer_for_save_dr_record, ��� �������� ������� ������. �� ������ � ����
          � ���� ���� ������ ������ ����� �����, �� ������ ��������� ������� ������ �����
          */
          number_records_dr_waiting_for_saving_operation = 2;
         }
      }
    }
    
    if (state_dr_record == STATE_DR_MAKE_RECORD)
    {
      //�������, �� �� ����� �������� ��������� �����
      state_dr_record = STATE_DR_NO_RECORD;
    }
    else if (state_dr_record == STATE_DR_CUT_RECORD)
    {
      //�������, �� ����� ��������� ��������� ����� �����
      state_dr_record = STATE_DR_FORCE_START_NEW_RECORD;
    }
      
  }
}
/*****************************************************/

/*****************************************************/
//������� ������� � ��� ����������� �� ���� ��������
/*****************************************************/
inline void main_protection(void)
{
  /**************************/
  //��������, �� ����� �������� ������ �������
  /**************************/
  if (reset_trigger_function_from_interface !=0)
  {
    
    //������� �� �� �������� ������� �� �Ѳ� �����������
    reset_trigger_function_from_interface = 0;
  }
  /**************************/

  /**************************/
  //����������� �������� ������
  /**************************/
  //��������� � �������
  if (pressed_buttons != 0)
  {

    //������� �������� ������, �� �� ��� ����������
    pressed_buttons =0;
  }
  /**************************/
    
  /**************************/
  //����������� ������� � ��/Goose
  /**************************/
  if (activation_function_from_interface != 0)
  {

    //������� ������ ��������� ������� � ���������, �� �� ��� ����������
    activation_function_from_interface = 0;
  }
  /**************************/

  /**************************/
  //����������� �������� �����
  /**************************/
  //���������� �� � ����� ������ �����
  if (state_inputs !=0)
  {
  }
  /**************************/

  /**************************/
  //�������� ���������� ���������� ������ ��� ��� �������, �� ����� ������������ ����� �� ���������� �������
  /**************************/
  {
  }
  /**************************/

  /***********************************************************/
  //���������� ���������
  /***********************************************************/
  calc_measurement();

  //������� ���������� ��� ����������������� � ����������������� �������
  unsigned int bank_measurement_high_tmp = (bank_measurement_high ^ 0x1) & 0x1;
  if(semaphore_measure_values_low1 == 0)
  {
    for (unsigned int i = 0; i < NUMBER_ANALOG_CANALES; i++) 
    {
      measurement_high[bank_measurement_high_tmp][i] = measurement_middle[i] = measurement[i];
    }
  }
  else
  {
    for (unsigned int i = 0; i < NUMBER_ANALOG_CANALES; i++) 
    {
      measurement_high[bank_measurement_high_tmp][i] = measurement[i];
    }
  }
  bank_measurement_high = bank_measurement_high_tmp;
  /**************************/

  /**************************/
  //������ "������������ �����"
  /**************************/
  diagnostyca_adc_execution();

  unsigned int diagnostyka_tmp[3];
  diagnostyka_tmp[0] = diagnostyka[0];
  diagnostyka_tmp[1] = diagnostyka[1];
  diagnostyka_tmp[2] = diagnostyka[2];

  diagnostyka_tmp[0] &= (unsigned int)(~clear_diagnostyka[0]); 
  diagnostyka_tmp[0] |= set_diagnostyka[0]; 

  diagnostyka_tmp[1] &= (unsigned int)(~clear_diagnostyka[1]); 
  diagnostyka_tmp[1] |= set_diagnostyka[1]; 

  diagnostyka_tmp[2] &= (unsigned int)(~clear_diagnostyka[2]); 
  diagnostyka_tmp[2] |= set_diagnostyka[2]; 
  
  diagnostyka_tmp[2] &= USED_BITS_IN_LAST_INDEX; 

  _CLEAR_BIT(diagnostyka_tmp, EVENT_START_SYSTEM_BIT);
  _CLEAR_BIT(diagnostyka_tmp, EVENT_DROP_POWER_BIT);
  if (
      (diagnostyka_tmp[0] != 0) ||
      (diagnostyka_tmp[1] != 0) ||
      (diagnostyka_tmp[2] != 0)
     )   
  {
    _SET_BIT(active_functions, RANG_DEFECT);
    /**************************/
    //������ "������������ ���������"
    /**************************/
    if (
        ((diagnostyka_tmp[0] & MASKA_AVAR_ERROR_0) != 0) ||
        ((diagnostyka_tmp[1] & MASKA_AVAR_ERROR_1) != 0) ||
        ((diagnostyka_tmp[2] & MASKA_AVAR_ERROR_2) != 0)
       )   
    {
      _SET_BIT(active_functions, RANG_AVAR_DEFECT);
    }
    else
    {
      _CLEAR_BIT(active_functions, RANG_AVAR_DEFECT);
    }
    /**************************/
  }
  else
  {
    _CLEAR_BIT(active_functions, RANG_DEFECT);
    _CLEAR_BIT(active_functions, RANG_AVAR_DEFECT);
  }
  /**************************/

  
  //����� ����� ����� ��������� ����� � ���� �������, ���� ��� ������� "��������� �������������"
  if (_CHECK_SET_BIT(active_functions, RANG_AVAR_DEFECT) == 0)
  {
    //������� �������� �� �����������

  }
  else
  {
    //������� �������� �����������
    
    //������� �� ������ �������, ��� �������������
    
    //������������ �� ����
    state_outputs = 0;
    
    //������� �� �������, �� ������� � �����
    
  }

  /**************************/
  //������ � ���������, �� ����� ������������ � ��������������� ���'���
  /**************************/
//  unsigned int comparison_true = true;
//
//  if (comparison_true != true)
//  {
//    /*
//    �������, �������� ���� ���������� � ��������������� ���'1��� ��������.
//    ������ ������� �� �� ����� � ��������������� ���'���
//    */
//    _SET_BIT(control_i2c_taskes, TASK_START_WRITE_TRG_FUNC_EEPROM_BIT);
//  }
  /**************************/

  /*
  ������ ���� �������� �������� ������� ��� ����, ��� ���� �� ������� ������
  �����������, �� ����� ���� � ����� �������  (����� � ��'������� ����������
  ���������) ����� ��������, ��� ��������� ��������
  */
  for (unsigned int i = 0; i < N_BIG; i++) active_functions_copy[i] = active_functions[i];
  /**************************/

  /**************************/
  //���� ���������� �� ������
  /**************************/
  
  if (_CHECK_SET_BIT(active_functions, RANG_AVAR_DEFECT) == 0)
  {
    //�� ����������� ������� ��������, ���� ������������� ���� �����
    
  }
  else
  {
    //����������� ������e ��������, ���� ���������� �� ����!!!

    //������������ �� ����
    state_outputs = 0;
  }
  
  //�������� ���������� �� ������� �� ��� ��������� (� ����������� �������)
  unsigned int temp_state_outputs = 0;
  for (unsigned int index = 0; index < NUMBER_OUTPUTS; index++)
  {
    if ((state_outputs & (1 << index)) != 0)
    {
      if (index < NUMBER_OUTPUTS_1)
        temp_state_outputs |= 1 << (NUMBER_OUTPUTS_1 - index - 1);
      else
        temp_state_outputs |= 1 << index;
    }
  }
  unsigned int temp_state_outputs_1 =  temp_state_outputs                      & ((1 << NUMBER_OUTPUTS_1) - 1);
  unsigned int temp_state_outputs_2 = (temp_state_outputs >> NUMBER_OUTPUTS_1) & ((1 << NUMBER_OUTPUTS_2) - 1);
  _DEVICE_REGISTER(Bank1_SRAM2_ADDR, OFFSET_OUTPUTS_1) = temp_state_outputs_1;
  _DEVICE_REGISTER(Bank1_SRAM2_ADDR, OFFSET_OUTPUTS_2) = temp_state_outputs_2;
  /**************************/

  /**************************/
  //���� ���������� �� ���������
  /**************************/
  //�������� ����������, �� �� �������������� ������� "����� ���������" - � ���� ���, �� ���������� ������� ��� ���������
  
  //�������� ���������� �� ��������������� �� ���������
  _DEVICE_REGISTER(Bank1_SRAM2_ADDR, OFFSET_LEDS) = state_leds;
  /**************************/

  /**************************/
  //
  /**************************/
  /**************************/
}
/*****************************************************/

/*****************************************************/
//����������� �� ������� TIM2, ���� ��������� ������� �������
/*****************************************************/
void TIM2_IRQHandler(void)
{
#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordEnterISR();
#endif
  
  if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
  {
    /***********************************************************************************************/
    //����������� �������� �� ������ 1, ���� ������ ����������� ���� 1 ��, ��� ��������������� ������� � ������ �������
    /***********************************************************************************************/
    TIM2->SR = (uint16_t)((~(uint32_t)TIM_IT_CC1) & 0xffff);
    uint32_t current_tick = TIM2->CCR1;
    
    /***********************************************************/
    //���������� ����������� ������� ����������� � ����������� ����������
    /***********************************************************/
    //���������� ���������
    if (
        ((clean_rejestrators & CLEAN_DR) != 0)
        &&  
        (
         (control_tasks_dataflash & (
                                     TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_DR | 
                                     TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB                    |
                                     TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485                  |
                                     TASK_MAMORY_READ_DATAFLASH_FOR_DR_MENU
                                    )
         ) == 0
        )
       )   
    {
      //���������� ������� �������� ����������� ���������

      //����������� ������� ������ ���� ��������� � EEPROM
      _SET_BIT(control_i2c_taskes, TASK_START_WRITE_INFO_REJESTRATOR_DR_EEPROM_BIT);

      //������� ��������� ���������� �� ����������� ���������
      info_rejestrator_dr.next_address = MIN_ADDRESS_DR_AREA;
      info_rejestrator_dr.saving_execution = 0;
      info_rejestrator_dr.number_records = 0;

      //�������, �� ����� ������ �� ��������
      number_record_of_dr_for_menu  = 0xffff;
      number_record_of_dr_for_USB   = 0xffff;
      number_record_of_dr_for_RS485 = 0xffff;

      //������ ������� ������� ����������� ����������
      clean_rejestrators &= (unsigned int)(~CLEAN_DR);
    }
    /***********************************************************/

    /***********************************************************/
    //���������� ������� �������� � ���������� ����� ����� ���� ��������� ������ ��������
    /***********************************************************/
    clocking_global_timers();
    /***********************************************************/
    
    /***********************************************************/
    //���������� ������� �������
    /***********************************************************/
    //ĳ��������� �����, ��� ����� ��������� ����� ��� ����� �������� ��������������� ����� ��������
    unsigned int control_state_outputs_1 = (( (~((unsigned int)(_DEVICE_REGISTER(Bank1_SRAM2_ADDR, OFFSET_OUTPUTS_1)))) >> 8) & ((1 << NUMBER_OUTPUTS_1) - 1));
    unsigned int control_state_outputs_2 = (( (~((unsigned int)(_DEVICE_REGISTER(Bank1_SRAM2_ADDR, OFFSET_OUTPUTS_2)))) >> 8) & ((1 << NUMBER_OUTPUTS_2) - 1));
    unsigned int control_state_outputs = control_state_outputs_1 | (control_state_outputs_2 << NUMBER_OUTPUTS_1);
    //������� ����� ������ � ���������� �� ������ ���������
    unsigned int temp_state_outputs = 0;
    for (unsigned int index = 0; index < NUMBER_OUTPUTS; index++)
    {
      if ((state_outputs & (1 << index)) != 0) 
      {
        if (index < NUMBER_OUTPUTS_1)
          temp_state_outputs |= 1 << (NUMBER_OUTPUTS_1 - index - 1);
        else
          temp_state_outputs |= 1 << index;
      }
    }
    if (control_state_outputs != temp_state_outputs) _SET_BIT(set_diagnostyka, ERROR_DIGITAL_OUTPUTS_BIT);
//    else _SET_BIT(clear_diagnostyka, ERROR_DIGITAL_OUTPUTS_BIT);
    
    //������� �������
    main_protection();
    /***********************************************************/

    /***********************************************************/
    //�������� �� ����������� ������� ������� ��ﳿ ����� ��� ������������
    /***********************************************************/
    //������� ����������
    if (periodical_tasks_TEST_TRG_FUNC != 0)
    {
      //����� � ���� ������� ������ ������� ������� ��ﳿ �����
      if ((state_i2c_task & STATE_TRG_FUNC_EEPROM_GOOD) != 0)
      {
        //������ ���� ����� ���, ���� ������� ���������� ������ ������� � ���������� ���������� ����
        if (
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_START_WRITE_TRG_FUNC_EEPROM_BIT) == 0) &&
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_WRITING_TRG_FUNC_EEPROM_BIT    ) == 0) &&
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_START_READ_TRG_FUNC_EEPROM_BIT ) == 0) &&
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_READING_TRG_FUNC_EEPROM_BIT    ) == 0)
           ) 
        {
          //�� ����� ������� �� ��� �������-����� ������� ����������, ���� ����� �������� ���������
          for (unsigned int i = 0; i < N_BIG; i++) trigger_active_functions_ctrl[i] = trigger_active_functions[i];
          crc_trg_func_ctrl = crc_trg_func;

          //������� ������� ������ ���������� �������� ��ﳿ 
          periodical_tasks_TEST_TRG_FUNC = false;
          //����������� ������� ������ �������� ����������� �� �������� ��ﳿ 
          periodical_tasks_TEST_TRG_FUNC_LOCK = true;
        }
      }
      else
      {
        //������� ������� ������ ���������� �������� ��ﳿ 
        periodical_tasks_TEST_TRG_FUNC = false;
      }
    }

    //���������� ���������
    if (periodical_tasks_TEST_INFO_REJESTRATOR_DR != 0)
    {
      //����� � ���� ������� ������ ������� ������� ��ﳿ �����
      if ((state_i2c_task & STATE_INFO_REJESTRATOR_DR_EEPROM_GOOD) != 0)
      {
        //������ ���� ����� ���, ���� ��������� ���������� ���������� ������ ������� � ���������� ���������� ����
        if (
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_START_WRITE_INFO_REJESTRATOR_DR_EEPROM_BIT) == 0) &&
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_WRITING_INFO_REJESTRATOR_DR_EEPROM_BIT    ) == 0) &&
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_START_READ_INFO_REJESTRATOR_DR_EEPROM_BIT ) == 0) &&
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_READING_INFO_REJESTRATOR_DR_EEPROM_BIT    ) == 0)
           ) 
        {
          //�� ����� ������� �� ��� �������-����� ��������� ���������� ����������, ���� ����� �������� ���������
          info_rejestrator_dr_ctrl = info_rejestrator_dr;
          crc_info_rejestrator_dr_ctrl = crc_info_rejestrator_dr;

          //������� ������� ������ ���������� �������� ��ﳿ 
          periodical_tasks_TEST_INFO_REJESTRATOR_DR = false;
          //����������� ������� ������ �������� ����������� �� �������� ��ﳿ 
          periodical_tasks_TEST_INFO_REJESTRATOR_DR_LOCK = true;
        }
      }
      else
      {
        //������� ������� ������ ���������� �������� ��ﳿ 
        periodical_tasks_TEST_INFO_REJESTRATOR_DR = false;
      }
    }
    /***********************************************************/

    /***********************************************************/
    //������������ "�������� ��������� ��� ���������� �����������"
    /***********************************************************/
    uint32_t capture_new;
    unsigned int delta;
    TIM2->CCR1 = (capture_new = (current_tick + (delta = TIM2_CCR1_VAL)));
    
    unsigned int repeat;
    unsigned int previous_tick;
    do
    {
      repeat = 0;
      current_tick = TIM2->CNT;

      uint64_t delta_time = 0;
      if (capture_new < current_tick)
        delta_time = capture_new + 0x100000000 - current_tick;
      else delta_time = capture_new - current_tick;

      if ((delta_time > delta) || (delta_time == 0))
      {
        if (TIM_GetITStatus(TIM2, TIM_IT_CC1) == RESET)
        {
          if (delta < TIM2_CCR1_VAL)
          {
            uint64_t delta_tick;
            if (current_tick < previous_tick)
              delta_tick = current_tick + 0x100000000 - previous_tick;
            else delta_tick = current_tick - previous_tick;
              
            delta = delta_tick + 1;
          }
          else if (delta == TIM2_CCR1_VAL)
            delta = 1; /*����������, ��� ���� ���������� ����������� �� ����� ������*/
          else
          {
            //���������� ����� ����� �� ���� � ����
            total_error_sw_fixed(83);
          }
          TIM2->CCR1 = (capture_new = (TIM2->CNT +  delta));
          previous_tick = current_tick;
          repeat = 0xff;
        }
      }
    }
    while (repeat != 0);
    /***********************************************************/
    
    /***********************************************************/
    //����������� ����������� ��� ��, ������� ������ �������
    /***********************************************************/
    control_word_of_watchdog |= WATCHDOG_PROTECTION;
    /***********************************************************/
    /***********************************************************************************************/
  }
  else
  {
    total_error_sw_fixed(23);
  }
  
#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordExitISR();
#endif
}
/*****************************************************/

