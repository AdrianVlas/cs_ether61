#include "header.h"

/*****************************************************/
/*
������� ���������� �� ����

����� ���������
(1 << BIT_REWRITE) - ������������ ����
(1 << BIT_KEY_DOWN) - �������� ����
(1 << BIT_KEY_UP) - �������� �����
*/
/*****************************************************/
void move_into_list_settings(unsigned int action, int max_row)
{
  if (action & ((1 << BIT_REWRITE) | (1 << BIT_KEY_DOWN)))
  {
    if (action & (1 << BIT_KEY_DOWN)) current_state_menu2.index_position++;
    do
    {
      if(current_state_menu2.index_position >= max_row) current_state_menu2.index_position = 0;
      while (
             (
              (current_state_menu2.index_position == INDEX_LIST_SETTINGS_M2_TIMERS) &&
              (current_config.n_timer == 0)
             )
             ||
             (
              (current_state_menu2.index_position == INDEX_LIST_SETTINGS_M2_MEANDER) &&
              (current_config.n_meander == 0)
             )
             ||
             (
              (current_state_menu2.index_position == INDEX_LIST_SETTINGS_M2_CTRL_ANALOG_INPUTS) &&
              (current_config.n_ctrl_analog_inputs == 0)
             )
             ||
             (
              (current_state_menu2.index_position == INDEX_LIST_SETTINGS_M2_ALARMS) &&
              (current_config.n_alarm == 0)
             )
            )
      {
        if(++current_state_menu2.index_position >= max_row) current_state_menu2.index_position = 0;
      }
    }
    while ((action & (1 << BIT_KEY_DOWN)) && (current_state_menu2.index_position >= max_row));
  }
  else if (action & (1 << BIT_KEY_UP))
  {
    current_state_menu2.index_position--;
    do
    {
      if(current_state_menu2.index_position < 0) current_state_menu2.index_position = max_row - 1;
      while (
             (
              (current_state_menu2.index_position == INDEX_LIST_SETTINGS_M2_TIMERS) &&
              (current_config.n_timer == 0)
             )
             ||
             (
              (current_state_menu2.index_position == INDEX_LIST_SETTINGS_M2_MEANDER) &&
              (current_config.n_meander == 0)
             )
             ||
             (
              (current_state_menu2.index_position == INDEX_LIST_SETTINGS_M2_ALARMS) &&
              (current_config.n_alarm == 0)
             )
             ||
             (
              (current_state_menu2.index_position == INDEX_LIST_SETTINGS_M2_CTRL_ANALOG_INPUTS) &&
              (current_config.n_ctrl_analog_inputs == 0)
             )
            )
      {
        if(--current_state_menu2.index_position < 0) current_state_menu2.index_position = max_row - 1;
      }
    }
    while (current_state_menu2.index_position < 0);
  }
}
/*****************************************************/

/*****************************************************/
//������� ����� ����������� ��������� ���������
/*****************************************************/
void make_ekran_list_settings(void)
{
  if (current_state_menu2.edition == ED_CONFIRM_CHANGES)
  {
    make_ekran_ask_rewrite();

    //����������� �� ���������� ������
    new_state_keyboard |= (1<<BIT_REWRITE);
  }
  else if (current_state_menu2.edition == ED_WARNING_ENTER) 
  {
    const unsigned char information_about_info[MAX_NAMBER_LANGUAGE][MAX_COL_LCD + 1] = 
    {
      "���.�� ���������",
      "���.�� ���������",
      "Ed.isn't allowed",
      "���.�� ���������",
    };
    make_ekran_about_info(false, information_about_info);
  }
  else if (current_state_menu2.edition == ED_INFO)
  {
    const unsigned char information_about_error[MAX_NAMBER_LANGUAGE][MAX_COL_LCD + 1] = 
    {
      " ���.���.������.",
      " ���.���.������.",
      " ���.���.������.",
      " ���.���.������."
    };
    make_ekran_about_info(false, information_about_error);
  }
  else if (current_state_menu2.edition == ED_ERROR) 
  {
    const uint8_t name_string_error[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD + 1] = 
    {
      {
        " ���.���.������.",
        " �������.������ "
      },
      {
        " ���.���.������.",
        " �������.������ "
        ""
      },
      {
        " ���.���.������.",
        " Restart device "
      },
      {
        " ���.���.������.",
        " �������.������ "
      }
    };

    int index_language = index_language_in_array(settings_fix.language);
    //�������  ����� � ������� �����
    for (size_t i = 0; i < MAX_ROW_LCD; i++)
    {
      for (size_t j = 0; j < MAX_COL_LCD; j++) working_ekran[i][j] = (i < 2) ? name_string_error[index_language][i][j] : ' ';
    }
  
    //������ ���������
    current_state_menu2.cursor_on = 0;
    //������ �� ����
    current_state_menu2.cursor_blinking_on = 0;
  }
  else
  {
    const uint8_t name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_LIST_SETTINGS_M2][MAX_COL_LCD + 1] = 
    {
      {
        " ������������   ",
        " ��-�������     ",
        " ���            ",
        " ��������� ����.",
        " C�����������   ",
        " ���            ",
        " ������������.  ",
        " ������ ������� ",
        " ������������   ",
        " ������         "
      },
      {
        " ������������   ",
        " ��-�������     ",
        " ���            ",
        " ������� ����.  ",
        " �����������   ",
        " ���            ",
        " �������������� ",
        " ������ ����   ",
        " ����������    ",
        " �����         "
      },
      {
        " Configuration  ",
        " MF-Timer       ",
        " PSG            ",
        " Group Alarms    ",
        " Alarms         ",
        " BIOS           ",
        " Parametrization",
        " Even Log       ",
        " Communication  ",
        " Passwords      "
      },
      {
        " ������������   ",
        " ��-�������     ",
        " ���            ",
        " ��������� ����.",
        " C�����������   ",
        " ���            ",
        " ������������.  ",
        " ������ ������� ",
        " ������������   ",
        " ������         "
      }
    };

    int index_language = index_language_in_array(settings_fix.language);
    unsigned int additional_current = 0;
    unsigned int position_temp = current_state_menu2.index_position;

    uint8_t name_string_tmp[MAX_ROW_LIST_SETTINGS_M2][MAX_COL_LCD + 1];
    for(size_t index_1 = 0; index_1 < (MAX_ROW_LIST_SETTINGS_M2 - additional_current); index_1++)
    {
      if (
          (
           (index_1 == (INDEX_LIST_SETTINGS_M2_TIMERS - additional_current)) &&
           (current_config.n_timer == 0)
          )
          ||
          (
           (index_1 == (INDEX_LIST_SETTINGS_M2_MEANDER - additional_current)) &&
           (current_config.n_meander == 0)
          )
          ||
          (
           (index_1 == (INDEX_LIST_SETTINGS_M2_CTRL_ANALOG_INPUTS - additional_current)) &&
           (current_config.n_ctrl_analog_inputs == 0)
          )
          ||
          (
           (index_1 == (INDEX_LIST_SETTINGS_M2_ALARMS - additional_current)) &&
           (current_config.n_alarm == 0)
          )
         )
      {
        if ((index_1 + 1) <= position_temp) position_temp--;
        additional_current++;
      }
      for(size_t index_2 = 0; index_2 < (MAX_COL_LCD + 1); index_2++)
      {
        name_string_tmp[index_1][index_2] = name_string[index_language][index_1 + additional_current][index_2];
      }
    }
    unsigned int index_in_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;;
  
    //�������  ����� � ������� �����
    for (size_t i = 0; i < MAX_ROW_LCD; i++)
    {
      //������� ����� ����� ���������, �� �� ����� ���������� � ������� ������������
      for (size_t j = 0; j < MAX_COL_LCD; j++) 
      {
        working_ekran[i][j] = (index_in_ekran < (MAX_ROW_LIST_SETTINGS_M2 - additional_current)) ? name_string_tmp[index_in_ekran][j] : ' ';
      }
      index_in_ekran++;
    }

    /*
    ��� �� ������� �� ���� � �������� �� �����������, �� ��������� ���� � �����, ���� ����� ������ �����������
    ��� ����� ���� ����, ��� � ���� ��� �� �� ����, �� ������� �� ����� �� ���� �������� �������� ������
    */
    if (current_state_menu2.edition == ED_EDITION )
    {
      current_state_menu2.edition = ED_CAN_BE_EDITED;
      //������ ���������
      current_state_menu2.cursor_blinking_on = 0;
    }
  
    //������ �� ���������� ������������ �� ������ �������
    current_state_menu2.position_cursor_x = 0;
    //³���������� ������ �� ��������
    current_state_menu2.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
    //������ �������
    current_state_menu2.cursor_on = 1;
    //������ �� ����
    current_state_menu2.cursor_blinking_on = 0;
  }
  
  //�������� ������� ���� �����
  current_state_menu2.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
/*
������������ ESC � ��� ������������
*/
/*****************************************************/
void press_esc_in_list_settings(void)
{
  if (config_settings_modified != 0)
  {
    if (current_state_menu2.edition == ED_CAN_BE_EDITED)
    {
      //����� ��������� ����� �� �������� ���
      current_state_menu2.edition = ED_CONFIRM_CHANGES;
    }
    else if (current_state_menu2.edition == ED_CONFIRM_CHANGES)
    {
      //����� ������ �������� ����� �����������
      unsigned int result = set_config_and_settings(0, 1);
      if (result == 0)
      {
        //������� ����� �����������
        current_state_menu2.edition = ED_VIEWING;
      }
      else
      {
        //����������� ��� �������� �������
        current_state_menu2.edition = ED_ERROR;
      }
      
      config_settings_modified = 0;
    }
  }
}
/*****************************************************/

/*****************************************************/
//
/*****************************************************/
/*****************************************************/
