#include <windows.h>
#include <commctrl.h>

extern HINSTANCE ghinstance;

HWND hstatusbar=0;
int add_statusbar(HWND hwnd)
{
	int result=FALSE;
	int id;
	if(hstatusbar)
		return result;
	id=get_new_child_id();
	hstatusbar=CreateWindow(STATUSCLASSNAME,TEXT("statusbar"),SBARS_SIZEGRIP|WS_CHILD|WS_VISIBLE,
		0,0,0,0,hwnd,id,ghinstance,0);
	if(hstatusbar){
		result=TRUE;
	}
	return result;
}