#include "parserlineinput.h"
#include "GLogger.h"

wxBEGIN_EVENT_TABLE(ParserLineInput, wxTextCtrl)
    EVT_KEY_DOWN(ParserLineInput::OnKeyDown)
    EVT_TEXT_ENTER(WX_PARSER_ID, ParserLineInput::OnTextEnter)
wxEND_EVENT_TABLE()

ParserLineInput::ParserLineInput(std::shared_ptr<core::TextGameEngine> engine, wxWindow *parent, wxWindowID id, const wxString &value,
               const wxPoint &pos, const wxSize &size, int style)
        : wxTextCtrl(parent, id, value, pos, size, style),
        _engine(engine)
{
}

void ParserLineInput::OnKeyDown(wxKeyEvent& event)
{
    switch (event.GetKeyCode())
    {
        case WXK_RETURN:
        {
            //wxLogMessage(wxT("Return hit!"));
            core::GLogger::get() << "Return hit!";
            break;
        }
        case WXK_UP: 
            //wxLogMessage(wxT("History up hit!"));
            core::GLogger::get() << "History up hit!";
            break;
        case WXK_DOWN:
            //wxLogMessage(wxT("History down hit!"));
            core::GLogger::get() << "History down hit!";
            break;
    }

    event.Skip();
}

void ParserLineInput::OnTextEnter(wxCommandEvent& event)
{
    //wxLogMessage(wxT("Event string: %s"), event.GetString());
    std::string inp(event.GetString().c_str());
    _engine->processUserInput(core::UserCommandType_String, inp);
    ParserLineInput* pinp = (ParserLineInput*)event.GetEventObject();
    if (pinp)
    {
        pinp->Clear();
    }
    //SetInsertionPoint(0);
    //WriteText(wxT(""));
}
