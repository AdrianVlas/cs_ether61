#include "header.h"

/*****************************************************/
//����������� � ����� � �������� � ����� ��� ����������� �� ����� ���������
/*****************************************************/
void convert_and_insert_char_for_measurement(unsigned int start_number_digit_after_point, unsigned int temp_meas, unsigned int koef_mul, unsigned int koef_div, unsigned char *name_string, unsigned int start_position)
{
  unsigned long long temp_value_long  = ((unsigned long long)temp_meas)*((unsigned long long)koef_mul)/((unsigned long long)koef_div);
  unsigned int temp_value = (unsigned int)temp_value_long;
  unsigned int number_digit_after_point = start_number_digit_after_point, error = 0, overflow = 0;
  
  //�������, �� ����� �� �� �� �������� � �� ����� ʲ�� � �� ����� ����
//  *(name_string + start_position + 7) = ' ';

  overflow = ((temp_value_long & 0xffffffff00000000ull) != 0);
  while (
         (
          (overflow != 0) ||
          (temp_value > 999999)
         )
         &&  
         (error == 0)  
        )   
  {
    //�� �� ���������� � �������� � ����� ���� � ����

    //³������� ������� ������� �����
    if (overflow != 0)
    {
      temp_value_long /= 10ull;
      overflow = ((temp_value_long & 0xffffffff00000000ull) != 0);
      if (overflow == 0)
        temp_value = temp_value_long;
    }
    else
      temp_value /=10;
    
    //������� ������� ��������� ���� �, ��� �������, ���������� �� ���� �����
    switch (number_digit_after_point)
    {
    case 1:
    case 2:
      {
        //���������� � ������� 1.5 �� 2.4 ��� � ������� 2.4 �� 3.3
        number_digit_after_point++;
        break;
      }
    case 3:
      {
        //���������� � ������� 3.3 �� 1.5 �� ������������� �������� ' '->'k' ��� 'k'->'M'
        number_digit_after_point = 1;
        if ((*(name_string + start_position + 7)) == ' ') 
        {
          if (current_settings.language == LANGUAGE_EN)
            *(name_string + start_position + 7) = 'k';
          else
            *(name_string + start_position + 7) = '�';
        }
        else if (
                 ((*(name_string + start_position + 7)) == '�') ||
                 ((*(name_string + start_position + 7)) == 'k')  
                )
        {
          *(name_string + start_position + 7) = '�';
        }
        else if ((*(name_string + start_position + 7)) == '�')
        {
          if (current_settings.language == LANGUAGE_EN)
            *(name_string + start_position + 7) = 'G';
          else
            *(name_string + start_position + 7) = '�';
        }
        else
        {
          /*
          ���������� ���� � �������� ����� ����� � ���� ��������:
          1) ������� � ����� ���� �� ���� �����, �� ��� ����� ������ �� �� ���� ����� � ����
          2) � 7-��� ������� ���� ����������� ������ - � �� � ��������� ��������, ��� � ����� ����� �� ���� ��������
          
          � ������ ��� ����� �������� ����������� ***.*** � ������ � 7-��� ������� ,���� ������ �� ���� ��������
          */
          error = 2;
        }
        break;
      }
    default:
      {
        /*
        ���������� ���� �������� ����� � ��  ���� �����
        
        � ������ ��� ����� �������� ����������� XXXXXXX
        */
        error = 1;
      }
    }
  }
  
  if (error == 0)
  {
    //��� �������� ����� �� ���������� ������� � �������� ����� ������� �� ���� �����������
    unsigned int first_symbol = 0, value, position = start_position; /*������� � ��� ��������� ��������� �������� - ���������� �����*/
    
    /*
    X?????
    */
    if (temp_value > 99999)
    {
      value = temp_value / 100000; 
      temp_value %= 100000;
      *(name_string + position) = value + 0x30;
      first_symbol = 1;
    }
    else
    {
      if (number_digit_after_point == 1)
      {
        *(name_string + position) = '0';
        first_symbol = 1;
      }
      else *(name_string + position) = ' ';
    }
    position++;

    if (number_digit_after_point == 1)
    {
      //��������� "����"
      *(name_string + position) = '.';
      position++;
    }

    /*
    *X????
    */
    if (temp_value > 9999)
    {
      value = temp_value / 10000; 
      temp_value %= 10000;
      *(name_string + position) = value + 0x30;
      first_symbol = 1;
    }
    else
    {
      if (
          (number_digit_after_point == 2) ||
          (first_symbol             != 0)  
         )   
      {
        *(name_string + position) = '0';
        first_symbol = 1;
      }
      else *(name_string + position) = ' ';
    }
    position++;

    if (number_digit_after_point == 2)
    {
      //��������� "����"
      *(name_string + position) = '.';
      position++;
    }
    
    /*
    **X???
    */
    value = temp_value / 1000; 
    temp_value %= 1000;
    *(name_string + position) = value + 0x30;
    position++;

    if (number_digit_after_point == 3)
    {
      //��������� "����"
      *(name_string + position) = '.';
      position++;
    }
    
    //� ����� ���� ����� position �� ����'������ ���� ����� (start_position + 4)
    if (position == (start_position + 4))
    {
      /*
      ***X??
      */
      value = temp_value / 100; 
      temp_value %= 100;
      *(name_string + (position++)) = value + 0x30;
  
      /*
      ****X?
      */
      value = temp_value / 10; 
      temp_value %= 10;
      *(name_string + (position++)) = value + 0x30;
  
      /*
      *****X
      */
      *(name_string + (position++)) = temp_value + 0x30;
    }
    else
    {
      //�������, ��� � ����� �� ���� �������� ������� �� ������� ������
      for (unsigned int i = 0; i < 7; i++) *(name_string + start_position + i) = '!';
      *(name_string + start_position + 7) = ' ';
    }
  }
  else if (error == 1)
  {
    //������� ������� ��� ����� ������������ �������
    for (unsigned int i = 0; i < 7; i++) *(name_string + start_position + i) = 'X';
    *(name_string + start_position + 7) = ' ';
  }
  else if (error == 2)
  {
    //������� ������� ��� ������� �� ���� �����
    for (unsigned int i = 0; i < 7; i++) *(name_string + start_position + i) = '*';
    *(name_string + start_position + 3) = '.';
  }
  else
  {
    //����������� �������
    for (unsigned int i = 0; i < 7; i++) *(name_string + start_position + i) = '?';
    *(name_string + start_position + 7) = ' ';
  }
}
/*****************************************************/

/*****************************************************/
//������� ����� ����������� ������
/*****************************************************/
void make_ekran_current(void)
{
  
  const uint8_t name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_MEASURMENT_ANALOG_INPUT][MAX_COL_LCD] = 
  {
    {
      " ���1 =         ",
      " ���2 =         ",
      " ���3 =         ",
      " ���4 =         "
    },
    {
      " ���1 =         ",
      " ���2 =         ",
      " ���3 =         ",
      " ���4 =         "
    },
    {
      " AIn1 =         ",
      " AIn2 =         ",
      " AIn3 =         ",
      " AIn4 =         "
    },
    {
      " ���1 =         ",
      " ���2 =         ",
      " ���3 =         ",
      " ���4 =         "
    }
  };
  uint8_t name_string_tmp[MAX_ROW_FOR_MEASURMENT_ANALOG_INPUT][MAX_COL_LCD];
  
  const uint32_t index_array[MAX_ROW_FOR_MEASURMENT_ANALOG_INPUT] = 
  {
    IM_AIN1,
    IM_AIN2,
    IM_AIN3,
    IM_AIN4
  };
  
  //������� ���������� �� ������ ��� �����������
  semaphore_measure_values_low1 = 1;
  for (unsigned int i = 0; i < MAX_ROW_FOR_MEASURMENT_ANALOG_INPUT; i++ ) 
  {
    unsigned int index_to_copy = index_array[i];
    measurement_low[index_to_copy] = measurement_middle[index_to_copy];
  }
  semaphore_measure_values_low1 = 0;


  int index_language = index_language_in_array(current_settings.language);
  for(int index_1 = 0; index_1 < MAX_ROW_FOR_MEASURMENT_ANALOG_INPUT; index_1++)
  {
    for(int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
    {
      name_string_tmp[index_1][index_2] = name_string[index_language][index_1][index_2];
    }

    name_string_tmp[index_1][MAX_COL_LCD - 1] = odynyci_vymirjuvannja[index_language][INDEX_A];
  }
  
  int position_temp = current_ekran.index_position;
  int index_of_ekran;

  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  //�������  ����� � ������� �����
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    //������� ����� ����� ���������, �� �� ����� ���������� � ������� �����������
    if (index_of_ekran < MAX_ROW_FOR_MEASURMENT_ANALOG_INPUT)
    {
      /********************************/
      //������� ���������� ��������  
      /********************************/
      unsigned int index = index_array[index_of_ekran];
      unsigned int start_number_digit_after_point = 3;
      convert_and_insert_char_for_measurement(start_number_digit_after_point, measurement_low[index], 1, 1, name_string_tmp[index_of_ekran], 7);
      /********************************/

      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string_tmp[index_of_ekran][j];
    }
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
