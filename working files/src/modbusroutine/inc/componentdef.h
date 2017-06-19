#ifndef __COMPONENTDEF_H
#define __COMPONENTDEF_H


typedef struct
{
  int operativMarker[2];//����������� ������
  int countObject;//�-�� �������
  int isActiveActualData;

  int (*getModbusRegister)(int);//�������� ���������� ��������
  int (*getModbusBit)(int);//�������� ���������� ����
  int (*setModbusRegister)(int, int);//�������� �������
  int (*setModbusBit)(int, int);//�������� ���

  void (*setCountObject)(int);//�������� �-�� �������

  void (*preReadAction)(void);//action �� ������
  void (*postReadAction)(void);//action ����� ������
  void (*preWriteAction)(void);//action �� ������
  void (*postWriteAction)(void);//action ����� ������

} COMPONENT_OBJ;

#endif



