#ifndef __TYPE_DEFINITION__
#define __TYPE_DEFINITION__

#define SRAM1 _Pragma("location=\"variables_RAM1\"")

typedef struct
{
  int current_level;                  //������� ����� �������������� ������
 
  int index_position;                 //����� �� ��� �� ����� � �������

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

typedef struct
{
  uint32_t device_id;                   //��� ��������
  
  uint32_t n_alarms;                    //ʳ������ ����� �����������
  uint32_t n_meanders;                  //ʳ������ ���������� �������

  uint32_t n_timers;                    //ʳ������ �������
  uint32_t n_triggers;                  //ʳ������ ������
  uint32_t n_and;                       //ʳ������ �������� "�"
  uint32_t n_or;                        //ʳ������ �������� "���"
  uint32_t n_xor;                       //ʳ������ �������� "����.���"
  uint32_t n_not;                       //ʳ������ �������� "��"
  
} __CONFIG;

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
} __SETTINGS;

typedef struct
{
  //���� �����
  int state_execution;        //-1 - ������� ������� �� ������� ����� ����������
                              // 0 - ���������� ����
                              // 1 - ���� ���������� ��� �������
                              // 2 - ���� ���������� � ���������
  
  
  //������������� ��������, � ���� ������������
  int device_id;              //-1 - ������� �� ����������
                              // EEPROM_ADDRESS - EEPROM
                              // RTC_ADDRESS - RTC
  //��� ��������
  int action;                 //-1 - �� ����������
                              // 0 - ����� ������ ��� ���������� ����������
                              // 1 - ����������
                              // 2 - ����� ������ � ����� �����
  

  //�������� ������ �� ��� ���� ��� ���������
  unsigned int internal_address;

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
