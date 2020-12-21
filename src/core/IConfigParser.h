#ifndef __I_CONFIG_PARSER_H__
#define __I_CONFIG_PARSER_H__

#include <string.h>
#include "GameDescription.h"

namespace core {
	//парсер конфигураций
	//при ошибке выдает исключение
	class IConfigParser
	{
	public:
		//парсинг списка игр из конфигурации, считанной с удалённого каталога
		virtual EngineGameList parseListFromConfigFile(std::string file) = 0;
	};

}

#endif