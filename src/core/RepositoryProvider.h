#ifndef __REPOSITORY_PROVIDER__
#define __REPOSITORY_PROVIDER__

#include <string>
#include <memory>
#include <map>
#include "GameDescription.h"
#include "ILocalFileStorage.h"
#include "INetworkDownloadManager.h"
#include "IConfigParser.h"
#include "IOSLayer.h"

namespace core
{

//Провайдер репозиториев для игры, обеспечивает доступ к информации по играм, а также обеспечивает загрузку по сети
class RepositoryProvider
{
public:
   RepositoryProvider(std::string rootLocalPath,     //путь к библиотеке игр на локальном ПК
					  std::string remoteConfigFile,  //путь к удаленной конфигурации
					  std::shared_ptr<INetworkDownloadManager> manager, //менеджер закачки
					  std::shared_ptr<IConfigParser> parser, //парсер конфигураций
					  std::shared_ptr<IOSLayer> os_layer //слой операционки для удобного доступа
   );
   //регистрация движка по имени и его представления на локальной машине
   void registerEngine(std::string engineName,
					   std::shared_ptr<ILocalFileStorage>);
   //обновление менеджера
   void update();
   GameList list(std::string engineName) const; //получить список игр для движка
   virtual void installFromRemote(std::string engineName, ShortId sid); //скачать и установить игру
private:
	const std::string _rootLocalPath;
	const std::string _remoteConfigFile;
	std::shared_ptr<INetworkDownloadManager> _manager;
	std::shared_ptr<IConfigParser> _parser;
	std::shared_ptr<IOSLayer> _os_layer;
	std::map<std::string, std::shared_ptr<ILocalFileStorage> > _engineStorage;
	EngineGameList _list;
};

}

#endif