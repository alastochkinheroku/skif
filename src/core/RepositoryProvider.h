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

//��������� ������������ ��� ����, ������������ ������ � ���������� �� �����, � ����� ������������ �������� �� ����
class RepositoryProvider
{
public:
   RepositoryProvider(std::string rootLocalPath,     //���� � ���������� ��� �� ��������� ��
					  std::string remoteConfigFile,  //���� � ��������� ������������
					  std::shared_ptr<INetworkDownloadManager> manager, //�������� �������
					  std::shared_ptr<IConfigParser> parser, //������ ������������
					  std::shared_ptr<IOSLayer> os_layer //���� ����������� ��� �������� �������
   );
   //����������� ������ �� ����� � ��� ������������� �� ��������� ������
   void registerEngine(std::string engineName,
					   std::shared_ptr<ILocalFileStorage>);
   //���������� ���������
   void update();
   GameList list(std::string engineName) const; //�������� ������ ��� ��� ������
   virtual void installFromRemote(std::string engineName, ShortId sid); //������� � ���������� ����
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