#ifndef __GAME_DESCRIPTION_H__
#define __GAME_DESCRIPTION_H__

#include <string>
#include <list>
#include <map>

namespace core
{

    typedef std::string ShortId;

    struct GameDescription
    {
        ShortId sid;
        bool haveLocal; //наличие в локальном репозитории
        std::wstring caption;
        std::wstring author;
        std::wstring details;
        std::string version;
        std::string remoteUrl; //ссылка на скачивание, если пустая - нет в удаленном репозитории
    };

    typedef std::list<GameDescription> GameList;
    typedef std::map<std::string/*engine name*/, GameList> EngineGameList;

}

#endif