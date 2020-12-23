#include "mainframe.h"

//Идентификаторы для команд
enum { ID_ShowLog = 1, ID_OpenGame };

// clang-format off
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_MENU(ID_ShowLog, MainFrame::OnShowLog)
EVT_MENU(wxID_EXIT, MainFrame::OnExit)
EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
EVT_CLOSE(MainFrame::OnClose)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(std::shared_ptr<core::TextGameEngine> engine, 
    std::shared_ptr<core::ITolk> tolker, 
    const wxString& title, 
    const wxPoint& pos, 
    const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size),
    _engine(engine),
    _tolker(tolker)
{
    wxMenu* menuFile = new wxMenu();
    menuFile->Append(ID_OpenGame, "&Открыть\tCtrl-O");
    menuFile->Append(ID_ShowLog, "&Лог\tCtrl-L");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu* menuHelp = new wxMenu();
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar* menuBar = new wxMenuBar();
    menuBar->Append(menuFile, "&Файл");
    menuBar->Append(menuHelp, "&Помощь");

    SetMenuBar(menuBar);
    CreateStatusBar();

    _panel = new CentralPanel(engine, this, 10, 10, 600, 600);
    _panel->GetSizer()->Fit(this);

    _frame = new LogFrame(this, wxT("Лог приложения"));

    LogInfo("Приложение создано.\n");
    _tolker->Speak(L"");
}

void MainFrame::LogInfo(const wxString& data)
{
    _frame->AddLog(data);
}

void MainFrame::OpenGame(std::string name)
{
    //Открываем приложение
    //curr_engine->startGame(argv[1]);
    _engine->startGame(name);
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
    if (decoded_string.Trim().size() > 0)
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
    wxMessageBox("Это wxWidgets' пример Hello world", "О программе",
        wxOK | wxICON_INFORMATION);
}

void MainFrame::OnShowLog(wxCommandEvent& event)
{
    _frame->Show(true);
}