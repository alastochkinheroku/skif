#ifndef __I_LOCAL_FILE_STORAGE__
#define __I_LOCAL_FILE_STORAGE__

#include <string>
#include "GameDescription.h"

namespace core
{

//”правл€ет локальным репозиторием, провер€ет файл и выдаЄт полный путь по ID, также корректно устанавливает его дл€ последующего доступа
class ILocalFileStorage
{
public:
   virtual bool haveGame(ShortId sid) const = 0;
   virtual std::string getFilePath(ShortId sid) const = 0; //получить путь к игре по ShortId
   virtual bool installFromRemote(ShortId sid, std::string fullPath) = 0; //распаковать и установить игру, скачанную из удаленного репозитори€
};

}

#endif