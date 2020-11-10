#ifndef __TEXT_GAME_ENGINE__
#define __TEXT_GAME_ENGINE__

//#include "INetworkDownloadManager.h"
#include "IUserOutputNotifier.h"
//#include "GameDescription.h"
//#include "IRepositoryProvider.h"
#include <string>
#include <memory>
#include <thread>

namespace core
{

    //TextGameEngine(std::string localGameDir, //директория с локальными играми
    //    std::string remoteDescUrl,//расположение ресурса с описанием игр в удаленном репозитории (текстовый файл)
    //    std::string tempDownloadDir,//временный каталог для загрузки
    //    std::shared_ptr<IRepositoryProvider> repoProvider,
    //    std::shared_ptr<INetworkDownloadManager> networkMan);

    //управлением списком игр
    //void updateGameList(bool update_remote); //обновить список игр, с учетом опроса удаленного репозитория
    //GameList gameList() const;               //список игр, доступных для игры
    //void updateOrDownload(ShortId sid);      //обновить или закачать игру из репозитория

    //private:
    //    std::string _localGameDir;
    //    std::string _remoteDescUrl;
    //    std::string _tempDownloadDir;
    //    std::shared_ptr<IRepositoryProvider> _repoProvider;
    //    std::shared_ptr<INetworkDownloadManager> _networkMan;
    //    GameList _gameList;

    //тип входного воздействия для движка
    enum UserCommandType {
        UserCommandType_Pass = 0, //нет реакции
        UserCommandType_Keypress, //нажатие на кнопку (символ)
        UserCommandType_String,   //ввод строки (строка парсера)
        UserCommandType_Menu      //выбор из меню, структура?
    };

    //Фасад для игрового движка
    //управляет отдельным потоком для игры с его игровым циклом
    //обеспечивает ввод через типовые команды, вывод через нотификатор
    class TextGameEngine
    {
    public:
        TextGameEngine(std::shared_ptr<IUserOutputNotifier> notifier);
        //Управление состоянием игры
        virtual void startGame(std::string file) = 0;
        virtual void saveGame(std::string name) = 0;
        virtual void loadGame(std::string name) = 0;
        virtual void stopGame() = 0;
        virtual bool isPlaying() const = 0;
        //ввод от пользователя
        virtual void processUserInput(UserCommandType cmd_type, std::string data) = 0;
    protected:
        std::shared_ptr<IUserOutputNotifier> _notifier; //может использоваться для уведомления асинхронно с вводом пользователя
        //инфраструктура управления потоками
        //gu.startThread();
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
        //gu.joinThread();
        void startThread() { _th.reset(new std::thread([this]() { runThread(); })); } //запуск отдельного потока для движка
        virtual void runThread() {} //реализация выполнения
        void joinThread() {  //присоединение обратно потока
            if (_th && _th->joinable()) {
                _th->join();
            }
        }
    private:
        std::unique_ptr<std::thread> _th;
    };

}

#endif