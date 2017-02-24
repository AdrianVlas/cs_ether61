#include "header.h"

/*****************************************************/
//������� ����� ����������� ��������� ������������ �������
/*****************************************************/
void make_ekran_chose_settings_any_protection(void)
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CHOSE_SETTINGS_ANY_PROTECTION][MAX_COL_LCD] = 
  {
    {
      " �������        ",
      " ��������       ",
      " ����������     "
    },
    {
      " �������        ",
      " ��������       ",
      " ���������     "
    },
    {
      " Pickups        ",
      " Delay          ",
      " Control        "
    },
    {
      " �������        ",
      " �y�����        ",
      " ������       "
    }
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  
  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  //�������  ����� � ������� �����
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    //������� ����� ����� ���������, �� �� ����� ���������� � ������� �����������
    if (index_of_ekran < MAX_ROW_FOR_CHOSE_SETTINGS_ANY_PROTECTION)
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran][j];
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //������ �� ���������� ������������ �� ������ �������
  current_ekran.position_cursor_x = 0;
  //³���������� ������ �� ��������
  current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
  //������ �������
  current_ekran.cursor_on = 1;
  //������ �� ����
  current_ekran.cursor_blinking_on = 0;
  //�������� ������� ���� �����
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//������� ����� ����������� ��������� ����������� �� � ����� ��������
/*****************************************************/
void make_ekran_choose_delay(void)
{
  const uint8_t name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_LIST_SETTINGS_D_M2][MAX_COL_LCD + 1] = 
  {
    {
      " ��������       "
    },
    {
      " ��������       "
    },
    {
      " Delay          "
    },
    {
      " ������       "
    }
  };
  int index_language;
  if (current_state_menu2.edition == ED_VIEWING) index_language = index_language_in_array(settings_fix_prt.language);
  else if (
           (current_state_menu2.edition == ED_EDITION) ||
           (current_state_menu2.edition == ED_CONFIRM_CHANGES)
          )  
  {
     index_language = index_language_in_array(settings_fix_edit.language);
  }
  else
  {
    index_language = index_language_in_array(settings_fix.language);
  }
  
  unsigned int position_temp = current_state_menu2.index_position;
  unsigned int index_in_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  //�������  ����� � ������� �����
  for (size_t i = 0; i < MAX_ROW_LCD; i++)
  {
    for (size_t j = 0; j < MAX_COL_LCD; j++) working_ekran[i][j] = (index_in_ekran < MAX_ROW_LIST_SETTINGS_D_M2) ? name_string[index_language][index_in_ekran][j] : ' ';
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
//������� ����� ����������� ��������� ������������ �� ����� � ���������
/*****************************************************/
void make_ekran_chose_control(void)
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_CONTROL][MAX_COL_LCD] = 
  {
    {
      " ����������     "
    },
    {
      " ���������     "
    },
    {
      " Control        "
    },
    {
      " �������        "
    }
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  
  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  //�������  ����� � ������� �����
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    if (index_of_ekran < MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_CONTROL)
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran][j];
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //������ �� ���������� ������������ �� ������ �������
  current_ekran.position_cursor_x = 0;
  //³���������� ������ �� ��������
  current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
  //������ �������
  current_ekran.cursor_on = 1;
  //������ �� ����
  current_ekran.cursor_blinking_on = 0;
  //�������� ������� ���� �����
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//������� ����� ����������� ��������� ������������ �� � �������� � ���������
/*****************************************************/
void make_ekran_choose_delay_control(void)
{
  const uint8_t name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_LIST_SETTINGS_DC_M2][MAX_COL_LCD + 1] = 
  {
    {
      " ��������       ",
      " ����������     "
    },
    {
      " ��������       ",
      " ���������     "
    },
    {
      " Delay          ",
      " Control        "
    },
    {
      " ������       ",
      " �������        "
    }
  };
  int index_language;
  if (current_state_menu2.edition == ED_VIEWING) index_language = index_language_in_array(settings_fix_prt.language);
  else if (
           (current_state_menu2.edition == ED_EDITION) ||
           (current_state_menu2.edition == ED_CONFIRM_CHANGES)
          )  
  {
     index_language = index_language_in_array(settings_fix_edit.language);
  }
  else
  {
    index_language = index_language_in_array(settings_fix.language);
  }
  
  unsigned int position_temp = current_state_menu2.index_position;
  unsigned int index_in_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  //�������  ����� � ������� �����
  for (size_t i = 0; i < MAX_ROW_LCD; i++)
  {
    for (size_t j = 0; j < MAX_COL_LCD; j++) working_ekran[i][j] = (index_in_ekran < MAX_ROW_LIST_SETTINGS_DC_M2) ? name_string[index_language][index_in_ekran][j] : ' ';
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
//������� ����� ����������� ��������� ������������ �� � ������� � ���������
/*****************************************************/
void make_ekran_chose_setpoint_control(void)
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_SETPOINTS_CONTROL][MAX_COL_LCD] = 
  {
    {
      " �������        ",
      " ����������     "
    },
    {
      " �������        ",
      " ���������     "
    },
    {
      " Pickups        ",
      " Control        "
    },
    {
      " �������        ",
      " �������        "
    }
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  
  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  //�������  ����� � ������� �����
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    if (index_of_ekran < MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_SETPOINTS_CONTROL)
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran][j];
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //������ �� ���������� ������������ �� ������ �������
  current_ekran.position_cursor_x = 0;
  //³���������� ������ �� ��������
  current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
  //������ �������
  current_ekran.cursor_on = 1;
  //������ �� ����
  current_ekran.cursor_blinking_on = 0;
  //�������� ������� ���� �����
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//
/*****************************************************/
/*****************************************************/
