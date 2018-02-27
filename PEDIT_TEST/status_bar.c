#include <windows.h>
#include <commctrl.h>
#include "anchor_system.h"

extern HINSTANCE ghinstance;

HWND hstatusbar=0;
int add_statusbar(HWND hparent)
{
	int result=FALSE;
	int id;
	if(hstatusbar)
		return result;
	id=get_new_child_id();
	hstatusbar=CreateWindow(STATUSCLASSNAME,TEXT("statusbar"),SBARS_SIZEGRIP|WS_CHILD|WS_VISIBLE,
		0,0,0,0,hparent,id,ghinstance,0);
	if(hstatusbar){
		add_pane(hparent,hstatusbar,ANCHOR_LEFT|ANCHOR_RIGHT|ANCHOR_BOTTOM,id);
	}
	return result;
}