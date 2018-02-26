#include <windows.h>

extern HWND hedit_pane;
extern HWND hstatusbar;
extern HWND hmenubar;

#define GROUP_USE_ALL      0
#define GROUP_FIXED_WIDTH  1
#define GROUP_FIXED_HEIGHT 2

struct MDI_PANE{
	HWND hwnd;
	int id;
	RECT rect;
};
struct MDI_PANE *pane_list=0;
int pane_count=0;

static int current_child_id=1000;
int get_new_child_id()
{
	int result=current_child_id;
	current_child_id++;
	return result;
}

int register_pane(HWND hwnd,int type)
{
	int result=FALSE;
	struct MDI_PANE *tmp;
	pane_count++;
	tmp=realloc(pane_list,sizeof(struct MDI_PANE)*pane_count);
	if(tmp){
		pane_list=tmp;

	}
	return result;
}
int get_main_rect(HWND hwnd,RECT *rect)
{
	int result=FALSE;
	return result;

}
int window_move(HWND hwnd)
{
	int result=FALSE;
	RECT rect;
	int x,y;
	GetClientRect(hwnd,&rect);
	MapWindowPoints(hwnd,NULL,(LPPOINT)&rect,2);
	x=rect.left;
	y=rect.top;
	if(hedit_pane){
		result=SetWindowPos(hedit_pane,NULL,x,y,0,0,SWP_NOSIZE);
	}
	return result;
}