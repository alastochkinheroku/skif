#include "RepositoryProvider.h"
#include "DomainException.h"
#include <cassert>

namespace core
{

    RepositoryProvider::RepositoryProvider(std::string rootLocalPath, std::string remoteConfigFile, std::shared_ptr<INetworkDownloadManager> manager,
        std::shared_ptr<IConfigParser> parser, std::shared_ptr<IOSLayer> os_layer):
        _rootLocalPath(rootLocalPath),
        _remoteConfigFile(remoteConfigFile),
        _manager(manager),
        _parser(parser),
        _os_layer(os_layer)
    {

    }

    void RepositoryProvider::registerEngine(std::string engineName, std::shared_ptr<ILocalFileStorage> storage)
    {
        _engineStorage.insert(std::make_pair(engineName, storage));
    }

    void RepositoryProvider::update()
    {
        //���������, ���� �� ������ ������� � ����
        std::string conf_path = _os_layer->getCachedConfigPath(_rootLocalPath);
        if (!_os_layer->fileExist(conf_path))
        {
            //���� ���, �� ��������� �� ��������� �������
            std::string temp_file(_os_layer->tempFilePath());
            _manager->downloadResource(_remoteConfigFile, temp_file);
            bool ok = _os_layer->copyFile(temp_file, conf_path);
            if (!ok)
            {
                throw DomainException("Cannot copy configuration from "+ temp_file + " to "+ conf_path);
            }
        }
        //TODO: �������� ������ � ��������� �
        //������ ������� ������ � ������
        _list = _parser->parseListFromConfigFile(conf_path);
        //��������� ����� ��� ��������� �������
        for (auto &engGames : _list)
        {
            assert(_engineStorage.contains(engGames.first));
            for (auto& game : engGames.second)
            {
                //�������������, ���� � ��������� ���� ��� ���
                if (_engineStorage[engGames.first]->haveGame(game.sid))
                {
                    game.haveLocal = true;
                }
                else
                {
                    game.haveLocal = false;
                }
            }
        }
    }

    GameList RepositoryProvider::list(std::string engineName) const
    {
        return _list.at(engineName);
    }

    void RepositoryProvider::installFromRemote(std::string engineName, ShortId sid)
    {
        //���� ���� � ������
        assert(_list.contains(engineName));
        auto it = std::find_if(std::begin(_list[engineName]),
            std::end(_list[engineName]),
            [&sid](const GameDescription& v) { return v.sid==sid; });
        assert(_engineStorage[engineName]);
        assert(_list[engineName].end() != it);

        std::string url = it->remoteUrl;
        std::string temp_file(_os_layer->tempFilePath());
        //��������� �� ��������� ����
        _manager->downloadResource(url, temp_file);
        //������������� �������
        _engineStorage[engineName]->installFromRemote(sid, temp_file);
    }

}