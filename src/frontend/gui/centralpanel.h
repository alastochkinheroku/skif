#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "parserlineinput.h"
#include "TextGameEngine.h"

class CentralPanel: public wxPanel
{
public:
    CentralPanel(std::shared_ptr<core::TextGameEngine> engine, wxFrame *frame, int x, int y, int w, int h);
    wxTextCtrl* getOutCtrl();
private:
    wxTextCtrl *_multiout;
	ParserLineInput *_linein;
};