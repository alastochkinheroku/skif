#ifndef __I_TEXT_GAME_ENGINE__
#define __I_TEXT_GAME_ENGINE__

#include "INetworkDownloadManager.h"
#include "IUserCommand.h"
#include "GameDescription.h"
#include "SettingsElement.h"
#include <memory>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

namespace core
{

class ITextGameEngine
{
public:
   virtual void updateLocalList() = 0;
   virtual void copyGameFromRemoteToLocal(fs::path, const GameDescription& desc) = 0;
   virtual GameList getGameList() const = 0;
   virtual SettingsDescList getSettingsDesc() const = 0;
   virtual SettingsElementList getSettingsValues() const = 0;
   virtual void setSettings(SettingsElementList) = 0;
   virtual void startGame(unsigned int id) = 0;
   virtual void saveGame(std::string path) = 0;
   virtual void loadGame(std::string path) = 0;
   virtual void stopCurrentGame() = 0;
   virtual void processUserInput(std::shared_ptr<IUserCommand> command) = 0;
   virtual std::list<std::string> getGameOutput() = 0;
};

}

#endif