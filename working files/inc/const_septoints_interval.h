#ifndef __CONST_SETPOINT_INTERVAL__
#define __CONST_SETPOINT_INTERVAL__

#define SETPOINT_CTRL_PHASE_U_MIN     100
#define SETPOINT_CTRL_PHASE_U_MAX     50000

#define SETPOINT_CTRL_PHASE_PHI_MIN   1         //������ xy <- x.y, ����� 0.1
#define SETPOINT_CTRL_PHASE_PHI_MAX   450       //������ xy <- x.y, ����� 45.0

#define SETPOINT_CTRL_PHASE_F_MIN     10        //������ xyzl <- x.yzl, ����� 0.010
#define SETPOINT_CTRL_PHASE_F_MAX     5000      //������ xyzl <- x.yzl, ����� 5.000

#define TIMEOUT_CTRL_PHASE_U_MIN      0
#define TIMEOUT_CTRL_PHASE_U_MAX      32000

#define TIMEOUT_CTRL_PHASE_U_D_MIN    0
#define TIMEOUT_CTRL_PHASE_U_D_MAX    32000

#define TIMEOUT_CTRL_PHASE_PHI_MIN    0
#define TIMEOUT_CTRL_PHASE_PHI_MAX    32000

#define TIMEOUT_CTRL_PHASE_PHI_D_MIN  0
#define TIMEOUT_CTRL_PHASE_PHI_D_MAX  32000

#define TIMEOUT_CTRL_PHASE_F_MIN      0
#define TIMEOUT_CTRL_PHASE_F_MAX      32000

#define TIMEOUT_CTRL_PHASE_F_D_MIN    0
#define TIMEOUT_CTRL_PHASE_F_D_MAX    32000

#define TIMEOUT_ALARM_SOUND_PERIOD_MIN 1000
#define TIMEOUT_ALARM_SOUND_PERIOD_MAX 30000

#define KOEF_DOPUSK_DV_POST_MIN       0
#define KOEF_DOPUSK_DV_ZMIN_MIN       10
#define KOEF_DOPUSK_DV_MAX            60

#define TIMEOUT_DEACTIVATION_PASSWORD_OFF       0
#define TIMEOUT_DEACTIVATION_PASSWORD_MIN       30
#define TIMEOUT_DEACTIVATION_PASSWORD_MAX       3600

#define TIMEOUT_NEW_SETTINGS_MIN       60
#define TIMEOUT_NEW_SETTINGS_MAX       3600

#define KOEF_ADDRESS_MIN              1
#define KOEF_ADDRESS_MAX              247

#define MAX_NUMBER_BAUD_RS485           7
#define VALUE_BAUD_RS485_MIN            0
#define VALUE_BAUD_RS485_MAX            (MAX_NUMBER_BAUD_RS485 - 1)

#define MAX_NUMBER_PARE_RS485           3
#define VALUE_PARE_RS485_MIN            0
#define VALUE_PARE_RS485_MAX            (MAX_NUMBER_PARE_RS485 - 1)

#define MAX_NUMBER_STOP_BITS_RS485 2
#define VALUE_STOP_BITS_RS485_MIN  0
#define VALUE_STOP_BITS_RS485_MAX  (MAX_NUMBER_STOP_BITS_RS485 - 1)

#define VALUE_TIME_OUT_1_INTERFACE_MIN 15
#define VALUE_TIME_OUT_1_INTERFACE_MAX 694

#define VALUE_SETTING_LANGUAGE_MIN    LANGUAGE_RU
#define VALUE_SETTING_LANGUAGE_MAX    LANGUAGE_EN

#define NUMBER_ITERATION_EL_MIN       1
#define NUMBER_ITERATION_EL_MAX       32

#define NUMBER_DEFINED_FUNCTIONS_MIN  0
#define NUMBER_DEFINED_FUNCTIONS_MAX  NUMBER_DEFINED_FUNCTIONS

#define NUMBER_DEFINED_TRIGGERS_MIN   0
#define NUMBER_DEFINED_TRIGGERS_MAX   NUMBER_DEFINED_TRIGGERS

#define NUMBER_DEFINED_AND_MIN        0
#define NUMBER_DEFINED_AND_MAX        NUMBER_DEFINED_AND

#define NUMBER_DEFINED_OR_MIN         0
#define NUMBER_DEFINED_OR_MAX         NUMBER_DEFINED_OR

#define NUMBER_DEFINED_XOR_MIN        0
#define NUMBER_DEFINED_XOR_MAX        NUMBER_DEFINED_XOR

#define NUMBER_DEFINED_NOT_MIN        0
#define NUMBER_DEFINED_NOT_MAX        NUMBER_DEFINED_NOT

#define TIMEOUT_TIMER_PAUSE_MIN       0
#define TIMEOUT_TIMER_PAUSE_MAX       600000

#define TIMEOUT_TIMER_WORK_MIN        0
#define TIMEOUT_TIMER_WORK_MAX        600000

#define TIMEOUT_MEANDER_PERIOD_MIN    100
#define TIMEOUT_MEANDER_PERIOD_MAX    10000

#define TIMEOUT_PREFAULT_MIN          100
#define TIMEOUT_PREFAULT_MAX          5000

#define TIMEOUT_POSTFAULT_MIN         1000
#define TIMEOUT_POSTFAULT_MAX         25000

#define NUMBER_LINES_FORWARD_MIN      1
#define NUMBER_LINES_BACKWARD_MIN     0
#define NUMBER_LINES_MAX              8

#define SETPOINT_DOVGYNA_VMP_MIN      1
#define SETPOINT_DOVGYNA_VMP_MAX      40000

#define SETPOINT_OPIR_VMP_MIN         1
#define SETPOINT_OPIR_VMP_MAX         20000

#endif
