#ifndef __CONST_SETTINGS__
#define __CONST_SETTINGS__

#define NUMBER_IN_AND                   8
#define NUMBER_IN_OR                    8

enum _control_array_index
{
  CA_STANDART_LOGIC_AND,
  
  CA_MAX
    
};



enum _protect_control_array
{
  PCA_TIMERS,        //�������
  PCA_AF,            //������ �������
  PCA_TF,            //������ �������
  
  PCA_MAX
};

#endif
