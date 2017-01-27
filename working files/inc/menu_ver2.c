#include "header.h"

/*****************************************************/
//������� ���� � ��� ����������� �� ���� ��������
/*****************************************************/
void main_manu_function_ver2(void)
{
  //���������� �� ����� ������ ���������
  if (new_state_keyboard !=0)
  {
    switch (current_state_menu2.current_level)
    {
    case MAIN_MANU2_LEVEL:
      {
        if (new_state_keyboard !=0)
        {
          //��������� ������ ����� � ������ ����� ����
          int max_row = (current_state_menu2.max_row_p != NULL) ? *current_state_menu2.max_row_p : current_state_menu2.max_row_c;
          
          //�������� ����� �� �������� ������
          if((new_state_keyboard & (1<<BIT_REWRITE)) !=0)
          {
            if(current_state_menu2.index_position >= max_row) current_ekran.index_position = 0;
            while (
                   (
                    (current_state_menu2.current_level == MAIN_MANU2_LEVEL) &&
                    (current_state_menu2.index_position == INDEX_MAIN_M2_MEASURMENTS) &&
                    (current_config.ctrl_analog_inputs != 0)
                   )
                  )
            {
              if(++current_state_menu2.index_position >= max_row) current_state_menu2.index_position = 0;
            }
            position_in_current_level_menu2[current_state_menu2.current_level] = current_state_menu2.index_position;
            
            //������� ����� �����������
            current_state_menu2.func_move();
            //������� �� ���������� ������
            new_state_keyboard &= (unsigned int)(~(1<<BIT_REWRITE));
          }
          else
          {
            if (new_state_keyboard == (1<<BIT_KEY_ENTER))
            {
              int temp_current_level = current_ekran.current_level;
              
              //��������� ������ ENTER
              if(current_ekran.index_position == INDEX_ML1_TIME)
              {
                //�����'������� ���������� �����
                //���������� �� ���� ����-���������
                time_rewrite = 0;
                current_ekran.current_level = EKRAN_TIME;
                current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                current_ekran.edition = 0;
                current_ekran.cursor_on = 0;
                current_ekran.cursor_blinking_on = 0;
                
                /**************************************************/
                //������ �� ���� �� ������� ������ ����� � ������
                /**************************************************/
                if(current_ekran.index_position == ROW_Y_)
                {
                  current_ekran.position_cursor_x = COL_DY1;
                }
                else if(current_ekran.index_position == ROW_T_)
                {
                  current_ekran.index_position = ROW_Y_;
                  current_ekran.position_cursor_x = COL_DY1;
                }
                else if(current_ekran.index_position == ROW_N_)
                {
                  current_ekran.index_position = ROW_K_;
                  current_ekran.position_cursor_x = COL_SK1;
                }
                else if(current_ekran.index_position == ROW_K_)
                {
                  current_ekran.position_cursor_x = COL_SK1;
                }
                /**************************************************/
              }
              else if(current_ekran.index_position == INDEX_ML1_MEASURMENTS)
              {
                //���������� �� ���� ������ ����������� ��������� ������
                current_ekran.current_level = EKRAN_MEASURMENT_CURRENT;
                current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                current_ekran.edition = 0;
                current_ekran.cursor_on = 1;
                current_ekran.cursor_blinking_on = 0;
              }
              else if(current_ekran.index_position == INDEX_ML1_INPUTS_OUTPUTS)
              {
                //���������� �� ���� ������ ����������� ������ ������ �����-������ ��� ����������� �� �������� �����
                current_ekran.current_level = EKRAN_LIST_INPUTS_OUTPUTS;
                current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
              }
              else if(current_ekran.index_position == INDEX_ML1_REGISTRATORS)
              {
                //���������� �� ���� ������ ����������� ������ ����������
                current_ekran.current_level = EKRAN_LIST_REGISTRATORS;
                current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
              }
              else if(current_ekran.index_position == INDEX_ML1_LANGUAGE)
              {
                //���������� �� ���� ������ ���� ����������� ����
                current_ekran.current_level = EKRAN_VIEW_SETTING_LANGUAGE;
                current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
              }
              else if(current_ekran.index_position == INDEX_ML1_SETTINGS)
              {
                //���������� �� ���� ���������
                current_ekran.current_level = EKRAN_CHOSE_SETTINGS;
                current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                current_ekran.edition = 0;
                current_ekran.cursor_on = 1;
                current_ekran.cursor_blinking_on = 0;
              }
              else if(current_ekran.index_position == INDEX_ML1_DIAGNOSTYKA)
              {
                //���������� �� ���� ���������
                current_ekran.current_level = EKRAN_DIAGNOSTYKA;
                //��� ����, ��� ��� ������� �������� ������ ������ ��������� �� ����� ������� ��������� �� �������
                position_in_current_level_menu[current_ekran.current_level] = 0;
                current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                current_ekran.edition = 0;
              }
              else if(current_ekran.index_position == INDEX_ML1_KONF)
              {
                //���������� �� ���� ������������
                current_ekran.current_level = EKRAN_COFIGURATION;
                current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                current_ekran.edition = 0;
                current_ekran.cursor_on = 1;
                current_ekran.cursor_blinking_on = 0;
              }
              else if(
                      (current_ekran.index_position == INDEX_ML1_CTRL_PHASE) 
                     )
              {
                //���������� �� ���� �������� ���������
                if(current_ekran.index_position == INDEX_ML1_CTRL_PHASE) current_ekran.current_level = EKRAN_CHOOSE_SETTINGS_CTRL_PHASE;
                
                current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                current_ekran.edition = 0;
                current_ekran.cursor_on = 1;
                current_ekran.cursor_blinking_on = 0;
              }
              //� ���, ���� ������� ����� �������, �� �����'������� ����� � ���� ����� ���� �����������
              if (temp_current_level != current_ekran.current_level) 
                previous_level_in_current_level_menu[current_ekran.current_level] = temp_current_level;

              //����������� ������� �� ����������� ������ ������
              new_state_keyboard |= (1<<BIT_REWRITE);
              
              //�������� �����������, �� ��������� ������ 
              new_state_keyboard &= (unsigned int)(~(1<<BIT_KEY_ENTER));
            }
            else if (new_state_keyboard == (1<<BIT_KEY_UP))
            {
              //��������� ������ UP
              current_state_menu2.index_position--;
              do
              {
                if(current_state_menu2.index_position < 0) current_state_menu2.index_position = max_row - 1;
                while (
                        (
                        (current_state_menu2.current_level == MAIN_MANU2_LEVEL) &&
                        (current_state_menu2.index_position == INDEX_MAIN_M2_MEASURMENTS) &&
                        (current_config.ctrl_analog_inputs != 0)
                       )
                      )
                {
                  if(--current_state_menu2.index_position < 0) current_state_menu2.index_position = max_row - 1;
                }
              }
              while (current_state_menu2.index_position < 0);
              position_in_current_level_menu2[current_state_menu2.current_level] = current_state_menu2.index_position;
              
              //������� ����� ���� ��������� ����
              current_state_menu2.func_move();
              //�������� �����������, �� ��������� ������ 
              new_state_keyboard &= (unsigned int)(~(1<<BIT_KEY_UP));
            }
            else if (new_state_keyboard == (1<<BIT_KEY_DOWN))
            {
              //��������� ������ DOWN
              //current_ekran.index_position ����� ����� � ������� ������ ����
              current_ekran.index_position++;
              do
              {
                if(current_ekran.index_position >= MAX_ROW_FOR_EKRAN_MAIN) current_ekran.index_position = 0;
 
                while (
                       (
                        (current_ekran.index_position == INDEX_ML1_MEASURMENTS) || 
                        (current_ekran.index_position == INDEX_ML1_CTRL_PHASE )
                       )
                       &&
                       ((current_settings.configuration & (1<<CTRL_PHASE_BIT_CONFIGURATION)) == 0)
                      )
                {
                  current_ekran.index_position++;
                }
              }
              while (current_ekran.index_position >= MAX_ROW_FOR_EKRAN_MAIN);
              position_in_current_level_menu[EKRAN_MAIN] = current_ekran.index_position;
              
              //������� ����� ���� ��������� ����
              make_ekran_main();
              //�������� �����������, �� ��������� ������ 
              new_state_keyboard &= (unsigned int)(~(1<<BIT_KEY_DOWN));
            }
            else
            {
              //��������� ����� ������� ������ - �� � ����������� ��������, ���� ������� ����������� ��� �������� ������ � ������ �����
              unsigned int temp_data = new_state_keyboard;
              new_state_keyboard &= ~temp_data;
            }
          }
          
        }
        break;
      }
    default:
      {
        //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
        total_error_sw_fixed(62);
      }
    }
  }
}
/*****************************************************/

