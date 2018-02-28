#include <windows.h>
#include "scintilla.h"
#include "scilexer.h"
#include "resource.h"
#include "anchor_system.h"
extern HINSTANCE ghinstance;
extern HWND hpedit;

HWND hedit_pane;

struct EDIT_CONTROL{
	HWND hscint;
	int val;
};

struct EDIT_CONTROL *edit_list=0;
unsigned int edit_count=0;

static LRESULT (CALLBACK *old_wproc_edit)(HWND,UINT,WPARAM,LPARAM)=0;

WNDPROC wproc_edit_pane(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
//	if(!(msg==WM_SETCURSOR || msg==WM_NCHITTEST || msg==WM_MOUSEFIRST || msg==WM_NCMOUSEMOVE))
		print_msg(msg,wparam,lparam);

	switch(msg){
	case WM_NOTIFY:
		{
			LPNMHDR nmhdr=(LPNMHDR)lparam;
			struct SCNotification *scn=(struct SCNotification *)lparam;
			printf("code=%08X %i\n",nmhdr->code,nmhdr->code);
			switch(nmhdr->code){
			case SCN_KEY:
				if(scn->ch==VK_ESCAPE)
					PostQuitMessage(0);
				break;
			}
		}
		break;
	}
	return DefWindowProc(hwnd,msg,wparam,lparam);
}

int add_edit_pane(HWND hparent)
{
	int result=FALSE;
	int id;
	ATOM edit_class;
	id=get_new_child_id();
	edit_class=get_edit_panel_atom(wproc_edit_pane);
	hedit_pane=CreateWindow(edit_class,TEXT("EDIT_PANEL"),WS_CHILD|WS_VISIBLE,
			0,0,0,0,hparent,id,ghinstance,0);
	if(hedit_pane){
		RECT rect={0};
		int w,h,x,y,id;
		old_wproc_edit=SetWindowLong(hedit_pane,GWL_WNDPROC,(LONG)wproc_edit_pane);
		get_max_edit_area(&rect);
		x=rect.left;
		y=rect.top;
		w=rect.right-rect.left;
		h=rect.bottom-rect.top;

		SetWindowPos(hedit_pane,NULL,x,y,w,h,SWP_NOZORDER|SWP_SHOWWINDOW);
		id=get_new_child_id();
		add_pane(hparent,hedit_pane,ANCHOR_LEFT|ANCHOR_RIGHT|ANCHOR_TOP|ANCHOR_BOTTOM,id);
		result=TRUE;
	}
	return result;
}

DWORD getRGB(COLORREF c)
{
	return (GetRValue(c)<<16)|(GetGValue(c)<<8)|GetBValue(c);
}
int setup_scint(HWND hscint)
{
	COLORREF fg,bg;
	HFONT hfont;
	fg=GetSysColor(COLOR_WINDOWTEXT);
	bg=GetSysColor(COLOR_WINDOW);
	fg=getRGB(fg);
	bg=getRGB(bg);
	SendMessage(hscint,SCI_SETCARETWIDTH,2,0);
	SendMessage(hscint,SCI_SETCARETPERIOD,600,0);
	SendMessage(hscint,SCI_SETCARETLINEVISIBLE,1,0);
	SendMessage(hscint,SCI_SETCARETLINEBACK,0x3F1F00,0);
	SendMessage(hscint,SCI_SETCARETFORE,0xFFFFFF,0);
	
	SendMessage(hscint,SCI_STYLESETBACK,STYLE_DEFAULT,bg);
	SendMessage(hscint,SCI_STYLESETFORE,STYLE_DEFAULT,fg);
	SendMessage(hscint,SCI_STYLESETFONT,STYLE_DEFAULT,"Fixedsys");
	SendMessage(hscint,SCI_STYLESETBOLD,STYLE_DEFAULT,0);
	SendMessage(hscint,SCI_STYLESETITALIC,STYLE_DEFAULT,0);
	SendMessage(hscint,SCI_STYLESETUNDERLINE,STYLE_DEFAULT,0);
	SendMessage(hscint,SCI_STYLECLEARALL,0,0);
	SendMessage(hscint,SCI_SETLEXER,SCLEX_NULL,0);
	SendMessage(hscint,SCI_SETSTYLEBITS,8,0);

	//SendMessage(hscint,SCI_SETSTYLING,0xFFFFFFFF,STYLE_DEFAULT);
	return 0;
}
int add_edit()
{
	int result=FALSE;
	HWND hwnd;
	struct EDIT_CONTROL *tmp=0;
	RECT rect;
	int w,h,x,y;
	if(0==hedit_pane)
		return result;
	hwnd=CreateWindowEx(0,TEXT("Scintilla"),TEXT("Main edit"),WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_CLIPCHILDREN,
				0,0,0,0,hedit_pane,0,ghinstance,0);
	if(0==hwnd)
		return result;
	setup_scint(hwnd);
	GetWindowRect(hedit_pane,&rect);
	MapWindowPoints(NULL,hedit_pane,(PPOINT)&rect,2);
	w=rect.right-rect.left;
	h=rect.bottom-rect.top;
	x=rect.left;
	y=rect.top;
	SetWindowPos(hwnd,NULL,x,y,w,h,SWP_NOZORDER|SWP_SHOWWINDOW);
	edit_count++;
	tmp=realloc(edit_list,edit_count*sizeof(struct EDIT_CONTROL));
	if(tmp){
		int index=edit_count-1;
		edit_list=tmp;
		if(index<0)
			return result;
		edit_list[index].hscint=hwnd;
		edit_list[index].val=0;
		result=TRUE;
	}
	return result;
}