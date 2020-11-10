#ifndef __ENGINE_MANAGER__
#define __ENGINE_MANAGER__

#include <memory>
#include <map>
#include <string>
#include <list>

#include "TextGameEngine.h"

namespace core
{

//Менеджер движков, управляет списком всех
class EngineManager
{
public:
   EngineManager();
   void registerEngine(
        std::string engineName,
        std::shared_ptr<TextGameEngine> engine);
   std::list<std::string> enginesList() const;
   void selectEngine(std::string engineName);
   std::shared_ptr<TextGameEngine> currentEngine();
private:
	std::map<std::string/*engine name*/, std::shared_ptr<TextGameEngine> /*engine*/> _engineList;
	std::shared_ptr<TextGameEngine> _currentEngine;
};

}

#endif