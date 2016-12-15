#include "header.h"

/*****************************************************/
//ĳ��������� ���
/*****************************************************/
inline void diagnostyca_adc_execution(void)
{
  if (gnd_adc1 >0x51) _SET_BIT(set_diagnostyka, ERROR_GND_ADC1_TEST_BIT);
  else _SET_BIT(clear_diagnostyka, ERROR_GND_ADC1_TEST_BIT);

  if (gnd_adc2 >0x51) _SET_BIT(set_diagnostyka, ERROR_GND_ADC2_TEST_BIT);
  else _SET_BIT(clear_diagnostyka, ERROR_GND_ADC2_TEST_BIT);

  if ((vref_adc1 <0x709) || (vref_adc1 > 0x8f5)) _SET_BIT(set_diagnostyka, ERROR_VREF_ADC1_TEST_BIT);
  else _SET_BIT(clear_diagnostyka,ERROR_VREF_ADC1_TEST_BIT);

  if ((vref_adc2 <0x709) || (vref_adc2 > 0x8f5)) _SET_BIT(set_diagnostyka, ERROR_VREF_ADC2_TEST_BIT);
  else _SET_BIT(clear_diagnostyka,ERROR_VREF_ADC2_TEST_BIT);

  if ((vdd_adc1 <0x8F9) || (vdd_adc1 > 0xC24)) _SET_BIT(set_diagnostyka, ERROR_VDD_ADC1_TEST_BIT);
  else _SET_BIT(clear_diagnostyka, ERROR_VDD_ADC1_TEST_BIT);

  if ((vdd_adc2 <0x8F9) || (vdd_adc2 > 0xC24)) _SET_BIT(set_diagnostyka, ERROR_VDD_ADC2_TEST_BIT);
  else _SET_BIT(clear_diagnostyka, ERROR_VDD_ADC2_TEST_BIT);
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
  int ortogonal_local[2*NUMBER_ANALOG_CANALES];
  //����������� ������� �������� ���������� ������� � ����������� �������

  //������� ����� �������� � ������� ����
  /*
  ������� ��� ����������� ������������ ��� ����������� ������� �� ����� ������� � ��������� �� 2 (��������� 2/�),
  �� �� �� ������ ������� ��������
  */
  /*
  �������� ���� ���������� ���������, �� ���� ����������� ������� ������� ������� �� �����/������� �� �����,
  � ���� ��������� ������� �� 2/� (������� �� ��������� �������, ��� ���� � ����� ����),
  �� ��������� ���������� ���������� ���� ���� ���������� ���������� �������� �������
  ����� ��� 3I0            - �� 19 �� + ���� = ((11 �� + ����)*16*16)
        ��� ������ ������ - �� 15 �� + ���� = ((11 �� + ����)*16   )
  ������� ��� �� ����� �� ����� ������� �� �������� � �� ���� �������� ����� ���������� � ���� ��������, ��
  ������ ����� ����� �������� �� �������� - ������������ �� ����, �� (15 �� *2) = 30 �� �� 32 �� unsigned int
  � �� ���� ������� ��������� �� 16, ��� �������� ������� ����������
  
  ��� 3I0 ������� ������������ - ���� ����� ���:
  1.  
  ����� 16-������ ��������� ������� ����� � ������������� ��� 3I0,
  ��� ����������� ������� �� ����� 15-���������� ����� + ����.
  ����� 16-������ ��������� ������� ��� � ����������� ���������
    
  2.����������� 64-���� ����������.
    
  �� 17 ��������� 2014 ���� ��������������� ������ �����.
  ��� �������� ������� ��� ���������� ����� ����� �������. ���������, �� ����� ����
  ��'����� � ���, �� ���� ���������� ������� ������� - �� ����� ����� �������� ������
  ����� ������ �� �������� �������� - � ��� �������� ����� ����� �������
    
  ���� ������ ����������� ������ ����� � 17 ��������� 2014 ����  
  
  ��� ���������� ������� � 18 ��������� 2014 ���� � ������� �������� sin/cos ���
  ����� ������ ������������
  */
  
  unsigned int bank_ortogonal_tmp = (bank_ortogonal + 1) & 0x1;
  for(unsigned int i=0; i<(2*NUMBER_ANALOG_CANALES); i++ )
  {
    ortogonal_local[i] = ortogonal[i][bank_ortogonal_tmp];
  }
  bank_ortogonal = bank_ortogonal_tmp;

  unsigned int copy_to_low_tasks = (semaphore_measure_values_low == 0) ? true : false;
  if (copy_to_low_tasks == true) current_delta_phi();
  
  /*******************************************************/
  //����������, �� ���������� ���� ���������
  /*******************************************************/
  if (changed_ustuvannja == CHANGED_ETAP_ENDED_EXTRA_ETAP) /*�� � �����, �� ��� ��� ���������� ��� �������� �� � ������ �������� �������(� ��� ����� ����� ��� �� ���������)*/
  {
    //������� ����� ��������� � ���� ����� ������ ��� � ���� ������ (���� � �����) ����� ����������� ���������
    for(unsigned int i = 0; i < NUMBER_ANALOG_CANALES; i++) 
    {
      phi_ustuvannja_meas[i] = phi_ustuvannja[i];
      phi_ustuvannja_sin_cos_meas[2*i    ] = phi_ustuvannja_sin_cos[2*i    ];
      phi_ustuvannja_sin_cos_meas[2*i + 1] = phi_ustuvannja_sin_cos[2*i + 1];
    }
      
    //�������, �� ���� ������� �������� �������
    changed_ustuvannja = CHANGED_ETAP_NONE;
  }
  /*****************************************************/

  /***
  ��������� ���� � ������� ����������� ��� ����������������� �����
  ***/
  for (unsigned int i = 0; i < NUMBER_ANALOG_CANALES; i++)
  {
    float sin_alpha = ((float)ortogonal_local[2*i    ])/((float)((1 << (VAGA_NUMBER_POINT - 1))));
    float cos_alpha = ((float)ortogonal_local[2*i + 1])/((float)((1 << (VAGA_NUMBER_POINT - 1))));
    
    float sin_beta;
    float cos_beta;
    if (i < I_Ua2)
    {
      sin_beta =  phi_ustuvannja_sin_cos_meas[2*i    ];
      cos_beta =  phi_ustuvannja_sin_cos_meas[2*i + 1];
    }
    else
    {
       unsigned int frequency_locking_bank_tmp = frequency_locking_bank & 0x1;
       sin_beta = phi_ustuvannja_sin_cos_meas[2*i    ]*frequency_locking_cos[frequency_locking_bank_tmp] + phi_ustuvannja_sin_cos_meas[2*i + 1]*frequency_locking_sin[frequency_locking_bank_tmp];
       cos_beta = phi_ustuvannja_sin_cos_meas[2*i + 1]*frequency_locking_cos[frequency_locking_bank_tmp] - phi_ustuvannja_sin_cos_meas[2*i    ]*frequency_locking_sin[frequency_locking_bank_tmp];
    }
    
    unsigned int new_index = index_converter_Ib_p[i];
    int ortogonal_sin = ortogonal_calc[2*new_index    ] = (int)(sin_alpha*cos_beta + cos_alpha*sin_beta);
    int ortogonal_cos = ortogonal_calc[2*new_index + 1] = (int)(cos_alpha*cos_beta - sin_alpha*sin_beta);

    
    //������� ����������� ��� ���������� ����
    if (copy_to_low_tasks == true)
    {
      ortogonal_calc_low[2*new_index    ] = ortogonal_sin;
      ortogonal_calc_low[2*new_index + 1] = ortogonal_cos;
    }
  }
  /***/
  
  /*
  ---------------------------------------------------------------------------------------------------------
  150� (150 000��) - ������������ ������ �����
  ����������� ����������� � ��  - Koef_1 = 84,978173543997808495193432804655 ��� ������ ������ (5439/64 = 84,984375)
  
  ��� ��� 150� ����������� ������� �������� ������������� ���� ����
  150000/Koef_1 = 1765,0303364589078874793160507446
  ���� ���������, �� ������ � ��������� � 16 ���, �� ����������� �������� ������������� ���� ����
  16*150000/Koef_1 = 28240,485383342526199669056811914 < 28241(0x6E51) �� � 15 ���� ����� (+ �������� ����)
  ---------------------------------------------------------------------------------------------------------

  ---------------------------------------------------------------------------------------------------------
  150� (150 000��) - ����������� ����� �������
  ����������� ����������� � ��  - Koef_1 = 64,883134509545420915167731259667 ��� ������ ������ (4152/64 = 64,875)
  
  ��� ��� 150� ����������� ������� �������� ������������� ���� ����
  150000/Koef_1 = 2311,848851536795430557291797995
  ���� ���������, �� ������ � ��������� � 16 ���, �� ����������� �������� ������������� ���� ����
  16*150000/Koef_1 = 36989,581624588726888916668767919 < 36990(0x907E) �� � 16 ���� ����� (+ �������� ����) - ����� ����� �������� 17-����
  
  ���� � ����� ������� ������������� ����� �������, ��, ���� ���������� �� ������� �������� � ��� �������, ��  �����������
  �������� ������������� ���� ����
  2*16*150000/Koef_1 = 73979,163249177453777833337535838 < 73980(0x120FC) �� � 17 ���� ����� (+ �������� ����) - ����� ����� �������� 18-����
  ---------------------------------------------------------------------------------------------------------
  
  ---------------------------------------------------------------------------------------------------------
  2� (2 000�� * 10 = 20 000(������� ��)) - ������������ ����� 3I0
  ����������� ����������� � ����� ��  - Koef_1* = 169,95634708799561699038686560931 ��� 3I0  ��� 3I0 ��� �������� � ������� �������� (170/1 = 170)

  
  ��� ��� 2� ����������� ������� �������� ������������� ���� ����
  20000/Koef_1* = 117,67727621049018824880803941698
  ���� ���������, �� ������ � ��������� � 16 ��� (��������� � 256 ��� �� ��� �������� �� 16), �� ����������� �������� ������������� ���� ����
  16*20000/Koef_1* = 1882,8364193678430119809286306717 < 1883(0x075B) �� � 11 ���� ����� (+ �������� ����)
  ---------------------------------------------------------------------------------------------------------
  */
  
  /***/
  //����������� ���� �������� ����� ������������ ���'�
  /***/
  for(unsigned int i = 0; i < NUMBER_ANALOG_CANALES; i++)
  {
#if (I_Ia == 0)
    if (i <= I_Ic)
#else
    if ((i >= I_Ia) && (i <= I_Ic))
#endif
    {
      unsigned int delta_index = (i - I_Ia);
      unsigned int index_m = IM_IA + delta_index;
      unsigned int index_ort = FULL_ORT_Ia + delta_index;
      
      measurement[index_m] = ( MNOGNYK_I_DIJUCHE*(sqrt_32((unsigned int)(ortogonal_calc[2*index_ort]*ortogonal_calc[2*index_ort]) + (unsigned int)(ortogonal_calc[2*index_ort+1]*ortogonal_calc[2*index_ort+1]))) ) >> (VAGA_DILENNJA_I_DIJUCHE + 4);
    }
    else
    {
      unsigned int delta_index = (i - I_Ua1);
      unsigned int index_m = IM_UA1 + delta_index;
      unsigned int index_ort = FULL_ORT_Ua1 + delta_index;
      
      measurement[index_m] = ( MNOGNYK_U_DIJUCHE*(sqrt_32((unsigned int)(ortogonal_calc[2*index_ort]*ortogonal_calc[2*index_ort]) + (unsigned int)(ortogonal_calc[2*index_ort+1]*ortogonal_calc[2*index_ort+1]))) ) >> (VAGA_DILENNJA_U_DIJUCHE + 4);
    }
    
  }
  /***/

  int _x, _y;
  //Ubc1
  _x = ortogonal_calc[2*FULL_ORT_Ubc1 + 0] = ortogonal_calc[2*FULL_ORT_Ub1    ] - ortogonal_calc[2*FULL_ORT_Uc1    ];
  _y = ortogonal_calc[2*FULL_ORT_Ubc1 + 1] = ortogonal_calc[2*FULL_ORT_Ub1 + 1] - ortogonal_calc[2*FULL_ORT_Uc1 + 1];
  if (copy_to_low_tasks == true)
  {
    ortogonal_calc_low[2*FULL_ORT_Ubc1 + 0] = _x;
    ortogonal_calc_low[2*FULL_ORT_Ubc1 + 1] = _y;
  }
  measurement[IM_UBC1] = ( MNOGNYK_U_DIJUCHE*(sqrt_64((unsigned long long)((long long)_x*(long long)_x) + (unsigned long long)((long long)_y*(long long)_y))) ) >> (VAGA_DILENNJA_U_DIJUCHE + 4);
  
  //Uca1
  _x = ortogonal_calc[2*FULL_ORT_Uca1 + 0] = ortogonal_calc[2*FULL_ORT_Uc1    ] - ortogonal_calc[2*FULL_ORT_Ua1    ];
  _y = ortogonal_calc[2*FULL_ORT_Uca1 + 1] = ortogonal_calc[2*FULL_ORT_Uc1 + 1] - ortogonal_calc[2*FULL_ORT_Ua1 + 1];
  if (copy_to_low_tasks == true)
  {
    ortogonal_calc_low[2*FULL_ORT_Uca1 + 0] = _x;
    ortogonal_calc_low[2*FULL_ORT_Uca1 + 1] = _y;
  }
  measurement[IM_UCA1] = ( MNOGNYK_U_DIJUCHE*(sqrt_64((unsigned long long)((long long)_x*(long long)_x) + (unsigned long long)((long long)_y*(long long)_y))) ) >> (VAGA_DILENNJA_U_DIJUCHE + 4);

  //Uab1
  _x = ortogonal_calc[2*FULL_ORT_Uab1 + 0] = ortogonal_calc[2*FULL_ORT_Ua1    ] - ortogonal_calc[2*FULL_ORT_Ub1    ];
  _y = ortogonal_calc[2*FULL_ORT_Uab1 + 1] = ortogonal_calc[2*FULL_ORT_Ua1 + 1] - ortogonal_calc[2*FULL_ORT_Ub1 + 1];
  if (copy_to_low_tasks == true)
  {
    ortogonal_calc_low[2*FULL_ORT_Uab1 + 0] = _x;
    ortogonal_calc_low[2*FULL_ORT_Uab1 + 1] = _y;
  }
  measurement[IM_UAB1] = ( MNOGNYK_U_DIJUCHE*(sqrt_64((unsigned long long)((long long)_x*(long long)_x) + (unsigned long long)((long long)_y*(long long)_y))) ) >> (VAGA_DILENNJA_U_DIJUCHE + 4);

  //Ubc2
  _x = ortogonal_calc[2*FULL_ORT_Ubc2 + 0] = ortogonal_calc[2*FULL_ORT_Ub2    ] - ortogonal_calc[2*FULL_ORT_Uc2    ];
  _y = ortogonal_calc[2*FULL_ORT_Ubc2 + 1] = ortogonal_calc[2*FULL_ORT_Ub2 + 1] - ortogonal_calc[2*FULL_ORT_Uc2 + 1];
  if (copy_to_low_tasks == true)
  {
    ortogonal_calc_low[2*FULL_ORT_Ubc2 + 0] = _x;
    ortogonal_calc_low[2*FULL_ORT_Ubc2 + 1] = _y;
  }
  measurement[IM_UBC2] = ( MNOGNYK_U_DIJUCHE*(sqrt_64((unsigned long long)((long long)_x*(long long)_x) + (unsigned long long)((long long)_y*(long long)_y))) ) >> (VAGA_DILENNJA_U_DIJUCHE + 4);
  
  //Uca2
  _x = ortogonal_calc[2*FULL_ORT_Uca2 + 0] = ortogonal_calc[2*FULL_ORT_Uc2    ] - ortogonal_calc[2*FULL_ORT_Ua2    ];
  _y = ortogonal_calc[2*FULL_ORT_Uca2 + 1] = ortogonal_calc[2*FULL_ORT_Uc2 + 1] - ortogonal_calc[2*FULL_ORT_Ua2 + 1];
  if (copy_to_low_tasks == true)
  {
    ortogonal_calc_low[2*FULL_ORT_Uca2 + 0] = _x;
    ortogonal_calc_low[2*FULL_ORT_Uca2 + 1] = _y;
  }
  measurement[IM_UCA2] = ( MNOGNYK_U_DIJUCHE*(sqrt_64((unsigned long long)((long long)_x*(long long)_x) + (unsigned long long)((long long)_y*(long long)_y))) ) >> (VAGA_DILENNJA_U_DIJUCHE + 4);

  //Uab2
  _x = ortogonal_calc[2*FULL_ORT_Uab2 + 0] = ortogonal_calc[2*FULL_ORT_Ua2    ] - ortogonal_calc[2*FULL_ORT_Ub2    ];
  _y = ortogonal_calc[2*FULL_ORT_Uab2 + 1] = ortogonal_calc[2*FULL_ORT_Ua2 + 1] - ortogonal_calc[2*FULL_ORT_Ub2 + 1];
  if (copy_to_low_tasks == true)
  {
    ortogonal_calc_low[2*FULL_ORT_Uab2 + 0] = _x;
    ortogonal_calc_low[2*FULL_ORT_Uab2 + 1] = _y;
  }
  measurement[IM_UAB2] = ( MNOGNYK_U_DIJUCHE*(sqrt_64((unsigned long long)((long long)_x*(long long)_x) + (unsigned long long)((long long)_y*(long long)_y))) ) >> (VAGA_DILENNJA_U_DIJUCHE + 4);
  /***/
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
  for (unsigned int i = (INDEX_TIMER_INPUT_START + NUMBER_INPUTS); i < MAX_NUMBER_GLOBAL_TIMERS; i++)
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
//����������� ������������� ������� - �� ����������� ���� ������������� ����� ��� �������
/*****************************************************/
inline void df_handler(volatile unsigned int *p_active_functions, unsigned int *p_changed_state_with_start_new_timeout)
{
}
/*****************************************************/

/*****************************************************/
//����������� ������������� ������ - �� ����������� ���� ������������� �������������� �������
/*****************************************************/
inline void dt_handler(volatile unsigned int *p_active_functions)
{
}
/*****************************************************/

/*****************************************************/
//����������� ������������ "�"
/*****************************************************/
inline void d_and_handler(volatile unsigned int *p_active_functions)
{
  unsigned int state_defined_and = 0;

  //��������� ���� ��� ������������ "�" (�� ����������� ���� �� �� � �������� �����)
  for (unsigned int i = 0; i < /*NUMBER_DEFINED_AND*/current_settings_prt.number_defined_and; i++)
  {
    if (
        ((current_settings_prt.ranguvannja_d_and[N_BIG*i + 0] & p_active_functions[0]) == current_settings_prt.ranguvannja_d_and[N_BIG*i + 0]) && 
        ((current_settings_prt.ranguvannja_d_and[N_BIG*i + 1] & p_active_functions[1]) == current_settings_prt.ranguvannja_d_and[N_BIG*i + 1]) &&
        ((current_settings_prt.ranguvannja_d_and[N_BIG*i + 2] & p_active_functions[2]) == current_settings_prt.ranguvannja_d_and[N_BIG*i + 2]) &&
        ((current_settings_prt.ranguvannja_d_and[N_BIG*i + 3] & p_active_functions[3]) == current_settings_prt.ranguvannja_d_and[N_BIG*i + 3]) &&
        ((current_settings_prt.ranguvannja_d_and[N_BIG*i + 4] & p_active_functions[4]) == current_settings_prt.ranguvannja_d_and[N_BIG*i + 4]) &&
        ((current_settings_prt.ranguvannja_d_and[N_BIG*i + 5] & p_active_functions[5]) == current_settings_prt.ranguvannja_d_and[N_BIG*i + 5]) &&
        ((current_settings_prt.ranguvannja_d_and[N_BIG*i + 6] & p_active_functions[6]) == current_settings_prt.ranguvannja_d_and[N_BIG*i + 6])
       )
    {
      state_defined_and |= (1 << i);
    }
  }

  //������������, ��� ������� ���������� "�" � ����� �������, �� ����� ������ ��������������
  /*
  ��� ���� � ��������� �� ��'���� � ����, � ������� �������, �� � ������� �� �������������
  ����� p_active_functions � ����� �� �� ���������� ��� �������� ������ �-"�", ���� �� ���� �-"�"
  ������ ���� �������� , ��� �������� ���� ���� �������� �������� �-"�", � �� ��,
  �� ����� ��������������. � �������� �� ������������ �������� � �����
  p_active_functions, �� ��������� ����� ����� ����, �� ��� �� ������ ��'������� ������������ ��������
  � ����������� �� ����� ���'����� (���� ������ ������ � ����� � ������ ������������/��������)
  */
  for (unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
  {
    //������������, ��� ������� �-"�"
    unsigned int index_d_and = RANG_D_AND1 + i;
      
    if ((state_defined_and & (1 << i)) != 0 ) _SET_BIT(p_active_functions, index_d_and);
    else _CLEAR_BIT(p_active_functions, index_d_and);
  }
}
/*****************************************************/

/*****************************************************/
//����������� ������������ "���"
/*****************************************************/
inline void d_or_handler(volatile unsigned int *p_active_functions)
{
  unsigned int state_defined_or = 0;

  //��������� ���� ��� ������������ "���" (�� ����������� ���� �� �� � �������� �����)
  for (unsigned int i = 0; i < /*NUMBER_DEFINED_OR*/current_settings_prt.number_defined_or; i++)
  {
    if (
        ((current_settings_prt.ranguvannja_d_or[N_BIG*i + 0] & p_active_functions[0]) != 0) || 
        ((current_settings_prt.ranguvannja_d_or[N_BIG*i + 1] & p_active_functions[1]) != 0) ||
        ((current_settings_prt.ranguvannja_d_or[N_BIG*i + 2] & p_active_functions[2]) != 0) ||
        ((current_settings_prt.ranguvannja_d_or[N_BIG*i + 3] & p_active_functions[3]) != 0) ||
        ((current_settings_prt.ranguvannja_d_or[N_BIG*i + 4] & p_active_functions[4]) != 0) ||
        ((current_settings_prt.ranguvannja_d_or[N_BIG*i + 5] & p_active_functions[5]) != 0) ||
        ((current_settings_prt.ranguvannja_d_or[N_BIG*i + 6] & p_active_functions[6]) != 0)
       )
    {
      state_defined_or |= (1 << i);
    }
  }

  //������������, ��� ������� ���������� "���" � ����� �������, �� ����� ������ ��������������
  /*
  ��� ���� � ��������� �� ��'���� � ����, � ������� �������, �� � ������� �� �������������
  ����� p_active_functions � ����� �� �� ���������� ��� �������� ������ �-"���", ���� �� ���� �-"���"
  ������ ���� �������� , ��� �������� ���� ���� �������� �������� �-"���", � �� ��,
  �� ����� ��������������. � �������� �� ������������ �������� � �����
  p_active_functions, �� ��������� ����� ����� ����, �� ��� �� ������ ��'������� ������������ ��������
  � ����������� �� ����� ���'����� (���� ������ ������ � ����� � ������ ������������/��������)
  */
  for (unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
  {
    //������������, ��� ������� �-"���"
    unsigned int index_d_or = RANG_D_OR1 + i;
      
    if ((state_defined_or & (1 << i)) != 0 ) _SET_BIT(p_active_functions, index_d_or);
    else _CLEAR_BIT(p_active_functions, index_d_or);
  }
}
/*****************************************************/

/*****************************************************/
//����������� ������������ "����.���"
/*****************************************************/
inline void d_xor_handler(volatile unsigned int *p_active_functions)
{
  unsigned int state_defined_xor = 0;

  //��������� ���� ��� ������������ "����.���" (�� ����������� ���� �� �� � �������� �����)
  for (unsigned int i = 0; i < /*NUMBER_DEFINED_XOR*/current_settings_prt.number_defined_xor; i++)
  {
    unsigned int temp_array[N_BIG];
    temp_array[0] = current_settings_prt.ranguvannja_d_xor[N_BIG*i + 0] & p_active_functions[0];
    temp_array[1] = current_settings_prt.ranguvannja_d_xor[N_BIG*i + 1] & p_active_functions[1];
    temp_array[2] = current_settings_prt.ranguvannja_d_xor[N_BIG*i + 2] & p_active_functions[2];
    temp_array[3] = current_settings_prt.ranguvannja_d_xor[N_BIG*i + 3] & p_active_functions[3];
    temp_array[4] = current_settings_prt.ranguvannja_d_xor[N_BIG*i + 4] & p_active_functions[4];
    temp_array[5] = current_settings_prt.ranguvannja_d_xor[N_BIG*i + 5] & p_active_functions[5];
    temp_array[6] = current_settings_prt.ranguvannja_d_xor[N_BIG*i + 6] & p_active_functions[6];
    
    if (
        (temp_array[0] != 0) || 
        (temp_array[1] != 0) ||
        (temp_array[2] != 0) ||
        (temp_array[3] != 0) ||
        (temp_array[4] != 0) ||
        (temp_array[5] != 0) ||
        (temp_array[6] != 0)
       )
    {
      unsigned int signals = 0;
      for (unsigned int j = 0; j < N_BIG; j++)
      {
        for (unsigned int k = 0; k < 32; k++)
        {
          if ((temp_array[j] & (1 << k)) != 0) signals++;
        }
      }
      if (signals == 1) state_defined_xor |= (1 << i);
    }
  }

  //������������, ��� ������� ���������� "����.���" � ����� �������, �� ����� ������ ��������������
  /*
  ��� ���� � ��������� �� ��'���� � ����, � ������� �������, �� � ������� �� �������������
  ����� p_active_functions � ����� �� �� ���������� ��� �������� ������ �-"����.���", ���� �� ���� �-"����.���"
  ������ ���� �������� , ��� �������� ���� ���� �������� �������� �-"����.���", � �� ��,
  �� ����� ��������������. � �������� �� ������������ �������� � �����
  p_active_functions, �� ��������� ����� ����� ����, �� ��� �� ������ ��'������� ������������ ��������
  � ����������� �� ����� ���'����� (���� ������ ������ � ����� � ������ ������������/��������)
  */
  for (unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
  {
    //������������, ��� ������� �-"����.���"
    unsigned int index_d_xor = RANG_D_XOR1 + i;
      
    if ((state_defined_xor & (1 << i)) != 0 ) _SET_BIT(p_active_functions, index_d_xor);
    else _CLEAR_BIT(p_active_functions, index_d_xor);
  }
}
/*****************************************************/

/*****************************************************/
//����������� ������������ "��"
/*****************************************************/
inline void d_not_handler(volatile unsigned int *p_active_functions)
{
  unsigned int state_defined_not = 0;

  //��������� ���� ��� ������������ "��" (�� ����������� ���� �� �� � �������� �����)
  for (unsigned int i = 0; i < /*NUMBER_DEFINED_NOT*/current_settings_prt.number_defined_not; i++)
  {
    if (
        ((current_settings_prt.ranguvannja_d_not[N_BIG*i + 0] & p_active_functions[0]) == 0) &&
        ((current_settings_prt.ranguvannja_d_not[N_BIG*i + 1] & p_active_functions[1]) == 0) &&
        ((current_settings_prt.ranguvannja_d_not[N_BIG*i + 2] & p_active_functions[2]) == 0) &&
        ((current_settings_prt.ranguvannja_d_not[N_BIG*i + 3] & p_active_functions[3]) == 0) &&
        ((current_settings_prt.ranguvannja_d_not[N_BIG*i + 4] & p_active_functions[4]) == 0) &&
        ((current_settings_prt.ranguvannja_d_not[N_BIG*i + 5] & p_active_functions[5]) == 0) &&
        ((current_settings_prt.ranguvannja_d_not[N_BIG*i + 6] & p_active_functions[6]) == 0)
       )
    {
      state_defined_not |= (1 << i);
    }
  }

  //������������, ��� ������� ���������� "��" � ����� �������, �� ����� ������ ��������������
  /*
  ��� ���� � ��������� �� ��'���� � ����, � ������� �������, �� � ������� �� �������������
  ����� p_active_functions � ����� �� �� ���������� ��� �������� ������ �-"��", ���� �� ���� �-"��"
  ������ ���� �������� , ��� �������� ���� ���� �������� �������� �-"��", � �� ��,
  �� ����� ��������������. � �������� �� ������������ �������� � �����
  p_active_functions, �� ��������� ����� ����� ����, �� ��� �� ������ ��'������� ������������ ��������
  � ����������� �� ����� ���'����� (���� ������ ������ � ����� � ������ ������������/��������)
  */
  for (unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
  {
    //������������, ��� ������� �-"��"
    unsigned int index_d_not = RANG_D_NOT1 + i;
      
    if ((state_defined_not & (1 << i)) != 0 ) _SET_BIT(p_active_functions, index_d_not);
    else _CLEAR_BIT(p_active_functions, index_d_not);
  }
}
/*****************************************************/

/*****************************************************/
// "�������� ���������"
/*****************************************************/
void ctrl_phase_handler(volatile unsigned int *p_active_functions)
{
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
//������� ������� ����� ����������� ����������
/*****************************************************/
inline void digital_registrator(volatile unsigned int* carrent_active_functions)
{
}
/*****************************************************/

/*****************************************************/
//����������� ����������� ������� ������ ����������� ����������
/*****************************************************/
void fix_undefined_error_ar(volatile unsigned int* carrent_active_functions)
{
  //����������� ������� � ������� � ��������� ���������
  _SET_BIT(set_diagnostyka, ERROR_AR_UNDEFINED_BIT);
  _SET_BIT(carrent_active_functions, RANG_DEFECT);
  //���������� ����� ������ � ����������� � ���� "�� ����� ������ ���� �� � ���������� ����������� �� ����������" 
  continue_previous_record_ar = 0; /*�������, �� �� �� ������ ����������� ��� ������ ��������� ����������� ����������*/
  state_ar_record = STATE_AR_NO_RECORD;
  //������� ������ ������ ����������� ����������
  _CLEAR_BIT(carrent_active_functions, RANG_WORK_A_REJESTRATOR);

  //����������� ������� ������ ��������� ��� ����������� ���������� � EEPROM
  _SET_BIT(control_i2c_taskes, TASK_START_WRITE_INFO_REJESTRATOR_AR_EEPROM_BIT);
  //³��������� ������������ ��������� ��� ����������� ����������
  /*
  ������ �������� ���������, ����� ���� ��� � ����
  */
  info_rejestrator_ar.saving_execution = 0;
  /*
  ������� ������ ������ ����� ������ ������ ��������, � �� �������, ��, ���� ��� ���� ������ ���
  ������� ������, �� ���� �������. ���� ����� �������, �� ���� � ����������� ��������� �� ����� ���� ����
  ����������� ������� ������� ������, �� ����� ������� � ��� � ���������, ����� ������� ������ ����� 
  � ������ ������� �� ������� �����
  */
  unsigned int max_number_records_ar_tmp = max_number_records_ar;
  if (info_rejestrator_ar.number_records >= max_number_records_ar_tmp) 
    info_rejestrator_ar.number_records = max_number_records_ar_tmp - 1; /*����� ��� �� ���� ==, ��� ��� ��������������� �� ����������� ������� � �������� >=*/
}
/*****************************************************/

/*****************************************************/
//������� ������� ����� ����������� ����������
/*****************************************************/
inline void analog_registrator(volatile unsigned int* carrent_active_functions)
{
  static unsigned int unsaved_bytes_of_header_ar;

  //���������� ������� ����������� �������  ������ ����������� ����������
  _SET_BIT(clear_diagnostyka, ERROR_AR_UNDEFINED_BIT);

  if (continue_previous_record_ar != 0)
  {
    /*
    �� �������� ������, �� ���� ��������� ������� ����������� ����������, �� ���������
    � ������ ���������� ���������, � ����� ��� ������������� ��������� ��������� ����� ����� �� 
    ������ ��������, �� �� ������� ��������� ������������� (� ����-���� ��� �� ��
    ���������� ����������� �������� ������ ����������� ����������, �� ��� ���� ����������
    �����������. �� ���� ������ ������������� ��������� ������ ������ ������)
    */
    if(
       ((carrent_active_functions[0] & current_settings_prt.ranguvannja_analog_registrator[0]) == 0) &&
       ((carrent_active_functions[1] & current_settings_prt.ranguvannja_analog_registrator[1]) == 0) &&
       ((carrent_active_functions[2] & current_settings_prt.ranguvannja_analog_registrator[2]) == 0) &&
       ((carrent_active_functions[3] & current_settings_prt.ranguvannja_analog_registrator[3]) == 0) &&
       ((carrent_active_functions[4] & current_settings_prt.ranguvannja_analog_registrator[4]) == 0) &&
       ((carrent_active_functions[5] & current_settings_prt.ranguvannja_analog_registrator[5]) == 0) &&
       ((carrent_active_functions[6] & current_settings_prt.ranguvannja_analog_registrator[6]) == 0)
      ) 
    {
      //����� ������������� ��������� ������� ������ ������ ��������
      continue_previous_record_ar = 0;
    }
  }

  switch (state_ar_record)
  {
  case STATE_AR_NO_RECORD:
    {
      if(semaphore_read_state_ar_record == 0)
      {
        /*
        ������� ���� ��������, �� ��� ��������� ����� ������ ����������� ���������� ���������� ���������� ������ ���������� ����������
        ������ ������� semaphore_read_state_ar_record ��� ����, �� ���� ������ ����������� ���������� STATE_AR_NO_RECORD ������,
        �� ����� ���� �������� �� ���� ������, ���� ������ ���������� ������� ���� ��� ��������� ����������� ������ ����������� ����������
        */
        _SET_BIT(clear_diagnostyka, ERROR_AR_TEMPORARY_BUSY_BIT);
      }

      //��������, �� ����� ����� ������� ����������� ����������
      if (
          (
           ((carrent_active_functions[0] & current_settings_prt.ranguvannja_analog_registrator[0]) != 0) ||
           ((carrent_active_functions[1] & current_settings_prt.ranguvannja_analog_registrator[1]) != 0) ||
           ((carrent_active_functions[2] & current_settings_prt.ranguvannja_analog_registrator[2]) != 0) ||
           ((carrent_active_functions[3] & current_settings_prt.ranguvannja_analog_registrator[3]) != 0) ||
           ((carrent_active_functions[4] & current_settings_prt.ranguvannja_analog_registrator[4]) != 0) ||
           ((carrent_active_functions[5] & current_settings_prt.ranguvannja_analog_registrator[5]) != 0) ||
           ((carrent_active_functions[6] & current_settings_prt.ranguvannja_analog_registrator[6]) != 0)
          )
          &&  
          (continue_previous_record_ar == 0) /*��� ��������� ����� ��.���������� (���� ���� ����) ��� �� ������� ��������� ���� ���� � ����� ���� ����� �������*/ 
         )
      {
        //����������, �� ��� ������� ������ ������ � ��� �� ��� ������ �������������� ���������� ���������
        if(semaphore_read_state_ar_record == 0)
        {
          //� ����� ������� ����������� ����������
          continue_previous_record_ar = 0xff; /*������� ����-���� ������, �� ������������ �������� ��.����������, �� ��������� ������ ����������� ����������*/
    
          //����� �������� ����� �����
          
          //�������� ���� ������� ������
          header_ar.label_start_record = LABEL_START_RECORD_AR;
          //�������� ��� ������� ������
          unsigned char *label_to_time_array;
          if (copying_time == 0) label_to_time_array = time;
          else label_to_time_array = time_copy;
          for(unsigned int i = 0; i < 7; i++) header_ar.time[i] = *(label_to_time_array + i);
          //�'�� ������
          for(unsigned int i=0; i<MAX_CHAR_IN_NAME_OF_CELL; i++)
            header_ar.name_of_cell[i] = current_settings_prt.name_of_cell[i] & 0xff;
          
          //�������, �� �� �� �� �� "��������" ����� �������� � ����� ��� ����������� ����������
          copied_number_samples = 0;
          //��������� �������� ������� ������� �������, �� ����� ���� ������� � ��������� dataFlash2
          total_number_samples = ((current_settings_prt.prefault_number_periods + current_settings_prt.postfault_number_periods) << VAGA_NUMBER_POINT_AR)*(NUMBER_ANALOG_CANALES + number_word_digital_part_ar);

          //���������,�� ������ ��������� ����������� ���������� �� ����������� � ����� ����� ������ ��� ����������� ��� ��� ������ � DataFlash
          unsaved_bytes_of_header_ar = sizeof(__HEADER_AR);

          //��������� � ��� ������ ����������
          temporary_address_ar = info_rejestrator_ar.next_address;

          //���������, �� ���� �� ���� ������������ ����� ��� ������
          count_to_save = 0;
          //����������� ����-���� ���������� ������ ����� �� ��������� ����� ����� ��� ������
          permit_copy_new_data = 0xff;

          //����� ������ ������������ ��� ������� ��������� ����������� ����������� � ������������ ����� � ����� ��� ������ � DataFlash
          if (making_buffer_for_save_ar_record(&unsaved_bytes_of_header_ar) != 0)
          {
            //³������� ���������� �������� - ���� �������� ���������� ����� �� ���� � ��������
            fix_undefined_error_ar(carrent_active_functions);
          }
          else
          {
            //���������� ����� ������ �� ���������� ����������� � ���� "����� ������ ������"
            state_ar_record = STATE_AR_START;
            //����������� ������� �������
            _SET_BIT(carrent_active_functions, RANG_WORK_A_REJESTRATOR);

            //� �������� �� ���������� ����� ���������� ����������� �����������, �� ������� ����� � �� �� ���������
            _SET_BIT(control_i2c_taskes, TASK_START_WRITE_INFO_REJESTRATOR_AR_EEPROM_BIT);
            info_rejestrator_ar.saving_execution = 1;
          }
        }
        else
        {
          //����������� �������, �� ��������� ���������� ��������� � ������� (���� ��, �� ��� �������� ������ ����� ��������)
          _SET_BIT(set_diagnostyka, ERROR_AR_TEMPORARY_BUSY_BIT);
          _SET_BIT(carrent_active_functions, RANG_DEFECT);
        }
      }
      break;
    }
  case STATE_AR_START:
    {
      //ͳ���� �� �� ��������, ���� �� ����������� ����� STATE_AR_SAVE_SRAM_AND_SAVE_FLASH,  ��� STATE_AR_ONLY_SAVE_FLASH
      break;
    }
  case STATE_AR_SAVE_SRAM_AND_SAVE_FLASH:
  case STATE_AR_ONLY_SAVE_FLASH:
    {
      if (state_ar_record == STATE_AR_ONLY_SAVE_FLASH)
      {
        /*
        ���� ������������ ����� ������������ �� ������
        */
        if (_CHECK_SET_BIT(carrent_active_functions, RANG_WORK_A_REJESTRATOR) != 0)
        {
          //������ ������ ������ ����������� ����������
          _CLEAR_BIT(carrent_active_functions, RANG_WORK_A_REJESTRATOR);
        }
        
        if (_CHECK_SET_BIT(carrent_active_functions, RANG_WORK_A_REJESTRATOR) == 0)
        {
          /*
          ��������� ����� ��� ������, ���� ������ ������� ������ ����������� ����������� ��� ������
          */
          if  (continue_previous_record_ar == 0)
          {
            /*
            ����������, �� ���� ����� ������� ������ ������� �� �������, 
            ���� �� ������ ����� �� ��������� �������
            */
            if (
                ((carrent_active_functions[0] & current_settings_prt.ranguvannja_analog_registrator[0]) != 0) ||
                ((carrent_active_functions[1] & current_settings_prt.ranguvannja_analog_registrator[1]) != 0) ||
                ((carrent_active_functions[2] & current_settings_prt.ranguvannja_analog_registrator[2]) != 0) ||
                ((carrent_active_functions[3] & current_settings_prt.ranguvannja_analog_registrator[3]) != 0) ||
                ((carrent_active_functions[4] & current_settings_prt.ranguvannja_analog_registrator[4]) != 0) ||
                ((carrent_active_functions[5] & current_settings_prt.ranguvannja_analog_registrator[5]) != 0) ||
                ((carrent_active_functions[6] & current_settings_prt.ranguvannja_analog_registrator[6]) != 0)
               ) 
            {
              //����������� �������, �� ��������� ���������� ��������� � ������� (���� ��, �� ����������� ��������� �����)
              _SET_BIT(set_diagnostyka, ERROR_AR_TEMPORARY_BUSY_BIT);
              _SET_BIT(carrent_active_functions, RANG_DEFECT);
            }
          }
        }
      }
      
      if (permit_copy_new_data != 0)
      {
        /*
        ����� ������ ������������ ��� ������� ��������� ����������� ����������� 
        � ������������ ����� � ����� ��� ������ � DataFlash ����� ���, ���� � �����
        �� �� ��������
        */
        if (making_buffer_for_save_ar_record(&unsaved_bytes_of_header_ar) != 0)
        {
          //³������� ���������� �������� - ���� �������� ���������� ����� �� ���� � ��������
          fix_undefined_error_ar(carrent_active_functions);
        }
      }
      else
      {
        if (
            (copied_number_samples == total_number_samples) &&
            (count_to_save == 0                           ) && 
            (
             (control_tasks_dataflash &
              (
               TASK_MAMORY_PART_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_AR | 
               TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_AR
              )
             ) == 0
            )   
           )
        {
          //����� ������� ������ ������������ ����������
          unsigned int index_array_ar_heat_tmp = index_array_ar_heat;/*�� ����� ��� ����, �� ��������� ���� ����������� ��� �������� ������ � ��������� volatile*/
          unsigned int index_array_ar_tail_tmp = index_array_ar_tail;/*�� ����� ��� ����, �� ��������� ���� ����������� ��� �������� ������ � ��������� volatile*/
          if(
             (index_array_ar_tail_tmp == index_array_ar_heat_tmp) &&
             (state_ar_record == STATE_AR_ONLY_SAVE_FLASH)  
            )  
          {
            //�������� ����� ������� ������ ����������� ����������
            state_ar_record = STATE_AR_NO_RECORD;

            //����������� ������� ������ ��������� ����������� ���������� � EEPROM
            _SET_BIT(control_i2c_taskes, TASK_START_WRITE_INFO_REJESTRATOR_AR_EEPROM_BIT);
            //��������� ���� ������ ���������� ������, ���� ������� ������ � ������ �����������, �� ����� ��� �����
            if ((temporary_address_ar + size_one_ar_record) > (NUMBER_PAGES_INTO_DATAFLASH_2 << VAGA_SIZE_PAGE_DATAFLASH_2))
              temporary_address_ar = 0; 
            info_rejestrator_ar.next_address = temporary_address_ar;
            info_rejestrator_ar.saving_execution = 0;
            unsigned int max_number_records_ar_tmp = max_number_records_ar;
            if (info_rejestrator_ar.number_records < max_number_records_ar_tmp) info_rejestrator_ar.number_records += 1;
            else info_rejestrator_ar.number_records = max_number_records_ar_tmp;
          }
          else
          {
            /*� ������ ������ ����������� ���������� ������� ���, ���� �����
            �� ����������������� ���������� ������ ����������� ����������
            
            �� ������ ������ ������� �������� ����, �� �� ��������� ����������
            ����������� ������� ����������� ������� �������
            */
            fix_undefined_error_ar(carrent_active_functions);
          }
        }
        else
        {
          //����� ������ ������� �� ����� ������������ �����
          if (
              (count_to_save != 0 ) 
              && 
              (
               (control_tasks_dataflash &
                (
                 TASK_MAMORY_PART_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_AR | 
                 TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_AR
                )
               ) == 0
              )   
             )
          {
            /*
            ������ ������� �� ����� ����� ����� ����� ��� ���� �� ��� ����� �����
            ���������� ����������� ����� � ���� � ��� ��� ��� ������
            */
            
            if (((temporary_address_ar & 0x1ff) + count_to_save) <= SIZE_PAGE_DATAFLASH_2)
            {
              //���� ������� ��� ��������� ������� ���� ��� ������ (� ���� ������� ��� ���������� � ������ ������)
              
              if (count_to_save == SIZE_PAGE_DATAFLASH_2) control_tasks_dataflash |= TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_AR;
              else control_tasks_dataflash |= TASK_MAMORY_PART_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_AR;
            }
            else
            {
              //³������� ���������� �������� - ���� �������� ���������� ����� �� ���� � ��������
              fix_undefined_error_ar(carrent_active_functions);
            }
          }
        }

      }
      break;
    }
  case STATE_AR_TEMPORARY_BLOCK:
    {
      //�� ����� ������ ���� ������� �������� �������� ������ ����������� �����������
      //��������, �� ����� ����� ������� ����������� ����������
      if (
          ((carrent_active_functions[0] & current_settings_prt.ranguvannja_analog_registrator[0]) != 0) ||
          ((carrent_active_functions[1] & current_settings_prt.ranguvannja_analog_registrator[1]) != 0) ||
          ((carrent_active_functions[2] & current_settings_prt.ranguvannja_analog_registrator[2]) != 0) ||
          ((carrent_active_functions[3] & current_settings_prt.ranguvannja_analog_registrator[3]) != 0) ||
          ((carrent_active_functions[4] & current_settings_prt.ranguvannja_analog_registrator[4]) != 0) ||
          ((carrent_active_functions[5] & current_settings_prt.ranguvannja_analog_registrator[5]) != 0) ||
          ((carrent_active_functions[6] & current_settings_prt.ranguvannja_analog_registrator[6]) != 0)
         )
      {
        //����������� �������, �� ��������� ���������� ��������� � �������
        _SET_BIT(set_diagnostyka, ERROR_AR_TEMPORARY_BUSY_BIT);
        _SET_BIT(carrent_active_functions, RANG_DEFECT);
      }
      break;
    }
  default:
    {
      //³������� ���������� �������� - ���� �������� ���������� ����� �� ���� � ��������
      fix_undefined_error_ar(carrent_active_functions);
      break;
    }
  }
}
/*****************************************************/


/*****************************************************/
//������� ������� � ��� ����������� �� ���� ��������
/*****************************************************/
inline void main_protection(void)
{
  copying_active_functions = 1; //�������, �� ����� ���������� �������� �������� �������
  
  //������� � �������, �� ���������� �� �����, ������� � ��������� � ����������
  const unsigned int maska_input_signals[N_BIG] = 
  {
    MASKA_FOR_INPUT_SIGNALS_0, 
    MASKA_FOR_INPUT_SIGNALS_1, 
    MASKA_FOR_INPUT_SIGNALS_2, 
    MASKA_FOR_INPUT_SIGNALS_3, 
    MASKA_FOR_INPUT_SIGNALS_4, 
    MASKA_FOR_INPUT_SIGNALS_5, 
    MASKA_FOR_INPUT_SIGNALS_6 
  };
  for (unsigned int i = 0; i < N_BIG; i++) active_functions[i] &= (unsigned int)(~maska_input_signals[i]);
  
  /**************************/
  //��������, �� ����� �������� ������ �������
  /**************************/
  if (reset_trigger_function_from_interface !=0)
  {
    if ((reset_trigger_function_from_interface & (1 << USB_RECUEST)) != 0)
    {
      for (unsigned int i = 0; i < N_BIG; i++) trigger_functions_USB[i] = 0;
    }
    if ((reset_trigger_function_from_interface & (1 << RS485_RECUEST)) != 0)
    {
      for (unsigned int i = 0; i < N_BIG; i++) trigger_functions_RS485[i] = 0;
    }
    
    //������� �� �� �������� ������� �� �Ѳ� �����������
    reset_trigger_function_from_interface = 0;
  }
  /**************************/

  /**************************/
  //����������� �������� ������ - ����� ��������������� ����������, �� ��� ��� ����� �� ������������ ���������� ������� � ������
  /**************************/
  if ((pressed_buttons !=0) || (activation_function_from_interface != 0))
  {
    unsigned int temp_value_for_activated_function = 0;
    
    //��������� � �������
    if (pressed_buttons != 0)
    {
//      for (unsigned int i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
//      {
//        if ((pressed_buttons & (1 << i)) != 0)
//          temp_value_for_activated_function |= current_settings_prt.ranguvannja_buttons[i];
//      }

      //������� �������� ������, �� �� ��� ����������
      pressed_buttons =0;
    }
    
    //��������� � ����������
    if (activation_function_from_interface != 0)
    {
      temp_value_for_activated_function |= activation_function_from_interface;
      
      //������� ������ ��������� ������� � ���������, �� �� ��� ����������
      activation_function_from_interface = 0;
    }
    
    //����  ������� �������������� ����� ������ �� ���������� �� � ���������� ����� �������, �� ����� ���� ����� ���������
    if(temp_value_for_activated_function != 0) 
    {
      //�������� ��������� � ����
      if (_GET_OUTPUT_STATE(temp_value_for_activated_function, RANG_TU_SILENCE)) 
        _SET_BIT(active_functions, RANG_SILENCE);
      if (_GET_OUTPUT_STATE(temp_value_for_activated_function, RANG_TU_RESET)) 
        _SET_BIT(active_functions, RANG_RESET);
      if (_GET_OUTPUT_STATE(temp_value_for_activated_function, RANG_TU_TEST)) 
        _SET_BIT(active_functions, RANG_TEST);
    }
  }
  /**************************/

  /**************************/
  //����������� �������� �����
  /**************************/
  //���������� �� � ����� ������ �����
  if (state_inputs !=0)
  {
    for (unsigned int i = 0; i < NUMBER_INPUTS; i++)
    {
      if ((state_inputs & (1 << i)) != 0) _SET_BIT(active_functions, (RANG_DI1 + i));
    }
  }
  /**************************/

  /**************************/
  //�������� ���������� ���������� ������ ��� ��� �������, �� ����� ������������ ����� �� ���������� �������
  /**************************/
  {
    static unsigned int previous_activating_functions[N_BIG];
    //������� ����� �������, �� ����� ���������, �� ������ ����� � ������ �������� � "0" � "1"
    unsigned int temp_maska_filter_function[N_BIG] = {0, 0, 0, 0, 0, 0, 0};
    unsigned int temp_activating_functions[N_BIG] = {0, 0, 0, 0, 0, 0, 0};
  
    //������ "����"
    _SET_BIT(temp_maska_filter_function, RANG_SILENCE);
  
    //������ "��������"
    _SET_BIT(temp_maska_filter_function, RANG_RESET);

    //������ "����"
    _SET_BIT(temp_maska_filter_function, RANG_TEST);

    for (unsigned int i = 0; i < N_BIG; i++)
    {
      //� ������ ��������� ����� �������� ����� � �������, ��� ��� �������� ����� ��� � ������� �� � ���������� �����
      temp_activating_functions[i] = previous_activating_functions[i] & temp_maska_filter_function[i];
  
      //� ����������� ����� �������� ����� � �������, � ���� ����������� ��� �������, ��� ����� ������
      temp_activating_functions[i] ^= (active_functions[i] & temp_maska_filter_function[i]);
  
      //����� �������� � ����������� ����� ����� � ������� � ���� ����� �������� ������ ��� "1" (����� ������� ������� � "0" � "1")
      temp_activating_functions[i] &= active_functions[i];

      /*
      ����� ���, �� ������� � �������, �� � ����� ���� ������ ������ �� ������ 
      � ����������� ����, �� ���� ������� ����������� �� ���������� �����
      ������� ��������� ���� ������� (��� ���������� ������) � ���������� �����
      ��� ��� ���������� ����� ���� ��������� ��������� ���� �������� �������
      */ 
      previous_activating_functions[i] = active_functions[i];

      //���������� ����� �������, �� ����� ����������� � ����������� ����, �� ����� �������� ������� ������ ��������� ����� ���� ����� �, � ���� ������� ��� � "0" � "1"
      active_functions[i] = (active_functions[i] & (~temp_maska_filter_function[i])) | temp_activating_functions[i];
    }
  }
  /**************************/

  /**************************/
  //������ �������
  /**************************/
  if (output_timer_meander) _SET_BIT(active_functions, RANG_MEANDER);
  /**************************/
  
  /***********************************************************/
  //���������� ���������
  /***********************************************************/
  calc_measurement();

#ifdef DEBUG_TEST
  /***/
  //ҳ���� ��� �������
  /***/
  if (temp_value_3I0_1 != 0)
    measurement[IM_3I0]         = temp_value_3I0_1;
  if (temp_value_3I0_other != 0)
    measurement[IM_3I0_other_g] = temp_value_3I0_other;
  if (temp_value_IA != 0)
    measurement[IM_IA]          = temp_value_IA;
  if (temp_value_IC != 0)
    measurement[IM_IC]          = temp_value_IC;
  if (temp_value_UA != 0)
    measurement[IM_UA]          = temp_value_UA;
  if (temp_value_UB != 0)
    measurement[IM_UB]          = temp_value_UB;
  if (temp_value_UC != 0)
    measurement[IM_UC]          = temp_value_UC;
  if (temp_value_3U0 != 0)
    measurement[IM_3U0]         = temp_value_3U0;
  if (temp_value_I2 != 0)
    measurement[IM_I2]          = temp_value_I2;
  if (temp_value_I1 != 0)
    measurement[IM_I1]          = temp_value_I1;
  /***/
#endif
    
      
  //ĳ��������� ��������� ��� �� �����
  diagnostyca_adc_execution();
  
  //������� ���������� ��� ����������������� � ����������������� �������
  unsigned int bank_measurement_high_tmp = (bank_measurement_high ^ 0x1) & 0x1;
  if(semaphore_measure_values_low1 == 0)
  {
    for (unsigned int i = 0; i < (NUMBER_ANALOG_CANALES + 8); i++) 
    {
      measurement_high[bank_measurement_high_tmp][i] = measurement_middle[i] = measurement[i];
    }
  }
  else
  {
    for (unsigned int i = 0; i < (NUMBER_ANALOG_CANALES + 8); i++) 
    {
      measurement_high[bank_measurement_high_tmp][i] = measurement[i];
    }
  }
  bank_measurement_high = bank_measurement_high_tmp;
  /***********************************************************/
  
  
  /**************************/
  //������ "������������ �����"
  /**************************/
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
  }
  else
  {
    _CLEAR_BIT(active_functions, RANG_DEFECT);
  }
  /**************************/

  /**************************/
  //������ "������������ ���������"
  /**************************/
  if (
      (_CHECK_SET_BIT(diagnostyka, ERROR_SETTINGS_EEPROM_BIT                     ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_SETTINGS_EEPROM_EMPTY_BIT               ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_SETTINGS_EEPROM_COMPARISON_BIT          ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_SETTINGS_EEPROM_CONTROL_BIT             ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_USTUVANNJA_EEPROM_BIT                   ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_USTUVANNJA_EEPROM_EMPTY_BIT             ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_USTUVANNJA_EEPROM_COMPARISON_BIT        ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_USTUVANNJA_EEPROM_CONTROL_BIT           ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_USTUVANNJA_EEPROM_ADJUSTMENT_ID_FAIL_BIT) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_TRG_FUNC_EEPROM_BIT                     ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_TRG_FUNC_EEPROM_EMPTY_BIT               ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_TRG_FUNC_EEPROM_COMPARISON_BIT          ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_TRG_FUNC_EEPROM_CONTROL_BIT             ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_GND_ADC1_TEST_BIT                       ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_VREF_ADC1_TEST_BIT                      ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_VDD_ADC1_TEST_BIT                       ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_GND_ADC1_TEST_COARSE_BIT                ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_VREF_ADC1_TEST_COARSE_BIT               ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_VDD_ADC1_TEST_COARSE_BIT                ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_GND_ADC2_TEST_BIT                       ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_VREF_ADC2_TEST_BIT                      ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_VDD_ADC2_TEST_BIT                       ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_GND_ADC2_TEST_COARSE_BIT                ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_VREF_ADC2_TEST_COARSE_BIT               ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_VDD_ADC2_TEST_COARSE_BIT                ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_DIGITAL_OUTPUTS_BIT                     ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_INTERNAL_FLASH_BIT                     ) != 0)/* ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_EXTERNAL_SRAM_BIT                       ) != 0)*/
     )   
  {
    _SET_BIT(active_functions, RANG_AVAR_DEFECT);
  }
  else
  {
    _CLEAR_BIT(active_functions, RANG_AVAR_DEFECT);
  }
  /**************************/
  
  //����� ����� ����� ��������� ����� � ���� �������, ���� ��� ������� "��������� �������������"
  if (_CHECK_SET_BIT(active_functions, RANG_AVAR_DEFECT) == 0)
  {
    //������� �������� �� �����������

    /**************************/
    //"�������� ���������"
    /**************************/
    if ((current_settings_prt.configuration & (1 << CTRL_PHASE_BIT_CONFIGURATION)) != 0) 
    {
      ctrl_phase_handler(active_functions);
    } 
    else 
    {
      //������� �������, �� �� ������ ���� � ���� ������������
      const unsigned int maska_ctrl_phase_signals[N_BIG] = 
      {
        MASKA_CTRL_PHASE_SIGNALS_0, 
        MASKA_CTRL_PHASE_SIGNALS_1, 
        MASKA_CTRL_PHASE_SIGNALS_2,
        MASKA_CTRL_PHASE_SIGNALS_3, 
        MASKA_CTRL_PHASE_SIGNALS_4, 
        MASKA_CTRL_PHASE_SIGNALS_5, 
        MASKA_CTRL_PHASE_SIGNALS_6
      };
      for (unsigned int i = 0; i < N_BIG; i++) active_functions[i] &= (unsigned int)(~maska_ctrl_phase_signals[i]);
      
      global_timers[INDEX_TIMER_CTRL_PHASE_U] = -1;
      global_timers[INDEX_TIMER_CTRL_PHASE_U_D] = -1;
      global_timers[INDEX_TIMER_CTRL_PHASE_PHI] = -1;
      global_timers[INDEX_TIMER_CTRL_PHASE_PHI_D] = -1;
      global_timers[INDEX_TIMER_CTRL_PHASE_F] = -1;
      global_timers[INDEX_TIMER_CTRL_PHASE_F_D] = -1;
      global_timers[INDEX_TIMER_CTRL_PHASE_TMP1_100MS] = -1;
      global_timers[INDEX_TIMER_CTRL_PHASE_TMP2_100MS] = -1;
    }
    /**************************/
    
    /**************************/
    //��������� �����
    /**************************/
    unsigned int active_functions_tmp[NUMBER_ITERATION_EL_MAX][N_BIG];
    unsigned int iteration = 0;
	unsigned int repeat_state = false;
    unsigned int df_changed_state_with_start_new_timeout = 0;
    do
    {
      for (unsigned int i = 0; i < iteration; i++)
      {
        if (
            (active_functions_tmp[i][0] == active_functions[0]) &&
            (active_functions_tmp[i][1] == active_functions[1]) &&
            (active_functions_tmp[i][2] == active_functions[2]) &&
            (active_functions_tmp[i][3] == active_functions[3]) &&
            (active_functions_tmp[i][4] == active_functions[4]) &&
            (active_functions_tmp[i][5] == active_functions[5]) &&
            (active_functions_tmp[i][6] == active_functions[6])
           )
        {
          repeat_state = true;
          break;
        }
      }
      if (repeat_state != false ) break;
        
      active_functions_tmp[iteration][0] = active_functions[0];
      active_functions_tmp[iteration][1] = active_functions[1];
      active_functions_tmp[iteration][2] = active_functions[2];
      active_functions_tmp[iteration][3] = active_functions[3];
      active_functions_tmp[iteration][4] = active_functions[4];
      active_functions_tmp[iteration][5] = active_functions[5];
      active_functions_tmp[iteration][6] = active_functions[6];

      d_and_handler(active_functions);
      d_or_handler(active_functions);
      d_xor_handler(active_functions);
      d_not_handler(active_functions);
      df_handler(active_functions, &df_changed_state_with_start_new_timeout);
      dt_handler(active_functions);
        
      iteration++;
    }
    while (
           (iteration < current_settings_prt.number_iteration_el)
           &&
           (
            (active_functions_tmp[iteration - 1][0] != active_functions[0]) ||
            (active_functions_tmp[iteration - 1][1] != active_functions[1]) ||
            (active_functions_tmp[iteration - 1][2] != active_functions[2]) ||
            (active_functions_tmp[iteration - 1][3] != active_functions[3]) ||
            (active_functions_tmp[iteration - 1][4] != active_functions[4]) ||
            (active_functions_tmp[iteration - 1][5] != active_functions[5]) ||
            (active_functions_tmp[iteration - 1][6] != active_functions[6])
           ) 
          );
      
    if (
        (repeat_state != false ) ||
        (iteration >= current_settings_prt.number_iteration_el)
       )
    {
      _SET_BIT(active_functions, RANG_ERROR_CONF_EL);
    }
    else
    {
      _CLEAR_BIT(active_functions, RANG_ERROR_CONF_EL);
    }
    /**************************/
  }
  else
  {
    //������� �������� �����������
    
    //������� �� ������ �������, ��� �������������
    active_functions[0] &= MASKA_INFO_SIGNALES_0;
    active_functions[1] &= MASKA_INFO_SIGNALES_1;
    active_functions[2] &= MASKA_INFO_SIGNALES_2;
    active_functions[3] &= MASKA_INFO_SIGNALES_3;
    active_functions[4] &= MASKA_INFO_SIGNALES_4;
    active_functions[5] &= MASKA_INFO_SIGNALES_5;
    active_functions[6] &= MASKA_INFO_SIGNALES_6;
    
    //������������ �� ����
    state_outputs = 0;
    
    previous_states_ready_tu = 0;
    trigger_ready_tu = 0;
    
    //������� �� �������, �� ������� � �����
    for(unsigned int i = INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START; i < MAX_NUMBER_GLOBAL_TIMERS; i++)
      global_timers[i] = -1;
    
    //���� ��������� �� ���������� � ����������
    for(unsigned int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
    {
      etap_execution_df[i] = NONE_DF;
    }
  }

  /**************************/
  //������� ����������� ����������
  /**************************/
  analog_registrator(active_functions);
  /**************************/

  /**************************/
  //������� ����������� ����������
  /**************************/
  digital_registrator(active_functions);
  /**************************/

  /**************************/
  //������ � ���������, �� ����� ������������ � ��������������� ���'���
  /**************************/
  const unsigned int maska_trg_func_array[N_BIG] = {MASKA_TRIGGER_SIGNALES_0, MASKA_TRIGGER_SIGNALES_1, MASKA_TRIGGER_SIGNALES_2, MASKA_TRIGGER_SIGNALES_3, MASKA_TRIGGER_SIGNALES_4, MASKA_TRIGGER_SIGNALES_5, MASKA_TRIGGER_SIGNALES_6};
  unsigned int comparison_true = true;
  for (unsigned int i = 0; i < N_BIG; i++)
  {
    unsigned int tmp_data;
    if (trigger_active_functions[i] != (tmp_data = (active_functions[i] & maska_trg_func_array[i])))
    {
      comparison_true = false;
      trigger_active_functions[i] = tmp_data;
    }
  }
  if (comparison_true != true)
  {
    /*
    �������, �������� ���� ���������� � ��������������� ���'1��� ��������.
    ������ ������� �� �� ����� � ��������������� ���'���
    */
    _SET_BIT(control_i2c_taskes, TASK_START_WRITE_TRG_FUNC_EEPROM_BIT);
  }
  /**************************/

  /**************************/
  //����������� ��������� � ������ �������� � ��������� �������
  /**************************/
  for (unsigned int i = 0; i < N_BIG; i++)
  {
    unsigned int temp_data = active_functions[i];
    trigger_functions_USB[i]   |= temp_data;
    trigger_functions_RS485[i] |= temp_data;
  }

  copying_active_functions = 0; //�������, �� ���������� �������� �������� ������� ���������
  
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
    
    //���������, �� ���� ����� ����� ���� ����������
    for (unsigned int i = 0; i < NUMBER_OUTPUTS; i++)
    {
      //� ���������� ����� ������� ��ò��� � ���������� ������, ���� ����������� ��������� "i" � �������, �� ����� � ���������
      unsigned int temp_array_of_outputs[N_BIG];
    
      for (unsigned int j = 0; j < N_BIG; j++) temp_array_of_outputs[j] = current_settings_prt.ranguvannja_outputs[N_BIG*i + j] & active_functions[j];

      //������ "������� �����������" ������ � ���������� �����: ������ ���� �� ����� ���������� � �������, ���� ����� ������ �� ��������
      if(_CHECK_SET_BIT((current_settings_prt.ranguvannja_outputs + N_BIG*i), RANG_AVAR_DEFECT) !=0)
      {
        //������ "A�������� �������������"  ������ ���������� �� ����� �����
        if (_CHECK_SET_BIT(temp_array_of_outputs, RANG_AVAR_DEFECT) == 0)
        {
          //������ "A�������� �������������" �� � ��������
          //���������� ������������ ���� � �������� ���� � �����, ���� �  ��ò���� � ��������� ������, ���� ����������� ��������� "i" � �������, �� ����� � ���������
          _SET_BIT(temp_array_of_outputs, RANG_AVAR_DEFECT);
        }
        else
        {
          //������ "A�������� �������������" � ��������
          //���������� ���������� ���� � �������� ���� � �����, ���� �  ��ò���� � ��������� ������, ���� ����������� ��������� "i" � �������, �� ����� � ���������
          _CLEAR_BIT(temp_array_of_outputs, RANG_AVAR_DEFECT);
        }
      }
      
      //������ "�������� �����������" ������ � ���������� �����: ������ ���� �� ����� ���������� � �������, ���� ����� ������ �� ��������
      if(_CHECK_SET_BIT((current_settings_prt.ranguvannja_outputs + N_BIG*i), RANG_DEFECT) !=0)
      {
        //������ "�������� �����������"  ������ ���������� �� ����� �����
        if (_CHECK_SET_BIT(temp_array_of_outputs, RANG_DEFECT) == 0)
        {
          //������ "�������� �����������" �� � ��������
          //���������� ������������ ���� � �������� ���� � �����, ���� �  ��ò���� � ��������� ������, ���� ����������� ��������� "i" � �������, �� ����� � ���������
          _SET_BIT(temp_array_of_outputs, RANG_DEFECT);
        }
        else
        {
          //������ "�������� �����������" � ��������
          //���������� ���������� ���� � �������� ���� � �����, ���� �  ��ò���� � ��������� ������, ���� ����������� ��������� "i" � �������, �� ����� � ���������
          _CLEAR_BIT(temp_array_of_outputs, RANG_DEFECT);
        }
      }

      //����������, �� � ��������� �� ����������� ��������� �� ����� ����� � ���������, �� ����� � ��������� - ����� ��������� ������
      if (
          (temp_array_of_outputs[0] !=0 ) ||
          (temp_array_of_outputs[1] != 0) ||
          (temp_array_of_outputs[2] != 0) ||
          (temp_array_of_outputs[3] != 0) ||
          (temp_array_of_outputs[4] != 0) ||
          (temp_array_of_outputs[5] != 0) ||
          (temp_array_of_outputs[6] != 0)
         )
      {
        //³������, �� ����� ����� - ���������
        state_outputs |= (1 << i);
      }
      else
      {
        //³������, �� ����� ����� - ��ǲ�������
        state_outputs &= ~(1 << i);
      }
    }
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
  
  //���������, �� �������������� ����� ����� ���� ���������
  for (unsigned int i = 0; i < NUMBER_LEDS; i++)
  {
    //� ���������� ����� ������� ���������� ��������������, ���� ����������� ��������� "i"
    unsigned int temp_array_of_leds[N_BIG];
    
    //����������, �� � ��������� �� ����������� ��������� �� ����� ������������� � ���������, �� ����� � ��������� - ����� ��������� ������
    for (unsigned int j = 0; j < N_BIG; j++ ) temp_array_of_leds[j] = current_settings_prt.ranguvannja_leds[N_BIG*i + j] & active_functions[j];

    if (
        (temp_array_of_leds[0] != 0) ||
        (temp_array_of_leds[1] != 0) ||
        (temp_array_of_leds[2] != 0) ||
        (temp_array_of_leds[3] != 0) ||
        (temp_array_of_leds[4] != 0) ||
        (temp_array_of_leds[5] != 0) ||
        (temp_array_of_leds[6] != 0)
       )
    {
      //³������, �� ����� ������������� - ������
      state_leds |= (1 << i);
    }
    else
    {
      //³������, �� ����� ������������� - ���������
      state_leds &= ~(1 << i);
    }
  }
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
    //����������, �� ���������� ���� ���������
    /***********************************************************/
    if (
        (changed_settings == CHANGED_ETAP_ENDED) && /*�� � �����, �� ��� ��� ���������� ��� �������� �� � ������ �������� ������� (� ��� ����� ����� ��� �� ���������)*/
        (state_ar_record  != STATE_AR_START    )    /*�� � �����, �� �� ����� ������ �� ���� ��������� ����������� �� ����������� ������� (� ����� ���������� �� �������� ������� �������) � ������ ������� ���������� ������ ����������� ����������. �� ����� ���� ����� ������ ����������� � ������������� ������*/ 
       )   
    {
      //������� ������� ��������� � ���� ���� ������� ��� � ���� ������ (���� � �����) ����� ������� �������
      current_settings_prt = current_settings;
      
      //�������, �� ���� ������� �������� �������
      changed_settings = CHANGED_ETAP_NONE;
    }
    /***********************************************************/

    /***********************************************************/
    //���������� ����������� ������� ����������� � ����������� ����������
    /***********************************************************/
    //���������� ���������
    if (
        ((clean_rejestrators & CLEAN_AR) != 0 )
        &&  
        (state_ar_record == STATE_AR_NO_RECORD)
        &&  
        (
         (control_tasks_dataflash & (
                                     TASK_MAMORY_PART_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_AR |
                                     TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_AR      |
                                     TASK_MAMORY_READ_DATAFLASH_FOR_AR_USB                         |
                                     TASK_MAMORY_READ_DATAFLASH_FOR_AR_RS485                       |
                                     TASK_MAMORY_READ_DATAFLASH_FOR_AR_MENU
                                    )
         ) == 0
        )   
       )
    {
      //���������� ������� �������� ����������� ����������

      //����������� ������� ������ ���� ��������� � EEPROM
      _SET_BIT(control_i2c_taskes, TASK_START_WRITE_INFO_REJESTRATOR_AR_EEPROM_BIT);
    
      //������� ��������� ���������� �� ����������� ���������
      info_rejestrator_ar.next_address = MIN_ADDRESS_AR_AREA;
      info_rejestrator_ar.saving_execution = 0;
      info_rejestrator_ar.number_records = 0;
    
      //�������, �� ����� ������ �� ��������
      number_record_of_ar_for_menu = 0xffff;
      number_record_of_ar_for_USB = 0xffff;
      number_record_of_ar_for_RS485 = 0xffff;

      //������ ������� ������� ����������� ����������
      clean_rejestrators &= (unsigned int)(~CLEAN_AR);
    }
    
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
    
    //���������� ����������� ������� ��� ����������� ����������
    if (
        (state_ar_record  != STATE_AR_TEMPORARY_BLOCK) &&
        (changed_settings == CHANGED_ETAP_NONE       )  
       )   
    {
      //�������� ��������� ����� ���, ���� �� ��� ���� ������� ���������
      unsigned int size_one_ar_record_tmp = size_one_ar_record, max_number_records_ar_tmp = max_number_records_ar;
      if (
          ((number_word_digital_part_ar*8*sizeof(short int)) < NUMBER_TOTAL_SIGNAL_FOR_RANG)
          ||  
          (size_one_ar_record_tmp != (sizeof(__HEADER_AR) + ((current_settings_prt.prefault_number_periods + current_settings_prt.postfault_number_periods) << VAGA_NUMBER_POINT_AR)*(NUMBER_ANALOG_CANALES + number_word_digital_part_ar)*sizeof(short int)))
          ||
          (
           !(
             (size_one_ar_record_tmp* max_number_records_ar_tmp      <= ((NUMBER_PAGES_INTO_DATAFLASH_2 << VAGA_SIZE_PAGE_DATAFLASH_2))) &&
             (size_one_ar_record_tmp*(max_number_records_ar_tmp + 1) >  ((NUMBER_PAGES_INTO_DATAFLASH_2 << VAGA_SIZE_PAGE_DATAFLASH_2)))
            ) 
          ) 
         )
      {
        //���������� �� ������� ����� �� ����� ������������
        /*���� ������� ���� �������� �� ����� ��������� ���������, ��� ���� ���� �� ���������� - 
        �� �������� ����������� ���������� ������� � �������������� ���������� ����������� ����������.
        �� �������� ���� ���� �������, ���� � ��, ���������� ������ ����������� ������������ �� ��������!*/
        total_error_sw_fixed(5);
      }
    }

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
    if (periodical_tasks_TEST_INFO_REJESTRATOR_AR != 0)
    {
      //����� � ���� ������� ������ ������� ������� ��ﳿ �����
      if ((state_i2c_task & STATE_INFO_REJESTRATOR_AR_EEPROM_GOOD) != 0)
      {
        //������ ���� ����� ���, ���� ��������� ���������� ���������� ������ ������� � ���������� ���������� ����
        if (
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_START_WRITE_INFO_REJESTRATOR_AR_EEPROM_BIT) == 0) &&
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_WRITING_INFO_REJESTRATOR_AR_EEPROM_BIT    ) == 0) &&
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_START_READ_INFO_REJESTRATOR_AR_EEPROM_BIT ) == 0) &&
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_READING_INFO_REJESTRATOR_AR_EEPROM_BIT    ) == 0)
           ) 
        {
          //�� ����� ������� �� ��� �������-����� ��������� ���������� ����������, ���� ����� �������� ���������
          info_rejestrator_ar_ctrl = info_rejestrator_ar;
          crc_info_rejestrator_ar_ctrl = crc_info_rejestrator_ar;

          //������� ������� ������ ���������� �������� ��ﳿ 
          periodical_tasks_TEST_INFO_REJESTRATOR_AR = false;
          //����������� ������� ������ �������� ����������� �� �������� ��ﳿ 
          periodical_tasks_TEST_INFO_REJESTRATOR_AR_LOCK = true;
        }
      }
      else
      {
        //������� ������� ������ ���������� �������� ��ﳿ 
        periodical_tasks_TEST_INFO_REJESTRATOR_AR = false;
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

