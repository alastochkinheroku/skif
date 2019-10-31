#include "GLogger.h"
#include <iostream>

//������ ������ � stderr
class CerrLogger : public core::GLogger
{
public:
	CerrLogger()
	{
		//������������� ������� ������ ��� ��������
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
