#include "mainframe.h"
#include "GLogger.h"
#include "../../terps/echo/EchoEngine.h"
#include "../../terps/tads2/Tads2Engine.h"
#include "../../sound/bass/BassSoundPlayer.h"
#include <regex>

static MainFrame* g_mainFrame = 0;//глобальный указатель на окно для доступа из логгера

//Нотификатор для окна приложения
class GuiNotifier : public core::IUserOutputNotifier
{
public:
	GuiNotifier() : _psound(), _init_bass(false), _next_stat(false) {

	}

	void clearOutput()
	{
		g_mainFrame->ClearOutText();
	}

	void notify(std::string text)
	{
		if (_next_stat == true)
		{
			_next_stat = false;
			g_mainFrame->DisplayStatus(text);
		}
		else if (text == ">")
		{
			_next_stat = true;
		}
		else
		{
			g_mainFrame->DisplayOutText(text);
		}
	}

	void waitInput()
	{

	}

	core::ISoundPlayer* sound() {
		if (!_init_bass) {
			_psound.init();
			_init_bass = true;
		}
		return &_psound;
	}
private:
	//PrintedSoundPlayer _psound;
	BassSoundPlayer _psound;
	bool _init_bass;
	bool _next_stat;
};

//Главное приложение
class App : public wxApp
{
public:
	//Инициализация главного приложения
    bool OnInit()
	{
		//Инициализация нотификаторов и движков
		std::shared_ptr<core::IUserOutputNotifier> notifier(new GuiNotifier());
		std::shared_ptr<core::TextGameEngine> tads2_engine(new Tads2Engine(notifier));

		//Инициализация главного окна
		MainFrame* frame = new MainFrame(tads2_engine, std::string("Скиф ")+SKIF_VER, wxPoint(50, 50), wxSize(450, 340));
		g_mainFrame = frame;
		frame->Show(true);
		if (wxTheApp->argc != 2)
		{
			frame->ClearOutText();
			frame->DisplayOutText("Запустите игру с параметром - именем игры.\nНапример: skif-gui game.gam");
			//frame->OpenGame("cloakofd.gam");
		}
		else
		{
			frame->OpenGame(wxTheApp->argv[1]);
		}
		return true;
	}
};

//Логгер выдачи во фрейм
class FrameLogger : public core::GLogger
{
public:
	FrameLogger()
	{
		//Устанавливаем текущий логгер как активный
		setCurrentLogger(this);
	}
	void log(std::string info, core::LOG_LEVEL lvl)
	{
		if (g_mainFrame)
		{
			g_mainFrame->LogInfo(info.c_str());
		}
	}
};
static FrameLogger gCerrLogger;


//Тут определяется точка входа main
wxIMPLEMENT_APP(App);