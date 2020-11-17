#pragma once

#include "logframe.h"
#include "centralpanel.h"
#include "TextGameEngine.h"
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

//Главное окно
class MainFrame : public wxFrame
{
public:
    MainFrame(std::shared_ptr<core::TextGameEngine> engine, const wxString& title, const wxPoint& pos, const wxSize& size);
    void LogInfo(const wxString& data);
    void OpenGame(std::string name);
    void ClearOutText();
    void DisplayOutText(std::string text);
    void DisplayStatus(std::string text);
private:
    //Handlers - обработчики событий от окна
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnShowLog(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);

    CentralPanel* _panel;
    LogFrame* _frame;
    std::shared_ptr<core::TextGameEngine> _engine;

    //Подключение таблицы событий
    wxDECLARE_EVENT_TABLE();
};