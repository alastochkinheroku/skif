#include "Tads2Engine.h"
#include "DomainException.h"
#include "GLogger.h"
#include <memory>
#include <string>

static std::shared_ptr<core::IUserOutputNotifier> g_tads_notifier = nullptr;
static bool g_have_input = false; //признак наличия ввода от пользователя
static std::string g_curr_input = ""; //текущий ввод

extern "C" {
	#include "os.h"
	#include "osifc.h"
	#include "trd.h"
	#include "lib.h"

	//ввод-вывод, обрабатывающийся через оболочку движка
	/*
	 *   print a null-terminated string the console
	 */
	void os_printz(const char* str)
	{
		if (g_tads_notifier)
		{
			g_tads_notifier->notify(str);
		}
	}

/*
 *   print a counted-length string, which isn't necessarily null-terminated
 */
	void os_print(const char* str, size_t len)
	{
		/* write the string to stdout, limiting the length */
		std::string out_str(str, len);
		if (g_tads_notifier)
		{
			g_tads_notifier->notify(out_str);
		}
	}

/*
 *   os_gets performs the same function as gets().  It should get a
 *   string from the keyboard, echoing it and allowing any editing
 *   appropriate to the system, and return the null-terminated string as
 *   the function's value.  The closing newline should NOT be included in
 *   the string.
 */
	uchar* os_gets(uchar* s, size_t bufl)
	{
		g_have_input = false;
		//уведомляем об ожидании ввода
		if (g_tads_notifier)
		{
			g_tads_notifier->waitInput();
		}

		while (!g_have_input)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

		//char* out_buf = new char[g_curr_input.size()];
		g_curr_input.push_back(0);
		strncpy((char*)s, g_curr_input.data(), g_curr_input.size() < bufl ? g_curr_input.size() : bufl);
		return s;

		//return((uchar*)fgets((char*)s, bufl, stdin));
	}

	//проиграть сэмпл
	void skif_sound_play_chan(char* id, int layer,char* data,	unsigned long sz, int repeat, int volume)
	{
		if (g_tads_notifier)
		{
			std::string sid(id);
			g_tads_notifier->sound()->play_sound(layer, sid, data, sz, repeat, volume);
		}
	}

	void skif_sound_stop_chan(int layer)
	{
		if (g_tads_notifier)
		{
			g_tads_notifier->sound()->stop_sound(layer);
		}
	}
}

Tads2Engine::Tads2Engine(std::shared_ptr<core::IUserOutputNotifier> notifier):
	core::TextGameEngine(notifier),
	_playing(false)
{
	for (int i = 0; i < TADS_TOTAL_OPT; i++) _argv[i] = NULL;
	strcpy(_game_name, "NONE.gam");

	int used_opt = 0;
	_argv[used_opt++] = "skif";
	_argv[used_opt++] = "-m700000";	/* Size of the cache */
	_argv[used_opt++] = "-double-";
	_argv[used_opt++] = "-t-";
	_argv[used_opt++] = "-plain";
	_argv[used_opt++] = _game_name;//Резервируем под имя игры
	_argc = used_opt;
	g_tads_notifier = notifier;
}

void Tads2Engine::startGame(std::string file)
{
	if (_playing) throw core::DomainException("Game already running!");

	core::GLogger::get() << "Start playing TADS2 file: " << file;
	strcpy(_game_name, file.c_str());
	//_argv[_argc - 1] = file.c_str();

	//os_plain();

	_playing = true;
	startThread();
}

void Tads2Engine::saveGame(std::string name)
{
	_notifier->notify(std::string("Saving progress in file: ") + name);
}

void Tads2Engine::loadGame(std::string name)
{
	_notifier->notify(std::string("Loading progress from file: ") + name);
}

void Tads2Engine::stopGame()
{
	if (!_playing) throw core::DomainException("Game not running!");
	core::GLogger::get() << "Stop playing current file.";
	_playing = false;
	joinThread();
}

bool Tads2Engine::isPlaying() const
{
	return _playing;
}

void Tads2Engine::processUserInput(core::UserCommandType cmd_type, std::string data)
{
	//_notifier->notify(std::string("come: ")+data);
	g_curr_input = data;
	g_have_input = true;
}

void Tads2Engine::runThread()
{
	/* call the main routine */
	trdmain(_argc, _argv, NULL, ".sav");
}
