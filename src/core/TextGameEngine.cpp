#include "TextGameEngine.h"
//#include <sstream>
//#include <fstream>
//#include <codecvt>
//#include <assert.h>

namespace core
{
    /*
    static std::wstring readFile(const char* filename)
    {
        std::wifstream wif(filename);
        wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
        std::wstringstream wss;
        wss << wif.rdbuf();
        return wss.str();
    }

    TextGameEngine::TextGameEngine(std::string localGameDir, std::string remoteDescUrl, std::string tempDownloadDir, std::shared_ptr<IRepositoryProvider> repoProvider, std::shared_ptr<INetworkDownloadManager> networkMan):
        _remoteDescUrl(remoteDescUrl),
        _localGameDir(localGameDir),
        _tempDownloadDir(tempDownloadDir),
        _repoProvider(repoProvider),
        _networkMan(networkMan)
    {
    }

    void TextGameEngine::updateGameList(bool update_remote)
    {
        //обновляем из локального хранилища
        GameList list = _repoProvider->parseOrCreateConfigFromLocal(_localGameDir);
        if (update_remote)
        {
            //загружаем спецификацию
            std::string downloadFullPath = _tempDownloadDir + "/spec";
            bool ok_load = _networkMan->downloadResource(_remoteDescUrl, downloadFullPath);
            if (ok_load)
            {
                //считываем в строку
                std::wstring remote_spec = readFile(downloadFullPath.data());
                //парсим в зависимости от её типа и обновляем список игр
                GameList list = _repoProvider->updateConfigFromRemote(list, remote_spec);
            }
            else
            {
                //todo error?
            }
        }
    }

    GameList TextGameEngine::gameList() const
    {
        return _gameList;
    }

    void TextGameEngine::updateOrDownload(ShortId sid)
    {

        for (auto const& it : _gameList) {
            if (it.sid == sid)
            {
                std::string downloadFullPath = _tempDownloadDir + "/" + _repoProvider->gameFileNameFromShortId(it.sid);
                bool ok_load = _networkMan->downloadResource(it.remoteUrl, downloadFullPath);
                if (ok_load)
                {
                    //распаковываем себе
                    _repoProvider->installFromRemote(downloadFullPath);
                    //обновляем список игр
                    updateGameList(false);
                }
                else
                {
                    //???
                }
                return;
            }
        }
        assert(false);
    }
    */

    TextGameEngine::TextGameEngine(std::shared_ptr<IUserOutputNotifier> notifier):
        _notifier(notifier)
    {
    }

}