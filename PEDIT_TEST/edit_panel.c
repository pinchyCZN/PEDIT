#include <windows.h>
#include "scintilla.h"
#include "resource.h"
extern HINSTANCE ghinstance;

HWND hedit_pane;

struct EDIT_CONTROL{
	HWND hscint;
	int val;
};

struct EDIT_CONTROL *edit_list=0;
int edit_count=0;

BOOL CALLBACK dlgfunc_edit(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
	if(!(msg==WM_SETCURSOR || msg==WM_NCHITTEST || msg==WM_MOUSEFIRST || msg==WM_NCMOUSEMOVE))
		print_msg(msg,wparam,lparam);

	switch(msg){
	case WM_MOVE:
		break;
	}
	return 0;
}

int add_edit_pane(HWND hwnd)
{
	int result=FALSE;
	hedit_pane=CreateDialog(ghinstance,MAKEINTRESOURCE(IDD_EDITPANE),hwnd,dlgfunc_edit);
	if(hedit_pane){
		RECT rect;
		int x,y,w,h;
		GetClientRect(hwnd,&rect);
		MapWindowPoints(hwnd,NULL,(LPPOINT)&rect,2);
		x=rect.left;
		y=rect.top;
		w=rect.right-rect.left;
		h=rect.bottom-rect.top;
		h-=25;
		SetWindowPos(hedit_pane,NULL,x,y,w,h,SWP_NOZORDER|SWP_SHOWWINDOW);
		result=TRUE;
	}
	return result;
}

int add_edit()
{
	int result=FALSE;
	HWND hwnd;
	struct EDIT_CONTROL *tmp=0;
	if(0==hedit_pane)
		return result;
	hwnd=CreateWindowEx(0,TEXT("Scintilla"),TEXT("Main edit"),WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_CLIPCHILDREN,
				0,0,100,100,hedit_pane,0,ghinstance,0);
	if(0==hwnd)
		return result;
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