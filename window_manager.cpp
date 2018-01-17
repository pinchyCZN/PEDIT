#include "window_manager.h"


bool PEDIT::AddEdit(const wxString &title)
{
	bool result=false;
	wxStyledTextCtrl *sec;
	sec=new wxStyledTextCtrl(this);
	if(m_notebook.AddPage(sec,title)){
		int i=m_notebook.GetPageCount();

	}
	return result;
}
void PEDIT::SetupPanes()
{
	typedef wxSystemSettings wss;
	wxColour tmp;
	
	m_mgr.SetManagedWindow(this);
	m_notebook.Create(this,wxID_ANY);
	wxAuiSimpleTabArt *tab=new wxAuiSimpleTabArt;
	m_notebook.SetArtProvider(tab);
	
	/*
	if(0==edit)
	return;
	//	tmp.Set(
	edit->StyleSetBackground(wxSTC_STYLE_DEFAULT,wss::GetColour(wxSYS_COLOUR_WINDOW));
	edit->StyleSetForeground(wxSTC_STYLE_DEFAULT,wss::GetColour(wxSYS_COLOUR_WINDOWTEXT));
	edit->StyleSetForeground(wxSTC_STYLE_DEFAULT,wss::GetColour(wxSYS_COLOUR_WINDOWTEXT));
	*/
	//wxStyledTextCtrl *e=new wxStyledTextCtrl(this);
	//m_mgr.AddPane(e,wxLeft,wxT("123"));
	//e=new wxStyledTextCtrl(this);
	//m_mgr.AddPane(e,wxRight,wxT("233"));
	
	wxTextCtrl* text1 = new wxTextCtrl(this, -1, wxT("Pane 1 - sample text"),
		wxDefaultPosition, wxSize(200,150),
		wxNO_BORDER | wxTE_MULTILINE);
	
	wxTextCtrl* text2 = new wxTextCtrl(this, -1, wxT("Pane 2 - sample text"),
		wxDefaultPosition, wxSize(200,150),
		wxNO_BORDER | wxTE_MULTILINE);
	
	wxTextCtrl* text3 = new wxTextCtrl(this, -1, wxT("Main content window"),
		wxDefaultPosition, wxSize(200,150),
		wxNO_BORDER | wxTE_MULTILINE);
	// add the panes to the manager
	m_mgr.AddPane(text1, wxLEFT, wxT("Pane Number One"));
	m_mgr.AddPane(text2, wxBOTTOM, wxT("Pane Number Two"));
	m_mgr.AddPane(text3, wxLEFT);
	AddEdit(wxT("tert"));
	AddEdit(wxT("tert"));
	AddEdit(wxT("tert"));
	m_mgr.AddPane(&m_notebook,wxCENTER);
	m_mgr.Update();
	
}
void PEDIT::OnKey(wxKeyEvent &event)
{
	if(event.m_keyCode==VK_ESCAPE){
		Close();
	}
}
PEDIT::~PEDIT()
{
	m_mgr.UnInit();
}


BEGIN_EVENT_TABLE(PEDIT,wxFrame)
EVT_KEY_DOWN(PEDIT::OnKey)
END_EVENT_TABLE()
