#include "mainframe.h"
#include <wx/filedlg.h>
#include <wx/wfstream.h>
#include <wx/fileconf.h>

//Идентификаторы для команд
enum { ID_ShowLog = 1, ID_OpenGame, ID_OpenSettings };

// clang-format off
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_MENU(ID_ShowLog, MainFrame::OnShowLog)
EVT_MENU(ID_OpenGame, MainFrame::OpenGame)
EVT_MENU(ID_OpenSettings, MainFrame::OpenSettings)
EVT_MENU(wxID_EXIT, MainFrame::OnExit)
EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
EVT_CLOSE(MainFrame::OnClose)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(std::shared_ptr<core::TextGameEngine> engine, 
    std::shared_ptr<core::ITolk> tolker, 
    std::shared_ptr<core::ISoundPlayer> player,
    const wxString& title, 
    const wxPoint& pos, 
    const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size),
    _engine(engine),
    _tolker(tolker),
    _player(player),
    _conf_file_name("skif_config.ini"),
    _enable_tolk(true)
{
    wxMenu* menuFile = new wxMenu();
    menuFile->Append(ID_OpenGame, _("&Open\tCtrl-O"));
    menuFile->Append(ID_ShowLog, _("&Log\tCtrl-L"));
    menuFile->Append(ID_OpenSettings, _("&Settings\tCtrl-S"));
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT, _("&Quit"));

    wxMenu* menuHelp = new wxMenu();
    menuHelp->Append(wxID_ABOUT, _("&About"));

    wxMenuBar* menuBar = new wxMenuBar();
    menuBar->Append(menuFile, _("&File"));
    menuBar->Append(menuHelp, _("&Help"));

    SetMenuBar(menuBar);
    CreateStatusBar();

    _panel = new CentralPanel(engine, this, 10, 10, 600, 600);
    _panel->GetSizer()->Fit(this);

    _frame = new LogFrame(this, _("Application log"));

    LogInfo(_("Application created.\n"));
    //применяем конфиг
    applySettings(getOrCreateConfig());

    _tolker->Speak(L"");
}

void MainFrame::LogInfo(const wxString& data)
{
    _frame->AddLog(data);
}

void MainFrame::OpenGame(wxCommandEvent&)
{
    //Открываем приложение
    //curr_engine->startGame(argv[1]);
    //_engine->startGame(name);
    wxFileDialog
        openFileDialog(this, _("Open TADS game"), "", "",
            "TADS files (*.gam)|*.gam", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...

    wxFileInputStream input_stream(openFileDialog.GetPath());
    if (!input_stream.IsOk())
    {
        wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
        return;
    }
    OpenAndRunGame(openFileDialog.GetPath());
}

void MainFrame::OpenSettings(wxCommandEvent& event)
{
    ConfigSettings sets(getOrCreateConfig());
    SettingsDialog* dlg = new SettingsDialog(sets,this,0,_("Settings"));
    if (dlg->ShowModal() == wxID_OK)
    {
        ConfigSettings newSets(dlg->newSettings());
        applySettings(newSets);
        saveSettingsToFile(newSets);
    }
    //else: dialog was cancelled or some another button pressed
    dlg->Destroy();
}

bool MainFrame::haveConfigFile() const
{
    return (wxFileConfig(_("SKIF_APP"), _("VENDOR"), _conf_file_name, wxEmptyString, wxCONFIG_USE_LOCAL_FILE).GetNumberOfEntries() > 0);
}

bool MainFrame::readFromConfigFile(ConfigSettings& sets) const
{
    wxFileConfig config(_("SKIF_APP"),_("VENDOR"), _conf_file_name, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);
    ConfigSettings new_sets;
    //TODO: сделать элегантнее
    if (!config.Read("use_screenreader", &new_sets.use_screenreader))
    {
        wxLogError(_("Couldn't read use_screenreader value from config."));
        return false;
    }
    
    sets = new_sets;
    return true;
}

ConfigSettings MainFrame::getOrCreateConfig() const
{
    ConfigSettings sets(defaultSettings());
    //if file exist, parse
    if (haveConfigFile())
    {
        readFromConfigFile(sets);
    }
    //else create and save default settings
    return sets;
}

ConfigSettings MainFrame::defaultSettings() const
{
    ConfigSettings sets;
    sets.use_screenreader = true;
    //add default settings
    return sets;
}

void MainFrame::applySettings(const ConfigSettings& sets)
{
    _enable_tolk = sets.use_screenreader;
    if (!_enable_tolk)
    {
        if (_tolker->IsSpeaking()) _tolker->Silence();
    }
}

void MainFrame::saveSettingsToFile(const ConfigSettings& sets)
{
    wxFileConfig config(_("SKIF_APP"), _("VENDOR"), _conf_file_name,wxEmptyString,wxCONFIG_USE_LOCAL_FILE);
    config.EnableAutoSave();
    config.Write("use_screenreader", sets.use_screenreader);
    config.Flush();
}

void MainFrame::OpenAndRunGame(std::string path)
{
    if (_engine->isPlaying())
    {
        _engine->stopGame();
        _player->stop_all_sound();
    }
    _engine->startGame(path);
}

void MainFrame::ClearOutText()
{
    _panel->getOutCtrl()->Clear();
    if (_tolker->IsSpeaking()) _tolker->Silence();
}

void MainFrame::DisplayOutText(std::string text)
{
    //in RELEASE???
    wxString decoded_string(text.c_str(), wxCSConv(wxT("cp-1251")));
    _panel->getOutCtrl()->AppendText(decoded_string);
    if (_enable_tolk && decoded_string.Trim().size() > 0)
    {
        _tolker->Speak(decoded_string.data());
        //LogInfo("Ok tolk? "+std::to_string(ok) + " :" + decoded_string + "\n");
    }
}

void MainFrame::DisplayStatus(std::string text)
{
    SetStatusText(text.c_str());
}

void MainFrame::OnExit(wxCommandEvent& event)
{ 
    _engine->processUserInput(core::UserCommandType_String, "");
    _engine->stopGame();
    if (_frame && _frame->IsVisible())
    {
        _frame->Close();
    }
	Close(true); 
}

void MainFrame::OnClose(wxCloseEvent& event)
{
    _engine->processUserInput(core::UserCommandType_String, "");
    _engine->stopGame();
    if (_frame && _frame->IsVisible())
    {
        _frame->Close();
    }
    Destroy();
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxString currReader(_tolker->DetectScreenReader());
    wxMessageBox(_("Universal text games interpreter. \n Current screen reader: ")+ currReader, _("About"),
        wxOK | wxICON_INFORMATION);
}

void MainFrame::OnShowLog(wxCommandEvent& event)
{
    _frame->Show(true);
}