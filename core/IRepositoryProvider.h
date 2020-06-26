#ifndef __I_REPOSITORY_PROVIDER__
#define __I_REPOSITORY_PROVIDER__

#include <string>
#include "GameDescription.h"

namespace core
{

class IRepositoryProvider
{
public:
   virtual std::string gameFileNameFromShortId(ShortId sid) const = 0; //получить имя игры по ShortId
   virtual GameList parseOrCreateConfigFromLocal(std::string fullPath) = 0; //распарсить или создать конфигурацию для игр в локальном репозитории
   virtual void installFromRemote(std::string fullPath) = 0; //распаковать и установить игру, скачанную из удаленного репозитория
   virtual GameList updateConfigFromRemote(const GameList& glist, std::wstring remoteSpec) = 0; //обновить конфигурацию для игры по спецификации, скачанной из репозитория
};

}

#endif