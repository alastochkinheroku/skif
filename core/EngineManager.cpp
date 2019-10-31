#include "EngineManager.h"
#include "DomainException.h"
#include <assert.h>

namespace core
{
	EngineManager::EngineManager():
		_isStartedGame(false)
	{
	}

	void EngineManager::registerEngine(std::string engineName, std::shared_ptr<ITextGameEngine> engine)
	{
		assert(_engineList.count(engineName)==0);
		_engineList.insert(std::make_pair(engineName,engine));
	}

	void EngineManager::initAllEngines()
	{
		//Для всех обновляем список локальных игр
		for (auto it = _engineList.begin(); it != _engineList.end(); ++it)
		{
			it->second->updateLocalList();
		}
	}

	EngineGameList EngineManager::getAllGames() const
	{
		EngineGameList res;
		//Для всех получаем список локальных игр
		for (auto it = _engineList.begin(); it != _engineList.end(); ++it)
		{
			GameList oneList(it->second->getGameList());
			res.insert(std::make_pair(it->first, oneList));
		}
		return res;
	}

	void EngineManager::startNewGame(std::string engineName, unsigned int game_id)
	{
		assert(_engineList.count(engineName) == 1);
		assert(_currentEngine == 0);
		assert(_isStartedGame == false);

		GameList allGames(_engineList[engineName]->getGameList());
		bool findGame = false;
		for (auto it = allGames.begin(); it != allGames.end(); ++it)
		{
			if (it->id == game_id)
			{
				_currentEngine == _engineList[engineName];
				_currentEngine->startGame(game_id);
				_isStartedGame = true;
				bool findGame = true;
				break;
			}
		}

		assert(findGame == true);
	}

	std::shared_ptr<ITextGameEngine> EngineManager::getCurrentEngine()
	{
		return _currentEngine;
	}

	void EngineManager::stopCurrentGame()
	{
		assert(_currentEngine != 0);
		assert(_isStartedGame == true);

		_currentEngine->stopCurrentGame();
		_isStartedGame = false;
	}

}