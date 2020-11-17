#include "logframe.h"


LogFrame::LogFrame(wxWindow* parent, const wxString& title)
    : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxSize(300, 400),wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    _textCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
									  wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
}

void LogFrame::AddLog(const wxString& text)
{
    //_textCtrl->SetInsertionPoint(0);
    *_textCtrl<<text;
}

void LogFrame::ClearLog()
{
    _textCtrl->Clear();
}
