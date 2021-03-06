#pragma once

#include "logframe.h"
#include "centralpanel.h"
#include "TextGameEngine.h"
#include "ITolk.h"
#include "settingsdialog.h"
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

//Главное окно
class MainFrame : public wxFrame
{
public:
    MainFrame(std::shared_ptr<core::TextGameEngine> engine, 
        std::shared_ptr<core::ITolk> tolker,
        std::shared_ptr<core::ISoundPlayer> player,
        const wxString& title, 
        const wxPoint& pos, 
        const wxSize& size);
    void LogInfo(const wxString& data);
    void OpenAndRunGame(std::string path);
    void ClearOutText();
    void DisplayOutText(std::string text);
    void DisplayStatus(std::string text);
private:
    //Handlers - обработчики событий от окна
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnShowLog(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
    void OpenGame(wxCommandEvent& event);
    void OpenSettings(wxCommandEvent& event);

    ConfigSettings getOrCreateConfig() const;
    bool haveConfigFile() const;
    bool readFromConfigFile(ConfigSettings& sets) const;
    ConfigSettings defaultSettings() const;
    void applySettings(const ConfigSettings&);
    void saveSettingsToFile(const ConfigSettings&);

    CentralPanel* _panel;
    LogFrame* _frame;
    std::shared_ptr<core::TextGameEngine> _engine;
    std::shared_ptr<core::ITolk> _tolker;
    std::shared_ptr<core::ISoundPlayer> _player;
    const wxString _conf_file_name;
    bool _enable_tolk;

    //Подключение таблицы событий
    wxDECLARE_EVENT_TABLE();
};