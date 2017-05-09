#ifndef __CONST_CHOOSE_COMMUNICATION_PARAMETERS_MENU2__
#define __CONST_CHOOSE_COMMUNICATION_PARAMETERS_MENU2__

enum __index_choose_comunication_parameters
{
  INDEX_CHCP_M2_NAME = 0,
  INDEX_CHCP_M2_ADDRESS,
  INDEX_CHCP_M2_SETTING_RS485,
  
  MAX_ROW_CHCP_M2
};

#define MAX_ROW_FOR_VIEW_NAME_OF_CELL                (MAX_CHAR_IN_NAME_OF_CELL >> POWER_MAX_COL_LCD)

#define MAX_ROW_ADDRESS_M2              1

#define COL_ADDRESS_BEGIN                6
#define COL_ADDRESS_END                  8

enum __index_choose_settings_RS485
{
  INDEX_SETTING_RS485_M2_BAUD = 0,
  INDEX_SETTING_RS485_M2_PARE,
  INDEX_SETTING_RS485_M2_STOP,
  INDEX_SETTING_RS485_M2_TIMEOUT,
  
  MAX_ROW_SETTING_RS485_M2
};

#define MAX_ROW_FOR_VIEW_BAUD_RS485                     1
#define MAX_ROW_FOR_VIEW_PARE_RS485                     1
#define MAX_ROW_FOR_VIEW_STOP_BITS_RS485                1
#define MAX_ROW_FOR_VIEW_TIMEOUT_RS485                  1


#define COL_TIMEOUT_RS485_BEGIN    3
#define COL_TIMEOUT_RS485_COMMA    5
#define COL_TIMEOUT_RS485_END      6

//const uint8_t name_string_choose_communication_parameters[MAX_NAMBER_LANGUAGE][MAX_ROW_CHCP_M2][MAX_COL_LCD + 1] = 
//{
//  {
//    " ��� ������     ",
//    " ����� ������   ",
//    " �����.RS-485   "
//  },
//  {
//    " ��'� ������   ",
//    " ������ ������ ",
//    " ������.RS-485  "
//  },
//  {
//    " Bay name       ",
//    " Bay Address    ",
//    " RS-485 Settings"
//  },
//  {
//    " ��� ������     ",
//    " ����� ������   ",
//    " �����.RS-485   "
//  }
//};
//
//const uint8_t name_string_choose_communication_parameters_address[MAX_NAMBER_LANGUAGE][MAX_ROW_ADDRESS_M2][MAX_COL_LCD + 1] = 
//{
//  "     �����      ",
//  "     ������     ",
//  "    Address     ",
//  "     �����      "
//};
//
//const uint8_t name_string_choose_communication_parameters_RS485[MAX_NAMBER_LANGUAGE][MAX_ROW_SETTING_RS485_M2][MAX_COL_LCD + 1] = 
//{
//  {
//    " �������� ������",
//    " �����.�������� ",
//    " �����.����-��� ",
//    " ����� �����   "
//  },
//  {
//    " �������� ���. ",
//    " �����.������� ",
//    "ʳ���.����-��  ",
//    "ʳ���� ���������"
//  },
//  {
//    " Baud rate      ",
//    " Parity         ",
//    " Stop Bits      ",
//    "End of Reception"
//  },
//  {
//    " �������� ������",
//    " �����.�������� ",
//    " �����.����-��� ",
//    " ����� �����   "
//  }
//};
//
//const uint8_t name_string_choose_communication_parameters_speed[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_VIEW_BAUD_RS485][MAX_COL_LCD + 1] = 
//{
//  " �������� ������",
//  " �������� ���. ",
//  "   Baud rate    ",
//  " �������� ������"
//};
//const uint8_t information_speed[MAX_NUMBER_BAUD_RS485][MAX_COL_LCD + 1] = 
//{
//  "      9600      ",
//  "     14400      ",
//  "     19200      ",
//  "     28800      ",
//  "     38400      ",
//  "     57600      ",
//  "     115200     "
//};
//
//const uint8_t name_stringe_communication_parameters_pare[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_VIEW_PARE_RS485][MAX_COL_LCD + 1] = 
//{
//  " �����.�������� ",
//  " �����.������� ",
//  "     Parity     ",
//  " �����.�������� "
//};
//const uint32_t cursor_x_communication_parameters_pare[MAX_NUMBER_BAUD_RS485] = {5, 4, 4, 4, 4, 4, 4};
//const uint8_t information_pare[MAX_NAMBER_LANGUAGE][MAX_NUMBER_PARE_RS485][MAX_COL_LCD + 1] = 
//{
//  {"      ���       ", "     �����.     ", "      ���.      "},
//  {"      ����      ", "    ������.     ", "      ����.     "},
//  {"      None      ", "      Odd       ", "      Even      "},
//  {"      ���       ", "     �����.     ", "      ���.      "}
//};
//const uint32_t cursor_x_pare[MAX_NAMBER_LANGUAGE][MAX_NUMBER_PARE_RS485 + 1] = 
//{
//  {5, 4, 5, 4},
//  {5, 3, 5, 3},
//  {5, 5, 5, 4},
//  {5, 4, 5, 4}
//};
//
//const uint8_t name_stringe_communication_parameters_stopbits[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_VIEW_STOP_BITS_RS485][MAX_COL_LCD + 1] = 
//{
//  " �����.����-��� ",
//  "ʳ���.����-��  ",
//  "   Stop Bits    ",
//  " �����.����-��� "
//};
//const uint8_t information_stopbits[MAX_NAMBER_LANGUAGE][MAX_NUMBER_STOP_BITS_RS485][MAX_COL_LCD + 1] = 
//{
//  {"      ����      ", "      ���       "},
//  {"      ����      ", "      ���       "},
//  {"      One       ", "      Two       "},
//  {"      ����      ", "      ���       "}
//};
//const uint32_t cursor_x_stopbits[MAX_NAMBER_LANGUAGE][MAX_NUMBER_STOP_BITS_RS485] = 
//{
//  {5, 5},
//  {5, 5},
//  {5, 5},
//  {5, 5}
//};
//
//const uint8_t name_stringe_communication_parameters_timeout[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_VIEW_TIMEOUT_RS485][MAX_COL_LCD + 1] = 
//{
//  "  ����� �����  ",
//  "ʳ���� ���������",
//  "End of Reception",
//  "  ����� �����  "
//};


#endif

//extern const uint8_t name_string_choose_communication_parameters[MAX_NAMBER_LANGUAGE][MAX_ROW_CHCP_M2][MAX_COL_LCD + 1];
//extern const uint8_t name_string_choose_communication_parameters_address[MAX_NAMBER_LANGUAGE][MAX_ROW_ADDRESS_M2][MAX_COL_LCD + 1];
//extern const uint8_t name_string_choose_communication_parameters_RS485[MAX_NAMBER_LANGUAGE][MAX_ROW_SETTING_RS485_M2][MAX_COL_LCD + 1];
//extern const uint8_t name_string_choose_communication_parameters_speed[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_VIEW_BAUD_RS485][MAX_COL_LCD + 1];
//extern const uint8_t information_speed[MAX_NUMBER_BAUD_RS485][MAX_COL_LCD + 1];
//extern const uint8_t name_stringe_communication_parameters_pare[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_VIEW_PARE_RS485][MAX_COL_LCD + 1];
//extern const uint32_t cursor_x_communication_parameters_pare[MAX_NUMBER_BAUD_RS485];
//extern const uint8_t information_pare[MAX_NAMBER_LANGUAGE][MAX_NUMBER_PARE_RS485][MAX_COL_LCD + 1];
//extern const uint32_t cursor_x_pare[MAX_NAMBER_LANGUAGE][MAX_NUMBER_PARE_RS485 + 1];
//extern const uint8_t name_stringe_communication_parameters_stopbits[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_VIEW_STOP_BITS_RS485][MAX_COL_LCD + 1];
//extern const uint8_t information_stopbits[MAX_NAMBER_LANGUAGE][MAX_NUMBER_STOP_BITS_RS485][MAX_COL_LCD + 1];
//extern const uint32_t cursor_x_stopbits[MAX_NAMBER_LANGUAGE][MAX_NUMBER_STOP_BITS_RS485];
//extern const uint8_t name_stringe_communication_parameters_timeout[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_VIEW_TIMEOUT_RS485][MAX_COL_LCD + 1];

