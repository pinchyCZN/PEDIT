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
int tile_panes(HWND hparent)
{
	int i;
	RECT rparent;
	int w,h;
	if(pane_count<=0)
		return 0;
	GetClientRect(hparent,&rparent);
	w=rparent.right-rparent.left;
	w=w/pane_count;
	h=rparent.bottom-rparent.top;
	for(i=0;i<pane_count;i++){
		struct CONTROL_ANCHOR *ca;
		RECT rect;
		rect=rparent;
		rect.left=i*w;
		rect.right=(i+1)*w;
		ca=&pane_list[i];
		SetWindowPos(ca->hwnd,NULL,rect.left,rect.top,w,h,SWP_NOZORDER);
	}
	anchor_init_by_id(hparent,pane_list,pane_count);
	return 0;
}
int add_default_pane(HWND hparent)
{
	int result=FALSE;
	int id;
	ATOM panel_class;
	id=get_new_child_id();
	panel_class=get_generic_panel_atom();
	/*
	hedit_pane=CreateWindow(edit_class,TEXT("EDIT_PANEL"),WS_CHILD|WS_VISIBLE,
			0,0,0,0,hparent,id,ghinstance,0);
	if(hedit_pane){
		RECT rect={0};
		int w,h,x,y,id;
		//old_wproc_edit=SetWindowLong(hedit_pane,GWL_WNDPROC,(LONG)wproc_edit_pane);
		get_max_edit_area(&rect);
		x=rect.left;
		y=rect.top;
		w=rect.right-rect.left;
		h=rect.bottom-rect.top;
//		w/=2;
//		h/=2;

		SetWindowPos(hedit_pane,NULL,x,y,w,h,SWP_NOZORDER|SWP_SHOWWINDOW);
		id=get_new_child_id();
		add_pane(hparent,hedit_pane,ANCHOR_LEFT|ANCHOR_RIGHT|ANCHOR_TOP|ANCHOR_BOTTOM,id);
		result=TRUE;
	}
*/
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

WNDPROC panel_wndproc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
	switch(msg){
	case WM_SIZE:
		{
			resize_edit_list(hwnd);
		}
		break;
	case WM_LBUTTONUP:
		{
			static int show=FALSE;
			show_menu(show);
			show=!show;
			adjust_for_menu();
		}
		break;
	case WM_PAINT:
		{
			RECT rect,ro;
			PAINTSTRUCT ps={0};
			HDC hdc=BeginPaint(hwnd,&ps);
			if(!hdc)
				break;
			GetClientRect(hwnd,&rect);
			ro=rect;
			FillRect(hdc,&rect,(HBRUSH)(COLOR_BTNFACE+1));
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
		}
		break;
	}
	return DefWindowProc(hwnd,msg,wparam,lparam);
}

int get_panel_atom(const char *name,WNDPROC *wndproc)
{
	ATOM result;
	WNDCLASSA wclass={0};
	wclass.hInstance=ghinstance;
	wclass.hCursor=NULL;
	wclass.hbrBackground=(HBRUSH)(COLOR_BACKGROUND+1); //(HBRUSH)(COLOR_BTNFACE+1);
	wclass.lpszClassName=name;
	wclass.lpfnWndProc=(WNDPROC)wndproc;
	result=RegisterClass(&wclass);
	return result;
}

int get_generic_panel_atom()
{
	static ATOM panel_class=0;
	if(0==panel_class){
		get_panel_atom("PANEL",panel_wndproc);
	}
	return panel_class;
}

int get_edit_panel_atom(WNDPROC *wndproc)
{
	static ATOM edit_class=0;
	if(0==edit_class){
		edit_class=get_panel_atom("EDIT_PANEL",wndproc);
	}
	return edit_class;
}
int get_console_panel_atom(WNDPROC *wndproc)
{
	static ATOM console_class=0;
	if(0==console_class){
		console_class=get_panel_atom("CONSOLE_PANEL",wndproc);
	}
	return console_class;
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