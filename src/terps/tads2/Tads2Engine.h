#ifndef __TADS2_TERP_ENGINE_H__
#define __TADS2_TERP_ENGINE_H__

#include "TextGameEngine.h"

#define TADS_TOTAL_OPT 7
#define TADS_MAX_GAME_NAME 512

class Tads2Engine : public core::TextGameEngine
{
public:
	Tads2Engine(std::shared_ptr<core::IUserOutputNotifier> notifier);
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
	int _argc;
	char* _argv[TADS_TOTAL_OPT];
	char _game_name[TADS_MAX_GAME_NAME];
};

#endif