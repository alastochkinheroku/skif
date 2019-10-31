#ifndef __ENGINE_MANAGER__
#define __ENGINE_MANAGER__

#include "ITextGameEngine"
#include <memory>

class EngineManager
{
public:
   void registerEngine(
        std::string engineName,
        std::shared_ptr<ITextGameEngine> engine);
   void initAllEngines();
   EngineGameList getAllGames() const;
   void startNewGame(std::string engineName, int game_id);
   std::shared_ptr<ITextGameEngine> getCurrentEngine();
};

#endif