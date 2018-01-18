#include "window_manager.h"
#include "tab_art.h"
#define wxStyledTextEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxStyledTextEventFunction, &func)

void PEDIT::onkey(wxStyledTextEvent &evt)
{
	static int i=0;
	int tmp;
	tmp=evt.GetModifiers();
	tmp=evt.GetModificationType();
	printf("tmp=%i\n",tmp);
	if(tmp&(wxSTC_MOD_INSERTTEXT|wxSTC_MOD_DELETETEXT)){
		wxStyledTextCtrl *stc;
		//=(wxStyledTextCtrl*)(m_notebook.GetPage(0));
		stc=(wxStyledTextCtrl *)evt.GetEventObject();
		stc->StyleSetBackground(i,wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
		printf("i=%i\n",i);
		i++;
	}


}
void SetupEditor(wxStyledTextCtrl &stc)
{
	stc.StyleSetBackground(wxSTC_STYLE_DEFAULT,wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
	stc.StyleSetForeground(wxSTC_STYLE_DEFAULT,wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));
	stc.StyleSetForeground(wxSTC_STYLE_DEFAULT,wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));
}

bool PEDIT::AddEdit(const wxString &title,wxAuiNotebook &book)
{
	wxStyledTextCtrl *stc;
	stc=new wxStyledTextCtrl(this);
	SetupEditor(*stc);
	//stc->Connect(wxEVT_STC_KEY,wxStyledTextEventFunction(PEDIT::onkey));
	stc->Connect(wxEVT_STC_MODIFIED,wxStyledTextEventHandler(PEDIT::onkey));
		//(wxObject *)stc);
	return book.AddPage(stc,title);
}
bool PEDIT::AddEdit(const wxString &title)
{
	return AddEdit(title,m_notebook);
}
bool PEDIT::AddAltEdit(const wxString &title)
{
	return AddEdit(title,m_altbook);
}
void SetupStatusBar(wxStatusBar &sbar)
{
	int widths[4];
	widths[0]=-1;
	widths[1]=40;
	widths[2]=40;
	widths[3]=40;
	sbar.SetFieldsCount(4,widths);
}
void PEDIT::SetupPanes()
{
	typedef wxSystemSettings wss;
	wxColour tmp;
	
	m_mgr.SetManagedWindow(this);

	m_statusbar.Create(this,wxID_ANY);
	SetupStatusBar(m_statusbar);
	m_statusbar.SetStatusText(wxT("qwe"));
	m_statusbar.SetStatusText(wxT("qwe"),1);
	m_statusbar.SetStatusText(wxT("qwe"),2);
	m_statusbar.SetStatusText(wxT("qwe"),3);
	SetStatusBar(&m_statusbar);

	m_notebook.Create(this,wxID_ANY);
	m_altbook.Create(this,wxID_ANY);
	CustomTabArt *tab=new CustomTabArt;
	m_notebook.SetArtProvider(tab);
	tab=new CustomTabArt;
	m_altbook.SetArtProvider(tab);

	
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
	AddAltEdit(wxT("werwer"));
	m_notebook.GetPage(0);
	m_mgr.AddPane(&m_notebook,wxCENTER,wxT("234243"));
	m_mgr.AddPane(&m_altbook,wxCENTER,wxT("345345"));
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
}


BEGIN_EVENT_TABLE(PEDIT,wxFrame)
EVT_KEY_DOWN(PEDIT::OnKey)
END_EVENT_TABLE()
