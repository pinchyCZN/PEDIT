#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <wx/aui/framemanager.h>
#include <wx/stc/stc.h>
#include <wx/settings.h>

#include "window_manager.h"

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
extern "C" void open_console();
bool PEDIT_APP::OnInit()
{
#ifdef _DEBUG
	open_console();
#endif

	m_frame=new PEDIT(NULL);
	m_frame->SetupPanes();
	SetTopWindow(m_frame);
	m_frame->Show();
	return true;
}
int PEDIT_APP::OnExit()
{
	return 0;
}
DECLARE_APP(PEDIT_APP);
IMPLEMENT_APP(PEDIT_APP);