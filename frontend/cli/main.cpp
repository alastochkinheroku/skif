#include "GLogger.h"
#include <iostream>

//Логгер выдачи в stderr
class CerrLogger : public core::GLogger
{
public:
	CerrLogger()
	{
		//Устанавливаем текущий логгер как активный
		setCurrentLogger(this);
	}
	void log(std::string info, core::LOG_LEVEL lvl)
	{
		std::cerr << info;
	}
};
static CerrLogger gCerrLogger;

int main(int argc, char *argv[])
{
	std::cout<<"Hello from cli!";
	return 0;
}
