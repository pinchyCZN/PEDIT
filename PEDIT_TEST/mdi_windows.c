#include <windows.h>
#include "anchor_system.h"

extern HINSTANCE ghinstance;
extern HWND hpedit;

struct CONTROL_ANCHOR *pane_list=0;
unsigned int pane_count=0;

static int current_child_id=1000;
int get_new_child_id()
{
	int result;
	current_child_id&=0x3FFF;
	if(0==current_child_id)
		current_child_id=1;
	result=current_child_id;
	current_child_id++;
	return result;
}

int add_pane(HWND hparent,HWND hwnd,int anchor,int id)
{
	int result=FALSE;
	struct CONTROL_ANCHOR tmp={0};
	void *ptr;
	pane_count++;
	ptr=realloc(pane_list,sizeof(struct CONTROL_ANCHOR)*pane_count);
	if(ptr){
		int index=pane_count-1;
		pane_list=ptr;
		pane_list=(struct CONTROL_ANCHOR *)ptr;
		tmp.ctrl_id=id;
		tmp.anchor_mask=anchor;
		GetClientRect(hparent,&tmp.rect_parent);
		GetWindowRect(hwnd,&tmp.rect_ctrl);
		MapWindowPoints(NULL,hparent,(PPOINT)&tmp.rect_ctrl,2);
		tmp.hwnd=hwnd;
		tmp.initialized=1;
		pane_list[index]=tmp;
	}
	return result;
}
int get_max_edit_area(RECT *rect)
{
	int result=FALSE;
	unsigned int i;
	GetClientRect(hpedit,rect);
	result=TRUE;
	if(0==pane_list)
		return result;
	for(i=0;i<pane_count;i++){
		RECT *tmp;
		struct CONTROL_ANCHOR *ca;
		int mask;
		ca=&pane_list[i];
		if(!IsWindowVisible(ca->hwnd))
			continue;
		mask=ca->anchor_mask;
		tmp=&ca->rect_ctrl;
		if(mask&(ANCHOR_LEFT|ANCHOR_RIGHT)){
			if(mask&ANCHOR_TOP){
				if(tmp->bottom>rect->top)
					rect->top=tmp->bottom;
			}else if(mask&ANCHOR_BOTTOM){
				if(tmp->top<rect->bottom)
					rect->bottom=tmp->top;
			}
		}else if(mask&(ANCHOR_TOP|ANCHOR_BOTTOM)){
			if(mask&ANCHOR_LEFT){
				if(tmp->right>rect->left)
					rect->left=tmp->right;
			}else if(mask&ANCHOR_RIGHT){
				if(tmp->left<rect->right)
					rect->right=tmp->left;
			}
		}
	}
	return result;
}
int window_move(HWND hwnd)
{
	anchor_resize(hwnd,pane_list,pane_count);
	return 0;
}

int get_edit_panel_atom(WNDPROC *wndproc)
{
	static ATOM edit_class=0;
	if(0==edit_class){
		WNDCLASS wclass={0};
		wclass.hInstance=ghinstance;
		wclass.hCursor=NULL;
		wclass.hbrBackground=(HBRUSH)(COLOR_SCROLLBAR+1); //(HBRUSH)(COLOR_BTNFACE+1);
		wclass.lpszClassName=TEXT("EDIT_PANEL");
		wclass.lpfnWndProc=(WNDPROC)wndproc;
		edit_class=RegisterClass(&wclass);
	}
	return edit_class;
}
int get_menu_panel_atom(WNDPROC *wndproc)
{
	static ATOM menu_class=0;
	if(0==menu_class){
		WNDCLASS wclass={0};
		wclass.hInstance=ghinstance;
		wclass.hCursor=NULL;
		wclass.hbrBackground=(HBRUSH)(COLOR_BTNFACE+1);
		wclass.lpszClassName=TEXT("MENU_PANEL");
		wclass.lpfnWndProc=(WNDPROC)wndproc;
		menu_class=RegisterClass(&wclass);
	}
	return menu_class;
}
int get_project_panel_atom(WNDPROC *wndproc)
{
	static ATOM project_class=0;
	if(0==project_class){
		WNDCLASS wclass={0};
		wclass.hInstance=ghinstance;
		wclass.hCursor=NULL;
		wclass.hbrBackground=(HBRUSH)(COLOR_BTNFACE+1);
		wclass.lpszClassName=TEXT("PROJECT_PANEL");
		wclass.lpfnWndProc=(WNDPROC)wndproc;
		project_class=RegisterClass(&wclass);
	}
	return project_class;
}
int get_menu_height(HWND hwnd)
{
	int height=0;
	MENUBARINFO mbi={0};
	mbi.cbSize=sizeof(mbi);
	if(GetMenuBarInfo(hwnd,OBJID_MENU,0,&mbi)){
		height=mbi.rcBar.bottom-mbi.rcBar.top;
	}
	return height;
}
int adjust_for_menu()
{
	int result=0;
	unsigned int i;
	RECT rparent;
	int height;
	GetClientRect(hpedit,&rparent);
	height=rparent.bottom-rparent.top;
	printf("i=%i\n",height);
	for(i=0;i<pane_count;i++){
		int delta;
		struct CONTROL_ANCHOR *ca;
		RECT *rparent,*rect;
		int h;
		ca=&pane_list[i];
		rparent=&ca->rect_parent;
		h=rparent->bottom-rparent->top;
		delta=height-h;
		rect=&ca->rect_ctrl;
		//rect->top+=delta;
		//rect->bottom+=delta;
		result=delta;
	}
	return result;
}