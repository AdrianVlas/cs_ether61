#ifndef __CONST_MAIN_MENU2__
#define __CONST_MAIN_MENU2__

enum __index_main_m2
{
INDEX_MAIN_M2_TIME = 0,
INDEX_MAIN_M2_MEASURMENTS,
INDEX_MAIN_M2_INPUTS_OUTPUTS,
INDEX_MAIN_M2_REGISTRATORS,
INDEX_MAIN_M2_SETTINGS,
INDEX_MAIN_M2_DIAGNOSTYKA,
INDEX_MAIN_M2_SETTINGS_POINT,
INDEX_MAIN_M2_INFO,

MAX_ROW_MAIN_M2
};

//const uint8_t name_string_main[MAX_NAMBER_LANGUAGE][MAX_ROW_MAIN_M2][MAX_COL_LCD + 1] = 
//{
//  {
//   " ����           ",
//   " ���������      ",
//   " �����-������   ",
//   " ������������   ",
//   " ���������      ",
//   " �����������    ",
//   " ����� �������� ",
//   " ������ �� � �� "
//  },
//  {
//   " ��������       ",
//   " ����������    ",
//   " �����-������   ",
//   " ����������    ",
//   " ������������   ",
//   " ĳ���������    ",
//   " ̳��� ������.  ",
//   " ����� �� � �� "
//  },
//  {
//   " Clock          ",
//   " Measurement    ",
//   " Inputs-Outputs ",
//   " Recorders      ",
//   " Settings       ",
//   " Diagnostic     ",
//   " Settings Mark  ",
//   " VER.of F/W & MM"
//  },
//  {
//   " ��f��          ",
//   " �����          ",
//   " ʳ��-��f��    ",
//   " ҳ��������    ",
//   " �������       ",
//   " �����������    ",
//   " ����� �������� ",
//   " ������ �� � �� "
//  }
//};

#endif

extern const uint8_t name_string_main[MAX_NAMBER_LANGUAGE][MAX_ROW_MAIN_M2][MAX_COL_LCD + 1];
