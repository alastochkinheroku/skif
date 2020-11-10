#include "GLogger.h"
#include "EngineManager.h"
#include "IUserOutputNotifier.h"
#include <iostream>

#include "../../terps/echo/EchoEngine.h"
#include "../../terps/tads2/Tads2Engine.h"
#include <cassert>

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

class PrintedSoundPlayer : public core::ISoundPlayer {
public:
	void play_sound(int layer, std::string title, char* data, size_t data_size, bool repeat, int volume)
	{
		std::cout << "<SND PLAY LAYER=" << layer << " TITLE= " << title << " DATA SZ=" << data_size << " VOLUME=" << volume << " REPEAT=" << repeat << ">" << std::endl;
	}

	void stop_sound(int layer)
	{
		std::cout << "<SND STOP LAYER=" << layer << ">" << std::endl;
	}
};

class CliNotifier : public core::IUserOutputNotifier
{
public:
	void notify(std::string text)
	{
		std::cout << text;
	}

	void waitInput()
	{

	}

	core::ISoundPlayer* sound() {
		return &_psound;
	}
private:
	PrintedSoundPlayer _psound;
};

int main(int argc, char *argv[])
{
	std::cout<<"Start cli. Enter `exit` for quit console.";
	if (argc != 2) {
		std::cout << "Usage: cli file.gam";
		return 1;
	}

	std::shared_ptr<core::IUserOutputNotifier> notifier(new CliNotifier());
	std::shared_ptr<core::TextGameEngine> echo_engine(new EchoEngine(notifier));
	std::shared_ptr<core::TextGameEngine> tads2_engine(new Tads2Engine(notifier));

	std::shared_ptr<core::TextGameEngine> curr_engine = tads2_engine;

	curr_engine->startGame(argv[1]);
	std::string str;
	while (true) {
		//приглашение
		std::cout << std::endl << ">";
		std::getline(std::cin, str);
		if (str == "exit") break;
		curr_engine->processUserInput(core::UserCommandType_String, str);
	}

	curr_engine->stopGame();
	//core::EngineManager manager;
	//manager.registerEngine("echo", echo_engine);

	return 0;
}
