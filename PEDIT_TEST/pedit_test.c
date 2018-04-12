#include <windows.h>
#include <commctrl.h>
#include "scintilla.h"
#include "resource.h"

HWND hpedit=0;
HINSTANCE ghinstance=0;

int setup_debug(HWND hwnd)
{
	RECT rect;
	GetWindowRect(hwnd,&rect);
	open_console();
	resize_console(120,40,5000);
	move_console(0,rect.bottom);
	return 0;
}

int setup_panels(HWND hwnd)
{
	int result=FALSE;
	HWND hedit=0;
	//add_statusbar(hwnd);
	add_menubar(hwnd);

	add_edit_pane(hwnd);
	add_edit_pane(hwnd);
	add_edit_pane(hwnd);
	add_edit_pane(hwnd);
	add_edit_pane(hwnd);
//	add_edit(&hedit);
	tile_panes(hwnd);
	if(hedit){
		char *tmp=malloc(0x100000);
		int i;
		char *p=tmp;
		SetFocus(hedit);
		memset(tmp,0,0x100000);
		for(i=0;i<20;i++){
			if(i==10){
				strcpy(p,"1234567890\r\n");
				p+=sizeof("1234567890\r\n")-1;
			}
			else{
				strcpy(p,"123\r\n");
				p+=(sizeof("123\r\n")-1);
			}
		}
		SendMessage(hedit,SCI_SETTEXT,0,tmp);
	}

	/*
	hscint=CreateWindowEx(0,TEXT("Scintilla"),TEXT("Main edit"),WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_CLIPCHILDREN,
				0,0,100,100,hwnd,0,ghinstance,0);
*/
	return result;
}
static int LMB=0;
static int DRAG=0;
static POINT pclick={0};

LRESULT CALLBACK DialogProc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
	if(!(msg==WM_SETCURSOR || msg==WM_NCHITTEST || msg==WM_MOUSEFIRST || msg==WM_NCMOUSEMOVE)){
		printf("--");
		print_msg(msg,wparam,lparam,hwnd);
	}

	switch(msg){
	case WM_INITDIALOG:
		break;
	case WM_SIZE:
	case WM_SIZING:
	case WM_MOVE:
	case WM_MOVING:
		window_move(hwnd);
		break;
	case WM_KEYDOWN:
		{
			extern HWND hmovwin;
			int key=wparam;
			printf("key=%02X\n",wparam);
			if(key==VK_ESCAPE){
				PostQuitMessage(0);
			}
		}
		break;
	case WM_MOUSEMOVE:
		{
			int x,y;
			x=LOWORD(lparam);
			y=HIWORD(lparam);
			if(LMB)
				DRAG=1;
		}
		break;
	case WM_LBUTTONDOWN:
		{
			int x,y;
			x=LOWORD(lparam);
			y=HIWORD(lparam);
			LMB=1;
			DRAG=0;
		}
		break;
	case WM_LBUTTONUP:
		{
			LMB=0;
			DRAG=0;
		}
		break;
	case WM_PAINT:
		{
			paint_main_win(hwnd);
		}
		break;
	case WM_COMMAND:
		{
			int id=LOWORD(wparam);
			switch(id){
			case IDCANCEL:
//				PostMessage(hwnd,WM_CLOSE,0,0);
				break;
			}
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		PostQuitMessage(0);
		return 1;
		break;
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hinstance,HINSTANCE hprevinstance,LPSTR cmdline,int cmdshow)
{

	INITCOMMONCONTROLSEX ctrls;

	ghinstance=hinstance;
	Scintilla_RegisterClasses(hinstance);

	ctrls.dwSize=sizeof(ctrls);
	ctrls.dwICC=ICC_LISTVIEW_CLASSES|ICC_TREEVIEW_CLASSES|ICC_BAR_CLASSES|ICC_TAB_CLASSES|ICC_PROGRESS_CLASS|ICC_HOTKEY_CLASS;
	InitCommonControlsEx(&ctrls);

	hpedit=CreateDialog(hinstance,MAKEINTRESOURCE(IDD_PEDIT),0,DialogProc);
	if(hpedit==0){
		MessageBox(0,TEXT("Unable to create Window"),TEXT("ERROR"),MB_OK|MB_SYSTEMMODAL);
		return 0;
	}
	setup_debug(hpedit);
	ShowWindow(hpedit,SW_SHOW);
	setup_panels(hpedit);
	while (1){
		int ret;
		MSG msg;
		ret=GetMessage(&msg,NULL,0,0);
		if(-1==ret || 0==ret){
			break;
		}else{
		//if(!IsDialogMessage(hpedit,&msg)){
			//TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
//		else{
//			print_msg(msg.message,msg.lParam,msg.wParam,msg.hwnd);
//		}
	}
	return 0;
}