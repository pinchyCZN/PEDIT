#define WINVER 0x500
#define _WIN32_WINNT 0x500
#include <windows.h>
#include "scintilla.h"

int WINAPI WinMain(
    HINSTANCE hInstance,	// handle to current instance
    HINSTANCE hPrevInstance,	// handle to previous instance
    LPSTR lpCmdLine,	// pointer to command line
    int nCmdShow 	// show state of window
				   )
{
	HWND hwndParent=0;
	HWND hwndScintilla;

	Scintilla_RegisterClasses(hInstance);
	hwndScintilla = CreateWindowEx(0,
		"Scintilla","", WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN,
		10,10,500,400,hwndParent,(HMENU)0, hInstance,NULL);
	ShowWindow(hwndScintilla,SW_SHOW);
	while(1){

	}
}