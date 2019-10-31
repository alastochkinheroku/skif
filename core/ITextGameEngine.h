#ifndef __I_TEXT_GAME_ENGINE__
#define __I_TEXT_GAME_ENGINE__

#include "INetworkDownloadManager.h"
#include "IUserCommand.h"
#include "GameDescription.h"
#include <memory>

class ITextGameEngine
{
public:
   virtual void updateLocalList() = 0;
   virtual void copyGameToLocal(std::filesystem::path, const GameDescription& desc) = 0;
   virtual GameList getGameList() const = 0;
   virtual SettingsList getSettings() const = 0;
   virtual void setSettingsJson(std::string) = 0;
   virtual void startGame(int id) = 0;
   virtual void stopCurrentGame() = 0;
   virtual void processUserInput(std::shared_ptr<IUserCommand> command) = 0;
   virtual std::list<std::string> getGameOutput() = 0;
};

#endif