#include <windows.h>
#include "anchor_system.h"

extern HINSTANCE ghinstance;
extern HWND hpedit;

struct CONTROL_ANCHOR *pane_list=0;
unsigned int pane_count=0;

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
		EndPaint(hdc,&ps);
	}

}
int mouse_pos_status(int x,int y)
{
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
			deltaxl=x-l;
			deltaxr=r-x;
			if(y>=t && y<=b){
				int deltayt,deltayb;
				deltayt=y-t;
				deltayb=b-y;
				printf("xl=%i xr=%i\n",deltaxl,deltaxr);
				if(deltaxl<=5){
					if(deltayt<=5)
						return 0;
					else if(deltayb<=5)
						return 0;
					else
						return 1;
				}
			}
		}
	}
	return result;
}