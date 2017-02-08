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
void move_into_main(unsigned int action, int max_row)
{
  if (action & ((1 << BIT_REWRITE) | (1 << BIT_KEY_DOWN)))
  {
    if (action & (1 << BIT_KEY_DOWN)) current_state_menu2.index_position++;
    do
    {
      if(current_state_menu2.index_position >= max_row) current_state_menu2.index_position = 0;
      while (
             (current_state_menu2.index_position == INDEX_MAIN_M2_MEASURMENTS) &&
             (current_config.n_ctrl_analog_inputs == 0)
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
             (current_state_menu2.index_position == INDEX_MAIN_M2_MEASURMENTS) &&
             (current_config.n_ctrl_analog_inputs == 0)
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
//������� ����� ��������� ����
/*****************************************************/
void make_ekran_main(void)
{
  const uint8_t name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_MAIN_M2][MAX_COL_LCD + 1] = 
  {
    {
     " ����           ",
     " ���������      ",
     " �����-������   ",
     " ������������   ",
     " ���������      ",
     " �����������    ",
     " ����� �������� ",
     " ������ �� � �� "
    },
    {
     " ��������       ",
     " ����������    ",
     " �����-������   ",
     " ����������    ",
     " ������������   ",
     " ĳ���������    ",
     " ̳��� ������.  ",
     " ����� �� � �� "
    },
    {
     " Clock          ",
     " Measurement    ",
     " Inputs-Outputs ",
     " Recorders      ",
     " Settings       ",
     " Diagnostic     ",
     " Settings Mark  ",
     " VER.of F/W & MM"
    },
    {
     " ��f��          ",
     " �����          ",
     " ʳ��-��f��    ",
     " ҳ��������    ",
     " �������       ",
     " �����������    ",
     " ����� �������� ",
     " ������ �� � �� "
    }
  };

  int index_language = index_language_in_array(settings_fix.language);
  unsigned int additional_current = 0;
  unsigned int position_temp = current_state_menu2.index_position;

  uint8_t name_string_tmp[MAX_ROW_MAIN_M2][MAX_COL_LCD + 1];
  for(size_t index_1 = 0; index_1 < (MAX_ROW_MAIN_M2 - additional_current); index_1++)
  {
    if (
        (index_1 == (INDEX_MAIN_M2_MEASURMENTS - additional_current)) &&
        (current_config.n_ctrl_analog_inputs == 0)
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
      working_ekran[i][j] = (index_in_ekran < (MAX_ROW_MAIN_M2 - additional_current)) ? name_string_tmp[index_in_ekran][j] : ' ';
    }
    index_in_ekran++;
  }

  //������ �� ���������� ������������ �� ������ �������
  current_state_menu2.position_cursor_x = 0;
  //³���������� ������ �� ��������
  current_state_menu2.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
  //������ �������
  current_state_menu2.cursor_on = 1;
  //������ �� ����
  current_state_menu2.cursor_blinking_on = 0;
  //�������� ������� ���� �����
  current_state_menu2.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//
/*****************************************************/
/*****************************************************/
