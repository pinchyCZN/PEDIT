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

int LMB=0;
WNDPROC mov_win_proc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
//	if(!(msg==WM_PAINT || msg==WM_NCHITTEST || msg==WM_MOUSEFIRST || msg==WM_NCMOUSEMOVE))
//		print_msg(msg,wparam,lparam,hwnd);
	switch(msg){
	case WM_CREATE:

		break;
	case WM_PAINT:
		{
			HDC hdc=GetDC(hwnd);
			HBRUSH hbr;
			if(!hdc)
				break;
			hbr=CreateHatchBrush(HS_DIAGCROSS,0x7F007F);
			if(hbr){
				RECT rect;
				GetClientRect(hwnd,&rect);
				SelectObject(hdc,hbr);
				Rectangle(hdc,rect.left,rect.top,rect.right,rect.bottom);
				DeleteObject(hbr);
			}
			ReleaseDC(hwnd,hdc);
		}
		return 0;
		break;
	}
	return DefWindowProc(hwnd,msg,wparam,lparam);
}

HWND hmovwin=0;
int movx=0,movy=0;
int create_mov_window(HWND hparent)
{
	int result=FALSE;
	if(hmovwin){
		ShowWindow(hmovwin,SW_SHOW);
		result=TRUE;
		return result;
	}else{
		ATOM pclass;
		HWND hwnd;
		int id;
		{
			WNDCLASSA wclass={0};
			wclass.hInstance=ghinstance;
			wclass.hCursor=NULL;
			wclass.hbrBackground=(HBRUSH)(COLOR_BACKGROUND+1); //(HBRUSH)(COLOR_BTNFACE+1);
			wclass.lpszClassName="MOV_WINDOW";
			wclass.lpfnWndProc=(WNDPROC)mov_win_proc;
			pclass=RegisterClass(&wclass);
		}

		//pclass=get_panel_atom("MOV_WINDOW",mov_win_proc);
		id=get_new_child_id();
		hwnd=CreateWindow(pclass,TEXT("MOV_WIN"),WS_CHILD|WS_VISIBLE,
						0,0,20,20,hparent,id,ghinstance,0);
		hmovwin=hwnd;
	}
	return result;
}

WNDPROC wproc_edit_pane(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
//	if(!(msg==WM_NCHITTEST || msg==WM_MOUSEFIRST || msg==WM_NCMOUSEMOVE
//		))//|| msg==WM_NOTIFY))
//	print_msg(msg,wparam,lparam);
	switch(msg){
	case WM_SIZE:
		{
			resize_edit_list(hwnd);
			InvalidateRect(hwnd,NULL,TRUE);
		}
		break;
	case WM_SETCURSOR:
		SetCursor(LoadCursor(NULL,IDC_ARROW));
		break;
	case WM_LBUTTONDOWN:
		{
		}
		break;
	case WM_LBUTTONUP:
		{
			static int show=FALSE;
			//show_menu(show);
			show=!show;
			//adjust_for_menu();
			if(LMB){
				InvalidateRect(GetParent(hwnd),NULL,TRUE);
			}
			ShowWindow(hmovwin,SW_HIDE);
			LMB=0;
		}
		break;
	case WM_KEYDOWN:
		{
			int key=wparam;
			if(key==VK_ESCAPE){
				LMB=0;
				if(hmovwin)
					ShowWindow(hmovwin,SW_HIDE);
				InvalidateRect(GetParent(hwnd),NULL,TRUE);
			}
		}
		break;
	case WM_MOUSEMOVE:
		{
			int keys=wparam;
			if(keys&MK_LBUTTON){
				if(!LMB)
					LMB=1;
			}
			if(LMB){
				POINT pt;
				HWND htmp=GetParent(hwnd);
				if(htmp){
					create_mov_window(htmp);
				}
				pt.x=LOWORD(lparam);
				pt.y=HIWORD(lparam);
				MapWindowPoints(hwnd,htmp,&pt,1);
				movx=pt.x-4;
				movy=pt.y-4;
				if(hmovwin)
					SetWindowPos(hmovwin,NULL,movx,movy,0,0,SWP_NOSIZE|SWP_NOZORDER);
			}
		}
		break;
	case WM_PAINT:
		{
			RECT rect,ro;
			PAINTSTRUCT ps={0};
			HDC hdc=BeginPaint(hwnd,&ps);
			HBRUSH hbr;
			if(!hdc)
				break;
			GetClientRect(hwnd,&rect);
			ro=rect;
			FillRect(hdc,&rect,(HBRUSH)(COLOR_BTNFACE+1));
			hbr=CreateSolidBrush(0x0000FF);
			if(hbr){
				FrameRect(hdc,&rect,hbr);
				DeleteObject(hbr);
			}
			rect.left+=10;
			rect.top+=4;
			rect.right-=20;
			if(rect.right>0){
				rect.bottom=rect.top+3;
				DrawEdge(hdc,&rect,BDR_RAISEDINNER,BF_RECT);
				rect.top+=4;
				rect.bottom+=4;
				DrawEdge(hdc,&rect,BDR_RAISEDINNER,BF_RECT);
			}
			EndPaint(hwnd,&ps);
			
			if(LMB){
			}
			printf("%08X %i %i\n",hwnd,rect.left,rect.right);
			return 0;
		}
		break;
	case WM_NOTIFY:
		{
			LPNMHDR nmhdr=(LPNMHDR)lparam;
			HWND hscint=nmhdr->hwndFrom;
			int code=nmhdr->code;
			if(nmhdr->code!=2013)
				printf("code=%08X %i\n",nmhdr->code,nmhdr->code);
			switch(code){
			case SCN_UPDATEUI:
				clear_highlight(hscint);
				highlight_match(hscint);
				break;
			}
		}
		break;
	}
	return DefWindowProc(hwnd,msg,wparam,lparam);
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
	ATOM edit_class;
	id=get_new_child_id();
	edit_class=get_edit_panel_atom(wproc_edit_pane);
	hedit_pane=CreateWindow(edit_class,TEXT("EDIT_PANEL"),WS_CHILD|WS_VISIBLE,
			0,0,0,0,hparent,id,ghinstance,0);
	if(hedit_pane){
		RECT rect={0};
		int w,h,x,y;
		//old_wproc_edit=SetWindowLong(hedit_pane,GWL_WNDPROC,(LONG)wproc_edit_pane);
		w=GetDlgCtrlID(hedit_pane);
		printf("%i %i\n",id,w);
		get_max_edit_area(&rect);
		x=rect.left;
		y=rect.top;
		w=rect.right-rect.left;
		h=rect.bottom-rect.top;
//		w/=2;
//		h/=2;

		SetWindowPos(hedit_pane,NULL,x,y,w,h,SWP_NOZORDER|SWP_SHOWWINDOW);
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