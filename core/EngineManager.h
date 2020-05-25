#ifndef __ENGINE_MANAGER__
#define __ENGINE_MANAGER__

#include "ITextGameEngine.h"
#include "IGameManager.h"
#include <memory>
#include <map>

namespace core
{
    
class EngineManager
{
public:
   EngineManager();
   void registerEngine(
        std::string engineName,
        std::shared_ptr<ITextGameEngine> engine,
	   std::shared_ptr<IGameManager> manager
	   );
   void initAllEngines();
   EngineGameList getAllGames() const;
   void startNewGame(std::string engineName, unsigned int game_id);
   std::shared_ptr<ITextGameEngine> getCurrentEngine();
   void stopCurrentGame();
private:
	std::map<std::string/*engine name*/, std::shared_ptr<ITextGameEngine> /*engine*/> _engineList;
	std::shared_ptr<ITextGameEngine> _currentEngine;
	bool _isStartedGame;
};

}

#endif