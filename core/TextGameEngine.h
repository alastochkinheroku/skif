#ifndef __TEXT_GAME_ENGINE__
#define __TEXT_GAME_ENGINE__

#include "INetworkDownloadManager.h"
#include "IUserCommand.h"
#include "GameDescription.h"
#include "IRepositoryProvider.h"
#include <memory>

namespace core
{

//Фасад для игрового движка
class TextGameEngine
{
public:
    TextGameEngine(std::string localGameDir, //директория с локальными играми
                   std::string remoteDescUrl,//расположение ресурса с описанием игр в удаленном репозитории (текстовый файл)
                   std::string tempDownloadDir,//временный каталог для загрузки
                   std::shared_ptr<IRepositoryProvider> repoProvider,
                   std::shared_ptr<INetworkDownloadManager> networkMan);
    //Управление состоянием игры
    virtual void startGame(ShortId sid) = 0;
    virtual void saveGame(std::string name) = 0;
    virtual void loadGame(std::string name) = 0;
    virtual void stopGame() = 0;
    virtual bool isPlaying() const = 0;
    //ввод-вывод во время игры
    virtual void processUserInput(std::shared_ptr<IUserCommand> command) = 0;
    virtual std::list<std::string> getGameOutput() = 0;
    //управлением списком игр
    void updateGameList(bool update_remote); //обновить список игр, с учетом опроса удаленного репозитория
    GameList gameList() const;               //список игр, доступных для игры
    void updateOrDownload(ShortId sid);      //обновить или закачать игру из репозитория
private:
    std::string _localGameDir;
    std::string _remoteDescUrl;
    std::string _tempDownloadDir;
    std::shared_ptr<IRepositoryProvider> _repoProvider;
    std::shared_ptr<INetworkDownloadManager> _networkMan;
    GameList _gameList;
};

}

#endif