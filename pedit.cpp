#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <wx/stc/stc.h>
#include <wx/settings.h>

class PEDIT: public wxFrame
{
public:
    PEDIT() : edit(0), wxFrame() { Init(); }
    PEDIT(wxWindow *parent, wxWindowID id,
                    const wxString& title = wxT("PEDIT"),
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxSize(400, 400),
                    long style = wxDEFAULT_FRAME_STYLE,
                    const wxString& name = wxT("wxEditorFrame")) : edit(0),wxFrame()
    {
        Init();
        Create(parent, id, title, pos, size, style, name);
    }
	wxStyledTextCtrl *edit;

	void SetupEditWin();
	void OnKey(wxKeyEvent& event);
	DECLARE_EVENT_TABLE();
};



void PEDIT::SetupEditWin()
{
	typedef wxSystemSettings wss;
	wxColour tmp;
	if(0==edit)
		return;
//	tmp.Set(
	edit->StyleSetBackground(wxSTC_STYLE_DEFAULT,wss::GetColour(wxSYS_COLOUR_WINDOW));
	edit->StyleSetForeground(wxSTC_STYLE_DEFAULT,wss::GetColour(wxSYS_COLOUR_WINDOWTEXT));
	edit->StyleSetForeground(wxSTC_STYLE_DEFAULT,wss::GetColour(wxSYS_COLOUR_WINDOWTEXT));

}
void PEDIT::OnKey(wxKeyEvent &event)
{
	if(event.m_keyCode==VK_ESCAPE){
		Close();
	}
}

BEGIN_EVENT_TABLE(PEDIT,wxFrame)
	EVT_KEY_DOWN(PEDIT::OnKey)
END_EVENT_TABLE()




class PEDIT_APP: public wxApp
{
public:
    PEDIT_APP() : wxApp(), m_frame(NULL) {}
    virtual bool OnInit();
    virtual int OnExit();

//    void CreateShell();
//    void OnMenuEvent(wxCommandEvent& event);

    PEDIT* m_frame;
};
bool PEDIT_APP::OnInit()
{
	m_frame=new PEDIT(NULL,wxID_ANY);
	m_frame->Show();
	m_frame->edit=new wxStyledTextCtrl(m_frame);
	m_frame->SetupEditWin();

	return true;
}
int PEDIT_APP::OnExit()
{
	return 0;
}
IMPLEMENT_APP(PEDIT_APP);