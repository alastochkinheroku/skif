#include "GLogger.h"
#include <assert.h>

namespace core
{
	//Единственный экземпляр логгера
	GLogger* GLogger::_currentLogger = 0;

	GLogger& GLogger::get()
	{
		assert(_currentLogger != 0);
		return *_currentLogger;
	}

	void GLogger::setCurrentLogger(GLogger* logger)
	{
		assert(logger != 0);
		_currentLogger = logger;
	}

}