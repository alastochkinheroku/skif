#ifndef __I_CONFIG_PARSER_H__
#define __I_CONFIG_PARSER_H__

#include <string.h>
#include "GameDescription.h"

namespace core {
	//������ ������������
	//��� ������ ������ ����������
	class IConfigParser
	{
	public:
		//������� ������ ��� �� ������������, ��������� � ��������� ��������
		virtual EngineGameList parseListFromConfigFile(std::string file) = 0;
	};

}

#endif