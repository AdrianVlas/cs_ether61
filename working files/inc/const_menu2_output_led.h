#ifndef __CONST_OUTPUTS_LEDS_MENU2__
#define __CONST_OUTPUTS_LEDS_MENU2__

#define MASKA_CTRL_OUTPUT_LED_M2  ((1 << _MAX_INDEX_CTRL_OUTPUT_LED_BITS_SETTINGS) - 1)

#define NAME_OUTPUT_LED_IN_SIGNALS_RU   \
  "�.��.           ",                   \
  "�����           ",                   \
  "��.���.         ",                   \
  "�1/�2           "

#define NAME_OUTPUT_LED_IN_SIGNALS_UA   \
  "�.��.           ",                   \
  "����.           ",                   \
  "��.���.         ",                   \
  "�1/�2           "

#define NAME_OUTPUT_LED_IN_SIGNALS_EN   \
  "L.In.           ",                   \
  "Reset           ",                   \
  "Blc.Imp         ",                   \
  "F1/F2           "

#define NAME_OUTPUT_LED_IN_SIGNALS_KZ   \
  "�.��.           ",                   \
  "�����           ",                   \
  "��.���.         ",                   \
  "�1/�2           "

    
#define NAME_OUTPUT_LED_OUT_SIGNALS_RU  \
  "�����           "

#define NAME_OUTPUT_LED_OUT_SIGNALS_UA  \
  "�����           "

#define NAME_OUTPUT_LED_OUT_SIGNALS_EN  \
  "Output          "

#define NAME_OUTPUT_LED_OUT_SIGNALS_KZ  \
  "�����           "


//const uint8_t name_string_output_led_control[MAX_NAMBER_LANGUAGE][MAX_INDEX_CTRL_OUTPUT_LED][MAX_COL_LCD + 1] = 
//{
//  {
//    "  ����./����.   ",
//    "   ����./���.   ",
//    " ���-��/���-����",
//    "   ������� 1    ",
//    "   ������� 2    "
//  },
//  {
//    "  ����./���.   ",
//    "   ����./���.   ",
//    " ���-��/���-����",
//    "   ������� 1    ",
//    "   ������� 2    "
//  },
//  {
//    "  Norm./Trig.   ",
//    "  Const./Imp.   ",
//    "  Imp-S/Imp-Ex  ",
//    "   Frequency 1  ",
//    "   Frequency 2  "
//  },
//  {
//    "   ���./����.   ",
//    "   ����./���.   ",
//    " ���-��/���-����",
//    "   ������� 1    ",
//    "   ������� 2    "
//  }
//};
//
//const uint8_t information_output_led_control_N_T[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD + 1] = 
//{
//  {"   ����������   ", "   ����������   "},
//  {"   ����������   ", "   ��ȥ�����    "},
//  {"     NORMAL     ", "    TRIGGER     "},
//  {"   ����������   ", "   ����������   "}
//};
//const uint32_t cursor_x_output_led_control_N_T[MAX_NAMBER_LANGUAGE][2] = 
//{
//  {2, 2},
//  {2, 2},
//  {4, 3},
//  {2, 2}
//};
//
//const uint8_t information_output_led_control_C_I[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD + 1] = 
//{
//  {"   ����������   ", "   ����������   "},
//  {"   ���Ҳ����    ", "   ����������   "},
//  {"    CONSTANT    ", "    IMPULSE     "},
//  {"   ����������   ", "   ����������   "},
//};
//const uint32_t cursor_x_output_led_control_C_I[MAX_NAMBER_LANGUAGE][2] = 
//{
//  {2, 2},
//  {2, 2},
//  {3, 3},
//  {2, 2}
//};
//          
//const uint8_t information_output_led_control_SI_EI[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD + 1] = 
//{
//  {"  ���.�������   ", " ���.�����������"},
//  {"  ���.�������   ", " ���.���������� "},
//  {"   IMP.SIMPLE   ", "  IMP.EXPANDED  "},
//  {"  ���.�������   ", " ���.�����������"},
//};
//const uint32_t cursor_x_output_led_control_SI_EI[MAX_NAMBER_LANGUAGE][2] = 
//{
//  {1, 0},
//  {1, 0},
//  {2, 1},
//  {1, 0}
//};
//
//const uint8_t value_name[MAX_NAMBER_LANGUAGE][MAX_COL_LCD + 1] = 
//{
//  {"���             "},
//  {"���             "},
//  {"MEANDER         "},
//  {"���             "},
//};
//const uint32_t number_symbols_in_name[MAX_NAMBER_LANGUAGE] = {3, 3, 7, 3};
//
//const uint8_t name_output_led_in_signals[MAX_NAMBER_LANGUAGE][OUTPUT_LED_SIGNALS_IN][MAX_COL_LCD + 1] =
//{
//  {NAME_OUTPUT_LED_IN_SIGNALS_RU},  
//  {NAME_OUTPUT_LED_IN_SIGNALS_UA},  
//  {NAME_OUTPUT_LED_IN_SIGNALS_EN},  
//  {NAME_OUTPUT_LED_IN_SIGNALS_KZ}
//};
//  
//const uint8_t name_output_led_out_signals[MAX_NAMBER_LANGUAGE][OUTPUT_LED_SIGNALS_OUT][MAX_COL_LCD + 1] =
//{
//  {NAME_OUTPUT_LED_OUT_SIGNALS_RU},  
//  {NAME_OUTPUT_LED_OUT_SIGNALS_UA},  
//  {NAME_OUTPUT_LED_OUT_SIGNALS_EN},  
//  {NAME_OUTPUT_LED_OUT_SIGNALS_KZ}
//};
//
//const uint8_t name_output[MAX_NAMBER_LANGUAGE][MAX_COL_LCD + 1] = 
//{
//  " �.����         ",
//  " �.����         ",
//  " DO             ",
//  " �.����         "
//};
//const uint32_t first_index_number_output[MAX_NAMBER_LANGUAGE] = {7, 7, 3, 7};
//
//const uint8_t name_led[MAX_NAMBER_LANGUAGE][MAX_COL_LCD + 1] = 
//{
//  " ��.            ",
//  " ��.            ",
//  " LED            ",
//  " ��.            "
//};
//const uint32_t first_index_number_led[MAX_NAMBER_LANGUAGE] = {4, 4, 4, 4};


#endif 

//extern const uint8_t name_string_output_led_control[MAX_NAMBER_LANGUAGE][MAX_INDEX_CTRL_OUTPUT_LED][MAX_COL_LCD + 1];
//extern const uint8_t information_output_led_control_N_T[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD + 1];
//extern const uint32_t cursor_x_output_led_control_N_T[MAX_NAMBER_LANGUAGE][2];
//extern const uint8_t information_output_led_control_C_I[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD + 1];
//extern const uint32_t cursor_x_output_led_control_C_I[MAX_NAMBER_LANGUAGE][2];
//extern const uint8_t information_output_led_control_SI_EI[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD + 1];
//extern const uint32_t cursor_x_output_led_control_SI_EI[MAX_NAMBER_LANGUAGE][2];
//extern const uint8_t value_name[MAX_NAMBER_LANGUAGE][MAX_COL_LCD + 1];
//extern const uint32_t number_symbols_in_name[MAX_NAMBER_LANGUAGE];
//extern const uint8_t name_output_led_in_signals[MAX_NAMBER_LANGUAGE][OUTPUT_LED_SIGNALS_IN][MAX_COL_LCD + 1];
//extern const uint8_t name_output_led_out_signals[MAX_NAMBER_LANGUAGE][OUTPUT_LED_SIGNALS_OUT][MAX_COL_LCD + 1];
//extern const uint8_t name_output[MAX_NAMBER_LANGUAGE][MAX_COL_LCD + 1];
//extern const uint32_t first_index_number_output[MAX_NAMBER_LANGUAGE];
//extern const uint8_t name_led[MAX_NAMBER_LANGUAGE][MAX_COL_LCD + 1]; 
//extern const uint32_t first_index_number_led[MAX_NAMBER_LANGUAGE];

  
