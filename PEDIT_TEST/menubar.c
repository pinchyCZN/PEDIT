#include <windows.h>
#include "resource.h"
#include "anchor_system.h"

extern HINSTANCE ghinstance;
extern HWND hpedit;
HWND hmenubar=0;
HMENU hmenu=0;


WNDPROC menu_proc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
//	print_msg(msg,wparam,lparam);
	switch(msg){
	case WM_SETCURSOR:
		{
			HCURSOR harrow;
			harrow=LoadCursor(NULL,IDC_ARROW);
			if(harrow)
				SetCursor(harrow);
		}
		break;
	}
	return DefWindowProc(hwnd,msg,wparam,lparam);
}

int setup_menubar(HWND hbar)
{
	int result=FALSE;
	RECT rect;
	int id,w,h;
	GetClientRect(hbar,&rect);
	h=rect.bottom-rect.top;
	return result;
}
int add_menubar(HWND hparent)
{
	int result=FALSE;
	if(hmenu)
		return TRUE;
	hmenu=LoadMenu(ghinstance,MAKEINTRESOURCE(IDR_MAINMENU));
	if(hmenu){
		RECT rect;
		int w,h;
		SetMenu(hparent,hmenu);
		adjust_for_menu();
		result=TRUE;
	}
	return result;
}
int show_menu(int show)
{
	return SetMenu(hpedit,show?hmenu:NULL);
}