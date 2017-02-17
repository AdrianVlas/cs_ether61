#include "header.h"

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
      for(enum _menu2_levels i = MAIN_MANU2_LEVEL; i < MAX_NUMBER_MENU2_LEVEL; i++)
      {
        if ((i == PASSWORD_MENU2_LEVEL)/* || (i == EKRAN_LEVEL_SET_NEW_PASSWORD)*/) position_in_current_level_menu2[i] = INDEX_PASSWORD_M2_LINE1;
        else  position_in_current_level_menu2[i] = 0;
      }
  
      //���������� ����������� ����� ������
      current_state_menu2.current_level = MAIN_MANU2_LEVEL;
      current_state_menu2.index_position = position_in_current_level_menu2[current_state_menu2.current_level];
      current_state_menu2.position_cursor_y = current_state_menu2.index_position;
      current_state_menu2.p_max_row = NULL;
      current_state_menu2.max_row = MAX_ROW_MAIN_M2;
      current_state_menu2.func_move = move_into_main;
      current_state_menu2.func_show = make_ekran_main;
      current_state_menu2.func_press_enter = press_enter_in_main;
      current_state_menu2.func_press_esc = NULL;
      current_state_menu2.func_change = NULL;
      current_state_menu2.edition = ED_VIEWING;
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
void control_config(unsigned int modified)
{
  uint8_t crc_config_tmp = 0, temp_value;
  uint8_t  *point_1 = (unsigned char*)(&current_config_prt); // � ���� ������ �������� �������
  uint8_t  *point_2 = (unsigned char*)(&current_config);
  unsigned int i = 0, difference = 0;
  while ((difference == 0) && (i < sizeof(__CONFIG)))
  {
    //�������� ���������� ���� ����������� ��� �������
    temp_value = *(point_1++);
    crc_config_tmp += temp_value;

    //�������� ������������ ��� ������� � �������������-�����������
    if ((modified & MASKA_CHANGED_CONFIGURATION) == 0)
    {
      if (temp_value != *(point_2++)) difference = 0xff;
    }

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
void control_settings(unsigned int modified)
{
  uint8_t crc_settings_tmp = 0, temp_value;
  uint8_t  *point_1 = NULL, *point_2 = NULL; 
  unsigned int difference = 0;

  size_t size_of_block = 0;
  uint32_t n_item = 0;
  unsigned int item = 0, shift = 0;
  enum _id_fb block = _ID_FB_FIRST_ALL;
  while(
        (difference == 0) &&
        (block < _ID_FB_LAST_ALL)
       )
  {
    if (shift == 0)
    {
      //��������� ����� ������ �����
      switch (block)
      {
      case ID_FB_CONTROL_BLOCK:
        {
          if (item == 0)
          {
            size_of_block = sizeof(__SETTINGS_FIX);
            n_item = 1;
            if (modified == 0) point_2 = (uint8_t *)(&settings_fix);
          }
          
          point_1 = (uint8_t *)(&settings_fix_prt);
          
          break;
        }
      case ID_FB_INPUT:
        {
          if (item == 0)
          {
            size_of_block = sizeof(__settings_for_INPUT);
            n_item = current_config_prt.n_input;
            if (modified == 0) point_2 = (uint8_t *)(sca_of_p[ID_FB_INPUT - _ID_FB_FIRST_VAR]);
          }

          point_1 = (uint8_t *)(&(((__LN_INPUT*)spca_of_p_prt[ID_FB_INPUT - _ID_FB_FIRST_VAR]) + item)->settings) ;

          break;
        }
      case ID_FB_OUTPUT:
        {
          if (item == 0)
          {
            size_of_block = sizeof(__settings_for_OUTPUT);
            n_item = current_config_prt.n_output;
            if  (modified == 0) point_2 = (uint8_t *)(sca_of_p[ID_FB_OUTPUT - _ID_FB_FIRST_VAR]);
          }

          point_1 = (uint8_t *)(&(((__LN_OUTPUT*)spca_of_p_prt[ID_FB_OUTPUT - _ID_FB_FIRST_VAR]) + item)->settings) ;

          break;
        }
      case ID_FB_LED:
        {
          if (item == 0)
          {
            size_of_block = sizeof(__settings_for_LED);
            n_item = current_config_prt.n_led;
            if  (modified == 0) point_2 = (uint8_t *)(sca_of_p[ID_FB_LED - _ID_FB_FIRST_VAR]);
          }

          point_1 = (uint8_t *)(&(((__LN_LED*)spca_of_p_prt[ID_FB_LED - _ID_FB_FIRST_VAR]) + item)->settings) ;

          break;
        }
      case ID_FB_AND:
        {
          if (item == 0)
          {
            size_of_block = sizeof(__settings_for_AND);
            n_item = current_config_prt.n_and;
            if  (modified == 0) point_2 = (uint8_t *)(sca_of_p[ID_FB_AND - _ID_FB_FIRST_VAR]);
          }

          point_1 = (uint8_t *)(&(((__LN_AND*)spca_of_p_prt[ID_FB_AND - _ID_FB_FIRST_VAR]) + item)->settings) ;

          break;
        }
      case ID_FB_OR:
        {
          if (item == 0)
          {
            size_of_block = sizeof(__settings_for_OR);
            n_item = current_config_prt.n_or;
            if  (modified == 0) point_2 = (uint8_t *)(sca_of_p[ID_FB_OR - _ID_FB_FIRST_VAR]);
          }

          point_1 = (uint8_t *)(&(((__LN_OR*)spca_of_p_prt[ID_FB_OR - _ID_FB_FIRST_VAR]) + item)->settings) ;

          break;
        }
      case ID_FB_XOR:
        {
          if (item == 0)
          {
            size_of_block = sizeof(__settings_for_XOR);
            n_item = current_config_prt.n_xor;
            if  (modified == 0) point_2 = (uint8_t *)(sca_of_p[ID_FB_XOR - _ID_FB_FIRST_VAR]);
          }

          point_1 = (uint8_t *)(&(((__LN_XOR*)spca_of_p_prt[ID_FB_XOR - _ID_FB_FIRST_VAR]) + item)->settings) ;

          break;
        }
      case ID_FB_NOT:
        {
          if (item == 0)
          {
            size_of_block = sizeof(__settings_for_NOT);
            n_item = current_config_prt.n_not;
            if  (modified == 0) point_2 = (uint8_t *)(sca_of_p[ID_FB_NOT - _ID_FB_FIRST_VAR]);
          }

          point_1 = (uint8_t *)(&(((__LN_NOT*)spca_of_p_prt[ID_FB_NOT - _ID_FB_FIRST_VAR]) + item)->settings) ;

          break;
        }
      case ID_FB_TIMER:
        {
          if (item == 0)
          {
            size_of_block = sizeof(__settings_for_TIMER);
            n_item = current_config_prt.n_timer;
            if  (modified == 0) point_2 = (uint8_t *)(sca_of_p[ID_FB_TIMER - _ID_FB_FIRST_VAR]);
          }

          point_1 = (uint8_t *)(&(((__LN_TIMER*)spca_of_p_prt[ID_FB_TIMER - _ID_FB_FIRST_VAR]) + item)->settings) ;

          break;
        }
      case ID_FB_TRIGGER:
        {
          if (item == 0)
          {
            size_of_block = sizeof(__settings_for_TRIGGER);
            n_item = current_config_prt.n_trigger;
            if  (modified == 0) point_2 = (uint8_t *)(sca_of_p[ID_FB_TRIGGER - _ID_FB_FIRST_VAR]);
          }

          point_1 = (uint8_t *)(&(((__LN_TRIGGER*)spca_of_p_prt[ID_FB_TRIGGER - _ID_FB_FIRST_VAR]) + item)->settings) ;

          break;
        }
      case ID_FB_MEANDER:
        {
          if (item == 0)
          {
            size_of_block = sizeof(__settings_for_MEANDER);
            n_item = current_config_prt.n_meander;
            if  (modified == 0) point_2 = (uint8_t *)(sca_of_p[ID_FB_MEANDER - _ID_FB_FIRST_VAR]);
          }

          point_1 = (uint8_t *)(&(((__LN_MEANDER*)spca_of_p_prt[ID_FB_MEANDER - _ID_FB_FIRST_VAR]) + item)->settings) ;

          break;
        }
      default:
        {
          //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
          total_error_sw_fixed(5);
        }
      }
    }
      
    if ((size_of_block != 0) && (n_item != 0))
    {
      //�������� ���������� ���� ����������� ��� �������
      temp_value = *(point_1 + shift);
      crc_settings_tmp += temp_value;

      //�������� ����������� ��� ������� � ��������������-�����������
      if  (modified == 0)
      {
        if (point_2 != NULL)
        {
          if (temp_value != *(point_2 + shift)) difference = 0xff;
        }
        else
        {
          //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
          total_error_sw_fixed(61);
        }
      }
          
      if ((++shift) >= size_of_block)
      {
        shift = 0;
        if ((++item) >= n_item)
        {
          item = 0;
          block++;
          size_of_block = 0;
          n_item = 0;
        }
      }
    }
    else 
    {
      size_of_block = 0;
      n_item = 0;
          
      shift = 0;
      item = 0;
      block++;
    }
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
//���� ������������
/*****************************************************/
__result_dym_mem_select allocate_dynamic_memory_for_settings(__action_dym_mem_select make_remake_restore, unsigned int mem_for_prt, uintptr_t *p_sca_of_p_current[], uintptr_t *p_sca_of_p_control[], __CONFIG *current, __CONFIG *edited, __CONFIG *control)
{
  __result_dym_mem_select result = DYN_MEM_SELECT_OK;
  enum _id_fb index_1;
  
  if (make_remake_restore != RESTORE_DYN_MEM)
  {
    index_1 = _ID_FB_FIRST_VAR;
    while(
          (result == DYN_MEM_SELECT_OK) &&
          (index_1 < _ID_FB_LAST_VAR)
         )    
    {
      uint32_t n_prev, n_cur;
      size_t size;
      void (*min_param)(unsigned int, uintptr_t *, size_t, size_t);
      uint32_t *p_current_field;
      switch (index_1)
      {
      case ID_FB_INPUT:
        {
          //���������� ����
          n_prev = (make_remake_restore != MAKE_DYN_MEM) ? current->n_input : 0;
          p_current_field = &current->n_input;
          n_cur = edited->n_input;
          
          min_param = min_settings_INPUT;
          size = n_cur*((mem_for_prt == true) ? sizeof(__LN_INPUT) : sizeof(__settings_for_INPUT));
          break;
        }
      case ID_FB_OUTPUT:
        {
          //���������� �����
          n_prev = (make_remake_restore != MAKE_DYN_MEM) ? current->n_output : 0;
          p_current_field = &current->n_output;
          n_cur = edited->n_output;
          
          min_param = min_settings_OUTPUT;
          size = n_cur*((mem_for_prt == true) ? sizeof(__LN_OUTPUT) : sizeof(__settings_for_OUTPUT));
          break;
        }
      case ID_FB_LED:
        {
          //�������������
          n_prev = (make_remake_restore != MAKE_DYN_MEM) ? current->n_led : 0;
          p_current_field = &current->n_led;
          n_cur = edited->n_led;
          
          min_param = min_settings_LED;
          size = n_cur*((mem_for_prt == true) ? sizeof(__LN_LED) : sizeof(__settings_for_LED));
          break;
        }
      case ID_FB_AND:
        {
          //������� "�"
          n_prev = (make_remake_restore != MAKE_DYN_MEM) ? current->n_and : 0;
          p_current_field = &current->n_and;
          n_cur = edited->n_and;
          
          min_param = min_settings_AND;
          size = n_cur*((mem_for_prt == true) ? sizeof(__LN_AND) : sizeof(__settings_for_AND));
          break;
        }
      case ID_FB_OR:
        {
          //������� "���"
          n_prev = (make_remake_restore != MAKE_DYN_MEM) ? current->n_or : 0;
          p_current_field = &current->n_or;
          n_cur = edited->n_or;
          
          min_param = min_settings_OR;
          size = n_cur*((mem_for_prt == true) ? sizeof(__LN_OR) : sizeof(__settings_for_OR));
          break;
        }
      case ID_FB_XOR:
        {
          //������� "����.���"
          n_prev = (make_remake_restore != MAKE_DYN_MEM) ? current->n_xor : 0;
          p_current_field = &current->n_xor;
          n_cur = edited->n_xor;
          
          min_param = min_settings_XOR;
          size = n_cur*((mem_for_prt == true) ? sizeof(__LN_XOR) : sizeof(__settings_for_XOR));
          break;
        }
      case ID_FB_NOT:
        {
          //������� "��"
          n_prev = (make_remake_restore != MAKE_DYN_MEM) ? current->n_not : 0;
          p_current_field = &current->n_not;
          n_cur = edited->n_not;
          
          min_param = min_settings_NOT;
          size = n_cur*((mem_for_prt == true) ? sizeof(__LN_NOT) : sizeof(__settings_for_NOT));
          break;
        }
      case ID_FB_TIMER:
        {
          //������� "������"
          n_prev = (make_remake_restore != MAKE_DYN_MEM) ? current->n_timer : 0;
          p_current_field = &current->n_timer;
          n_cur = edited->n_timer;
          
          min_param = min_settings_TIMER;
          size = n_cur*((mem_for_prt == true) ? sizeof(__LN_TIMER) : sizeof(__settings_for_TIMER));
          break;
        }
      case ID_FB_TRIGGER:
        {
          //������� "�����"
          n_prev = (make_remake_restore != MAKE_DYN_MEM) ? current->n_trigger : 0;
          p_current_field = &current->n_trigger;
          n_cur = edited->n_trigger;
          
          min_param = min_settings_TRIGGER;
          size = n_cur*((mem_for_prt == true) ? sizeof(__LN_TRIGGER) : sizeof(__settings_for_TRIGGER));
          break;
        }
      case ID_FB_MEANDER:
        {
          //�������������� ���� "��������� ���������� �������"
          n_prev = (make_remake_restore != MAKE_DYN_MEM) ? current->n_meander : 0;
          p_current_field = &current->n_meander;
          n_cur = edited->n_meander;
          
          min_param = min_settings_MEANDER;
          size = n_cur*((mem_for_prt == true) ? sizeof(__LN_MEANDER) : sizeof(__settings_for_MEANDER));
          break;
        }
      default:
        {
          //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
          total_error_sw_fixed(22);
        }
      }
    
      if ((make_remake_restore == MAKE_DYN_MEM) || (n_cur != n_prev))
      {
        //��� ��� �������� ������ ��� ������ ���'��, ��� ������� �������������� ����� ��������
        if(size == 0) 
        {
          free(p_sca_of_p_current[index_1 - _ID_FB_FIRST_VAR]);
          p_sca_of_p_current[index_1 - _ID_FB_FIRST_VAR] = NULL;
        }
        else
        {
          uintptr_t *ptr= (uintptr_t*)realloc(p_sca_of_p_current[index_1 - _ID_FB_FIRST_VAR], size);
          if (ptr != NULL)
          {
            p_sca_of_p_current[index_1 - _ID_FB_FIRST_VAR] = ptr;
            if (n_cur > n_prev)
            {
              //��������� ������� ������������ ����� ����������� � ������� ��������
              (*min_param)(mem_for_prt, ptr, n_prev, n_cur);
            }
          }
          else 
          {
            if ((make_remake_restore == MAKE_DYN_MEM) || (mem_for_prt != false) || (control == NULL)) result = DYN_MEM_TOTAL_ERROR;
            else result = DYN_MEM_NO_ENOUGH_MEM;
          }
        }
      }

      //�������� �� ���� ���������� ��������������� ����� (� ������� ������ ���� ��� ��������, ��� � ��������� ����������)
      if (result == DYN_MEM_SELECT_OK) 
      {
        *p_current_field = n_cur;
        index_1++;
      }
      else if (result == DYN_MEM_NO_ENOUGH_MEM) index_1--;
    }
  }
  else
  {
    //���� ������������ ��� ���������� ����� �� ���������� �����
    index_1 = _ID_FB_LAST_VAR;
    index_1--;
  }
  
  /*
  ��� while ������� ����� � ���� �������, ���� ���'�� ��� �������� ��������� � �����������
  � � ������ ����������� �� ���������� ������������
  */
  
  /*****
  �������� �� ��, �� �� � ������� ������ ���������� ����� � ����������, ���� �� �� �������
  *****/
  if(
     (
      (
       (make_remake_restore == RESTORE_DYN_MEM) &&
       (result == DYN_MEM_SELECT_OK)  
      )   
      ||  
      (result == DYN_MEM_NO_ENOUGH_MEM)
     )
     && 
     (
      (make_remake_restore == MAKE_DYN_MEM) ||
      (mem_for_prt != false) ||
      (control == NULL)
     )
    )   
  {
    //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
    total_error_sw_fixed(66);
  }
  /*****/
  
  
  while(
        (
         (
          (make_remake_restore == RESTORE_DYN_MEM) &&
          (result == DYN_MEM_SELECT_OK)  
         )   
         ||  
         (result == DYN_MEM_NO_ENOUGH_MEM)
        )
        &&  
        (index_1 >= _ID_FB_FIRST_VAR)
       )   
  {
    uint32_t n_prev, n_cur;
    size_t size;
    void (*copy_settings_LN)(unsigned int, unsigned int, uintptr_t *, uintptr_t *, size_t, size_t);
    switch (index_1)
    {
    case ID_FB_INPUT:
      {
        //���������� ����
        n_cur  = current->n_input;
        current->n_input = n_prev = control->n_input;
        
        copy_settings_LN = copy_settings_INPUT;
        size = n_prev*sizeof(__settings_for_INPUT);
        break;
      }
    case ID_FB_OUTPUT:
      {
        //���������� �����
        n_cur  = current->n_output;
        current->n_output = n_prev = control->n_output;
        
        copy_settings_LN = copy_settings_OUTPUT;
        size = n_prev*sizeof(__settings_for_OUTPUT);
        break;
      }
    case ID_FB_LED:
      {
        //��������������
        n_cur  = current->n_led;
        current->n_led = n_prev = control->n_led;
        
        copy_settings_LN = copy_settings_LED;
        size = n_prev*sizeof(__settings_for_LED);
        break;
      }
    case ID_FB_AND:
      {
        //������� "�"
        n_cur  = current->n_and;
        current->n_and = n_prev = control->n_and;
        
        copy_settings_LN = copy_settings_AND;
        size = n_prev*sizeof(__settings_for_AND);
        break;
      }
    case ID_FB_OR:
      {
        //������� "���"
        n_cur  = current->n_or;
        current->n_or = n_prev = control->n_or;
        
        copy_settings_LN = copy_settings_OR;
        size = n_prev*sizeof(__settings_for_OR);
        break;
      }
    case ID_FB_XOR:
      {
        //������� "����.���"
        n_cur  = current->n_xor;
        current->n_xor = n_prev = control->n_xor;
        
        copy_settings_LN = copy_settings_XOR;
        size = n_prev*sizeof(__settings_for_XOR);
        break;
      }
    case ID_FB_NOT:
      {
        //������� "��"
        n_cur  = current->n_not;
        current->n_not = n_prev = control->n_not;
        
        copy_settings_LN = copy_settings_NOT;
        size = n_prev*sizeof(__settings_for_NOT);
        break;
      }
    case ID_FB_TIMER:
      {
        //������� "������"
        n_cur  = current->n_timer;
        current->n_timer = n_prev = control->n_timer;
        
        copy_settings_LN = copy_settings_TIMER;
        size = n_prev*sizeof(__settings_for_TIMER);
        break;
      }
    case ID_FB_TRIGGER:
      {
        //������� "�����"
        n_cur  = current->n_trigger;
        current->n_trigger = n_prev = control->n_trigger;
        
        copy_settings_LN = copy_settings_TRIGGER;
        size = n_prev*sizeof(__settings_for_TRIGGER);
        break;
      }
    case ID_FB_MEANDER:
      {
        //�������������� ���� "��������� ���������� �������"
        n_cur  = current->n_meander;
        current->n_meander = n_prev = control->n_meander;
        
        copy_settings_LN = copy_settings_MEANDER;
        size = n_prev*sizeof(__settings_for_MEANDER);
        break;
      }
    default:
      {
        //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
        total_error_sw_fixed(42);
      }
    }
    
    if (n_cur != n_prev)
    {
      //��� ��� �������� ������ ��� ������ ���'��, ��� ������� �������������� ����� ��������
      if(size == 0) 
      {
        free(p_sca_of_p_current[index_1 - _ID_FB_FIRST_VAR]);
        p_sca_of_p_current[index_1 - _ID_FB_FIRST_VAR] = NULL;
      }
      else
      {
        uintptr_t *ptr= (uintptr_t*)realloc(p_sca_of_p_current[index_1 - _ID_FB_FIRST_VAR], size);
        if (ptr != NULL)
        {
          p_sca_of_p_current[index_1 - _ID_FB_FIRST_VAR] = ptr;
          if (n_prev > n_cur)
          {
            if ( p_sca_of_p_control[index_1 - _ID_FB_FIRST_VAR] != NULL)
            {
              //��������� ������� ���������� ����� ����������� � �������� ��������
              (*copy_settings_LN)(false, (p_sca_of_p_control == spca_of_p_prt), ptr, p_sca_of_p_control[index_1 - _ID_FB_FIRST_VAR], n_cur, n_prev);
            }
            else
            {
              //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
              total_error_sw_fixed(43);
            }
          }
        }
        else  result = DYN_MEM_TOTAL_ERROR;
      }
    }
    
    //�������� �� ���� ���������� ��������������� �����
    if (result != DYN_MEM_TOTAL_ERROR) index_1--;
  }
  
  return result;
}
/*****************************************************/

/*****************************************************/
//������������ ��������� ��������� ��� ����������� �����
/*****************************************************/
void min_settings_INPUT(unsigned int mem_to_prt, uintptr_t *base, size_t index_first, size_t index_last)
{
  for (size_t shift = index_first; shift < index_last; shift++)
  {
    if (mem_to_prt == true) 
    {
      ((__LN_INPUT *)(base) + shift)->settings.delay.delay = KOEF_DOPUSK_DV_POST_MIN;
      ((__LN_INPUT *)(base) + shift)->settings.control = 0;
    }
    else 
    {
      ((__settings_for_INPUT *)(base) + shift)->delay.delay = KOEF_DOPUSK_DV_POST_MIN;
      ((__settings_for_INPUT *)(base) + shift)->control = 0;
    }
    
    if (mem_to_prt == true)
    {
      ((__LN_INPUT *)(base) + shift)->delay.delay = -1;
      for (size_t i = 0; i < DIV_TO_HIGHER(INPUT_SIGNALS, 8); i++)
      {
        ((__LN_INPUT *)(base) + shift)->active_state[i] = 0;
      }
    }
  }
}
/*****************************************************/

/*****************************************************/
//³��������� ��������� ��������� ��� ����������� �����
/*****************************************************/
void copy_settings_INPUT(unsigned int mem_to_prt, unsigned int mem_from_prt, uintptr_t *base_target, uintptr_t *base_source, size_t index_target, size_t index_source)
{
  for (size_t shift = index_target; shift < index_source; shift++)
  {
    if ((mem_to_prt == false) && (mem_from_prt == true))
    {
      ((__settings_for_INPUT *)(base_target) + shift)->control = ((__LN_INPUT *)(base_source) + shift)->settings.control;
      ((__settings_for_INPUT *)(base_target) + shift)->delay.delay = ((__LN_INPUT *)(base_source) + shift)->settings.delay.delay;
    }
    else if ((mem_to_prt == true) && (mem_from_prt == false))
    {
      ((__LN_INPUT *)(base_target) + shift)->settings.control = ((__settings_for_INPUT *)(base_source) + shift)->control;
      ((__LN_INPUT *)(base_target) + shift)->settings.delay.delay = ((__settings_for_INPUT *)(base_source) + shift)->delay.delay;
    }
    else if ((mem_to_prt == false) && (mem_from_prt == false))
    {
      ((__settings_for_INPUT *)(base_target) + shift)->control = ((__settings_for_INPUT *)(base_source) + shift)->control;
      ((__settings_for_INPUT *)(base_target) + shift)->delay.delay = ((__settings_for_INPUT *)(base_source) + shift)->delay.delay;
    }
    else
    {
      //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
      total_error_sw_fixed(90);
    }
  }
}
/*****************************************************/

/*****************************************************/
//������������ ��������� ��������� ��� ����������� ������
/*****************************************************/
void min_settings_OUTPUT(unsigned int mem_to_prt, uintptr_t *base, size_t index_first, size_t index_last)
{
  for (size_t shift = index_first; shift < index_last; shift++)
  {
    if (mem_to_prt == true) 
    {
      ((__LN_OUTPUT *)(base) + shift)->settings.control = 0;
      ((__LN_OUTPUT *)(base) + shift)->settings.param = 0;
    }
    else 
    {
      ((__settings_for_OUTPUT *)(base) + shift)->control = 0;
      ((__settings_for_OUTPUT *)(base) + shift)->param = 0;
    }
    
    if (mem_to_prt == true)
    {
      for (size_t i = 0; i < DIV_TO_HIGHER(OUTPUT_SIGNALS, 8); i++)
      {
        ((__LN_OUTPUT *)(base) + shift)->active_state[i] = 0;
      }
    }
  }
}
/*****************************************************/

/*****************************************************/
//³��������� ��������� ��������� ��� ����������� ������
/*****************************************************/
void copy_settings_OUTPUT(unsigned int mem_to_prt, unsigned int mem_from_prt, uintptr_t *base_target, uintptr_t *base_source, size_t index_target, size_t index_source)
{
  for (size_t shift = index_target; shift < index_source; shift++)
  {
    if ((mem_to_prt == false) && (mem_from_prt == true))
    {
      ((__settings_for_OUTPUT *)(base_target) + shift)->control = ((__LN_OUTPUT *)(base_source) + shift)->settings.control;
      ((__settings_for_OUTPUT *)(base_target) + shift)->param = ((__LN_OUTPUT *)(base_source) + shift)->settings.param;
    }
    else if ((mem_to_prt == true) && (mem_from_prt == false))
    {
      ((__LN_OUTPUT *)(base_target) + shift)->settings.control = ((__settings_for_OUTPUT *)(base_source) + shift)->control;
      ((__LN_OUTPUT *)(base_target) + shift)->settings.param = ((__settings_for_OUTPUT *)(base_source) + shift)->param;
    }
    else if ((mem_to_prt == false) && (mem_from_prt == false))
    {
      ((__settings_for_OUTPUT *)(base_target) + shift)->control = ((__settings_for_OUTPUT *)(base_source) + shift)->control;
      ((__settings_for_OUTPUT *)(base_target) + shift)->param = ((__settings_for_OUTPUT *)(base_source) + shift)->param;
    }
    else
    {
      //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
      total_error_sw_fixed(91);
    }
  }
}
/*****************************************************/

/*****************************************************/
//������������ ��������� ��������� ��� ��������������
/*****************************************************/
void min_settings_LED(unsigned int mem_to_prt, uintptr_t *base, size_t index_first, size_t index_last)
{
  for (size_t shift = index_first; shift < index_last; shift++)
  {
    if (mem_to_prt == true) 
    {
      ((__LN_LED *)(base) + shift)->settings.control = 0;
      ((__LN_LED *)(base) + shift)->settings.param = 0;
    }
    else 
    {
      ((__settings_for_LED *)(base) + shift)->control = 0;
      ((__settings_for_LED *)(base) + shift)->param = 0;
    }
    
    if (mem_to_prt == true)
    {
      for (size_t i = 0; i < DIV_TO_HIGHER(LED_SIGNALS, 8); i++)
      {
        ((__LN_LED *)(base) + shift)->active_state[i] = 0;
      }
    }
  }
}
/*****************************************************/

/*****************************************************/
//³��������� ��������� ��������� ��� ��������������
/*****************************************************/
void copy_settings_LED(unsigned int mem_to_prt, unsigned int mem_from_prt, uintptr_t *base_target, uintptr_t *base_source, size_t index_target, size_t index_source)
{
  for (size_t shift = index_target; shift < index_source; shift++)
  {
    if ((mem_to_prt == false) && (mem_from_prt == true))
    {
      ((__settings_for_LED *)(base_target) + shift)->control = ((__LN_LED *)(base_source) + shift)->settings.control;
      ((__settings_for_LED *)(base_target) + shift)->param = ((__LN_LED *)(base_source) + shift)->settings.param;
    }
    else if ((mem_to_prt == true) && (mem_from_prt == false))
    {
      ((__LN_LED *)(base_target) + shift)->settings.control = ((__settings_for_LED *)(base_source) + shift)->control;
      ((__LN_LED *)(base_target) + shift)->settings.param = ((__settings_for_LED *)(base_source) + shift)->param;
    }
    else if ((mem_to_prt == false) && (mem_from_prt == false))
    {
      ((__settings_for_LED *)(base_target) + shift)->control = ((__settings_for_LED *)(base_source) + shift)->control;
      ((__settings_for_LED *)(base_target) + shift)->param = ((__settings_for_LED *)(base_source) + shift)->param;
    }
    else
    {
      //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
      total_error_sw_fixed(92);
    }
  }
}
/*****************************************************/

/*****************************************************/
//������������ ��������� ��������� ��� �������� ���������� ����� "�"
/*****************************************************/
void min_settings_AND(unsigned int mem_to_prt, uintptr_t *base, size_t index_first, size_t index_last)
{
  for (size_t shift = index_first; shift < index_last; shift++)
  {
    for (size_t i = 0; i < NUMBER_IN_AND; i++)
    {
      if (mem_to_prt == true) ((__LN_AND *)(base) + shift)->settings.param[i] = 0;
      else ((__settings_for_AND *)(base) + shift)->param[i] = 0;
    }
    
    if (mem_to_prt == true)
    {
      for (size_t i = 0; i < DIV_TO_HIGHER(AND_SIGNALS, 8); i++)
      {
        ((__LN_AND *)(base) + shift)->active_state[i] = 0;
        ((__LN_AND *)(base) + shift)->trigger_state[i] = 0;
      }
    }
  }
}
/*****************************************************/

/*****************************************************/
//³��������� ��������� ��������� ��� �������� ���������� ����� "�"
/*****************************************************/
void copy_settings_AND(unsigned int mem_to_prt, unsigned int mem_from_prt, uintptr_t *base_target, uintptr_t *base_source, size_t index_target, size_t index_source)
{
  for (size_t shift = index_target; shift < index_source; shift++)
  {
    for (size_t i = 0; i < NUMBER_IN_AND; i++)
    {
      if ((mem_to_prt == false) && (mem_from_prt == true))
      {
        ((__settings_for_AND *)(base_target) + shift)->param[i] = ((__LN_AND *)(base_source) + shift)->settings.param[i];
      }
      else if ((mem_to_prt == true) && (mem_from_prt == false))
      {
        ((__LN_AND *)(base_target) + shift)->settings.param[i] = ((__settings_for_AND *)(base_source) + shift)->param[i];
      }
      else if ((mem_to_prt == false) && (mem_from_prt == false))
      {
        ((__settings_for_AND *)(base_target) + shift)->param[i] = ((__settings_for_AND *)(base_source) + shift)->param[i];
      }
      else
      {
        //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
        total_error_sw_fixed(93);
      }
    }
  }
}
/*****************************************************/

/*****************************************************/
//������������ ��������� ��������� ��� �������� ���������� ����� "���"
/*****************************************************/
void min_settings_OR(unsigned int mem_to_prt, uintptr_t *base, size_t index_first, size_t index_last)
{
  for (size_t shift = index_first; shift < index_last; shift++)
  {
    for (size_t i = 0; i < NUMBER_IN_OR; i++)
    {
      if (mem_to_prt == true) ((__LN_OR *)(base) + shift)->settings.param[i] = 0;
      else ((__settings_for_OR *)(base) + shift)->param[i] = 0;
    }
    
    if (mem_to_prt == true)
    {
      for (size_t i = 0; i < DIV_TO_HIGHER(OR_SIGNALS, 8); i++)
      {
        ((__LN_OR *)(base) + shift)->active_state[i] = 0;
        ((__LN_OR *)(base) + shift)->trigger_state[i] = 0;
      }
    }
  }
}
/*****************************************************/

/*****************************************************/
//³��������� ��������� ��������� ��� �������� ���������� ����� "���"
/*****************************************************/
void copy_settings_OR(unsigned int mem_to_prt, unsigned int mem_from_prt, uintptr_t *base_target, uintptr_t *base_source, size_t index_target, size_t index_source)
{
  for (size_t shift = index_target; shift < index_source; shift++)
  {
    for (size_t i = 0; i < NUMBER_IN_OR; i++)
    {
      if ((mem_to_prt == false) && (mem_from_prt == true))
      {
        ((__settings_for_OR *)(base_target) + shift)->param[i] = ((__LN_OR *)(base_source) + shift)->settings.param[i];
      }
      else if ((mem_to_prt == true) && (mem_from_prt == false))
      {
        ((__LN_OR *)(base_target) + shift)->settings.param[i] = ((__settings_for_OR *)(base_source) + shift)->param[i];
      }
      else if ((mem_to_prt == false) && (mem_from_prt == false))
      {
        ((__settings_for_OR *)(base_target) + shift)->param[i] = ((__settings_for_OR *)(base_source) + shift)->param[i];
      }
      else
      {
        //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
        total_error_sw_fixed(94);
      }
    }
  }
}
/*****************************************************/

/*****************************************************/
//������������ ��������� ��������� ��� �������� ���������� ����� "����.���"
/*****************************************************/
void min_settings_XOR(unsigned int mem_to_prt, uintptr_t *base, size_t index_first, size_t index_last)
{
  for (size_t shift = index_first; shift < index_last; shift++)
  {
    for (size_t i = 0; i < 2; i++)
    {
      if (mem_to_prt == true) ((__LN_XOR *)(base) + shift)->settings.param[i] = 0;
      else ((__settings_for_XOR *)(base) + shift)->param[i] = 0;
    }
    
    if (mem_to_prt == true)
    {
      for (size_t i = 0; i < DIV_TO_HIGHER(XOR_SIGNALS, 8); i++)
      {
        ((__LN_XOR *)(base) + shift)->active_state[i] = 0;
        ((__LN_XOR *)(base) + shift)->trigger_state[i] = 0;
      }
    }
  }
}
/*****************************************************/

/*****************************************************/
//³��������� ��������� ��������� ��� �������� ���������� ����� "����.���"
/*****************************************************/
void copy_settings_XOR(unsigned int mem_to_prt, unsigned int mem_from_prt, uintptr_t *base_target, uintptr_t *base_source, size_t index_target, size_t index_source)
{
  for (size_t shift = index_target; shift < index_source; shift++)
  {
    for (size_t i = 0; i < 2; i++)
    {
      if ((mem_to_prt == false) && (mem_from_prt == true))
      {
        ((__settings_for_XOR *)(base_target) + shift)->param[i] = ((__LN_XOR *)(base_source) + shift)->settings.param[i];
      }
      else if ((mem_to_prt == true) && (mem_from_prt == false))
      {
        ((__LN_XOR *)(base_target) + shift)->settings.param[i] = ((__settings_for_XOR *)(base_source) + shift)->param[i];
      }
      else if ((mem_to_prt == false) && (mem_from_prt == false))
      {
        ((__settings_for_XOR *)(base_target) + shift)->param[i] = ((__settings_for_XOR *)(base_source) + shift)->param[i];
      }
      else
      {
        //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
        total_error_sw_fixed(95);
      }
    }
  }
}
/*****************************************************/

/*****************************************************/
//������������ ��������� ��������� ��� �������� ���������� ����� "��"
/*****************************************************/
void min_settings_NOT(unsigned int mem_to_prt, uintptr_t *base, size_t index_first, size_t index_last)
{
  for (size_t shift = index_first; shift < index_last; shift++)
  {
    if (mem_to_prt == true) ((__LN_NOT *)(base) + shift)->settings.param = 0;
    else ((__settings_for_NOT *)(base) + shift)->param = 0;
    
    if (mem_to_prt == true)
    {
      for (size_t i = 0; i < DIV_TO_HIGHER(NOT_SIGNALS, 8); i++)
      {
        ((__LN_NOT *)(base) + shift)->active_state[i] = 0;
        ((__LN_NOT *)(base) + shift)->trigger_state[i] = 0;
      }
    }
  }
}
/*****************************************************/

/*****************************************************/
//³��������� ��������� ��������� ��� �������� ���������� ����� "��"
/*****************************************************/
void copy_settings_NOT(unsigned int mem_to_prt, unsigned int mem_from_prt, uintptr_t *base_target, uintptr_t *base_source, size_t index_target, size_t index_source)
{
  for (size_t shift = index_target; shift < index_source; shift++)
  {
    if ((mem_to_prt == false) && (mem_from_prt == true))
    {
      ((__settings_for_NOT *)(base_target) + shift)->param = ((__LN_NOT *)(base_source) + shift)->settings.param;
    }
    else if ((mem_to_prt == true) && (mem_from_prt == false))
    {
      ((__LN_NOT *)(base_target) + shift)->settings.param = ((__settings_for_NOT *)(base_source) + shift)->param;
    }
    else if ((mem_to_prt == false) && (mem_from_prt == false))
    {
      ((__settings_for_NOT *)(base_target) + shift)->param = ((__settings_for_NOT *)(base_source) + shift)->param;
    }
    else
    {
      //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
      total_error_sw_fixed(96);
    }
  }
}
/*****************************************************/

/*****************************************************/
//������������ ��������� ��������� ��� �������� "������"
/*****************************************************/
void min_settings_TIMER(unsigned int mem_to_prt, uintptr_t *base, size_t index_first, size_t index_last)
{
  for (size_t shift = index_first; shift < index_last; shift++)
  {
    if (mem_to_prt == true) 
    {
      ((__LN_TIMER *)(base) + shift)->settings.param = 0;
      ((__LN_TIMER *)(base) + shift)->settings.control = 0;
      ((__LN_TIMER *)(base) + shift)->settings.delay.delay_pause = TIMEOUT_DF_PAUSE_MIN;
      ((__LN_TIMER *)(base) + shift)->settings.delay.delay_work  = TIMEOUT_DF_WORK_MIN;
    }
    else 
    {
      ((__settings_for_TIMER *)(base) + shift)->param = 0;
      ((__settings_for_TIMER *)(base) + shift)->control = 0;
      ((__settings_for_TIMER *)(base) + shift)->delay.delay_pause = TIMEOUT_DF_PAUSE_MIN;
      ((__settings_for_TIMER *)(base) + shift)->delay.delay_work  = TIMEOUT_DF_WORK_MIN;
    }
    
    if (mem_to_prt == true)
    {
      ((__LN_TIMER *)(base) + shift)->delay.delay_pause = -1;
      ((__LN_TIMER *)(base) + shift)->delay.delay_work = -1;
      for (size_t i = 0; i < DIV_TO_HIGHER(TIMER_SIGNALS, 8); i++)
      {
        ((__LN_TIMER *)(base) + shift)->active_state[i] = 0;
        ((__LN_TIMER *)(base) + shift)->trigger_state[i] = 0;
      }
    }
  }
}
/*****************************************************/

/*****************************************************/
//³��������� ��������� ��������� ��� �������� "������"
/*****************************************************/
void copy_settings_TIMER(unsigned int mem_to_prt, unsigned int mem_from_prt, uintptr_t *base_target, uintptr_t *base_source, size_t index_target, size_t index_source)
{
  for (size_t shift = index_target; shift < index_source; shift++)
  {
    if ((mem_to_prt == false) && (mem_from_prt == true))
    {
      ((__settings_for_TIMER *)(base_target) + shift)->param             = ((__LN_TIMER *)(base_source) + shift)->settings.param;
      ((__settings_for_TIMER *)(base_target) + shift)->control           = ((__LN_TIMER *)(base_source) + shift)->settings.control;
      ((__settings_for_TIMER *)(base_target) + shift)->delay.delay_pause = ((__LN_TIMER *)(base_source) + shift)->settings.delay.delay_pause;
      ((__settings_for_TIMER *)(base_target) + shift)->delay.delay_work  = ((__LN_TIMER *)(base_source) + shift)->settings.delay.delay_work;
    }
    else if ((mem_to_prt == true) && (mem_from_prt == false))
    {
      ((__LN_TIMER *)(base_target) + shift)->settings.param             = ((__settings_for_TIMER *)(base_source) + shift)->param;
      ((__LN_TIMER *)(base_target) + shift)->settings.control           = ((__settings_for_TIMER *)(base_source) + shift)->control;
      ((__LN_TIMER *)(base_target) + shift)->settings.delay.delay_pause = ((__settings_for_TIMER *)(base_source) + shift)->delay.delay_pause;
      ((__LN_TIMER *)(base_target) + shift)->settings.delay.delay_work  = ((__settings_for_TIMER *)(base_source) + shift)->delay.delay_work;
    }
    else if ((mem_to_prt == false) && (mem_from_prt == false))
    {
      ((__settings_for_TIMER *)(base_target) + shift)->param             = ((__settings_for_TIMER *)(base_source) + shift)->param;
      ((__settings_for_TIMER *)(base_target) + shift)->control           = ((__settings_for_TIMER *)(base_source) + shift)->control;
      ((__settings_for_TIMER *)(base_target) + shift)->delay.delay_pause = ((__settings_for_TIMER *)(base_source) + shift)->delay.delay_pause;
      ((__settings_for_TIMER *)(base_target) + shift)->delay.delay_work  = ((__settings_for_TIMER *)(base_source) + shift)->delay.delay_work;
    }
    else
    {
      //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
      total_error_sw_fixed(97);
    }
  }
}
/*****************************************************/

/*****************************************************/
//������������ ��������� ��������� ��� �������� "�����"
/*****************************************************/
void min_settings_TRIGGER(unsigned int mem_to_prt, uintptr_t *base, size_t index_first, size_t index_last)
{
  for (size_t shift = index_first; shift < index_last; shift++)
  {
    for (size_t i = 0; i < INPUT_TRIGGER_SIGNALS; i++)
    {
      if (mem_to_prt == true) ((__LN_TRIGGER *)(base) + shift)->settings.param[i] = 0;
      else ((__settings_for_TRIGGER *)(base) + shift)->param[i] = 0;
    }
    
    if (mem_to_prt == true)
    {
      for (size_t i = 0; i < DIV_TO_HIGHER(TRIGGER_SIGNALS, 8); i++)
      {
        ((__LN_TRIGGER *)(base) + shift)->active_state[i] = 0;
        ((__LN_TRIGGER *)(base) + shift)->trigger_state[i] = 0;
      }
    }
  }
}
/*****************************************************/

/*****************************************************/
//³��������� ��������� ��������� ��� �������� "�����"
/*****************************************************/
void copy_settings_TRIGGER(unsigned int mem_to_prt, unsigned int mem_from_prt, uintptr_t *base_target, uintptr_t *base_source, size_t index_target, size_t index_source)
{
  for (size_t shift = index_target; shift < index_source; shift++)
  {
    for (size_t i = 0; i < INPUT_TRIGGER_SIGNALS; i++)
    {
      if ((mem_to_prt == false) && (mem_from_prt == true))
      {
        ((__settings_for_TRIGGER *)(base_target) + shift)->param[i] = ((__LN_TRIGGER *)(base_source) + shift)->settings.param[i];
      }
      else if ((mem_to_prt == true) && (mem_from_prt == false))
      {
        ((__LN_TRIGGER *)(base_target) + shift)->settings.param[i] = ((__settings_for_TRIGGER *)(base_source) + shift)->param[i];
      }
      else if ((mem_to_prt == false) && (mem_from_prt == false))
      {
        ((__settings_for_TRIGGER *)(base_target) + shift)->param[i] = ((__settings_for_TRIGGER *)(base_source) + shift)->param[i];
      }
      else
      {
        //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
        total_error_sw_fixed(98);
      }
    }
  }
}
/*****************************************************/

/*****************************************************/
//������������ ��������� ��������� ��� ��������������� ����� "��������� ���������� �������"
/*****************************************************/
void min_settings_MEANDER(unsigned int mem_to_prt, uintptr_t *base, size_t index_first, size_t index_last)
{
  for (size_t shift = index_first; shift < index_last; shift++)
  {
    if (mem_to_prt == true) 
    {
      ((__LN_MEANDER *)(base) + shift)->settings.delay.delay = TIMEOUT_MEANDER_MIN;
    }
    else 
    {
      ((__settings_for_MEANDER *)(base) + shift)->delay.delay = TIMEOUT_MEANDER_MIN;
    }
    
    if (mem_to_prt == true)
    {
      ((__LN_MEANDER *)(base) + shift)->delay.delay = -1;
      for (size_t i = 0; i < DIV_TO_HIGHER(MEANDER_SIGNALS, 8); i++)
      {
        ((__LN_MEANDER *)(base) + shift)->active_state[i] = 0;
        ((__LN_MEANDER *)(base) + shift)->trigger_state[i] = 0;
      }
    }
  }
}
/*****************************************************/

/*****************************************************/
//³��������� ��������� ��������� ��� ��������������� ����� "��������� ���������� �������"
/*****************************************************/
void copy_settings_MEANDER(unsigned int mem_to_prt, unsigned int mem_from_prt, uintptr_t *base_target, uintptr_t *base_source, size_t index_target, size_t index_source)
{
  for (size_t shift = index_target; shift < index_source; shift++)
  {
    if ((mem_to_prt == false) && (mem_from_prt == true))
    {
      ((__settings_for_MEANDER *)(base_target) + shift)->delay.delay = ((__LN_MEANDER *)(base_source) + shift)->settings.delay.delay;
    }
    else if ((mem_to_prt == true) && (mem_from_prt == false))
    {
      ((__LN_MEANDER *)(base_target) + shift)->settings.delay.delay = ((__settings_for_MEANDER *)(base_source) + shift)->delay.delay;
    }
    else if ((mem_to_prt == false) && (mem_from_prt == false))
    {
      ((__settings_for_MEANDER *)(base_target) + shift)->delay.delay = ((__settings_for_MEANDER *)(base_source) + shift)->delay.delay;
    }
    else
    {
      //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
      total_error_sw_fixed(99);
    }
  }
}
/*****************************************************/

/*****************************************************/
//����� � ������ ��� ����������� (���������� � ������)
/*****************************************************/
size_t size_all_settings(void)
{
  size_t size = sizeof(__SETTINGS_FIX);
  for (enum _id_fb i = _ID_FB_FIRST_VAR; i < _ID_FB_LAST_VAR; i++)
  {
    size_t size_block;
    switch (i)
    {
    case ID_FB_INPUT:
      {
        size_block = current_config.n_input*sizeof(__settings_for_INPUT);
        break;
      }
    case ID_FB_OUTPUT:
      {
        size_block = current_config.n_output*sizeof(__settings_for_OUTPUT);
        break;
      }
    case ID_FB_LED:
      {
        size_block = current_config.n_led*sizeof(__settings_for_LED);
        break;
      }
    case ID_FB_AND:
      {
        size_block = current_config.n_and*sizeof(__settings_for_AND);
        break;
      }
    case ID_FB_OR:
      {
        size_block = current_config.n_or*sizeof(__settings_for_OR);
        break;
      }
    case ID_FB_XOR:
      {
        size_block = current_config.n_xor*sizeof(__settings_for_XOR);
        break;
      }
    case ID_FB_NOT:
      {
        size_block = current_config.n_not*sizeof(__settings_for_NOT);
        break;
      }
    case ID_FB_TIMER:
      {
        size_block = current_config.n_timer*sizeof(__settings_for_TIMER);
        break;
      }
    case ID_FB_TRIGGER:
      {
        size_block = current_config.n_trigger*sizeof(__settings_for_TRIGGER);
        break;
      }
    case ID_FB_MEANDER:
      {
        size_block = current_config.n_meander*sizeof(__settings_for_MEANDER);
        break;
      }
    default:
      {
        //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
        total_error_sw_fixed(52);
      } 
    }
    
    size += size_block;
  }
  
  return size;
  
}
/*****************************************************/

/*****************************************************/
//³��������� ����������� � ���������� ����
/*****************************************************/
void copy_settings(
                     __CONFIG *source_conf, 

                     __SETTINGS_FIX *target_fix, 
                     __SETTINGS_FIX *source_fix, 
                     uintptr_t *target_dyn[], 
                     uintptr_t *source_dyn[]
                    )
{
  *target_fix = *source_fix;
  
  for (enum _id_fb i = _ID_FB_FIRST_VAR; i < _ID_FB_LAST_VAR; i++)
  {
    if (source_dyn[i - _ID_FB_FIRST_VAR] != NULL)
    {
      uint32_t n_prev;
      void (*copy_settings_LN)(unsigned int, unsigned int, uintptr_t *, uintptr_t *, size_t, size_t);
      switch (i)
      {
        case ID_FB_INPUT:
          {
            //���������� ����
            n_prev = source_conf->n_input;
            copy_settings_LN = copy_settings_INPUT;

            break;
          }
        case ID_FB_OUTPUT:
          {
            //���������� �����
            n_prev = source_conf->n_output;
            copy_settings_LN = copy_settings_OUTPUT;

            break;
          }
        case ID_FB_LED:
          {
            //��������������
            n_prev = source_conf->n_led;
            copy_settings_LN = copy_settings_LED;

            break;
          }
        case ID_FB_AND:
          {
            //������� "�"
            n_prev = source_conf->n_and;
            copy_settings_LN = copy_settings_AND;

            break;
          }
        case ID_FB_OR:
          {
            //������� "���"
            n_prev = source_conf->n_or;
            copy_settings_LN = copy_settings_OR;

            break;
          }
        case ID_FB_XOR:
          {
            //������� "����.���"
            n_prev = source_conf->n_xor;
            copy_settings_LN = copy_settings_XOR;

            break;
          }
        case ID_FB_NOT:
          {
            //������� "��"
            n_prev = source_conf->n_not;
            copy_settings_LN = copy_settings_NOT;

            break;
          }
        case ID_FB_TIMER:
          {
            //������� "������"
            n_prev = source_conf->n_timer;
            copy_settings_LN = copy_settings_TIMER;

            break;
          }
        case ID_FB_TRIGGER:
          {
            //������� "�����"
            n_prev = source_conf->n_trigger;
            copy_settings_LN = copy_settings_TRIGGER;

            break;
          }
        case ID_FB_MEANDER:
          {
            //�������������� ���� "��������� ���������� �������"
            n_prev = source_conf->n_meander;
            copy_settings_LN = copy_settings_MEANDER;

            break;
          }
        default:
          {
            //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
            total_error_sw_fixed(57);
          }
      }
      
      if ((n_prev != 0) && (target_dyn[i - _ID_FB_FIRST_VAR] != NULL))
      {
        //��������� ������� ���������� ����� ����������� � �������� ��������
        (*copy_settings_LN)((target_dyn == spca_of_p_prt), (source_dyn == spca_of_p_prt), target_dyn[i - _ID_FB_FIRST_VAR], source_dyn[i - _ID_FB_FIRST_VAR], 0, n_prev);
      }
      else
      {
        //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
        total_error_sw_fixed(59);
      }
    }
  }
}
/*****************************************************/

/*****************************************************/
/*
��������� ��� � ������������ � �������������

������ ����������
-----------------
1 - ������ ���� � "��� �������" ���������� � �������
0 - �������� �������� ���� �� "��� �������" � ��������� � ������� "����������" � "��� �����������"

������� ���������� ��� �������
-----------------
0 - ������ ��������� ��������
1 - ����������� ���'��, ��� ������� �������� ��������� ���� �� "��� �������"
2 - ����������� ���'�� � �� ������� �������� ��������� ���� �� "��� �������"
*/
/*****************************************************/
unsigned int set_config_and_settings(unsigned int direction, unsigned int source)
{
  unsigned int error = 0;
  if (direction)
  {
    __result_dym_mem_select result;
    //��������� �������� ���
    if (config_settings_modified & MASKA_CHANGED_CONFIGURATION)
    {
      __disable_interrupt();
      result = allocate_dynamic_memory_for_settings(REMAKE_DYN_MEM, true, spca_of_p_prt, NULL, &current_config_prt, &current_config, NULL);
    }

    if (result == DYN_MEM_SELECT_OK)
    {
      if (config_settings_modified & MASKA_CHANGED_SETTINGS)
      {
        //³��������� ���� � �������������
        __disable_interrupt(); /*������������ ���� ������������, � ���� �� ������������, ���� � ����� ���� ����������� ��� ������ ���� ������������, ��� �� �*/
        copy_settings(&current_config, &settings_fix_prt, &settings_fix, spca_of_p_prt, sca_of_p);
      }
      __enable_interrupt(); /*����� ���� ��������. �� ������������ ���������� ��� ���� ����������*/
    }
    else if (result == DYN_MEM_NO_ENOUGH_MEM) 
    {
      __enable_interrupt();
      /*
      ��� ������ ����������� �������� ���� ������������ ��� � ���� ������������ ����������� 
      �� ����� ������������ �����, ���� ����� �������� ����� ���� ����������� ������, �������,
      ������� �������.
      � ������ ������� ����� ������ �� �����. �� �� �� �����, � ����� ���� ����� ������ �����������,
      �� ���������� � ������ ���� ������������
      */
    }
    
    
    if (result == DYN_MEM_SELECT_OK)
    {
      _SET_BIT(clear_diagnostyka, ERROR_NO_FREE_DYNAMIC_MEMORY_BIT);
      
      if (config_settings_modified & MASKA_CHANGED_CONFIGURATION)
      {
        //�������� ��� �������� ���� ������������
        uint8_t *label_to_time_array;
        if (copying_time == 0) label_to_time_array = time;
        else label_to_time_array = time_copy;
        
        for (size_t i = 0; i < 7; i++) current_config_prt.time_config[i] = current_config.time_config[i] = current_config_edit.time_config[i] = *(label_to_time_array + i);
        current_config_prt.time_config[7] = current_config.time_config[7] = current_config_edit.time_config[7] = (uint8_t)(source & 0xff);
        
        _SET_BIT(control_i2c_taskes, TASK_START_WRITE_CONFIG_EEPROM_BIT);
        
        //���� ����������� ���� ������ ������ �����������. � �� ���� �������� �� ��������� ������� ����������, ���� �� ����� ��������
        _SET_BIT(control_i2c_taskes, TASK_START_WRITE_TRG_FUNC_EEPROM_BIT);
      }
      
      //�������� ��� �������� ���� ������������
      uint8_t *label_to_time_array;
      if (copying_time == 0) label_to_time_array = time;
      else label_to_time_array = time_copy;
        
      for (size_t i = 0; i < 7; i++) settings_fix_prt.time_setpoints[i] = settings_fix.time_setpoints[i] = settings_fix_edit.time_setpoints[i] = *(label_to_time_array + i);
      settings_fix_prt.time_setpoints[7] = settings_fix.time_setpoints[7] = settings_fix_edit.time_setpoints[7] = (uint8_t)(source & 0xff);
      
      _SET_BIT(control_i2c_taskes, TASK_START_WRITE_SETTINGS_EEPROM_BIT);
    }
    else if (result == DYN_MEM_NO_ENOUGH_MEM) 
    {
      _SET_BIT(set_diagnostyka, ERROR_NO_FREE_DYNAMIC_MEMORY_BIT);
      
      error = 1;
    }
    else 
    {
      _SET_BIT(set_diagnostyka, ERROR_NO_FREE_DYNAMIC_MEMORY_BIT);
      
      error = 2;
    }
  }
  else
  {
    //���������� �� ������ �� �����������
    if (config_settings_modified & MASKA_CHANGED_CONFIGURATION)
    {
      //³��������� ���� � ������������

      //��������� �� ����� "��� �������" �������� ��� � "���������" � "��� �����������"
      __result_dym_mem_select          result = allocate_dynamic_memory_for_settings(RESTORE_DYN_MEM, false, sca_of_p     , spca_of_p_prt, &current_config     , &current_config_prt, &current_config_prt);
      if (result == DYN_MEM_SELECT_OK) result = allocate_dynamic_memory_for_settings(RESTORE_DYN_MEM, false, sca_of_p_edit, sca_of_p     , &current_config_edit, &current_config    , &current_config    );
      
      if (result != DYN_MEM_SELECT_OK) 
      {
        _SET_BIT(set_diagnostyka, ERROR_NO_FREE_DYNAMIC_MEMORY_BIT);
        error = 2;
      }
      else
      {
        _SET_BIT(clear_diagnostyka, ERROR_NO_FREE_DYNAMIC_MEMORY_BIT);
      }
    }
    
    if (
        (error == 0) &&
        (config_settings_modified & MASKA_CHANGED_SETTINGS)
       )   
    {
      //³��������� ���� � �������������
      copy_settings(&current_config_prt, &settings_fix     , &settings_fix_prt, sca_of_p     , spca_of_p_prt);
      copy_settings(&current_config    , &settings_fix_edit, &settings_fix    , sca_of_p_edit, sca_of_p     );
    }
        
  }
  
  return error;
}
/*****************************************************/

/*****************************************************/
//������� ���������� ������ ��� ��� ������������
/*****************************************************/
__result_dym_mem_select action_after_changing_of_configuration(void)
{
  /*
  ���, �� ������� allocate_dynamic_memory_for_settings ���������� ���� ������������ � ������ ���'��� �� ��,
  � �� ��� �������� � ����������� ������ � �������� ��� ����������� current_config_edit,
  �� �� ��������� �������� ������� � ��������� ���������, ���������� �������� ��������
  � ��� ��� �������� allocate_dynamic_memory_for_settings �������� 䳿 �� ����������� ���� ������������ � �������
  �������� ���'��
  */
  __CONFIG current_config_tmp = current_config_edit;
  current_config_edit = current_config;
  __result_dym_mem_select result = allocate_dynamic_memory_for_settings(REMAKE_DYN_MEM, false, sca_of_p_edit, sca_of_p, &current_config_edit, &current_config_tmp , &current_config);
  if (result == DYN_MEM_SELECT_OK) 
  {
    result = allocate_dynamic_memory_for_settings(REMAKE_DYN_MEM, false, sca_of_p, spca_of_p_prt, &current_config, &current_config_edit, &current_config_prt);
    if (result == DYN_MEM_NO_ENOUGH_MEM)
    {
      //����� �������� � ���'�� ��� ����������� ��������� ������������
      __result_dym_mem_select result_1 =  allocate_dynamic_memory_for_settings(RESTORE_DYN_MEM, false, sca_of_p_edit, sca_of_p, &current_config_edit, &current_config, &current_config);
      if (result_1 == DYN_MEM_SELECT_OK) 
      {
        //³��������� ���� � �������������
        copy_settings(&current_config, &settings_fix_edit, &settings_fix, sca_of_p_edit, sca_of_p);
      }
      else result = DYN_MEM_TOTAL_ERROR;
    }
  }
  if (result == DYN_MEM_SELECT_OK)
  {
    uint32_t number[NUMBER_ALL_BLOCKS] = {
                                          1,
                                          current_config.n_input, 
                                          current_config.n_output,
                                          current_config.n_led, 
                                          current_config.n_and,
                                          current_config.n_or,
                                          current_config.n_xor,
                                          current_config.n_not,
                                          current_config.n_timer,
                                          current_config.n_trigger,
                                          current_config.n_meander
                                         };
    for (enum _id_fb i = _ID_FB_FIRST_VAR; i < _ID_FB_LAST_VAR; i++)
    {
      if (
           (i != ID_FB_INPUT) &&
           (i != ID_FB_MEANDER)
         )
      {
        uint32_t *p_param, *p_param_edit;
        intptr_t _n;
        switch (i)
        {
        case ID_FB_OUTPUT:
          {
            _n = 1;
            p_param      = &(((__settings_for_OUTPUT*)sca_of_p[i - _ID_FB_FIRST_VAR])->param);
            p_param_edit = &(((__settings_for_OUTPUT*)sca_of_p_edit[i - _ID_FB_FIRST_VAR])->param);
            break;
          }
        case ID_FB_LED:
          {
            _n = 1;
            p_param      = &(((__settings_for_LED*)sca_of_p[i - _ID_FB_FIRST_VAR])->param);
            p_param_edit = &(((__settings_for_LED*)sca_of_p_edit[i - _ID_FB_FIRST_VAR])->param);
            break;
          }
        case ID_FB_AND:
          {
            _n = NUMBER_IN_AND;
            p_param      = (((__settings_for_AND*)sca_of_p[i - _ID_FB_FIRST_VAR])->param);
            p_param_edit = (((__settings_for_AND*)sca_of_p_edit[i - _ID_FB_FIRST_VAR])->param);
            break;
          }
        case ID_FB_OR:
          {
            _n = NUMBER_IN_OR;
            p_param      = (((__settings_for_OR*)sca_of_p[i - _ID_FB_FIRST_VAR])->param);
            p_param_edit = (((__settings_for_OR*)sca_of_p_edit[i - _ID_FB_FIRST_VAR])->param);
            break;
          }
        case ID_FB_XOR:
          {
            _n = 2;
            p_param      = (((__settings_for_XOR*)sca_of_p[i - _ID_FB_FIRST_VAR])->param);
            p_param_edit = (((__settings_for_XOR*)sca_of_p_edit[i - _ID_FB_FIRST_VAR])->param);
            break;
          }
        case ID_FB_NOT:
          {
            _n = 1;
            p_param      = &(((__settings_for_NOT*)sca_of_p[i - _ID_FB_FIRST_VAR])->param);
            p_param_edit = &(((__settings_for_NOT*)sca_of_p_edit[i - _ID_FB_FIRST_VAR])->param);
            break;
          }
        case ID_FB_TIMER:
          {
            _n = 1;
            p_param      = &(((__settings_for_TIMER*)sca_of_p[i - _ID_FB_FIRST_VAR])->param);
            p_param_edit = &(((__settings_for_TIMER*)sca_of_p_edit[i - _ID_FB_FIRST_VAR])->param);
            break;
          }
        case ID_FB_TRIGGER:
          {
            _n = INPUT_TRIGGER_SIGNALS;
            p_param      = (((__settings_for_TRIGGER*)sca_of_p[i - _ID_FB_FIRST_VAR])->param);
            p_param_edit = (((__settings_for_TRIGGER*)sca_of_p_edit[i - _ID_FB_FIRST_VAR])->param);
            break;
          }
        default:
          {
            //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
            total_error_sw_fixed(76);
          }
        }
        
        intptr_t shift = 0;
        for (intptr_t j = 0; j < _n; j++)
        {
          uint32_t param_input = *(p_param + j - shift);
          unsigned int id_input   = (param_input >> SFIFT_PARAM_ID ) & MASKA_PARAM_ID ;
          unsigned int n_input    = (param_input >> SFIFT_PARAM_N  ) & MASKA_PARAM_N  ;
//          unsigned int out_input  = (param_input >> SFIFT_PARAM_OUT) & MASKA_PARAM_OUT;
          
          //������ ������� ��������������, ��� �� ���� ��������
          if (n_input > number[id_input - _ID_FB_FIRST_ALL])
          {
            *(p_param + j - shift) = *(p_param_edit + j - shift) = 0;
            
            for (intptr_t k = (j + 1); k < _n; k++)
            {
              *(p_param + (k - 1) - shift) = *(p_param_edit + (k - 1) - shift) = *(p_param + k - shift);
            }
            
            shift++;
          }
        }
      }
    }
  }
  
  return result;
}
/*****************************************************/

/*****************************************************/
/*****************************************************/
//
/*****************************************************/
