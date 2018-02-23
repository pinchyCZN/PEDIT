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
	//printf("tmp=%i\n",tmp);
	if(tmp&(wxSTC_MOD_INSERTTEXT|wxSTC_MOD_DELETETEXT)){
		wxStyledTextCtrl *stc;
		//=(wxStyledTextCtrl*)(m_notebook.GetPage(0));
		stc=(wxStyledTextCtrl *)evt.GetEventObject();
		//stc->StyleSetBackground(i,wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
		//printf("i=%i\n",i);
		i++;
	}

}
void SetupEditor(wxStyledTextCtrl &stc)
{
	wxColour fg,bg,cwin;
	fg=wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT);
	bg=wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW);
	cwin=wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE);

	stc.StyleSetForeground(wxSTC_STYLE_DEFAULT,fg);
	stc.StyleSetBackground(wxSTC_STYLE_DEFAULT,bg);

	stc.StyleSetForeground(0,fg);
	stc.StyleSetBackground(0,bg);

	//stc.SetMarginWidth(0,20);
	//stc.StyleSetForeground(wxSTC_STYLE_LINENUMBER,fg);
	//stc.StyleSetBackground(wxSTC_STYLE_LINENUMBER,cwin);
	//stc.SetMarginType(0,wxSTC_MARGIN_NUMBER);

}

bool PEDIT::AddEdit(const wxString &title,wxAuiNotebook &book)
{
	wxStyledTextCtrl *stc;
	stc=new wxStyledTextCtrl(this);
	SetupEditor(*stc);
	stc->Connect(wxEVT_KEY_DOWN,wxKeyEventHandler(PEDIT::OnKey));
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
//	text.Connect(
	
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
	AddAltEdit(wxT("werwer"));
	wxStyledTextCtrl *stc=(wxStyledTextCtrl*)m_notebook.GetPage(0);
	int i;
	for(i=0;i<1000;i++)
		stc->AddText(wxT("smaple text\r\n"));
	stc=(wxStyledTextCtrl*)m_altbook.GetPage(0);
	for(i=0;i<1000;i++)
		stc->AddText(wxT("smaple text\r\n"));
	m_mgr.AddPane(&m_notebook,wxCENTER,wxT("notebook"));
	m_mgr.AddPane(&m_altbook,wxCENTER,wxT("altbooka"));
	m_mgr.Update();
	
}
void PEDIT::TMP_EVENT(wxEvent &event)
{
	exit(0);
}
void PEDIT::OnKey(wxKeyEvent &event)
{
	if(event.m_keyCode==VK_ESCAPE){
		wxCloseEvent tmp(wxEVT_CLOSE_WINDOW, m_windowId);
		tmp.SetEventObject(this);
		tmp.SetCanVeto(false);
		wxPostEvent(this,tmp);
		exit(0);
//		Close(true);
	}
	wxKeyEvent::DoAllowNextEvent();
}
PEDIT::~PEDIT()
{
	m_mgr.UnInit();
}


BEGIN_EVENT_TABLE(PEDIT,wxFrame)
EVT_KEY_DOWN(PEDIT::OnKey)
END_EVENT_TABLE()
