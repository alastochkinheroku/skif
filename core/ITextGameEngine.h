#ifndef __I_TEXT_GAME_ENGINE__
#define __I_TEXT_GAME_ENGINE__

#include "IUserCommand.h"
#include "IGameOutput.h"
#include "SettingsElement.h"
#include <memory>

namespace core
{

class ITextGameEngine
{
public:
   virtual void setSettings(SettingsElementList) = 0;
   virtual void startGame(unsigned int id) = 0;
   virtual void saveGame(std::string path) = 0;
   virtual void loadGame(std::string path) = 0;
   virtual void stopCurrentGame() = 0;
   
   virtual void processUserInput(std::shared_ptr<IUserCommand> command) = 0;
   virtual bool haveOutput() const = 0;
   virtual std::shared_ptr<IGameOutput> getGameOutput() const = 0;
};

}

#endif