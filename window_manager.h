#ifndef _WINDOW_MANAGER_H_
#define _WINDOW_MANAGER_H_
#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <wx/aui/framemanager.h>
#include <wx/aui/auibook.h>
#include <wx/stc/stc.h>
#include <wx/settings.h>
#include "notebook_manager.h"

class PEDIT: public wxFrame
{
public:
    PEDIT(wxWindow *parent=NULL, wxWindowID id=wxID_ANY,
                    const wxString& title = wxT("PEDIT"),
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxSize(400, 400),
                    long style = wxDEFAULT_FRAME_STYLE,
                    const wxString& name = wxT("wxEditorFrame")) : wxFrame()
    {
        Init();
        Create(parent, id, title, pos, size, style, name);
    }
	~PEDIT();
    wxAuiManager m_mgr;
	wxAuiNotebook m_notebook;

	bool AddEdit(const wxString &title=wxT("BLANK"));
	void SetupPanes();
	void OnKey(wxKeyEvent& event);
	DECLARE_EVENT_TABLE();
};

#endif //_WINDOW_MANAGER_H_
