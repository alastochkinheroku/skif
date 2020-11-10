#ifndef __ECHO_ENGINE_H__
#define __ECHO_ENGINE_H__

#include "TextGameEngine.h"

class EchoEngine : public core::TextGameEngine
{
public:
	EchoEngine(std::shared_ptr<core::IUserOutputNotifier> notifier);
	void startGame(std::string file);
	void saveGame(std::string name);
	void loadGame(std::string name);
	void stopGame();
	bool isPlaying() const;
	void processUserInput(core::UserCommandType cmd_type, std::string data);
protected:
	void runThread();
private:
	bool _playing;
};

#endif