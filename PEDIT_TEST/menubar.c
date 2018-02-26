#include <windows.h>
#include "resource.h"

extern HINSTANCE ghinstance;
HWND hmenubar=0;

BOOL CALLBACK menu_proc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
	return 0;
}

int add_menubar(HWND hwnd)
{
	int result=FALSE;
	RECT rect;
	int w,h,id;
	if(hmenubar)
		return TRUE;
	GetClientRect(hwnd,&rect);
	w=rect.right-rect.left;
	h=30;
	id=get_new_child_id();
	hmenubar=CreateDialog(ghinstance,MAKEINTRESOURCE(IDD_MENUBAR),hwnd,menu_proc);
	SetWindowPos(hmenubar,NULL,0,0,200,40,SW_SHOW);
	//hmenubar=CreateWindow(MAKEINTRESOURCE(IDD_MENUBAR),TEXT("MENU"),WS_VISIBLE|WS_CHILD,
	//	0,0,w,h,hwnd,id,ghinstance,0);
	if(hmenubar){
		result=TRUE;
		id=get_new_child_id();
		CreateWindow(("BUTTON"),"TEST",WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,
			0,0,80,20,hmenubar,id,ghinstance,0);
		print_lasterror();
	}
	return result;
}