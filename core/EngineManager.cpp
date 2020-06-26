#include "EngineManager.h"
#include "DomainException.h"
#include <assert.h>

namespace core
{
	EngineManager::EngineManager()
	{
	}

	void EngineManager::registerEngine(std::string engineName, std::shared_ptr<TextGameEngine> engine)
	{
		assert(_engineList.count(engineName)==0);
		_engineList.insert(std::make_pair(engineName,engine));
	}

	std::list<std::string> EngineManager::enginesList() const
	{
		return std::list<std::string>();
	}

	void EngineManager::selectEngine(std::string engineName)
	{
	}

	std::shared_ptr<TextGameEngine> EngineManager::currentEngine()
	{
		assert(_currentEngine);
		return _currentEngine;
	}
}