#ifndef __I_GAME_MANAGER__
#define __I_GAME_MANAGER__

#include "INetworkDownloadManager.h"
#include "GameDescription.h"
#include "SettingsElement.h"
#include <memory>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

namespace core
{

class IGameManager
{
public:
   virtual void updateLocalList() = 0;
   virtual void copyGameFromRemoteToLocal(fs::path, const GameDescription& desc) = 0;
   virtual GameList getGameList() const = 0;
   virtual SettingsDescList getSettingsDesc() const = 0;
   virtual SettingsElementList getSettingsValues() const = 0;
};

}

#endif