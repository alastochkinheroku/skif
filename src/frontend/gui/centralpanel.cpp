#include "centralpanel.h"


CentralPanel::CentralPanel(std::shared_ptr<core::TextGameEngine> engine, wxFrame *frame, int x, int y, int w, int h )
       : wxPanel( frame, wxID_ANY, wxPoint(x, y), wxSize(w, h) )
{

    _multiout = new wxTextCtrl( this, wxID_ANY, _("Help"),
               wxPoint(10, 120), wxSize(500,300), wxTE_MULTILINE | wxTE_READONLY );
    _multiout->SetInsertionPoint(0);
    _linein = new ParserLineInput(engine, this, WX_PARSER_ID, _(" "),
                             wxDefaultPosition, wxDefaultSize,
                            wxTE_PROCESS_ENTER);
    _linein->SetInsertionPoint(0);
    // lay out the controls
    wxBoxSizer *column1 = new wxBoxSizer(wxVERTICAL);
    column1->Add( _multiout, 1, wxALL | wxEXPAND, 10 );
    column1->Add( _linein, 0, wxALL | wxEXPAND, 10 );

    SetSizer(column1);
    _linein->SetFocus();
}

wxTextCtrl* CentralPanel::getOutCtrl()
{
    return _multiout;
}
