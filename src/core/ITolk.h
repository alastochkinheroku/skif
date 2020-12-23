#ifndef __I_TOLK__
#define __I_TOLK__

#include <wchar.h>

namespace core
{

//�������� ��� ������ ���������� �� ����������/������
class ITolk
{
public:
   virtual bool Load() = 0; //�������� ���������� �� ����������
   virtual const wchar_t* DetectScreenReader() = 0; //�������� ��� ������������� ������
   virtual bool IsSpeaking() const = 0; //���� �� ����
   virtual bool Speak(const wchar_t* str) = 0; //����������� �����
   virtual void Silence() = 0; //��������� ������� ����
};

}

#endif