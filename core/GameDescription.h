#ifndef __GAME_DESCRIPTION_H__
#define __GAME_DESCRIPTION_H__

#include <string>
#include <list>
#include <map>

namespace core
{

struct GameDescription
{
    unsigned int id;
    std::string caption;
    std::string details;
    std::string localPath;
    std::string remotePath;
};

typedef std::list<GameDescription> GameList;
typedef std::map<std::string/*engine*/, GameList/*games*/ > EngineGameList;

}

#endif