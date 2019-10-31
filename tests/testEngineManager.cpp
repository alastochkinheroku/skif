#include "CppUnitLite/TestHarness.h"
//Специальная библиотека для создания заглушек: https://github.com/eranpeer/FakeIt/wiki/Quickstart
#include "fakeit.hpp"

using namespace fakeit;

//Макрос для перехвата исключения метода verify в fakeit и отображению через cppunitlite
#define CHECK_VERIFY(a) try{(a);CHECK(true);}catch(VerificationException e){FAIL(e.what());};


//Проверка для строк std в CppUnitLite
static SimpleString StringFrom(const std::string& value)
{
	return SimpleString(value.c_str());
}

TEST(TestEngineManager, demo)
{
	CHECK(true);
}