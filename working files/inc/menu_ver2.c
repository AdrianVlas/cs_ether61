#include "header.h"

/*****************************************************/
//������� ���� � ��� ����������� �� ���� ��������
/*****************************************************/
void main_manu_function_ver2(void)
{
  //���������� �� ����� ������ ���������
  if (new_state_keyboard !=0)
  {
    static unsigned int prev_edit;
    
    switch (current_state_menu2.current_level)
    {
    case PASSWORD_MENU2_LEVEL:
      {
        //����� ��� �������� ����� ������� � ����������� ���� ����
        static __PREVIOUS_STATE_CURSOR previous_state_cursor;
        //���� ��� �������� ��������� ������
        static unsigned int new_password;
        static unsigned int number_symbols;
        
        //������� �� ��� ���� �����������
        new_state_keyboard &= (1<<BIT_KEY_ENTER)|(1<<BIT_KEY_ESC)|(1<<BIT_KEY_UP)|(1<<BIT_KEY_DOWN)|(1<<BIT_KEY_RIGHT)|(1<<BIT_KEY_LEFT)|(1<<BIT_REWRITE);

        if (new_state_keyboard !=0)
        {
          //�������� ����� �� �������� ������
          if((new_state_keyboard & (1<<BIT_REWRITE)) !=0)
          {
            previous_state_cursor.position_cursor_x = current_state_menu2.position_cursor_x;              
            previous_state_cursor.cursor_on = current_state_menu2.cursor_on;
            previous_state_cursor.cursor_blinking_on = current_state_menu2.cursor_blinking_on;
            current_state_menu2.cursor_on = 1;
            current_state_menu2.cursor_blinking_on = 1;
            current_state_menu2.position_cursor_x = COL_NEW_PASSWORD_BEGIN;
            position_in_current_level_menu[current_ekran.current_level] = 1;
            //������� ����� ������ � ���� � ������� ������� �������� �������
            new_password = 0;
            number_symbols = 0;
            
            //������� ����� �����������
            p_menu_param_1 = &new_password;
            unsigned int view = 0;
            p_menu_param_2 = &view;
            if (current_state_menu2.func_show != NULL) current_state_menu2.func_show();
            else
            {
              //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
              total_error_sw_fixed(100);
            }
            //������� �� ���������� ������
            new_state_keyboard &= (unsigned int)(~(1<<BIT_REWRITE));
          }
          else
          {
            if (new_state_keyboard == (1<<BIT_KEY_ENTER))
            {
              //��������� ������ ENTER
              current_state_menu2.current_level = next_level_in_current_level_menu2[current_state_menu2.current_level];
              current_state_menu2.index_position = position_in_current_level_menu2[current_state_menu2.current_level];
              current_state_menu2.position_cursor_x = previous_state_cursor.position_cursor_x;
              current_state_menu2.cursor_on = previous_state_cursor.cursor_on;
              current_state_menu2.cursor_blinking_on = previous_state_cursor.cursor_blinking_on;
              new_level_menu();

              if (
                  (new_password == settings_fix.password_1) || 
                  (new_password == settings_fix.password_2)
                 )   
              {
                //������ �������
                //���������� � �������� ���� � ������ ����� ������� �����������
                current_state_menu2.edition =  (new_password == settings_fix.password_2) ? 2 : 1;
              }
              else
              {
                //������ �� �������
                //���������� � �������� ���� � ��������� ������� �����������
                current_ekran.edition = prev_edit;
              }
              
              //�������� �����������, �� ��������� ������ 
              new_state_keyboard &= (unsigned int)(~(1<<BIT_KEY_ENTER));
              //����������� �� ���������� ������
              new_state_keyboard |= (1<<BIT_REWRITE);
            }
            else if (new_state_keyboard == (1<<BIT_KEY_ESC))
            {
              //��������� ������ ESC

              //���������� � �������� ���� � ��������� ������� �����������
              current_state_menu2.current_level = previous_level_in_current_level_menu2[current_state_menu2.current_level];
              current_state_menu2.index_position = position_in_current_level_menu2[current_state_menu2.current_level];
              current_state_menu2.position_cursor_x = previous_state_cursor.position_cursor_x;
              current_state_menu2.cursor_on = previous_state_cursor.cursor_on;
              current_state_menu2.cursor_blinking_on = previous_state_cursor.cursor_blinking_on;
              new_level_menu();
              current_state_menu2.edition = prev_edit;
              
              //�������� �����������, �� ��������� ������ 
              new_state_keyboard &= (unsigned int)(~(1<<BIT_KEY_ESC));
              //����������� �� ���������� ������
              new_state_keyboard |= (1<<BIT_REWRITE);
            }
            else if (new_state_keyboard == (1<<BIT_KEY_UP))
            {
              //��������� ������ UP
              new_password = new_password*10 + 1;
              number_symbols++;
              current_state_menu2.position_cursor_x++;
              
              //������� ����� �����������
              if (current_state_menu2.func_show != NULL) current_state_menu2.func_show();
              else
              {
                //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
                total_error_sw_fixed(101);
              }
              //�������� �����������, �� ��������� ������ 
              new_state_keyboard &= (unsigned int)(~(1<<BIT_KEY_UP));
              //���������� �� �� ��������� ����������� ������� ������� ��� ������ (4) � ���� �� ���, �� ����������� �������� ����
              if (number_symbols >= 4) new_state_keyboard |= (1<<BIT_KEY_ENTER);
            }
            else if (new_state_keyboard == (1<<BIT_KEY_DOWN))
            {
              //��������� ������ DOWN
              new_password = new_password*10 + 3;
              number_symbols++;
              current_state_menu2.position_cursor_x++;

              //������� ����� �����������
              if (current_state_menu2.func_show != NULL) current_state_menu2.func_show();
              else
              {
                //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
                total_error_sw_fixed(102);
              }
              //�������� �����������, �� ��������� ������ 
              new_state_keyboard &= (unsigned int)(~(1<<BIT_KEY_DOWN));
              //���������� �� �� ��������� ����������� ������� ������� ��� ������ (4) � ���� �� ���, �� ����������� �������� ����
              if (number_symbols >= 4) new_state_keyboard |= (1<<BIT_KEY_ENTER);
            }
            else if (new_state_keyboard == (1<<BIT_KEY_RIGHT))
            {
              //��������� ������ RIGHT
              new_password = new_password*10 + 2;
              number_symbols++;
              current_state_menu2.position_cursor_x++;

              //������� ����� �����������
              if (current_state_menu2.func_show != NULL) current_state_menu2.func_show();
              else
              {
                //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
                total_error_sw_fixed(103);
              }
              //�������� �����������, �� ��������� ������ 
              new_state_keyboard &= (unsigned int)(~(1<<BIT_KEY_RIGHT));
              //���������� �� �� ��������� ����������� ������� ������� ��� ������ (4) � ���� �� ���, �� ����������� �������� ����
              if (number_symbols >= 4) new_state_keyboard |= (1<<BIT_KEY_ENTER);
            }
            else if (new_state_keyboard == (1<<BIT_KEY_LEFT))
            {
              //��������� ������ LEFT
              new_password = new_password*10 + 4;
              number_symbols++;
              current_state_menu2.position_cursor_x++;

              //������� ����� �����������
              if (current_state_menu2.func_show != NULL) current_state_menu2.func_show();
              else
              {
                //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
                total_error_sw_fixed(104);
              }
              //�������� �����������, �� ��������� ������ 
              new_state_keyboard &= (unsigned int)(~(1<<BIT_KEY_LEFT));
              //���������� �� �� ��������� ����������� ������� ������� ��� ������ (4) � ���� �� ���, �� ����������� �������� ����
              if (number_symbols >= 4) new_state_keyboard |= (1<<BIT_KEY_ENTER);
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
    case MAIN_MANU2_LEVEL:
      {
        //������� ����� ������. �� ������ ���� �����������
        unsigned int maska_keyboard_bits = (1<<BIT_KEY_ENTER)| (1<<BIT_KEY_ESC)|(1<<BIT_REWRITE)| (1<<BIT_KEY_UP)|(1<<BIT_KEY_DOWN);
        //������� �� ��� ���� �����������
        new_state_keyboard &= maska_keyboard_bits;

        if (new_state_keyboard !=0)
        {
          //�������� ����� �� �������� ������
          if((new_state_keyboard & (1<<BIT_REWRITE)) !=0)
          {
            if (current_state_menu2.func_show != NULL) current_state_menu2.func_move(0);
            else
            {
              //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
              total_error_sw_fixed(84);
            }
            position_in_current_level_menu2[current_state_menu2.current_level] = current_state_menu2.index_position;
            //������� ����� �����������
            if (current_state_menu2.func_show != NULL) current_state_menu2.func_show();
            else
            {
              //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
              total_error_sw_fixed(65);
            }
            //������� �� ���������� ������
            new_state_keyboard &= (unsigned int)(~(1<<BIT_REWRITE));
          }
          else
          {
            if (new_state_keyboard == (1<<BIT_KEY_ENTER))
            {
              //��������� ������ ENTER
              const enum _menu2_levels next_for_main_menu2[MAX_ROW_MAIN_M2] = {TIME_MANU2_LEVEL};
              const enum _menu2_levels *p;
              
              switch (current_state_menu2.current_level)
              {
              case MAIN_MANU2_LEVEL:
                {
                  p = next_for_main_menu2;
                  break;
                }
              default:
                {
                  //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
                  total_error_sw_fixed(63);
                }
              }
              
              enum _menu2_levels temp_current_level = p[current_state_menu2.index_position];
              if (current_state_menu2.current_level != temp_current_level) 
              {
                previous_level_in_current_level_menu2[temp_current_level] = current_state_menu2.current_level;
                
                current_state_menu2.current_level = temp_current_level;
                current_state_menu2.index_position = position_in_current_level_menu2[temp_current_level];
                new_level_menu();
              }
              
              
//              else if(current_state_menu2.index_position == INDEX_MAIN_M2_MEASURMENTS)
//              {
//                //���������� �� ���� ������ ����������� ��������� ������
//                current_ekran.current_level = EKRAN_MEASURMENT_CURRENT;
//                current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
//              }
//              else if(current_state_menu2.index_position == INDEX_MAIN_M2_INPUTS_OUTPUTS)
//              {
//                //���������� �� ���� ������ ����������� ������ ������ �����-������ ��� ����������� �� �������� �����
//                current_ekran.current_level = EKRAN_LIST_INPUTS_OUTPUTS;
//                current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
//              }
//              else if(current_state_menu2.index_position == INDEX_MAIN_M2_REGISTRATORS)
//              {
//                //���������� �� ���� ������ ����������� ������ ����������
//                current_ekran.current_level = EKRAN_LIST_REGISTRATORS;
//                current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
//              }
//              else if(current_state_menu2.index_position == INDEX_MAIN_M2_SETTINGS)
//              {
//                //���������� �� ���� ���������
//                current_ekran.current_level = EKRAN_CHOSE_SETTINGS;
//                current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
//              }
//              else if(current_state_menu2.index_position == INDEX_MAIN_M2_DIAGNOSTYKA)
//              {
//                //���������� �� ���� ���������
//                current_ekran.current_level = EKRAN_DIAGNOSTYKA;
//                //��� ����, ��� ��� ������� �������� ������ ������ ��������� �� ����� ������� ��������� �� �������
//                position_in_current_level_menu[current_ekran.current_level] = 0;
//                current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
//              }

              //����������� ������� �� ����������� ������ ������
              new_state_keyboard |= (1<<BIT_REWRITE);
              
              //�������� �����������, �� ��������� ������ 
              new_state_keyboard &= (unsigned int)(~(1<<BIT_KEY_ENTER));
            }
            else if (new_state_keyboard == (1<<BIT_KEY_UP))
            {
              //��������� ������ UP
              if (current_state_menu2.func_show != NULL) current_state_menu2.func_move(2);
              else
              {
                //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
                total_error_sw_fixed(85);
              }
              position_in_current_level_menu2[current_state_menu2.current_level] = current_state_menu2.index_position;
              
              //������� �����
              if (current_state_menu2.func_show != NULL) current_state_menu2.func_show();
              else
              {
                //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
                total_error_sw_fixed(66);
              }
              //�������� �����������, �� ��������� ������ 
              new_state_keyboard &= (unsigned int)(~(1<<BIT_KEY_UP));
            }
            else if (new_state_keyboard == (1<<BIT_KEY_DOWN))
            {
              //��������� ������ DOWN
              if (current_state_menu2.func_show != NULL) current_state_menu2.func_move(1);
              else
              {
                //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
                total_error_sw_fixed(86);
              }
              position_in_current_level_menu2[current_state_menu2.current_level] = current_state_menu2.index_position;
              
              //������� ����� ���� ��������� ����
              if (current_state_menu2.func_show != NULL) current_state_menu2.func_show();
              else
              {
                //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
                total_error_sw_fixed(67);
              }
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
    case TIME_MANU2_LEVEL:
      {
        //������� ����� ������. �� ������ ���� �����������
        unsigned int maska_keyboard_bits = (1<<BIT_KEY_ENTER)| (1<<BIT_KEY_ESC)|(1<<BIT_REWRITE)| (1<<BIT_KEY_UP)|(1<<BIT_KEY_DOWN);
        
        if (current_state_menu2.edition == 2)
          maska_keyboard_bits |= (1<<BIT_KEY_RIGHT) | (1<<BIT_KEY_LEFT) | (1<<BIT_KEY_UP)|(1<<BIT_KEY_DOWN);
        
        //������� �� ��� ���� �����������
        new_state_keyboard &= maska_keyboard_bits;
        //������ �������� 䳿, ���� ��������� ������ �� ��� ����� ��������, ��� ����� ������� �������� �����
        if (new_state_keyboard !=0)
        {
          //�������� ����� �� �������� ������
          if((new_state_keyboard & (1<<BIT_REWRITE)) !=0)
          {
            if (current_state_menu2.func_show != NULL) current_state_menu2.func_move(0);
            else
            {
              //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
              total_error_sw_fixed(87);
            }
            position_in_current_level_menu2[current_state_menu2.current_level] = current_state_menu2.index_position;

            if (current_state_menu2.edition <= 1)
            {
              current_state_menu2.cursor_on = 0;
              current_state_menu2.cursor_blinking_on = 0;
            }
            else
            {
              current_state_menu2.cursor_on = 1;
              current_state_menu2.cursor_blinking_on = 1;
            }

            //������� ����� �����������
            if (current_state_menu2.func_show != NULL) current_state_menu2.func_show();
            else
            {
              //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
              total_error_sw_fixed(68);
            }
            //������� �� ���������� ������
            new_state_keyboard &= (unsigned int)(~(1<<BIT_REWRITE));
          }
          else
          {
            if (new_state_keyboard == (1<<BIT_KEY_ENTER))
            {
              //��������� ������ ENTER
              unsigned int result;
              if (current_state_menu2.func_edit != NULL) result = current_state_menu2.func_edit();
              else
              {
                //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
                total_error_sw_fixed(76);
              }
              if(current_state_menu2.edition <= 1)
              {
                prev_edit = current_state_menu2.edition;
                
                if ((settings_fix.password_2 != 0) || (current_state_menu2.edition == 0))
                {
                  //���������� �� ���� ������ ������
                  next_level_in_current_level_menu2[PASSWORD_MENU2_LEVEL] = previous_level_in_current_level_menu2[PASSWORD_MENU2_LEVEL] = current_state_menu2.current_level;
                  current_state_menu2.current_level = PASSWORD_MENU2_LEVEL;
                  current_state_menu2.index_position = position_in_current_level_menu2[current_state_menu2.current_level];
                  new_level_menu();
                }
                else
                {
                  //���������� � ����� �����������
                  current_state_menu2.edition = 2;
                }
              }
              else if (current_state_menu2.edition == 2)
              {
                //��������� ����� ��� ������������ ������
                switch (result)
                {
                case 1:
                  {
                    //��� �� ������� ���
                    current_state_menu2.edition = prev_edit;
                    break;
                  }
                case 2:
                  {
                    //��� ������� ��� � � � �������
                    current_state_menu2.edition = 3;
                    break;
                  }
                case 3:
                  {
                    //��� ������� ��� ��� �� � � �������
                    current_state_menu2.edition = 4;
                    break;
                  }
                }
              }
              else if (current_state_menu2.edition == 3)
              {
                current_state_menu2.edition = prev_edit;
              }
              else if (current_state_menu2.edition == 4)
              {
                current_state_menu2.edition = 2;
              }

              
              if(current_state_menu2.edition == 3) make_ekran_ask_rewrite();
              else if(current_state_menu2.edition == 4) 
              {
                const unsigned char information_about_error[MAX_NAMBER_LANGUAGE][MAX_COL_LCD + 1] = 
                {
                  " ���.�� ��������",
                  " ���.�� �������",
                  "  Out of Limits ",
                  "���.�� �������� "
                };
                make_ekran_about_error(information_about_error);
              }
              else
                //����������� �� ���������� ������
                new_state_keyboard |= (1<<BIT_REWRITE);
              
              //�������� �����������, �� ��������� ������ 
              new_state_keyboard &= (unsigned int)(~(1<<BIT_KEY_ENTER));
            }
            else if (new_state_keyboard == (1<<BIT_KEY_ESC))
            {
              //��������� ������ ESC

              if(current_state_menu2.edition <= 1)
              {
                //����� � ����� ������������
                //���������� � �������� ����
                current_state_menu2.current_level = previous_level_in_current_level_menu2[current_state_menu2.current_level];
                current_state_menu2.index_position = position_in_current_level_menu2[current_state_menu2.current_level];
                new_level_menu();
              }
              else
              {
                //����� � ����� ����������� ��� �������� ���
                current_state_menu2.edition = prev_edit;
              }

              //����������� ������� �� ����������� ������ ������
              new_state_keyboard |= (1<<BIT_REWRITE);

              //�������� �����������, �� ��������� ������ 
              new_state_keyboard &= (unsigned int)(~(1<<BIT_KEY_ESC));
            }
            else if (new_state_keyboard == (1<<BIT_KEY_UP))
            {
              //��������� ������ UP
              if(current_state_menu2.edition <= 1)
              {
                //���������� � ����� ������������
                if (current_state_menu2.func_show != NULL) current_state_menu2.func_move(2);
                else
                {
                  //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
                  total_error_sw_fixed(88);
                }
                position_in_current_level_menu2[current_state_menu2.current_level] = current_state_menu2.index_position;
              }
              else
              {
                //����������� �����
                if (current_state_menu2.func_change != NULL) current_state_menu2.func_change(1); 
                else
                {
                  //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
                  total_error_sw_fixed(77);
                }
              }

              //������� �����
              if (current_state_menu2.func_show != NULL) current_state_menu2.func_show();
              else
              {
                //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
                total_error_sw_fixed(69);
              }
              //�������� �����������, �� ��������� ������ 
              new_state_keyboard &= (unsigned int)(~(1<<BIT_KEY_UP));
            }
            else if (new_state_keyboard == (1<<BIT_KEY_DOWN))
            {
              //��������� ������ DOWN
              if(current_state_menu2.edition <= 1)
              {
                //���������� � ����� ������������
                if (current_state_menu2.func_show != NULL) current_state_menu2.func_move(1);
                else
                {
                  //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
                  total_error_sw_fixed(89);
                }
                position_in_current_level_menu2[current_state_menu2.current_level] = current_state_menu2.index_position;
              }
              else
              {
                //����������� �����
                if (current_state_menu2.func_change != NULL) current_state_menu2.func_change(0); 
                else
                {
                  //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
                  total_error_sw_fixed(77);
                }
              }

              //������� �����
              if (current_state_menu2.func_show != NULL) current_state_menu2.func_show();
              else
              {
                //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
                total_error_sw_fixed(70);
              }
              //�������� �����������, �� ��������� ������ 
              new_state_keyboard &= (unsigned int)(~(1<<BIT_KEY_DOWN));
            }
            else if (new_state_keyboard == (1<<BIT_KEY_RIGHT))
            {
              if (current_state_menu2.func_change != NULL) current_state_menu2.func_change(3); 
              else
              {
                //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
                total_error_sw_fixed(81);
              }
                
              //������� �����
              if (current_state_menu2.func_show != NULL) current_state_menu2.func_show();
              else
              {
                //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
                total_error_sw_fixed(73);
              }
              //�������� �����������, �� ��������� ������ 
              new_state_keyboard &= (unsigned int)(~(1<<BIT_KEY_RIGHT));
            }
            else if (new_state_keyboard == (1<<BIT_KEY_LEFT))
            {
              if (current_state_menu2.func_change != NULL) current_state_menu2.func_change(2); 
              else
              {
                //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
                total_error_sw_fixed(82);
              }

              //������� �����
              if (current_state_menu2.func_show != NULL) current_state_menu2.func_show();
              else
              {
                //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
                total_error_sw_fixed(74);
              }
              //�������� �����������, �� ��������� ������ 
              new_state_keyboard &= (unsigned int)(~(1<<BIT_KEY_LEFT));
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
/*****************************************************/
//������ ����������� ��� �� �� ����� ��� ���� ������ ����
/*****************************************************/
void make_ekran_ask_rewrite(void)
{
  const uint8_t name_string[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD + 1] = 
  {
    {
     "������ ���������",
     "��-ENTER ���-ESC"
    },
    {
     "  ������ ����  ",
     "���-ENTER ͳ-ESC"
    },
    {
     " Enter changes  ",
     "Yes-ENTER No-ESC"
    },
    {
     "������ ���������",
     "��-ENTER ���-ESC"
    }
  };

  int index_language;
  
  if (current_ekran.current_level == EKRAN_VIEW_SETTING_LANGUAGE)
    index_language = index_language_in_array(edition_settings.language);
  else
    index_language = index_language_in_array(settings_fix.language);

  //�������  ����� � ������� �����
  for (size_t i = 0; i < MAX_ROW_LCD; i++)
  {
    for (size_t j = 0; j < MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][i][j];
  }
  
  //������ �� �������
  current_state_menu2.cursor_on = 0;
  //������ �� ����
  current_state_menu2.cursor_blinking_on = 0;
  //�������� ������� ���� �����
  current_state_menu2.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/


/*****************************************************/
//���������� ���� ��� �������
/*****************************************************/
void make_ekran_about_error(const unsigned char information[][MAX_COL_LCD + 1])
{
  const uint8_t name_string[MAX_NAMBER_LANGUAGE][MAX_COL_LCD + 1] = 
  {
    "������?Enter/Esc",
    "������?Enter/Esc",
    "Repeat?Enter/Esc",
    "������?Enter/Esc"
  };
  int index_language;

  if (current_ekran.current_level == EKRAN_VIEW_SETTING_LANGUAGE)
    index_language = index_language_in_array(edition_settings.language);
  else
    index_language = index_language_in_array(settings_fix.language);
  
  //�������  ����� � ������� �����
  for (size_t i = 0; i< MAX_ROW_LCD; i++)
  {
    for (size_t j = 0; j < MAX_COL_LCD; j++) working_ekran[i][j] = (i == 0) ? information[index_language][j] : name_string[index_language][j];
  }
  
  //������ �� �������
  current_state_menu2.cursor_on = 0;
  //������ �� ����
  current_state_menu2.cursor_blinking_on = 0;
  //�������� ������� ���� �����
  current_state_menu2.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//��������� ��� ��������� ����� ��� ��������� ������
/*
  ����� ���������
  label_value - �������� �� ��������  �����
  inc_dec 
    0 - �������� �� �������
    1 - �������� �� �������
*/
/*****************************************************/
void inc_or_dec_value(unsigned int *label_value, unsigned int inc_dec)
{
  int temp_value = *label_value;
  
  if(inc_dec == 0) temp_value--;
  else if(inc_dec == 1) temp_value++;
  
  if(temp_value > 9) temp_value = 0;
  else if(temp_value < 0) temp_value = 9;
  
  *label_value = temp_value;
}
/*****************************************************/


/*****************************************************/
//���� ����� ��� ��� �������� ������
/*****************************************************/
void new_level_menu(void)
{
  switch (current_state_menu2.current_level)
  {
  case MAIN_MANU2_LEVEL:
    {
      current_state_menu2.func_move = move_into_main;
      current_state_menu2.func_show = make_ekran_main;
      current_state_menu2.func_edit = NULL;
      current_state_menu2.func_change = NULL;
      current_state_menu2.edition = 0;
      current_state_menu2.cursor_on = 1;
      current_state_menu2.cursor_blinking_on = 0;

      break;
    }
  case TIME_MANU2_LEVEL:
    {
      time_rewrite = 0;
      
      current_state_menu2.func_move = move_into_time;
      current_state_menu2.func_show = make_ekran_time;
      current_state_menu2.func_edit = edit_time;
      current_state_menu2.func_change = change_time;
      current_state_menu2.edition = 1;
      current_state_menu2.cursor_on = 0;
      current_state_menu2.cursor_blinking_on = 0;
      break;
    }
  case PASSWORD_MENU2_LEVEL:
    {
      current_state_menu2.func_move = NULL;
      current_state_menu2.func_show = make_ekran_password;
      current_state_menu2.func_edit = NULL;
      current_state_menu2.func_change = NULL;
      current_state_menu2.edition = 0;
      current_state_menu2.cursor_on = 1;
      current_state_menu2.cursor_blinking_on = 1;
      break;
    }
  default:
    {
      //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
      total_error_sw_fixed(64);
    }
  }
}
/*****************************************************/
