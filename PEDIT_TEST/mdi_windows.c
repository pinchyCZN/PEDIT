#include <windows.h>
#include <stdlib.h>
#include "anchor_system.h"

extern HINSTANCE ghinstance;
extern HWND hpedit;

struct CONTROL_ANCHOR *pane_list=0;
unsigned int pane_count=0;

int show_divider=0;
RECT divider={0};

int compare_int(const void *a,const void *b)
{
	int x,y;
	x=((int*)a)[0];
	y=((int*)b)[0];
	if(x<y)
		return -1;
	else if(x==y)
		return 0;
	else
		return 1;
}
int get_new_child_id()
{
	int result=0;
	int i;
	int *list;
	if(0==pane_count)
		return result;
	list=malloc(pane_count*sizeof(int));
	if(0==list)
		return result;
	for(i=0;i<pane_count;i++){
		list[i]=pane_list[i].ctrl_id;
	}
	qsort(list,pane_count,sizeof(int),compare_int);
	result=list[0];
	for(i=1;i<pane_count;i++){
		int x=list[i];
		int delta;
		delta=x-result;
		if(delta>1){
			break;
		}
		result=list[i];
	}
	result++;
	free(list);
	return result;
}

int update_divider(int edge,RECT *rect)
{
#define DIVIDER_WIDTH 4
	switch(edge){
	case 0: //W
		divider.top=rect->top;
		divider.bottom=rect->bottom;
		divider.left=rect->left;
		divider.right=divider.left+DIVIDER_WIDTH;
		break;
	case 2: //E
		divider.top=rect->top;
		divider.bottom=rect->bottom;
		divider.left=rect->right-DIVIDER_WIDTH;
		divider.right=rect->right;
		break;
	}
	return 0;
}

int init_drag(int x,int y,int edge,int ctrl_id,RECT *rect)
{
	int i;
	for(i=0;i<pane_count;i++){
		struct CONTROL_ANCHOR *ca=&pane_list[i];
		ca->rect_drag=ca->rect_ctrl;
		ca->drag=1;
	}
	show_divider=TRUE;
	update_divider(edge,rect);
	return TRUE;
}
int end_drag(int restore)
{
	int i;
	for(i=0;i<pane_count;i++){
		struct CONTROL_ANCHOR *ca=&pane_list[i];
		if(ca->drag){
			if(!restore)
				ca->rect_ctrl=ca->rect_drag;
		}
		ca->drag=0;
	}
	show_divider=0;
	return TRUE;
}
int add_pane(HWND hparent,HWND hwnd,int anchor,int id)
{
	int result=FALSE;
	void *ptr;
	pane_count++;
	ptr=realloc(pane_list,sizeof(struct CONTROL_ANCHOR)*pane_count);
	if(ptr){
		struct CONTROL_ANCHOR tmp={0};
		int index=pane_count-1;
		pane_list=ptr;
		pane_list=(struct CONTROL_ANCHOR *)ptr;
		tmp.ctrl_id=id;
		tmp.anchor_mask=anchor;
		GetClientRect(hparent,&tmp.rect_parent);
		tmp.hwnd=hwnd;
		tmp.initialized=1;
		pane_list[index]=tmp;
		result=TRUE;
	}
	return result;
}
int get_snap_bias(HWND hwnd)
{
	int i;
	for(i=0;i<pane_count;i++){
		struct CONTROL_ANCHOR *ca;
		ca=&pane_list[i];
		if(hwnd==ca->hwnd)
			return ca->snap_bias;
	}
	return 0;
}
int tile_panes(HWND hparent)
{
	int i,j;
	RECT rparent;
	int pw,ph;
	if(pane_count<=0)
		return 0;
	GetClientRect(hparent,&rparent);
	pw=rparent.right-rparent.left;
	ph=rparent.bottom-rparent.top;

	for(i=0;i<pane_count;i++){
		struct CONTROL_ANCHOR *ca;
		RECT rect;
		int w,h,x,y;
		int snap_bias=0;
		rect=rparent;
		w=pw;
		h=ph;
		if(i<3){
			rect.left=(w/3)*i;
			rect.top=0;
			rect.right=rect.left+w/3;
			rect.bottom=h/2;
			snap_bias=0;
		}else{
			rect.left=(w/2)*(i-3);
			rect.top=h/2;
			rect.right=rect.left+w/2;
			rect.bottom=h;
			snap_bias=1;
		}
		x=rect.left;
		y=rect.top;
		w=rect.right-rect.left;
		h=rect.bottom-rect.top;
		ca=&pane_list[i];
		ca->snap_bias=snap_bias;
		ca->rect_ctrl=rect;
		printf("l=%i r=%i t=%i b=%i\n",x,x+w,y,y+h);
	}
	
	anchor_init_by_id(hparent,pane_list,pane_count);
	return 0;
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

HBRUSH get_hatch_handle()
{
	static HBITMAP hbmap=0;
	static HBRUSH hbrush=0;
	if(!hbmap){
		BYTE bits[16]={0x55,0x55,0xAA,0xAA,0x55,0x55,0xAA,0xAA,
			0x55,0x55,0xAA,0xAA,0x55,0x55,0xAA,0xAA};
		hbmap=CreateBitmap(8,8,1,1,&bits);
	}
	if(hbmap){
		hbrush=CreatePatternBrush(hbmap);
	}
	return hbrush;
}

int paint_main_win(HWND hwnd)
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc=BeginPaint(hwnd,&ps);
	if(hdc){
		int i;
		for(i=0;i<pane_count;i++){
			struct CONTROL_ANCHOR *ca;
			RECT *rc;
			RECT rect;
			ca=&pane_list[i];
			if(ca->drag)
				rc=&ca->rect_drag;
			else
				rc=&ca->rect_ctrl;
			DrawFrameControl(hdc,rc,DFC_BUTTON,DFCS_BUTTONPUSH);
			rect=*rc;			
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
		}
		if(show_divider){
			HBRUSH hbrush=get_hatch_handle();
			if(hbrush){
				int x,y,w,h;
				SelectObject(hdc,hbrush);
				x=divider.left;
				y=divider.top;
				w=divider.right-divider.left;
				h=divider.bottom-divider.top;
				PatBlt(hdc,x,y,w,h,PATINVERT);
			}
		}
		EndPaint(hdc,&ps);
	}

}
int mouse_pos_status(int x,int y,int *id,int *side,RECT *rect)
{
#define DIVIDER_SIZE 5
	int result=0;
	int i;
	for(i=0;i<pane_count;i++){
		struct CONTROL_ANCHOR *ca;
		int l,r,t,b;
		ca=&pane_list[i];
		l=ca->rect_ctrl.left;
		r=ca->rect_ctrl.right;
		t=ca->rect_ctrl.top;
		b=ca->rect_ctrl.bottom;
		if(x>=l && x<=r){
			int deltaxl,deltaxr;
			int HLEFT,HRIGHT;
			deltaxl=x-l;
			deltaxr=r-x;
			HLEFT=deltaxl<=DIVIDER_SIZE;
			HRIGHT=deltaxr<=DIVIDER_SIZE;
			if(y>=t && y<=b){
				int deltayt,deltayb;
				int VTOP,VBOT;
				deltayt=y-t;
				deltayb=b-y;
				if(id)
					*id=ca->ctrl_id;
				if(rect)
					*rect=ca->rect_ctrl;
				VTOP=deltayt<=DIVIDER_SIZE;
				VBOT=deltayb<=DIVIDER_SIZE;
//				printf("xl=%i xr=%i\n",deltaxl,deltaxr);

				if(HLEFT || HRIGHT){
					if(VTOP)
						return 0;
					else if(VBOT)
						return 0;
					else{
						if(HLEFT)
							if(side)
								*side=0;
						if(HRIGHT)
							if(side)
								*side=2;
						return 1;
					}
				}else if(VTOP || VBOT){
					if(HLEFT)
						return 0;
					else if(HRIGHT)
						return 0;
					else{
						if(VTOP)
							if(side)
								*side=1;
						if(VBOT)
							if(side)
								*side=3;
						return 2;
					}
				}
			}
		}
	}
	return result;
}

int is_inside(int x,int y,RECT *rect)
{
	int result=FALSE;
	if(x>=rect->left && x<=rect->right){
		if(y>=rect->top && y<=rect->bottom){
			result=TRUE;
		}
	}
	return TRUE;
}

int is_in_hband(RECT *rect,RECT *target)
{
	int result=FALSE;
	if(rect->top < target->bottom){
		if(rect->bottom > target->top)
			result=TRUE;
	}
	return result;
}
int is_in_vband(RECT *rect,RECT *target)
{
	int result=FALSE;
	if(rect->left < target->right){
		if(rect->right > target->left)
			result=TRUE;
	}
	return result;
}
#define MIN_WIN_WIDTH 15
int update_neighbors(int id,int side,RECT *rect)
{
	int i,j;
	for(i=0;i<pane_count;i++){
		struct CONTROL_ANCHOR *ca=&pane_list[i];
		RECT *crect;
		int hband,vband;
		if(id==ca->ctrl_id)
			continue;
		crect=&ca->rect_drag;
		hband=is_in_hband(crect,rect);
		vband=is_in_vband(crect,rect);
		printf("id=%i hb=%i vb=%i\n",ca->ctrl_id,hband,vband);
		if(hband){
			if(vband){
				int x;
				switch(side){
				case 0:
					crect->right=rect->left;
					x=crect->left+MIN_WIN_WIDTH;
					if(crect->right<x){
						crect->left=crect->right-MIN_WIN_WIDTH;
					}
					x=crect->right-MIN_WIN_WIDTH;
					if(crect->left>x)
						crect->left=x;
					break;
				case 2:
					//ca->rect_ctrl.left=rect->right;
					break;
				}
			}else{

			}
		}
	}
	return 0;
}
int clamp_control(int id,int side,RECT *rect)
{
	int x;
	if(0==side){
		x=rect->right-MIN_WIN_WIDTH;
		if(rect->left>x)
			rect->left=x;
	}else if(2==side){
		x=rect->left+MIN_WIN_WIDTH;
		if(rect->right<x)
			rect->right=x;
	}
	return 0;
}

int resize_panel(int id,int side,RECT *rect,int type,int x,int y,int ox,int oy)
{
	int i;
	extern HWND hpedit;
	for(i=0;i<pane_count;i++){
		struct CONTROL_ANCHOR *ca=&pane_list[i];
		RECT *crect;
		int inside=FALSE;
		int left,right,top,bottom;
		int dx,dy;
		if(id!=ca->ctrl_id){
			continue;
		}
		crect=&ca->rect_drag;
		dx=x-ox;
		dy=y-oy;
		left=rect->left+dx;
		right=rect->right+dx;
		inside= id==ca->ctrl_id;
		printf("dx=%i dy=%i left=%i right=%i inside=%i side=%i\n",dx,dy,left,right,inside,side);

		switch(type){
		case 1: //WE
			if(inside){
				if(0==side){
					int x;
					crect->left=left;
					clamp_control(id,side,crect);
					update_neighbors(id,side,crect);
					update_divider(side,crect);
					InvalidateRect(hpedit,NULL,TRUE);
				}else if(2==side){
					crect->right=right;
					clamp_control(id,side,crect);
					//update_neighbors(id,side,&ca->rect_ctrl);
					update_divider(side,crect);
					InvalidateRect(hpedit,NULL,TRUE);
				}
			}
			break;
		case 2: //NS

			break;
		}
	}
}