#ifndef __TYPE_DEFINITION__
#define __TYPE_DEFINITION__

#define SRAM1 _Pragma("location=\"variables_RAM1\"")

enum _edition_stats
{
  ED_VIEWING = 0,       /* 0 - ���� ����� �� � ����� ����������  */
  ED_CAN_BE_EDITED,     /* 1 - ���� ����� �� � ����� ���������� ��� � ������ �������� � ����� ����������� ��� ������� �������� ������  */
  ED_EDITION,           /* 2 - ���� ����� � ����� �����������  */
  ED_CONFIRM_CHANGES,   /* 3 - ���� ���� ������������ �����������  */ 
  ED_WARNING,           /* 4 - � ������ ��������� ��������� �������  */
  ED_INFO,              /* 5 - � ������ ��������� ��������� ����������� ������������� �� ����� ����� �����������  */
  ED_ERROR              /* 6 - � ������ ��������� ��������� �������� �������  */
};

typedef struct
{
  enum _menu2_levels current_level;   //������� ����� �������������� ������
  
  int16_t index_position;             //����� �� ��� �� ����� � �������

  int16_t position_cursor_x;          //X - ���������� �������
  int16_t position_cursor_y;          //Y - ���������� �������
  
  int *p_max_row;                     //�������� �� ����������� ������� �����
  int max_row;                        //����������� ������� �����
  
  void (*func_move)(unsigned int, int);//�������� �� �������, ��� ���������� �� ��������� ����  
  void (*func_show)(void);            //�������� �� �������, ��� ����������� ��������� ����  
  unsigned int (*func_press_enter)(void);//�������� �� �������, ��� ��������� 䳿 �� ����������� ������ Enter
  void (*func_change)(unsigned int);  //�������� �� �������, ��� ���� �������� ������� ������� � ��������� ����
  
  enum _edition_stats edition;
  
  unsigned int cursor_on;             //1 - ������ ������������/ 0 - ������ �� ������������
  unsigned int cursor_blinking_on;    //1 - ������ ����/ 0 - ������ �� ����
  
  unsigned int current_action;        //ĳ� ��� ����� �������� � ������� �������
                                      //0 - ͳ���� �� ����������
                                      //1 - ���������� ������, �/��� ������ ���� ���� �����������
                                      //2 - ������� �������� ����������
}__CURRENT_STATE_MENU2;

typedef struct
{
  int16_t current_level;              //������� ����� �������������� ������
 
  int16_t index_position;             //����� �� ��� �� ����� � �������

  int position_cursor_x;              //X - ���������� �������
  int position_cursor_y;              //Y - ���������� �������
  
  unsigned int edition;               //0 - ���� ����� �� � ����� ����������;
                                      //1 - ���� ����� � ����� �����������;
                                      //2 - ���� ���� ������������ �����������
                                      //3 - � ��� �������� ����������� ��� ��, �� �� ��� �������� ����� ���� ����������� ������� �������������
  
  unsigned int cursor_on;             //1 - ������ ������������/ 0 - ������ �� ������������
  unsigned int cursor_blinking_on;    //1 - ������ ����/ 0 - ������ �� ����
  
  unsigned int current_action;        //ĳ� ��� ����� �������� � ������� �������
                                      //0 - ͳ���� �� ����������
                                      //1 - ���������� ������, �/��� ������ ���� ���� �����������
                                      //2 - ������� �������� ����������

} __CURRENT_EKRAN;

typedef struct
{
  int position_cursor_x;              //X - ���������� �������

  unsigned int cursor_on;             //1 - ������ ������������/ 0 - ������ �� ������������
  unsigned int cursor_blinking_on;    //1 - ������ ����/ 0 - ������ �� ����
} __PREVIOUS_STATE_CURSOR;

/**********
���������� ����
**********/
typedef struct
{
  int32_t delay;
  
} __delays_for_INPUT;

typedef struct
{
  
  __delays_for_INPUT delay;
  uint32_t control;
  
} __settings_for_INPUT;

typedef struct
{
  __settings_for_INPUT settings;
  
  __delays_for_INPUT delay;
  uint8_t active_state[DIV_TO_HIGHER(INPUT_SIGNALS, 8)];

} __LN_INPUT;
/**********/

/**********
���������� ���� (����)
**********/
typedef struct
{
  
  uint32_t control;
  uint32_t param;
  
} __settings_for_OUTPUT;

typedef struct
{
  __settings_for_OUTPUT settings;

  uint8_t active_state[DIV_TO_HIGHER(OUTPUT_SIGNALS, 8)];

} __LN_OUTPUT;
/**********/

/**********
�������������
**********/
typedef struct
{
  
  uint32_t control;
  uint32_t param;
  
} __settings_for_LED;

typedef struct
{
  __settings_for_LED settings;

  uint8_t active_state[DIV_TO_HIGHER(LED_SIGNALS, 8)];

} __LN_LED;
/**********/

/**********
������� ���������� ����� "�"
**********/
typedef struct
{
  
  uint32_t param[NUMBER_IN_AND];
  
} __settings_for_AND;

typedef struct
{
  __settings_for_AND settings;

  uint8_t active_state[DIV_TO_HIGHER(AND_SIGNALS, 8)];
  uint8_t trigger_state[DIV_TO_HIGHER(AND_SIGNALS, 8)];

} __LN_AND;
/**********/

/**********
������� ���������� ����� "���"
**********/
typedef struct
{
  
  uint32_t param[NUMBER_IN_OR];
  
} __settings_for_OR;

typedef struct
{
  
  __settings_for_OR settings;

  uint8_t active_state[DIV_TO_HIGHER(OR_SIGNALS, 8)];
  uint8_t trigger_state[DIV_TO_HIGHER(OR_SIGNALS, 8)];
  
} __LN_OR;
/**********/

/**********
������� ���������� ����� "����.���"
**********/
typedef struct
{
  
  uint32_t param[2];
  
} __settings_for_XOR;

typedef struct
{
  __settings_for_XOR settings;
  
  uint8_t active_state[DIV_TO_HIGHER(XOR_SIGNALS, 8)];
  uint8_t trigger_state[DIV_TO_HIGHER(XOR_SIGNALS, 8)];
  
} __LN_XOR;
/**********/

/**********
������� ���������� ����� "��"
**********/
typedef struct
{
  
  uint32_t param;
  
} __settings_for_NOT;

typedef struct
{
  
  __settings_for_NOT settings;
  
  uint8_t active_state[DIV_TO_HIGHER(NOT_SIGNALS, 8)];
  uint8_t trigger_state[DIV_TO_HIGHER(NOT_SIGNALS, 8)];
  
} __LN_NOT;
/**********/

/**********
�������������������� ������
**********/
typedef struct
{
  int32_t delay_pause;
  int32_t delay_work;
  
} __delays_for_TIMER;

typedef struct
{
  
  uint32_t param;
  uint32_t control;
  __delays_for_TIMER delay;
  
} __settings_for_TIMER;

typedef struct
{
  
  __settings_for_TIMER settings;
  
  __delays_for_TIMER delay;
  uint8_t active_state[DIV_TO_HIGHER(TIMER_SIGNALS, 8)];
  uint8_t trigger_state[DIV_TO_HIGHER(TIMER_SIGNALS, 8)];
  
} __LN_TIMER;
/**********/

/**********
�����
**********/
typedef struct
{
  
  uint32_t set_param;
  uint32_t reset_param;
  uint32_t D_param; 
  uint32_t C_param; 
  
} __settings_for_TRIGGER;

typedef struct
{
  
  __settings_for_TRIGGER settings;
  
  uint8_t active_state[DIV_TO_HIGHER(TRIGGER_SIGNALS, 8)];
  uint8_t trigger_state[DIV_TO_HIGHER(TRIGGER_SIGNALS, 8)];
  
} __LN_TRIGGER;
/**********/

/**********
�������������� ���� "��������� ����������� �������"
**********/
typedef struct
{
  int32_t delay;
  
} __delays_for_MEANDER;

typedef struct
{
  
  __delays_for_MEANDER delay;
  
} __settings_for_MEANDER;

typedef struct
{
  
  __settings_for_MEANDER settings;
  
  __delays_for_MEANDER delay;
  uint8_t active_state[DIV_TO_HIGHER(MEANDER_SIGNALS, 8)];
  uint8_t trigger_state[DIV_TO_HIGHER(MEANDER_SIGNALS, 8)];
  
} __LN_MEANDER;
/**********/

typedef enum _action_dyn_mem_select
{
  MAKE_DYN_MEM = 0,
  REMAKR_DYN_MEM,
  RESTORE_DYN_MEM
    
} __action_dym_mem_select;

typedef enum _result_dyn_mem_select
{
  DYN_MEM_TOTAL_ERROR = 0,
  DYN_MEM_NO_ENOUGH_MEM,
  DYN_MEM_SELECT_OK
    
} __result_dym_mem_select;

/**********
������������
**********/
typedef struct
{
  uint32_t device_id;                   //��� ��������
  
  uint32_t n_input;                     //ʳ������ ���������� �����
  uint32_t n_output;                    //ʳ������ ���������� ������
  uint32_t n_led;                       //ʳ������ ���������� ��������������
  
  uint32_t n_and;                       //ʳ������ �������� "�"
  uint32_t n_or;                        //ʳ������ �������� "���"
  uint32_t n_xor;                       //ʳ������ �������� "����.���"
  uint32_t n_not;                       //ʳ������ �������� "��"
  uint32_t n_timer;                     //ʳ������ �������
  uint32_t n_trigger;                   //ʳ������ ������

  uint32_t n_meander;                   //ʳ������ ���������� �������
  uint32_t n_alarm;                     //ʳ������ ����� �����������
  uint32_t n_ctrl_analog_inputs;        //����./����. �������� �������� ������


  uint8_t time_config[7+1];       //��� ������� ��� �������-��������-���������
                                        //������� ���� ������ �������� ���� ����� ���� ���� ��������
                                        //0 - ������� ���������
                                        //1 - ���������
                                        //2 - USB
                                        //3 - RS-485
  
} __CONFIG;
/**********/

/**********
������������, �� ������ ������� � ������ ��� ��������
**********/
typedef struct
{
  //����
  uint32_t ranguvannja_silence;                                 //���������� c����������
  //��������
  uint32_t ranguvannja_reset;                                   //���������� c����������
  //����
  uint32_t ranguvannja_test;                                    //���������� �����
  
  uint32_t password_1;                                          //������ ��� ����������� � ����
  uint32_t password_2;                                          //������ ��� ����������� � ����
  uint32_t timeout_deactivation_password_interface_USB;         //��� ����������� ������ ��� ����������� � ���������� USB
  uint32_t password_interface_USB;                              //������ ��� ����������� � ���������� USB
  uint32_t timeout_deactivation_password_interface_RS485;       //��� ����������� ������ ��� ����������� � ���������� RS485
  uint32_t password_interface_RS485;                             //������ ��� ����������� � ���������� RS485
  
  uint32_t timeout_idle_new_settings;
  
  //����������
  uint32_t name_of_cell[MAX_CHAR_IN_NAME_OF_CELL];              //�'�� ������
  uint32_t volatile address;                                    //������
  int32_t speed_RS485;                                          //�������� �����
                                                                // 0 - 9600
                                                                // 1 - 14400
                                                                // 2 - 19200
                                                                // 3 - 28800
                                                                // 4 - 38400
                                                                // 5 - 57600
                                                                // 6 - 115200
  int32_t pare_bit_RS485;                                       //�������
                                                                // 0 - NONE
                                                                // 1 - ODD
                                                                // 2 - EVEN
  int32_t number_stop_bit_RS485;                                //������� ����-��
                                                                // 0 - 1 stop-bit
                                                                // 1 - 2 stop-bits
  uint32_t time_out_1_RS485;                                    //time-out ���������� ������� = X/10 �������
  
  int32_t language;                                             //���� ����  0= ����� ��� �� �����������; 1=RU; 2=UA; 3=EN; 4=KZ; 5=��.
  
  unsigned char time_setpoints[7+1];                            //��� ������� ��� �������-��������-���������
                                                                //������� ���� ������ �������� ���� ����� ���� ���� ��������
                                                                //0 - ������� ���������
                                                                //1 - ���������
                                                                //2 - USB
                                                                //3 - RS-485
  
} __SETTINGS_FIX;
/**********/

typedef struct
{
  //��� ��������
  unsigned int device_id;
  
  //�������� �����
  unsigned int type_of_input_signal;                   //��� ������� ����������� ����� 0 - �������� , 1 - ������
  unsigned int dopusk_dv[NUMBER_INPUTS];              //������� ��
    
  unsigned int ranguvannja_outputs[N_BIG*NUMBER_OUTPUTS]; //���������� ���������� ������
  unsigned int ranguvannja_leds[N_BIG*NUMBER_LEDS];       //���������� �������������

  unsigned int ranguvannja_analog_registrator[N_BIG];     //���������� ����������� ����������
  unsigned int ranguvannja_digital_registrator[N_BIG];    //���������� ����������� ����������
  
  //���� �����������
  unsigned int ranguvannja_alarms[N_BIG*NUMBER_ALARMS];   //���������� c����������
  //����
  unsigned int ranguvannja_silence[N_BIG];      //���������� c����������
  //��������
  unsigned int ranguvannja_reset[N_BIG];        //���������� c����������
  //����
  unsigned int ranguvannja_test;                //���������� �����
  
  unsigned int number_iteration_el;                                             //����������� ������� �������� ��� ��������� �����
  unsigned int number_defined_df;                                               //ʳ������ ������������ ������� � ������������ �������
  unsigned int number_defined_dt;                                               //ʳ������ ������������ ������� � ������������ �������
  unsigned int number_defined_and;                                              //ʳ������ ������������ "�" � ������������ �������
  unsigned int number_defined_or;                                               //ʳ������ ������������ "���" � ������������ �������
  unsigned int number_defined_xor;                                              //ʳ������ ������������ "����.���" � ������������ �������
  unsigned int number_defined_not;                                              //ʳ������ ������������ "��" � ������������ �������

  //������������ �������
  unsigned int type_df;                                                 //��� ������������� �������
                                                                        //0 - �����
                                                                        //1 - ��������
  unsigned int timeout_pause_df[NUMBER_DEFINED_FUNCTIONS];              //��� �������� ���������� ������������� �������
  unsigned int timeout_work_df[NUMBER_DEFINED_FUNCTIONS];               //��� ������ ������������� �������
  unsigned int ranguvannja_df[N_BIG*NUMBER_DEFINED_FUNCTIONS];          //���������� ������ ������������� �������

  unsigned int ranguvannja_set_dt[N_BIG*NUMBER_DEFINED_TRIGGERS];       //���������� ������ ������ ������������ �������������� ������
  unsigned int ranguvannja_reset_dt[N_BIG*NUMBER_DEFINED_TRIGGERS];     //���������� ������ ������ �������� �������������� ������

  unsigned int ranguvannja_d_and[N_BIG*NUMBER_DEFINED_AND];                     //���������� ������ ������ ������������ "�"
  unsigned int ranguvannja_d_or[N_BIG*NUMBER_DEFINED_OR];                       //���������� ������ ������ ������������ "���"
  unsigned int ranguvannja_d_xor[N_BIG*NUMBER_DEFINED_XOR];                     //���������� ������ ������ ������������ "����.���"
  unsigned int ranguvannja_d_not[N_BIG*NUMBER_DEFINED_NOT];                     //���������� ������ ������ ������������ "��"
  
  unsigned int configuration;         //������������ �������
  
  //�������� ���������
  unsigned int setpoint_ctrl_phase_U;                                           //������� U���
  unsigned int setpoint_ctrl_phase_phi;                                         //������� PHI���
  unsigned int setpoint_ctrl_phase_f;                                           //������� F���
  int timeout_ctrl_phase_U;                                                     //�������� U���
  int timeout_ctrl_phase_U_d;                                                   //�������� U��� ����������
  int timeout_ctrl_phase_phi;                                                   //�������� PHI���
  int timeout_ctrl_phase_phi_d;                                                 //�������� PHI��� ����������
  int timeout_ctrl_phase_f;                                                     //�������� F���
  int timeout_ctrl_phase_f_d;                                                   //�������� F��� ����������
  unsigned int control_ctrl_phase;

  unsigned int password;                                        //������ ��� ����������� � ����
  unsigned int timeout_deactivation_password_interface_USB;     //��� ����������� ������ ��� ����������� � ���������� USB
  unsigned int password_interface_USB;                          //������ ��� ����������� � ���������� USB
  unsigned int timeout_deactivation_password_interface_RS485;   //��� ����������� ������ ��� ����������� � ���������� RS485
  unsigned int password_interface_RS485;                        //������ ��� ����������� � ���������� RS485
  
  unsigned int timeout_idle_new_settings;
  
  //�����������
  int timeout_alarm_sound_period[NUMBER_ALARMS];        //�������� "��������� ��������� �������"
  unsigned int control_alarm;                           //���� ��� ��������� �������������
  
  //���������� ���������
  unsigned int prefault_number_periods; //��� ����������� ������ (������� ������ ���������� �������)
  unsigned int postfault_number_periods;//��� ����������� ������ (������� ������ ���������� �������)

  //����������
  unsigned int name_of_cell[MAX_CHAR_IN_NAME_OF_CELL];//�'�� ������
  unsigned short int user_register[(M_ADDRESS_LAST_USER_REGISTER_DATA - M_ADDRESS_FIRST_USER_REGISTER_DATA) + 1]; //������� �����������
  unsigned int volatile address;                      //������
  int speed_RS485;                                    //�������� �����
                                                        // 0 - 9600
                                                        // 1 - 14400
                                                        // 2 - 19200
                                                        // 3 - 28800
                                                        // 4 - 38400
                                                        // 5 - 57600
                                                        // 6 - 115200
  int pare_bit_RS485;                                 //�������
                                                        // 0 - NONE
                                                        // 1 - ODD
                                                        // 2 - EVEN
  int number_stop_bit_RS485;                          //������� ����-��
                                                        // 0 - 1 stop-bit
                                                        // 1 - 2 stop-bits
  unsigned int time_out_1_RS485;                        //time-out ���������� ������� = X/10 �������
  
  int language;                                         //���� ����  0= ����� ��� �� �����������; 1=RU; 2=UA; 3=EN; 4=KZ; 5=��.
  
  unsigned char time_setpoints[7+1];                     //��� ������� ��� �������-��������-���������
                                                         //������� ���� ������ �������� ���� ����� ���� ���� ��������
                                                            //0 - ������� ���������
                                                            //1 - ���������
                                                            //2 - USB
                                                            //3 - RS-485
  
  unsigned char time_ranguvannja[7+1];                    //��� ������� ��� ����������
                                                            //0 - ������� ���������
                                                            //1 - ���������
                                                            //2 - USB
                                                            //3 - RS-485
} __SETTINGS_OLD;

typedef struct
{
  //���� �����
  int16_t state_execution;        //-1 - ������� ������� �� ������� ����� ����������
                              // 0 - ���������� ����
                              // 1 - ���� ���������� ��� �������
                              // 2 - ���� ���������� � ���������
  
  
  //������������� ��������, � ���� ������������
  int16_t device_id;              //-1 - ������� �� ����������
                              // EEPROM_ADDRESS - EEPROM
                              // RTC_ADDRESS - RTC
  //��� ��������
  int16_t action;                 //-1 - �� ����������
                              // 0 - ����� ������ ��� ���������� ����������
                              // 1 - ����������
                              // 2 - ����� ������ � ����� �����
  

  //�������� ������ �� ��� ���� ��� ���������
  uint16_t internal_address;

  //ʳ������ ���� ��� �������/������ (��������)
  unsigned int number_bytes;
  
  //�������� �� ����� (��������)
  uint8_t volatile* point_buffer;

} __DRIVER_I2C;

typedef struct
{
  unsigned int next_address;
  unsigned int saving_execution;
  unsigned int number_records;
} __INFO_REJESTRATOR;

typedef struct
{
  unsigned int state_execution;     //���� ���������� �������������
  
  unsigned int code_operation;      //��� ���������� ��������

} __DRIVER_SPI_DF;

typedef struct
{
  unsigned char label_start_record;
  unsigned char time[7]; 
  unsigned char name_of_cell[MAX_CHAR_IN_NAME_OF_CELL];
} __HEADER_AR;

typedef enum __STATE_READING_ADCs {
STATE_READING_ADCs_NONE = 0,
STATE_READING_WRITE,
STATE_READING_WRITE_READ,
STATE_READING_READ
} STATE_READING_ADCs;

typedef struct
{
  uint32_t tick;
  unsigned int value;

} EXTENDED_OUTPUT_DATA;

typedef struct
{
  uint32_t tick;
  int value;
} EXTENDED_SAMPLE;

typedef struct
{
  unsigned int present;
  int start_index;
  int stop_index;
  int number_per_index;
  int real_number;
} EL_FILTER_STRUCT;

#endif
