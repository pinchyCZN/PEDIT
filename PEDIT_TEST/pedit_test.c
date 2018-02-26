#include <windows.h>
#include <commctrl.h>
#include "scintilla.h"
#include "resource.h"

HWND hpedit=0;
HWND hscint=0;
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
//int setup_
int setup_panels(HWND hwnd)
{
	int result=FALSE;
	add_statusbar(hwnd);
	add_menubar(hwnd);
	//add_edit_pane(hwnd);
	//add_edit();
	/*
	hscint=CreateWindowEx(0,TEXT("Scintilla"),TEXT("Main edit"),WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_CLIPCHILDREN,
				0,0,100,100,hwnd,0,ghinstance,0);
*/
	return result;
}
BOOL CALLBACK DialogProc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{

	switch(msg){
	case WM_INITDIALOG:
		{
			setup_debug(hwnd);
			setup_panels(hwnd);
		}
		break;
	case WM_MOVE:
	case WM_MOVING:
		window_move(hwnd);
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

	int ret;
	MSG msg;
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
	ShowWindow(hpedit,SW_SHOW);
	while ((ret=GetMessage(&msg,NULL,0,0))!=0){
		if (ret==-1){
			break;
		}
		else if(!IsDialogMessage(hpedit,&msg)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return 0;
}