#include <windows.h>
#include <stdio.h>

#define _O_TEXT         0x4000  /* file mode is text (translated) */

HWND ghconsole=0;
int _open_osfhandle(long,int);
static DWORD(WINAPI *SetConsoleIcon)(HICON)=0;

void open_console()
{
	HWND hcon;
	FILE *hf;
	static BYTE consolecreated=FALSE;
	static int hcrt=0;
	static HWND (WINAPI *GetConsoleWindow)(void)=0;

	if(consolecreated==TRUE)
	{
		if(ghconsole!=0)
			ShowWindow(ghconsole,SW_SHOW);
		hcon=(HWND)GetStdHandle(STD_INPUT_HANDLE);
		FlushConsoleInputBuffer(hcon);
		return;
	}
	AllocConsole();
	hcrt=_open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE),_O_TEXT);

	fflush(stdin);
	hf=_fdopen(hcrt,"w");
	*stdout=*hf;
	setvbuf(stdout,NULL,_IONBF,0);
	consolecreated=TRUE;
	if(GetConsoleWindow==0){
		HMODULE hmod=LoadLibrary(TEXT("kernel32.dll"));
		if(hmod!=0){
			GetConsoleWindow=(HWND (WINAPI *)(void))GetProcAddress(hmod,"GetConsoleWindow");
			if(GetConsoleWindow!=0){
				ghconsole=GetConsoleWindow();
			}
			SetConsoleIcon=(DWORD(WINAPI *)(HICON))GetProcAddress(hmod,"SetConsoleIcon");
		}
	}

}
void hide_console()
{
	if(ghconsole!=0){
		ShowWindow(ghconsole,SW_HIDE);
		SetForegroundWindow(ghconsole);
	}
}
void move_console(int x,int y,int w,int h)
{
	int set_window_pos(HWND,int,int,int,int,int);
	if(0==ghconsole)
		return;
	if(w==0 || h==0){
		RECT rect;
		GetWindowRect(ghconsole,&rect);
		w=rect.right-rect.left;
		h=rect.bottom-rect.top;
		if(x==rect.left && y==rect.top)
			return;
	}
	set_window_pos(ghconsole,x,y,w,h,0);
	return;
}
DWORD set_console_icon(HICON hicon)
{
	int result=FALSE;
	if(SetConsoleIcon && hicon){
		result=SetConsoleIcon(hicon);
	}
	return result;
}