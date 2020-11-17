#ifndef __I_USER_OUT_NOTIFIER__
#define __I_USER_OUT_NOTIFIER__

#include <string>

namespace core
{
    //��������� � ����� ������
    class ISoundPlayer {
    public:
        //��������� ���� �� ������ � �������� ����
        virtual void play_sound(int layer, std::string title, char* data, size_t data_size, bool repeat, int volume) = 0;
        //���������� ���� � �������� ����
        virtual void stop_sound(int layer) = 0;
    };

    //����������� � ������� ������ ������, ��� ������� �� ���������� ������
    //���������� ������ ���� ����������������
    class IUserOutputNotifier
    {
    public:
        virtual void clearOutput() = 0;//�������� �����
        virtual void notify(std::string text) = 0; //��������� �� ��������� ������
        virtual void waitInput() = 0; //�������� �����
        virtual ISoundPlayer* sound() = 0;         //������ � ��������� �������������
    };
}

#endif