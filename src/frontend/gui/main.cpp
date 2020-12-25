#include "mainframe.h"
#include "GLogger.h"
#include "../../terps/echo/EchoEngine.h"
#include "../../terps/tads2/Tads2Engine.h"
#include "../../sound/bass/BassSoundPlayer.h"
#include "../../sound/tolk_dll/tolk_dll.h"
#include <regex>

static MainFrame* g_mainFrame = 0;//глобальный указатель на окно для доступа из логгера

//Нотификатор для окна приложения
class GuiNotifier : public core::IUserOutputNotifier
{
public:
	GuiNotifier(std::shared_ptr<core::ISoundPlayer> psound) : _psound(psound), _init_bass(false), _next_stat(false) {
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
		return _psound.get();
	}
private:
	//PrintedSoundPlayer _psound;
	std::shared_ptr<core::ISoundPlayer> _psound;
	bool _init_bass;
	bool _next_stat;
};

//Главное приложение
class App : public wxApp
{
	std::shared_ptr<core::ITolk> tolker;
	std::shared_ptr<core::IUserOutputNotifier> notifier;
	std::shared_ptr<core::TextGameEngine> tads2_engine;
	std::shared_ptr<core::ISoundPlayer> sound_player;
protected:
	wxLanguage m_lang;  // language specified by user
	wxLocale m_locale;  // locale we'll be using
public:
    bool OnInit()
	{
		tolker = std::shared_ptr<core::ITolk>(new TolkDllWrapper());
		auto player = new BassSoundPlayer();
		player->init();
		sound_player = std::shared_ptr<core::ISoundPlayer>(player);
		notifier = std::shared_ptr<core::IUserOutputNotifier>(new GuiNotifier(sound_player));
		tads2_engine = std::shared_ptr<core::TextGameEngine>(new Tads2Engine(notifier));

		//явная установка языка
		m_lang = wxLANGUAGE_RUSSIAN;
		// don't use wxLOCALE_LOAD_DEFAULT flag so that Init() doesn't return
		// false just because it failed to load wxstd catalog
		if (!m_locale.Init(m_lang, wxLOCALE_DONT_LOAD_DEFAULT))
		{
			wxLogWarning(_("This language is not supported by the system."));

			// continue nevertheless
		}

		// normally this wouldn't be necessary as the catalog files would be found
		// in the default locations, but when the program is not installed the
		// catalogs are in the build directory where we wouldn't find them by
		// default
		wxLocale::AddCatalogLookupPathPrefix(".");

		// Initialize the catalogs we'll be using
		const wxLanguageInfo* pInfo = wxLocale::GetLanguageInfo(m_lang);
		if (!m_locale.AddCatalog("skif-gui"))
		{
			wxLogError(_("Couldn't find/load the 'skif-gui' catalog for locale '%s'."),
				pInfo ? pInfo->GetLocaleName() : _("unknown"));
		}

		bool ok = tolker->Load();
		std::wstring reader(tolker->DetectScreenReader());
		std::string reader_str(reader.begin(), reader.end());

		MainFrame* frame = new MainFrame(tads2_engine, tolker, sound_player, _("SKIF ") + SKIF_VER, wxPoint(50, 50), wxSize(450, 340));
		g_mainFrame = frame;
		frame->Show(true);
		//core::GLogger::get() << "Load translation: " << ok_load;


		if (wxTheApp->argc != 2)
		{
			frame->ClearOutText();

			frame->DisplayOutText(_("Open TADS game file from menu."));
		}
		else
		{
			frame->OpenAndRunGame(wxTheApp->argv[1]);
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