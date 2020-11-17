#pragma once

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include "TextGameEngine.h"

enum {WX_PARSER_ID=5000};

class ParserLineInput : public wxTextCtrl
{
public:
	ParserLineInput(std::shared_ptr<core::TextGameEngine> engine, wxWindow* parent, wxWindowID id, const wxString& value,
        			const wxPoint& pos, const wxSize& size, int style = 0);
    void OnKeyDown(wxKeyEvent& event);
    void OnTextEnter(wxCommandEvent& event);
private:
    std::shared_ptr<core::TextGameEngine> _engine;

    wxDECLARE_EVENT_TABLE();
};
