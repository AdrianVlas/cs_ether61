#include "header.h"

/*****************************************************/
//������� ���������� ������ ��� ��� ������������
/*****************************************************/
unsigned int action_after_changing_of_configuration(unsigned int new_configuration, __SETTINGS *target_label)
{
  unsigned int error_window = 0;
  
  /************************/
  //������ ���������� �� �� ����������� ����� �� � ������ ���, ��� ��������� �������� ������ ������������
  /************************/
  //�������� "�������� ���������"
  if ((new_configuration & (1<<CTRL_PHASE_BIT_CONFIGURATION)) == 0)
  {
    if(
       (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_CTRL_PHASE)
       ||
       (current_ekran.current_level == EKRAN_SETPOINT_CTRL_PHASE)
       ||  
       (current_ekran.current_level == EKRAN_TIMEOUT_CTRL_PHASE)
       ||  
       (current_ekran.current_level == EKRAN_CONTROL_CTRL_PHASE)
      )
      error_window |= (1 << CTRL_PHASE_BIT_CONFIGURATION );
  }
  /************************/
  
  if (error_window == 0)
  {
    //������� ���� ����������� � ������� ���������
    target_label->configuration = new_configuration;
    
    unsigned int maska[N_BIG] = {0, 0, 0, 0, 0, 0, 0};

    //����������, �� "�������� ���������" ����� ����� � ������������
    if ((target_label->configuration & (1<<CTRL_PHASE_BIT_CONFIGURATION)) == 0)
    {
      //������� �� "�������� ���������"
      target_label->control_ctrl_phase &= (unsigned int)(~CTR_CTRL_PHASE_U | CTR_CTRL_PHASE_PHI | CTR_CTRL_PHASE_F | CTR_CTRL_PHASE_SEQ_TN1 | CTR_CTRL_PHASE_SEQ_TN2);
   
      //������� ����� ������� "�������� ���������"
      maska[0] = 0;
      maska[1] = 0;
      maska[2] = 0;
      maska[3] = 0;
      maska[4] = 0;
      maska[5] = 0;
      for (int i = 0; i < NUMBER_CTRL_PHASE_SIGNAL_FOR_RANG; i++)
        _SET_BIT(
                 maska, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG    + 
                  i
                 )
                );


      //������ �� ������� ��� ���������� ������
      for (int i = 0; i < NUMBER_OUTPUTS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_outputs[N_BIG*i+j] &= ~maska[j];
      }
      //������ �� ������� ��� ���������� �������������
      for (int i = 0; i < NUMBER_LEDS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_leds[N_BIG*i+j] &= ~maska[j];
      }
      //������ �� ������� ��� ���������� �����������, ����������� ����������, ���� � �������� ����
      for (unsigned int j = 0; j < N_BIG; j++ ) 
      {
        target_label->ranguvannja_analog_registrator[j]  &= ~maska[j];
        target_label->ranguvannja_digital_registrator[j] &= ~maska[j];
        target_label->ranguvannja_silence[j] &= ~maska[j];
        target_label->ranguvannja_reset[j] &= ~maska[j];
      }
      //������ �� ������� ��� ���������� �����������
      for (int i = 0; i < NUMBER_ALARMS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_alarms[N_BIG*i+j] &= ~maska[j];
      }
      //������ �� ������� ��� ���������� �������������� �������
      for (int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_df[N_BIG*i+j] &= ~maska[j];
      }
      //������ �� ������� ��� ���������� �������������� ������
      for (int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_set_dt[N_BIG*i+j] &= ~maska[j];
      }
  
      //������ �� ������� ��� ���������� ������������ "�"
      for(unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_and[N_BIG*i+j] &= ~maska[j];
      }
  
      //������ �� ������� ��� ���������� ������������ "���"
      for(unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_or[N_BIG*i+j] &= ~maska[j];
      }
  
      //������ �� ������� ��� ���������� ������������ "����.���"
      for(unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_xor[N_BIG*i+j] &= ~maska[j];
      }
  
      //������ �� ������� ��� ���������� ������������ "��"
      for(unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_not[N_BIG*i+j] &= ~maska[j];
      }
    }

    //"��������� �����"
    for (unsigned int i = 0; i < N_BIG; i++ ) maska[i] = 0;

    //������� ����� ����� ��� ������� ��������� �����, �� ������� � ������������ � ��������� �������
    unsigned int array_defined_logic[NUMBER_DEFINED_ELEMENTS][2] =
    {
      {target_label->number_defined_df , NUMBER_DEFINED_FUNCTIONS},
      {target_label->number_defined_dt , NUMBER_DEFINED_TRIGGERS },
      {target_label->number_defined_and, NUMBER_DEFINED_AND      },
      {target_label->number_defined_or , NUMBER_DEFINED_OR       },
      {target_label->number_defined_xor, NUMBER_DEFINED_XOR      },
      {target_label->number_defined_not, NUMBER_DEFINED_NOT      }
    };
    const unsigned int others_signals[NUMBER_DEFINED_ELEMENTS] = {2, 3, 1, 1, 1, 1};
      
    unsigned int others_shift = NUMBER_GENERAL_SIGNAL_FOR_RANG;
    
    for (unsigned int index = 0; index < NUMBER_DEFINED_ELEMENTS; index++)
    {
      for (unsigned int i = 0; i < others_signals[index]*(array_defined_logic[index][1] - array_defined_logic[index][0]); i++)
        _SET_BIT(maska, (others_shift + others_signals[index]*array_defined_logic[index][0] + i));
       
      others_shift += others_signals[index]*array_defined_logic[index][1];
    }

    //������ �� ������� ��� ���������� ������
    for (int i = 0; i < NUMBER_OUTPUTS; i++)
    {
      for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_outputs[N_BIG*i+j] &= ~maska[j];
    }
    //������ �� ������� ��� ���������� �������������
    for (int i = 0; i < NUMBER_LEDS; i++)
    {
      for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_leds[N_BIG*i+j] &= ~maska[j];
    }
    //������ �� ������� ��� ���������� �����������, ����������� ����������, ���� � �������� ����
    for (unsigned int j = 0; j < N_BIG; j++ ) 
    {
      target_label->ranguvannja_analog_registrator[j]  &= ~maska[j];
      target_label->ranguvannja_digital_registrator[j] &= ~maska[j];
      target_label->ranguvannja_silence[j] &= ~maska[j];
      target_label->ranguvannja_reset[j] &= ~maska[j];
    }
    //������ �� ������� ��� ���������� �����������
    for (int i = 0; i < NUMBER_ALARMS; i++)
    {
      for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_alarms[N_BIG*i+j] &= ~maska[j];
    }
    //������� �� ������� ��� ���������� �������������� �������
    for (int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
    {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_df[N_BIG*i+j] &= ~maska[j];
    }
    //������� �� ������� ��� ���������� �������������� ������
    for (int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
    {
      for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_set_dt[N_BIG*i+j] &= ~maska[j];
    }
  
    //������� �� ������� ��� ���������� ������������ "�"
    for(unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
    {
      for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_and[N_BIG*i+j] &= ~maska[j];
    }
  
    //������� �� ������� ��� ���������� ������������ "���"
    for(unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
    {
      for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_or[N_BIG*i+j] &= ~maska[j];
    }
  
    //������� �� ������� ��� ���������� ������������ "����.���"
    for(unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
    {
      for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_xor[N_BIG*i+j] &= ~maska[j];
    }
  
    //������� �� ������� ��� ���������� ������������ "��"
    for(unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
    {
      for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_not[N_BIG*i+j] &= ~maska[j];
    }
  }
  
  return error_window;
}
/*****************************************************/

/*****************************************************/
//������� ���������� ������ ��� ��� ������� �������� ��������� �����
/*****************************************************/
void action_after_changing_number_el(__SETTINGS *target_label, unsigned int element)
{
  unsigned int maska[N_BIG] = {0, 0, 0, 0, 0, 0, 0};

  //������� ����� ����� ��� ������� ��������� �����, �� ������� � ������������ � ��������� �������
  unsigned int array_defined_logic[NUMBER_DEFINED_ELEMENTS][2] =
  {
    {target_label->number_defined_df , NUMBER_DEFINED_FUNCTIONS},
    {target_label->number_defined_dt , NUMBER_DEFINED_TRIGGERS },
    {target_label->number_defined_and, NUMBER_DEFINED_AND      },
    {target_label->number_defined_or , NUMBER_DEFINED_OR       },
    {target_label->number_defined_xor, NUMBER_DEFINED_XOR      },
    {target_label->number_defined_not, NUMBER_DEFINED_NOT      }
  };
  const unsigned int others_signals[NUMBER_DEFINED_ELEMENTS] = {2, 3, 1, 1, 1, 1};
  
  unsigned int others_shift = NUMBER_GENERAL_SIGNAL_FOR_RANG;
      
  for (unsigned int index = 0; index < element; index++)
  {
    others_shift += others_signals[index]*array_defined_logic[index][1];
  }
  
  //������� ����� ������� �������� ��������� �����
  for (unsigned int i = 0; i < others_signals[element]*(array_defined_logic[element][1] - array_defined_logic[element][0]); i++)
    _SET_BIT(maska, (others_shift + others_signals[element]*array_defined_logic[element][0] + i));
    
  //������ �� ������� ��� ���������� ������
  for (int i = 0; i < NUMBER_OUTPUTS; i++)
  {
    for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_outputs[N_BIG*i+j] &= ~maska[j];
  }
  //������ �� ������� ��� ���������� �������������
  for (int i = 0; i < NUMBER_LEDS; i++)
  {
    for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_leds[N_BIG*i+j] &= ~maska[j];
  }
  //������ �� ������� ��� ���������� �����������, ����������� ����������, ���� � �������� ����
  for (unsigned int j = 0; j < N_BIG; j++ ) 
  {
    target_label->ranguvannja_analog_registrator[j]  &= ~maska[j];
    target_label->ranguvannja_digital_registrator[j] &= ~maska[j];
    target_label->ranguvannja_silence[j] &= ~maska[j];
    target_label->ranguvannja_reset[j] &= ~maska[j];
  }
  //������ �� ������� ��� ���������� �����������
  for (int i = 0; i < NUMBER_ALARMS; i++)
  {
    for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_alarms[N_BIG*i+j] &= ~maska[j];
  }
  //������� �� ������� ��� ���������� �������������� �������
  for (int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
  {
    for (unsigned int j = 0; j < N_BIG; j++ ) 
    {
        target_label->ranguvannja_df[N_BIG*i+j] &= ~maska[j];
    }
  }
  //������� �� ������� ��� ���������� �������������� ������
  for (int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
  {
    for (unsigned int j = 0; j < N_BIG; j++ ) 
    {
      target_label->ranguvannja_set_dt[N_BIG*i+j]    &= ~maska[j];
      target_label->ranguvannja_reset_dt[N_BIG*i+j]  &= ~maska[j];
    }
  }
  
  //������� �� ������� ��� ���������� ������������ "�"
  for(unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
  {
    for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_and[N_BIG*i+j] &= ~maska[j];
  }
  
  //������� �� ������� ��� ���������� ������������ "���"
  for(unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
  {
    for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_or[N_BIG*i+j] &= ~maska[j];
  }
  
  //������� �� ������� ��� ���������� ������������ "����.���"
  for(unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
  {
    for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_xor[N_BIG*i+j] &= ~maska[j];
  }
  
  //������� �� ������� ��� ���������� ������������ "��"
  for(unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
  {
    for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_not[N_BIG*i+j] &= ~maska[j];
  }
}
/*****************************************************/

/*****************************************************/
//�������, ��� ������� ������� �� ��������, ���� ����������� �� ������� � RS-485 ����� � ����������� �����������
/*****************************************************/
void calculate_namber_bit_waiting_for_rs_485(void)
{
  unsigned int number_bits_rs_485_waiting_tmp = 10;
  
  //ϳ��������� ������� �� � ����
  if (current_settings.pare_bit_RS485 > 0) number_bits_rs_485_waiting_tmp++;
  if (current_settings.number_stop_bit_RS485 > 0) number_bits_rs_485_waiting_tmp++;
  
  //���������� ��������
  unsigned int multiplicator;
  switch (current_settings.speed_RS485)
  {
  case 0:
    {
      multiplicator = 12; //9600
      break;
    }
  case 1:
    {
      multiplicator = 8;  //14400
      break;
    }
  case 2:
    {
      multiplicator = 6;  //19200
      break;
    }
  case 3:
    {
      multiplicator = 4;  //28800
      break;
    }
  case 4:
    {
      multiplicator = 3;  //38400
      break;
    }
  case 5:
    {
      multiplicator = 2;  //57600
      break;
    }
  default:
    {
      multiplicator = 1;  //115200
      break;
    }
  }
  number_bits_rs_485_waiting_tmp *= (multiplicator*current_settings.time_out_1_RS485);
  number_bits_rs_485_waiting_tmp /= 10;
  
  number_bits_rs_485_waiting = number_bits_rs_485_waiting_tmp;
}
/*****************************************************/

/******************************************************
//�������, ��� ����� ���� � ���������� � ������� ����� ��������� � EEPROM
-----------------------------------------------------
setting_rang: 0 - ����� �������
              1 - ����� ����������
              2 - ���������� ��� ��������

source:       0 - ������� ���������
              1 - ���������
              2 - USB
              3 - RS-485
---------------------------------------------------
******************************************************/
void fix_change_settings(unsigned int setting_rang, unsigned int source)
{
  if (setting_rang < 2)
  {
    unsigned char *point_to_target;
  
    if (setting_rang == 0) point_to_target = (&current_settings)->time_setpoints;
    else point_to_target = (&current_settings)->time_ranguvannja;
    
    //�������� ��� ������� ���
    unsigned char *label_to_time_array;
    if (copying_time == 0) label_to_time_array = time;
    else label_to_time_array = time_copy;
    for (unsigned int i = 0; i < 7; i++) *(point_to_target + i) = *(label_to_time_array + i);
    *(point_to_target + 7) = (unsigned char)(source & 0xff);
  }
  
  //�������, �� ������� �������� � �� ����� ���� � ������� ������� ��������� � ������� � ���� ������ ������� �������
  changed_settings = CHANGED_ETAP_ENDED;
  if (_CHECK_SET_BIT(active_functions, RANG_SETTINGS_CHANGED) == 0) current_settings_interfaces = current_settings;
  
  //��������� ����� � EEPROM
  _SET_BIT(control_i2c_taskes, TASK_START_WRITE_SETTINGS_EEPROM_BIT);
}
/*****************************************************/

/*****************************************************/
/*
��������� �������� ��� � ������������ ����� ����������
*/
/*****************************************************/
unsigned int set_new_settings_from_interface(unsigned int source)
{
  unsigned int error = 0;
  
  //�������� �� ��������� ���
  unsigned char *label_to_time_array;
  if (copying_time == 0) label_to_time_array = time;
  else label_to_time_array = time_copy;
  
  if ((type_of_settings_changed & (1 << DEFAULT_SETTINGS_SET_BIT)) != 0)
  {
    for (unsigned int i = 0; i < 7; i++) current_settings_interfaces.time_setpoints[i] = current_settings_interfaces.time_ranguvannja[i] = *(label_to_time_array + i);
    current_settings_interfaces.time_setpoints[7] = current_settings_interfaces.time_ranguvannja[7] = 0;
  }
  
  if ((type_of_settings_changed & (1 << SETTINGS_DATA_CHANGED_BIT)) != 0)
  {
    for (unsigned int i = 0; i < 7; i++) current_settings_interfaces.time_setpoints[i] = *(label_to_time_array + i);
    current_settings_interfaces.time_setpoints[7] = source;
  }
  
  if ((type_of_settings_changed & (1 << RANGUVANNJA_DATA_CHANGED_BIT)) != 0)
  {
    for (unsigned int i = 0; i < 7; i++) current_settings_interfaces.time_ranguvannja[i] = *(label_to_time_array + i);
    current_settings_interfaces.time_ranguvannja[7] = source;
  }
  
  unsigned int reconfiguration_RS_485 = 0, reconfiguration_RS_485_with_reset_usart = 0;
  if (
      (current_settings.speed_RS485 != current_settings_interfaces.speed_RS485) ||
      (current_settings.pare_bit_RS485 != current_settings_interfaces.pare_bit_RS485) ||
      (current_settings.number_stop_bit_RS485 != current_settings_interfaces.number_stop_bit_RS485) ||
      (current_settings.time_out_1_RS485 != current_settings_interfaces.time_out_1_RS485)
     )
  {
    //�������, �� ����� ����������������� ��������� RS-485
    reconfiguration_RS_485 = 1;
    
    if (
        (current_settings.speed_RS485 != current_settings_interfaces.speed_RS485) ||
        (current_settings.pare_bit_RS485 != current_settings_interfaces.pare_bit_RS485) ||
        (current_settings.number_stop_bit_RS485 != current_settings_interfaces.number_stop_bit_RS485)
       )
    {
      //�������, �� ����� ����������������� USART ��� ���������� RS-485
      reconfiguration_RS_485_with_reset_usart = 1;
    }
  }
  
  unsigned int change_timeout_ar = 0;
  if (
      (current_settings.prefault_number_periods != current_settings_interfaces.prefault_number_periods) ||
      (current_settings.postfault_number_periods != current_settings_interfaces.postfault_number_periods)
     ) 
  {
    //�������, �� ��������� �� ����� ���� �������� 䳿 �� ��� ������� �������� ����������� ����������
    change_timeout_ar = 1;
    
    unsigned int semaphore_read_state_ar_record_copy = semaphore_read_state_ar_record;

    /*������������ ������� - ���� ����� ������ � ����, �� ���� ������ ������ ���� 
    ���� �� ��� - �� �� ��� ���� ������������ ����� ����� �������� �� �����, ���� � ��� ��� ���� �����,
    �� �� �� �������������� �, �������, ��������� ������ ����������� ���������� �� �����*/
    semaphore_read_state_ar_record = 1;

    if (
        (state_ar_record == STATE_AR_NO_RECORD      ) ||
        (state_ar_record == STATE_AR_TEMPORARY_BLOCK)
       )   
    {
      /*�� ����� ������ �� ��� ����� �������� ����������� ��������� �������,
      ���� ��� ���� ������� ��������� ��������� ������������ ���� ������
      ����������� ���������� � ������������ �����*/
      state_ar_record = STATE_AR_TEMPORARY_BLOCK; 
      
    }
    else
    {
      //�������� ��������� ����������, �� ��� ������ ����������� ����������
      error = ERROR_SLAVE_DEVICE_BUSY;
      semaphore_read_state_ar_record = semaphore_read_state_ar_record_copy;
    }
  }
  
  unsigned int set_password_USB = false;
  if (
      (current_settings.password_interface_USB != current_settings_interfaces.password_interface_USB) &&
      (current_settings_interfaces.password_interface_USB != 0)  
     )   
  {
    set_password_USB = true;
  }
  
  unsigned int set_password_RS485 = false;
  if (
      (current_settings.password_interface_RS485 != current_settings_interfaces.password_interface_RS485) &&
      (current_settings_interfaces.password_interface_RS485 != 0)  
     )   
  {
    set_password_RS485 = true;
  }
  
  if (error == 0)
  {
    if ((type_of_settings_changed & (1 << DEFAULT_SETTINGS_SET_BIT)) != 0)
    {
      //���������� ���� � ��������� ����
      for(unsigned int i=0; i<MAX_LEVEL_MENU; i++)
      {
        if ((i == EKRAN_LEVEL_PASSWORD) || (i == EKRAN_LEVEL_SET_NEW_PASSWORD)) position_in_current_level_menu[i] = INDEX_LINE_NUMBER_1_FOR_LEVEL_PASSWORD;
        else  position_in_current_level_menu[i] = 0;
        previous_level_in_current_level_menu[i] = -1;
      }
  
      //���������� ����������� ����� ������
      current_ekran.current_level = EKRAN_MAIN;
      current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
      current_ekran.position_cursor_y = current_ekran.index_position;
      current_ekran.edition = 0;
      current_ekran.cursor_on = 0;
      current_ekran.cursor_blinking_on = 0;  
    }
    
    if (set_password_USB   != false) password_set_USB   = 1;
    if (set_password_RS485 != false) password_set_RS485 = 1;
    
    //�������, �� ���� ��������� ����� ������ �����
    changed_settings = CHANGED_ETAP_EXECUTION;
              
    //������� ������ ���� � ������ ���������
    current_settings = current_settings_interfaces;
    if (reconfiguration_RS_485 != 0)
    {
      //ϳ��������� ���� �������� �������� � ����, ��� ����������� �� ������� � RS-485 ����� � ����������� �����������
      calculate_namber_bit_waiting_for_rs_485();
      //����������� ������� ��� ������������������ RS-485
      if (reconfiguration_RS_485_with_reset_usart != 0) make_reconfiguration_RS_485 = 0xff;
    }
    if (
        (state_ar_record == STATE_AR_TEMPORARY_BLOCK) ||
        (semaphore_read_state_ar_record != 0)  
       )
    {
      /*
      �� �������� ���� ����, ���� �������������� ������� ���������,
      ��� ���� ��������� ������ ����������� ��� ������������� �������
      ����������� ����������.
      ��� ����� ������ �� ����, �� ����� state_ar_record ���� �������
      STATE_AR_TEMPORARY_BLOCK � ����� semaphore_read_state_ar_record
      �� ���� ����. ��� �� 
      ����� �� ����������� - �� ����� ������������� ������,
      �� ��������� ������������ ���� ���� ���������������� ������.
      */
      if(
         ((change_timeout_ar != 0)) &&
         (state_ar_record == STATE_AR_TEMPORARY_BLOCK) &&
         (semaphore_read_state_ar_record != 0)  
        )
      {
        //�������� 䳿 �� ��� ������� �������� ����������� ����������
        actions_after_changing_tiomouts_ar();

        //������������ ������ ����������� ����������
        state_ar_record = STATE_AR_NO_RECORD;

        //������ �������
        semaphore_read_state_ar_record = 0;
      }
      else
      {
        //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
        total_error_sw_fixed(41);
      }
    }

    fix_change_settings(2, source);

    //����������� �������, �� �� ����� ����� �������� ����������
    new_state_keyboard |= (1<<BIT_REWRITE);
  }
  
  return error;
}
/*****************************************************/

/******************************************************
³��������� �������� �������
******************************************************/
void restore_trigger_functions(unsigned int *active_functions_point)
{
  active_functions[RANG_DT1_OUT >> 5] |= _CHECK_SET_BIT(active_functions_point, RANG_DT1_OUT);
  active_functions[RANG_DT2_OUT >> 5] |= _CHECK_SET_BIT(active_functions_point, RANG_DT2_OUT);
  active_functions[RANG_DT3_OUT >> 5] |= _CHECK_SET_BIT(active_functions_point, RANG_DT3_OUT);
  active_functions[RANG_DT4_OUT >> 5] |= _CHECK_SET_BIT(active_functions_point, RANG_DT4_OUT);
}
/*****************************************************/

/*****************************************************/
//������� ���� ���������� �� ����������
/*****************************************************/
void changing_diagnostyka_state(void)
{
  /*****
  ��������� ����� ���� ����������
  
  ��������:
  - ������� � �������� ������ ���, �� ����� ������� � �� ����� ��������� � 
    ����������
  - ϳ��� ����� ��������� ���� ��������� �������� ������ ��������� (�� 
    �������� �����!!!) ��� ��� � �������� ������
  - ���� , �� �� ������ ���������/��������������� ������� � ���������� ������
    (���� � ������ ����� ��������� ������������� ������� �������� �� 
    ������������� ��� ���, �� ���� �� ���������, � ����������� ��� ���������� 
    ������� ���� �������)

  - �������� ������� ��� � ���� ������������, �� �������� ��䳿 �� ������ 
    �������� �� ��������
  *****/
  unsigned int clear_diagnostyka_tmp[3], set_diagnostyka_tmp[3];
  
  clear_diagnostyka_tmp[0] = clear_diagnostyka[0];
  clear_diagnostyka_tmp[1] = clear_diagnostyka[1];
  clear_diagnostyka_tmp[2] = clear_diagnostyka[2];

  set_diagnostyka_tmp[0] = set_diagnostyka[0];
  set_diagnostyka_tmp[1] = set_diagnostyka[1];
  set_diagnostyka_tmp[2] = set_diagnostyka[2];
    
  diagnostyka[0] &= (unsigned int)(~clear_diagnostyka_tmp[0]); 
  diagnostyka[0] |= set_diagnostyka_tmp[0]; 

  diagnostyka[1] &= (unsigned int)(~clear_diagnostyka_tmp[1]); 
  diagnostyka[1] |= set_diagnostyka_tmp[1]; 

  diagnostyka[2] &= (unsigned int)(~clear_diagnostyka_tmp[2]); 
  diagnostyka[2] |= set_diagnostyka_tmp[2]; 
  
  diagnostyka[2] &= USED_BITS_IN_LAST_INDEX; 

  clear_diagnostyka[0] &= (unsigned int)(~clear_diagnostyka_tmp[0]);
  clear_diagnostyka[1] &= (unsigned int)(~clear_diagnostyka_tmp[1]);
  clear_diagnostyka[2] &= (unsigned int)(~clear_diagnostyka_tmp[2]);
  
  set_diagnostyka[0] &= (unsigned int)(~set_diagnostyka_tmp[0]);
  set_diagnostyka[1] &= (unsigned int)(~set_diagnostyka_tmp[1]);
  set_diagnostyka[2] &= (unsigned int)(~set_diagnostyka_tmp[2]);
  /*****/
  
  //���������, �� �������� ����
  unsigned int value_changes[3], diagnostyka_now[3];
  /*
  ������ ���� ��������� ����������, �� �� ������� ������ �� ���������� ��������,
  ���� �� ��� ������ ���� ��������� ��� ��������, �� �� �� ��������� � ��� �������
  */
  diagnostyka_now[0] = diagnostyka[0];
  diagnostyka_now[1] = diagnostyka[1];
  diagnostyka_now[2] = diagnostyka[2];
  value_changes[0] = diagnostyka_before[0] ^ diagnostyka_now[0];
  value_changes[1] = diagnostyka_before[1] ^ diagnostyka_now[1];
  value_changes[2] = diagnostyka_before[2] ^ diagnostyka_now[2];
  
  /*
  � ��������� ���������� ���� �� ������������ ����� ������� � ��������� ����� � ��������
  ����� ���� �� " ����� ����.    " � " ������� ����.  "
  ���� ������� � ��������� � ��������� �� �������� � ��� ���� �������� ���, �� ���� "��������������"
  */
  
  /*****/
  //���� " ����� ����.    "
  /*****/
  if (_CHECK_SET_BIT(value_changes, EVENT_START_SYSTEM_BIT) != 0)
  {
    //����������� �� ���� " ����� ����.    " ������ ��� ����
    if (_CHECK_SET_BIT(diagnostyka_now, EVENT_START_SYSTEM_BIT) == 0)
    {
      /*
      ����� ���� ��䳿 " ����� ����.    " � ���������� ����
      ���� ������ ���, ��� �� ���� �� ������ � ��������� ���������� ���� ����� ����������
      - ������ ������������ �� ��� ���� ����� ����������
      - ������ �����������, �� � ������������ ���� ���������� �� ���� ���� ��������
      - � �������� ���� ���������� ����� ����� �� �����, �� ��� ������ � ����������
      */
      _CLEAR_BIT(value_changes, EVENT_START_SYSTEM_BIT);
      _CLEAR_BIT(diagnostyka_before, EVENT_START_SYSTEM_BIT);
    }
  }
  /*****/
  
  /*****/
  //���� " ������� ����.  "
  /*****/
  if (_CHECK_SET_BIT(value_changes, EVENT_RESTART_SYSTEM_BIT) != 0)
  {
    //����������� �� ���� " ������� ����.  " ������ ��� ����
    if (_CHECK_SET_BIT(diagnostyka_now, EVENT_RESTART_SYSTEM_BIT) == 0)
    {
      /*
      ����� ���� ��䳿 " ������� ����.  " � ���������� ����
      ���� ������ ���, ��� �� ���� �� ������ � ��������� ���������� ���� ����� ����������
      - ������ ������������ �� ��� ���� ����� ����������
      - ������ �����������, �� � ������������ ���� ���������� �� ���� ���� ��������
      - � �������� ���� ���������� ����� ����� �� �����, �� ��� ������ � ����������
      */
      _CLEAR_BIT(value_changes, EVENT_RESTART_SYSTEM_BIT);
      _CLEAR_BIT(diagnostyka_before, EVENT_RESTART_SYSTEM_BIT);
    }
  }
  /*****/

  //����������, �� ����� ���������� 䳿 ��� ��� ����������
  if (
      (value_changes[0] != 0) ||
      (value_changes[1] != 0) ||
      (value_changes[2] != 0)
     )
  {
    //� ���, �� ����� ����������, ��� �����
    
    /*****/
    //��� ��������� ������� ����� � ��������� ���������� �������
    /*****/
    if (_CHECK_SET_BIT(diagnostyka, ERROR_PR_ERR_OVERLOAD_BIT) == 0)
    {
      /*
      ����� ����� ������ ����� ���, ���� ���������� �� ���� ����������� 
      ������������ ������, ������ ������, ���� ����� � ��������� ���������� ����
      ������ ���� ��� ������������ ������
      */
      
      //��������� ������� ��������� ������ � ����� ��� ���������� ���������� ����
      int number_empty_cells;
      unsigned int head = head_fifo_buffer_pr_err_records, tail = tail_fifo_buffer_pr_err_records;
      number_empty_cells = (int)(((unsigned int)tail) - ((unsigned int)head));
      while (number_empty_cells <= 0) number_empty_cells += MAX_NUMBER_RECORDS_PR_ERR_INTO_BUFFER;
      if (number_empty_cells == 1)
      {
        //�� � ������� ����� ������, �� �������, �� � ��� ������� ���������� ����� ���������� ������
        _SET_BIT(diagnostyka, ERROR_PR_ERR_OVERLOAD_BIT);
        /*
        �������� ���� ����� ����������, ��� ����� ��������� � ������ ������� ,����
        ������ ��������� ���� ��������� ����������, ��� ���� ��������� � � ���� �� ��������
        �� ���������� ��������� ����� ���������� �� ������� ���� ������� �
        ���������, �� ����� �� ������ ����������
        */
        diagnostyka_now[0] = diagnostyka[0];
        diagnostyka_now[1] = diagnostyka[1];
        diagnostyka_now[2] = diagnostyka[2];
        
        //ϳ��������� ���� ������� ��� � ����������
        value_changes[0] = diagnostyka_before[0] ^ diagnostyka_now[0];
        value_changes[1] = diagnostyka_before[1] ^ diagnostyka_now[1];
        value_changes[2] = diagnostyka_before[2] ^ diagnostyka_now[2];
      }

      //���������� ������� ��� �������
      unsigned int number_changes = 0;
      for(unsigned int i = 0; i < (8*sizeof(value_changes)); i++)
      {
        if (_CHECK_SET_BIT(value_changes, i) != 0) number_changes++;
      }

      if(number_changes != 0)
      {
        /*
        ���������� ���� ���� ��������, �� �� ������ ����������� ��� ������������ ������
        FIFO ��� ������ ���������� ���������� ����  � �� ���� ����� ����, ��� ��� �����
        � ����� ���� ����� ������, ���� �� ����� ��������� ����������� ��� ������������.
        ���� �� �� ������ ����� �� ����������� - ���� � �� ������ ������ ������ ������, 
        �� �������� � ���� ���� ����
        */
        
        //��������� ������ � ����� ������ ���������� ������� � ����� ���� ������ ����������� ���
        unsigned int index_into_buffer_pr_err = head*SIZE_ONE_RECORD_PR_ERR;
      
        //������� ���� ������� ������
        buffer_pr_err_records[index_into_buffer_pr_err + 0] = LABEL_START_RECORD_PR_ERR;

        //��� �������� ���� � ����������
        if(
           (_CHECK_SET_BIT(diagnostyka, EVENT_START_SYSTEM_BIT   ) == 0) &&
           (_CHECK_SET_BIT(diagnostyka, EVENT_RESTART_SYSTEM_BIT ) == 0) &&
           (_CHECK_SET_BIT(diagnostyka, EVENT_STOP_SYSTEM_BIT    ) == 0)
          )
        {
          //��� �������� ����� ����������� ���� - ����� ��������� ��� � ��� �
          unsigned char *label_to_time_array;
          if (copying_time == 0) label_to_time_array = time;
          else label_to_time_array = time_copy;
          for(unsigned int i = 0; i < 7; i++) buffer_pr_err_records[index_into_buffer_pr_err + 1 + i] = *(label_to_time_array + i);
        }
        else
        {
          //�� �� �������� ����� ����������� ���� - ���� ������ �� ���� �������� ������ 0xff, � ����, ���� ��������� ��� ���������, �� �� �� ���� ��������
          for(unsigned int i = 0; i < 7; i++)  buffer_pr_err_records[index_into_buffer_pr_err + 1 + i] = 0xff;
        }

        buffer_pr_err_records[index_into_buffer_pr_err + 8] = number_changes & 0xff;
      
        //�������� ��������� ���� ����������
        buffer_pr_err_records[index_into_buffer_pr_err + 9 ] =  diagnostyka_before[0]        & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 10] = (diagnostyka_before[0] >> 8 ) & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 11] = (diagnostyka_before[0] >> 16) & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 12] = (diagnostyka_before[0] >> 24) & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 13] =  diagnostyka_before[1]        & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 14] = (diagnostyka_before[1] >> 8 ) & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 15] = (diagnostyka_before[1] >> 16) & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 16] = (diagnostyka_before[1] >> 24) & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 17] =  diagnostyka_before[2]        & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 18] = (diagnostyka_before[2] >> 8 ) & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 19] = (diagnostyka_before[2] >> 16) & 0xff;

        //�������� �������� ���� ����������
        buffer_pr_err_records[index_into_buffer_pr_err + 20] =  diagnostyka_now[0]        & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 21] = (diagnostyka_now[0] >> 8 ) & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 22] = (diagnostyka_now[0] >> 16) & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 23] = (diagnostyka_now[0] >> 24) & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 24] =  diagnostyka_now[1]        & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 25] = (diagnostyka_now[1] >> 8 ) & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 26] = (diagnostyka_now[1] >> 16) & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 27] = (diagnostyka_now[1] >> 24) & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 28] =  diagnostyka_now[2]        & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 29] = (diagnostyka_now[2] >> 8 ) & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 30] = (diagnostyka_now[2] >> 16) & 0xff;
        
        /*
        � ��������� ���������� ���� �� ������������ ����� ������� � ��������� ����� � ��������
        ����� ��䳿 �� " �������.����.  " � �� ���� �� �� ���� �������� � ������� ����������,
        �� ��, ��������, ����, ��� ���� ���� �� ���������� ��������� ������� � ������.
        ����� �� ���� "������������ ������ ������"
        ���� ������� � ��������� � ������� �� ������� � ����� �������� ��� �� � �������� ���������,
        ��� ���� ����� ���� ���������� � �����. ���� ������� �� ��������� ���� ����������, ��� � � �����
        ����������, ���� ������� �� ������� ���� ���� ����������
        */
        if (_CHECK_SET_BIT(value_changes, EVENT_STOP_SYSTEM_BIT) != 0)
        {
          /*
          C��� ��䳿 " �������.����.  " ������� (����������� ����� ���������� �������������
          ���� � ��������� ����� � �������� ����)
          ���� ������ ��� ��������
          - ������ ������������ �� � �����, ���� ����� ���� ���������� � ����� ������������ �����
          - ������ ������������ �� � �����, ���� ������� �� ������� ���� ���� ����������
          */
          _CLEAR_BIT(diagnostyka_now, EVENT_STOP_SYSTEM_BIT);
          _CLEAR_BIT(diagnostyka, EVENT_STOP_SYSTEM_BIT);
        }

        //Գ����� ��������� ����, ���� �� ��� �������� � ������� ����� ������ ��������� ��� ����
        diagnostyka_before[0] = diagnostyka_now[0];
        diagnostyka_before[1] = diagnostyka_now[1];
        diagnostyka_before[2] = diagnostyka_now[2];

        //ϳ������������� �� ������� ������ � ��������� ���������� ����
          unsigned int next_index_into_fifo_buffer = head + 1;
          while(next_index_into_fifo_buffer >= MAX_NUMBER_RECORDS_PR_ERR_INTO_BUFFER) next_index_into_fifo_buffer -= MAX_NUMBER_RECORDS_PR_ERR_INTO_BUFFER;
          
          //������������ ���� �������� ������ ������ FIFO 
          head_fifo_buffer_pr_err_records = next_index_into_fifo_buffer;
      }
    }
    /*****/
  }
}
/*****************************************************/

/*****************************************************/
//ϳ�������� ������� ����������� �������� � ������ ���'�� ������� ��� ����������� ���������� source
/*****************************************************/
unsigned int count_number_set_bit(unsigned int* source, unsigned int total_number)
{
  unsigned int count = 0;
  unsigned int offset, shift;
  
  for (unsigned int i = 0; i < total_number; i++)  
  {
    offset = i >> 5; //�� �, ��������, ������ �� 32
    shift  = (i & (32 - 1)); //�� �, ��������, ���������� ������ �� ������ �� 32
    if ( ( (*(source + offset))  & ((unsigned int)(1 << shift)) ) != 0 ) count++;
  }

  return count;
}
/*****************************************************/

/*****************************************************/
//�������� ����������� ������������
/*****************************************************/
void control_config(void)
{
  unsigned char crc_config_tmp = 0, temp_value_1, temp_value_2;
  unsigned char  *point_1 = (unsigned char*)(&current_config); 
  unsigned char  *point_2 = (unsigned char*)(&current_config_prt); 
  unsigned int i = 0, difference = 0;
  while ((difference == 0) && (i < sizeof(__CONFIG)))
  {
    temp_value_1 = *(point_1);
    temp_value_2 = *(point_2);
    crc_config_tmp += temp_value_1;
    if (temp_value_1 != temp_value_2) difference = 0xff;
    point_1++;
    point_2++;
    i++;
  }
  
  if ((difference == 0) && (crc_config == crc_config_tmp))
  {
    //�������� ����������� ������������ ������� ������
    
    //������� ����������� � ���� ����������
    _SET_BIT(clear_diagnostyka, ERROR_CONFIG_EEPROM_CONTROL_BIT);
  }
  else
  {
    //�������� ����������� ������������ �� �������

    //���������� ����������� � ���� ����������
    _SET_BIT(set_diagnostyka, ERROR_CONFIG_EEPROM_CONTROL_BIT);
  }
}
/*****************************************************/

/*****************************************************/
//�������� ����������� ���������
/*****************************************************/
void control_settings(void)
{
  unsigned char crc_settings_tmp = 0, temp_value_1, temp_value_2;
  unsigned char  *point_1 = (unsigned char*)(&current_settings); 
  unsigned char  *point_2 = (unsigned char*)(&current_settings_prt); 
  unsigned int i = 0, difference = 0;
  while ((difference == 0) && (i < sizeof(__SETTINGS)))
  {
    temp_value_1 = *(point_1);
    temp_value_2 = *(point_2);
    crc_settings_tmp += temp_value_1;
    if (temp_value_1 != temp_value_2) difference = 0xff;
    point_1++;
    point_2++;
    i++;
  }
  
  if ((difference == 0) && (crc_settings == crc_settings_tmp))
  {
    //�������� ����������� ������� ��������� ������� ������
    
    //������� ����������� � ���� ����������
    _SET_BIT(clear_diagnostyka, ERROR_SETTINGS_EEPROM_CONTROL_BIT);
  }
  else
  {
    //�������� ����������� ������� ��������� �� �������

    //���������� ����������� � ���� ����������
    _SET_BIT(set_diagnostyka, ERROR_SETTINGS_EEPROM_CONTROL_BIT);
  }
}
/*****************************************************/

/*****************************************************/
//�������� ����������� ���������
/*****************************************************/
void control_ustuvannja(void)
{
  unsigned char crc_ustuvannja_tmp = 0, temp_value_1, temp_value_2;
  unsigned char  *point_1; 
  unsigned char  *point_2; 
  unsigned int i = 0, difference = 0;

  /*
  � ��������� ���� ������� ������������� ���� ������ �������� �����������,
  ���� ����� ����� ���� unsigned int
  */
  unsigned int adjustment_id_tmp = ADJUSTMENT_ID;
  point_1 = (unsigned char*)(&adjustment_id_tmp); 
  while (i < sizeof(unsigned int))
  {
    temp_value_1 = *(point_1);
    crc_ustuvannja_tmp += temp_value_1;
    point_1++;
    i++;
  }
  
  point_1 = (unsigned char*)(&ustuvannja); 
  point_2 = (unsigned char*)(&ustuvannja_meas);
  i = 0;
  while ((difference == 0) && (i < sizeof(ustuvannja)))
  {
    temp_value_1 = *(point_1);
    temp_value_2 = *(point_2);
    crc_ustuvannja_tmp += temp_value_1;
    if (temp_value_1 != temp_value_2) difference = 0xff;
    point_1++;
    point_2++;
    i++;
  }

  /*
  �� ����� ������������ ����������� ������� ������, ���� ������ � ����������� ��������,
  �� �������� �������, �� �� ������ ����������������� � ������������� �� ����� ��������,
  �� ���� ������ �� ����
  */
  point_1 = (unsigned char*)(&serial_number_dev); 
  i = 0;
  while ((difference == 0) && (i < sizeof(serial_number_dev)))
  {
    temp_value_1 = *(point_1);
    crc_ustuvannja_tmp += temp_value_1;
    point_1++;
    i++;
  }
  
  if ((difference == 0) && (crc_ustuvannja == crc_ustuvannja_tmp))
  {
    //�������� ����������� ��������� ������� ������
    
    //������� ����������� � ���� ����������
    _SET_BIT(clear_diagnostyka, ERROR_USTUVANNJA_EEPROM_CONTROL_BIT);
  }
  else
  {
    //�������� ����������� ��������� �� �������

    //���������� ����������� � ���� ����������
    _SET_BIT(set_diagnostyka, ERROR_USTUVANNJA_EEPROM_CONTROL_BIT);
  }
}
/*****************************************************/

/*****************************************************/
//�������� ����������� ������� ����������
/*****************************************************/
void control_trg_func(void)
{
  unsigned char crc_trg_func_tmp = 0, temp_value_1;
  unsigned char  *point; 
  unsigned int i;
  
  point = (unsigned char*)(trigger_active_functions_ctrl);
  i = 0;  
  while (i < sizeof(trigger_active_functions_ctrl))
  {
    temp_value_1 = *(point);
    crc_trg_func_tmp += temp_value_1;
    point++;
    i++;
  }
  
  if (crc_trg_func == crc_trg_func_tmp)
  {
    //�������� ����������� ��������� ������� ������
    
    //������� ����������� � ���� ����������
    _SET_BIT(clear_diagnostyka, ERROR_TRG_FUNC_EEPROM_CONTROL_BIT);
  }
  else
  {
    //�������� ����������� ��������� �� �������

    //���������� ����������� � ���� ����������
    _SET_BIT(set_diagnostyka, ERROR_TRG_FUNC_EEPROM_CONTROL_BIT);
  }
}
/*****************************************************/

/*****************************************************/
//�������� ����������� ���������� �� �����������
/*****************************************************/
unsigned int control_info_rejestrator(__INFO_REJESTRATOR* info_rejestrator_point, unsigned char crc_info_rejestrator)
{
  unsigned int result;
  unsigned char crc_info_rejestrator_tmp = 0, temp_value_1;
  unsigned char  *point = (unsigned char*)(info_rejestrator_point); 
  unsigned int i = 0;
  while (i < sizeof(__INFO_REJESTRATOR))
  {
    temp_value_1 = *(point);
    crc_info_rejestrator_tmp += temp_value_1;
    point++;
    i++;
  }
  
  if (crc_info_rejestrator == crc_info_rejestrator_tmp)
  {
    //�������� ����������� ���������� ������� ������
    result = 1;    
  }
  else
  {
    //�������� ����������� ���������� �� �������
    result = 0;    
  }
  
  return result;
}
/*****************************************************/

/*****************************************************/
//���������� ������ ����� � ��������� __DATE__
/*****************************************************/
int str_to_int_DATE_Mmm(void)
{
  static const char month_names[12*3] = "JanFebMarAprMayJunJulAugSepOctNovDec";
  
  int i = 0;
  while(i < 12)
  {
    if (
        (month_names[3*i + 0] == __DATE__[0]) &&
        (month_names[3*i + 1] == __DATE__[1]) &&
        (month_names[3*i + 2] == __DATE__[2])
       )
    {
      break;
    }
    
    i++;
  } 
  
  if (i < 12)
  { 
    i += 1; /*��� ��������� ������ ���� � "1", � �� � "0"*/
    return ((i / 10) << 4) + (i % 10);
  }
  else return 0;
}
/*****************************************************/

/*****************************************************/
/*****************************************************/
//
/*****************************************************/
