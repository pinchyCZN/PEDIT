#define WINVER 0x500
#define _WIN32_WINNT 0x500
#include <windows.h>
#include "scintilla.h"
#include "resource.h"

HWND hpedit=0;
HWND hscint=0;
BOOL CALLBACK DialogProc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{

	print_msg(msg,wparam,lparam);
	switch(msg){
	case WM_INITDIALOG:
		open_console();
		move_console(50,100);
		resize_console(120,40,5000);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 1;
		break;
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hinstance,HINSTANCE hprevinstance,LPSTR cmdline,int cmdshow)
{

	int ret;
	MSG msg;

	Scintilla_RegisterClasses(hinstance);

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