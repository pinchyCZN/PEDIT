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

int resize_edit_list(HWND hwnd)
{
	RECT rect;
	int w,h;
	unsigned int i;
	GetWindowRect(hwnd,&rect);
	w=rect.right-rect.top;
	h=rect.bottom-rect.top;
	w+=10;
	for(i=0;i<edit_count;i++){
		int x,y;
		struct EDIT_CONTROL *ec;
		ec=&edit_list[i];
		x=y=0;
		SetWindowPos(ec->hscint,NULL,0,y,w,h,SWP_NOZORDER|SWP_SHOWWINDOW);
	}
	return edit_count;
}

int set_transaparency(HWND hwnd,COLORREF ckey,int alpha)
{
#define WS_EX_LAYERED 0x00080000
#define LWA_COLORKEY 1
	int result=FALSE;
	static BOOL (WINAPI *SetLayeredWindowAttributes)(HWND,COLORREF,BYTE,DWORD)=0;
	if(0==SetLayeredWindowAttributes){
		static HMODULE huser=0;
		if(0==huser)
			huser=LoadLibrary(TEXT("user32.dll"));
		if(huser){
			SetLayeredWindowAttributes=GetProcAddress(huser,"SetLayeredWindowAttributes");
		}
	}
	SetWindowLong(hwnd,GWL_EXSTYLE,GetWindowLong(hwnd,GWL_EXSTYLE)|WS_EX_LAYERED);
	result=GetWindowLong(hwnd,GWL_EXSTYLE);
	print_lasterror();
	if(SetLayeredWindowAttributes){
		SetLayeredWindowAttributes(hwnd,ckey,alpha,LWA_COLORKEY);
		print_lasterror();
		result=TRUE;
	}
	return result;
}

static WNDPROC (*orig_scint)=0;
WNDPROC scint_subclass(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
	switch(msg){
	case WM_KEYDOWN:
		{
			int key=wparam;
			switch(key){
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			case VK_F1:
				{
					static int show=FALSE;
					show_menu(show);
					show=!show;
					adjust_for_menu();
				}
				break;
			}
		}
		break;
	}
	return CallWindowProc(orig_scint,hwnd,msg,wparam,lparam);
}
int add_edit_pane(HWND hparent)
{
	int result=FALSE;
	int id;
	id=get_new_child_id();
	add_pane(hparent,0,ANCHOR_LEFT|ANCHOR_RIGHT|ANCHOR_TOP|ANCHOR_BOTTOM,id);
	result=TRUE;
	return result;
}


DWORD getRGB(COLORREF c)
{
	return (GetRValue(c)<<16)|(GetGValue(c)<<8)|GetBValue(c);
}
int setup_scint(HWND hscint)
{
	COLORREF fg,bg;
	fg=GetSysColor(COLOR_WINDOWTEXT);
	bg=GetSysColor(COLOR_WINDOW);
	fg=getRGB(fg);
	bg=getRGB(bg);
	SendMessage(hscint,SCI_SETCARETWIDTH,2,0);
	SendMessage(hscint,SCI_SETCARETPERIOD,600,0);
	SendMessage(hscint,SCI_SETCARETLINEVISIBLE,1,0);
	SendMessage(hscint,SCI_SETCARETLINEBACK,0x3F1F00,0);
	SendMessage(hscint,SCI_SETCARETFORE,0xFFFFFF,0);
	SendMessage(hscint,SCI_SETVIRTUALSPACEOPTIONS,SCVS_RECTANGULARSELECTION,0); //rectangular selection
	
	SendMessage(hscint,SCI_STYLESETBACK,STYLE_DEFAULT,bg);
	SendMessage(hscint,SCI_STYLESETFORE,STYLE_DEFAULT,fg);
	SendMessage(hscint,SCI_STYLESETFONT,STYLE_DEFAULT,"Fixedsys");
	SendMessage(hscint,SCI_STYLESETBOLD,STYLE_DEFAULT,0);
	SendMessage(hscint,SCI_STYLESETITALIC,STYLE_DEFAULT,0);
	SendMessage(hscint,SCI_STYLESETUNDERLINE,STYLE_DEFAULT,0);
	SendMessage(hscint,SCI_STYLECLEARALL,0,0); //required for colors
	SendMessage(hscint,SCI_SETLEXER,SCLEX_NULL,0);
	SendMessage(hscint,SCI_SETSTYLEBITS,8,0);
	setup_highlight(hscint);
	return 0;
}
int add_edit(HWND *hedit)
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
	if(hedit)
		*hedit=hwnd;
	orig_scint=GetWindowLong(hwnd,GWL_WNDPROC);
	SetWindowLong(hwnd,GWL_WNDPROC,scint_subclass);
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