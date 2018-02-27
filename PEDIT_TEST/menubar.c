#include <windows.h>
#include "resource.h"
#include "anchor_system.h"

extern HINSTANCE ghinstance;
HWND hmenubar=0;

WNDPROC menu_proc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
	switch(msg){
	}
	return DefWindowProc(hwnd,msg,wparam,lparam);
}

int add_menubar(HWND hparent)
{
	int result=FALSE;
	int id;
	ATOM menu_class;
	if(hmenubar)
		return TRUE;
	menu_class=get_menu_panel_atom(menu_proc);
	id=get_new_child_id();
	hmenubar=CreateWindow(menu_class,TEXT("MENU"),WS_CHILD|WS_VISIBLE,
		0,0,0,0,hparent,id,ghinstance,NULL);
	if(hmenubar){
		RECT rect;
		int w,h;
		GetClientRect(hparent,&rect);
		w=rect.right-rect.left;
		h=GetSystemMetrics(SM_CYMENU);
		SetWindowPos(hmenubar,NULL,0,0,w,h,SWP_SHOWWINDOW|SWP_NOZORDER);
		add_pane(hparent,hmenubar,ANCHOR_LEFT|ANCHOR_RIGHT|ANCHOR_TOP,id);
		result=TRUE;
	}
	return result;
}