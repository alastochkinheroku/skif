#ifndef __I_TOLK__
#define __I_TOLK__

#include <wchar.h>

namespace core
{

//Интерфес для выдачи информации на скринридер/брайль
class ITolk
{
public:
   virtual bool Load() = 0; //загрузка интерфейса на скринридер
   virtual const wchar_t* DetectScreenReader() = 0; //получить имя обнаруженного ридера
   virtual bool IsSpeaking() const = 0; //идет ли речь
   virtual bool Speak(const wchar_t* str) = 0; //проговорить текст
   virtual void Silence() = 0; //заглушить текущую речь
};

}

#endif