#ifndef __TEXT_GAME_ENGINE__
#define __TEXT_GAME_ENGINE__

#include "INetworkDownloadManager.h"
#include "IUserCommand.h"
#include "GameDescription.h"
#include "IRepositoryProvider.h"
#include <memory>

namespace core
{

//����� ��� �������� ������
class TextGameEngine
{
public:
    TextGameEngine(std::string localGameDir, //���������� � ���������� ������
                   std::string remoteDescUrl,//������������ ������� � ��������� ��� � ��������� ����������� (��������� ����)
                   std::string tempDownloadDir,//��������� ������� ��� ��������
                   std::shared_ptr<IRepositoryProvider> repoProvider,
                   std::shared_ptr<INetworkDownloadManager> networkMan);
    //���������� ���������� ����
    virtual void startGame(ShortId sid) = 0;
    virtual void saveGame(std::string name) = 0;
    virtual void loadGame(std::string name) = 0;
    virtual void stopGame() = 0;
    virtual bool isPlaying() const = 0;
    //����-����� �� ����� ����
    virtual void processUserInput(std::shared_ptr<IUserCommand> command) = 0;
    virtual std::list<std::string> getGameOutput() = 0;
    //����������� ������� ���
    void updateGameList(bool update_remote); //�������� ������ ���, � ������ ������ ���������� �����������
    GameList gameList() const;               //������ ���, ��������� ��� ����
    void updateOrDownload(ShortId sid);      //�������� ��� �������� ���� �� �����������
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