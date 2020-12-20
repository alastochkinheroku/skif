#ifndef __REPOSITORY_PROVIDER__
#define __REPOSITORY_PROVIDER__

#include <string>
#include "GameDescription.h"

namespace core
{

class RepositoryProvider
{
public:
   RepositoryProvider(std::string rootLocalPath, std::string remoteConfigFile, std::shared_ptr<INetworkDownloadManager>);
   void registerEngine(std::string engineName,
					   std::shared_ptr<ILocalStorage>);
   GameList list(std::string engineName) const; //получить список игр для движка
   virtual void installFromRemote(std::string engineName, ShortId sid); //распаковать и установить игру, скачанную из удаленного репозитория
};

}

#endif