#include "EchoEngine.h"
#include "DomainException.h"

EchoEngine::EchoEngine(std::shared_ptr<core::IUserOutputNotifier> notifier):
	core::TextGameEngine(notifier),
	_playing(false)
{
}

void EchoEngine::startGame(std::string file)
{
	if (_playing) throw core::DomainException("Game already running!");
	_notifier->notify(std::string("Start playing file: ") + file);
	_playing = true;
	startThread();
}

void EchoEngine::saveGame(std::string name)
{
	_notifier->notify(std::string("Saving progress in file: ") + name);
}

void EchoEngine::loadGame(std::string name)
{
	_notifier->notify(std::string("Loading progress from file: ") + name);
}

void EchoEngine::stopGame()
{
	if (!_playing) throw core::DomainException("Game not running!");
	_notifier->notify(std::string("Stop playing current file."));
	_playing = false;
	joinThread();
}

bool EchoEngine::isPlaying() const
{
	return _playing;
}

void EchoEngine::processUserInput(core::UserCommandType cmd_type, std::string data)
{
	_notifier->notify(std::string("come: ")+data);
}

void EchoEngine::runThread()
{
	//демонстрация вывода текста из потока, независимо от ввода
	while (_playing) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		_notifier->notify(std::string("\n<impulse>\n"));
	}
}
