#include "CppUnitLite/TestHarness.h"
#include "GLogger.h"

//������ �������� ������
class FakeLogger : public core::GLogger
{
public:
	FakeLogger()
	{
		//������������� ������� ������ ��� ��������
		setCurrentLogger(this);
	}
	void log(std::string info, core::LOG_LEVEL lvl)
	{
		//std::cout << "Hello!!! " << info;
	}
};
static FakeLogger gFakeLogger;

int main()
{
	TestResult tr;
	TestRegistry::runAllTests(tr);
	return 0;
}
