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

    //TextGameEngine(std::string localGameDir, //���������� � ���������� ������
    //    std::string remoteDescUrl,//������������ ������� � ��������� ��� � ��������� ����������� (��������� ����)
    //    std::string tempDownloadDir,//��������� ������� ��� ��������
    //    std::shared_ptr<IRepositoryProvider> repoProvider,
    //    std::shared_ptr<INetworkDownloadManager> networkMan);

    //����������� ������� ���
    //void updateGameList(bool update_remote); //�������� ������ ���, � ������ ������ ���������� �����������
    //GameList gameList() const;               //������ ���, ��������� ��� ����
    //void updateOrDownload(ShortId sid);      //�������� ��� �������� ���� �� �����������

    //private:
    //    std::string _localGameDir;
    //    std::string _remoteDescUrl;
    //    std::string _tempDownloadDir;
    //    std::shared_ptr<IRepositoryProvider> _repoProvider;
    //    std::shared_ptr<INetworkDownloadManager> _networkMan;
    //    GameList _gameList;

    //��� �������� ����������� ��� ������
    enum UserCommandType {
        UserCommandType_Pass = 0, //��� �������
        UserCommandType_Keypress, //������� �� ������ (������)
        UserCommandType_String,   //���� ������ (������ �������)
        UserCommandType_Menu      //����� �� ����, ���������?
    };

    //����� ��� �������� ������
    //��������� ��������� ������� ��� ���� � ��� ������� ������
    //������������ ���� ����� ������� �������, ����� ����� �����������
    class TextGameEngine
    {
    public:
        TextGameEngine(std::shared_ptr<IUserOutputNotifier> notifier);
        //���������� ���������� ����
        virtual void startGame(std::string file) = 0;
        virtual void saveGame(std::string name) = 0;
        virtual void loadGame(std::string name) = 0;
        virtual void stopGame() = 0;
        virtual bool isPlaying() const = 0;
        //���� �� ������������
        virtual void processUserInput(UserCommandType cmd_type, std::string data) = 0;
    protected:
        std::shared_ptr<IUserOutputNotifier> _notifier; //����� �������������� ��� ����������� ���������� � ������ ������������
        //�������������� ���������� ��������
        //gu.startThread();
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
        //gu.joinThread();
        void startThread() { _th.reset(new std::thread([this]() { runThread(); })); } //������ ���������� ������ ��� ������
        virtual void runThread() {} //���������� ����������
        void joinThread() {  //������������� ������� ������
            if (_th && _th->joinable()) {
                _th->join();
            }
        }
    private:
        std::unique_ptr<std::thread> _th;
    };

}

#endif