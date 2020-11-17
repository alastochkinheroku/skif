#ifndef __I_USER_OUT_NOTIFIER__
#define __I_USER_OUT_NOTIFIER__

#include <string>

namespace core
{
    //интерфейс к аудио плееру
    class ISoundPlayer {
    public:
        //проиграть звук из памяти в заданном слое
        virtual void play_sound(int layer, std::string title, char* data, size_t data_size, bool repeat, int volume) = 0;
        //остановить звук в заданном слое
        virtual void stop_sound(int layer) = 0;
    };

    //Уведомление о событии вывода движка, как правило из отдельного потока
    //реализации должны быть потокобезопасные
    class IUserOutputNotifier
    {
    public:
        virtual void clearOutput() = 0;//очистить экран
        virtual void notify(std::string text) = 0; //уведомить об изменении экрана
        virtual void waitInput() = 0; //ожидание ввода
        virtual ISoundPlayer* sound() = 0;         //доступ к звуковому проигрывателю
    };
}

#endif