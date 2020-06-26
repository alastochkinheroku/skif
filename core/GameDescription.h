#ifndef __GAME_DESCRIPTION_H__
#define __GAME_DESCRIPTION_H__

#include <string>
#include <list>
#include <map>

namespace core
{

    typedef std::string ShortId;

    //�������������� ���� 
    enum LocationState
    {
        LocationState_Local, //������ �� ��������� �����
        LocationState_Remote,//������ � ��������� �����������
        LocationState_Both   //� ����� ������
    };

    struct GameDescription
    {
        ShortId sid;
        std::wstring caption;
        std::wstring author;
        std::wstring details;
        LocationState locationState;
        std::string localVersion;
        std::string remoteVersion;
        std::string remoteUrl;
        std::string localFullPath;
    };

    typedef std::list<GameDescription> GameList;

}

#endif