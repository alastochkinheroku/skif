#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

//Окно логирования
class LogFrame : public wxDialog
{
public:
    LogFrame(wxWindow* parent, const wxString& title);
    void AddLog(const wxString& text);
    void ClearLog();
private:
    wxTextCtrl* _textCtrl;
};