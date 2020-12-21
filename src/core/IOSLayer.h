#ifndef __I_OS_LAYER_H__
#define __I_OS_LAYER_H__

#include <string.h>

namespace core {

	class IOSLayer
	{
	public:
		virtual std::string tempFilePath() const = 0; //получить временный путь для скачивания
		virtual std::string getCachedConfigPath(std::string rootLocalPath) const = 0; //расположение кешированной конфигурация
		virtual bool fileExist(std::string filepath) const = 0;
		virtual std::string getGameDir(std::string rootLocalPath, std::string engineName) const = 0; //расположение игровой директории
		virtual std::string getSaveDir(std::string rootLocalPath, std::string engineName) const = 0; //расположение директории сохранения
		virtual bool copyFile(std::string filepath_from, std::string filepath_to) = 0;
		virtual bool extractArchive(std::string filepath_archive, std::string filepath_to) = 0;
	};

}

#endif