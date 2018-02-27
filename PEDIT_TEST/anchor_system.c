#include <windows.h>
#include "anchor_system.h"
#include "resource.h"

int anchor_init_by_id(HWND hparent,struct CONTROL_ANCHOR *clist,int clist_len)
{
	int i;
	RECT rparent={0};
	GetClientRect(hparent,&rparent);
	for(i=0;i<clist_len;i++){
		HWND hctrl;
		struct CONTROL_ANCHOR *anchor;
		anchor=&clist[i];
		anchor->rect_parent=rparent;
		hctrl=GetDlgItem(hparent,anchor->ctrl_id);
		if(hctrl){
			RECT rctrl={0};
			GetWindowRect(hctrl,&rctrl);
			MapWindowPoints(NULL,hparent,(PPOINT)&rctrl,2);
			anchor->rect_ctrl=rctrl;
			anchor->hwnd=hctrl;
		}
		anchor->initialized=1;
	}
	return 0;
}

int anchor_resize(HWND hparent,struct CONTROL_ANCHOR *clist,int clist_count)
{
	int i;
	RECT rparent={0};
	GetClientRect(hparent,&rparent);
	for(i=0;i<clist_count;i++){
		HWND hctrl;
		struct CONTROL_ANCHOR *anchor;
		anchor=&clist[i];
		if(!anchor->initialized)
			continue;
		hctrl=anchor->hwnd;
		if(hctrl){
			int x=0,y=0,cx=0,cy=0,delta;
			int flags=0;
			switch(anchor->anchor_mask){
			case ANCHOR_RIGHT|ANCHOR_TOP:
			case ANCHOR_RIGHT:
				{
					delta=anchor->rect_parent.right-anchor->rect_ctrl.left;
					x=rparent.right-delta;
					y=anchor->rect_ctrl.top-anchor->rect_parent.top;
					flags=SWP_NOSIZE;
				}
				break;
			case ANCHOR_LEFT|ANCHOR_BOTTOM:
			case ANCHOR_BOTTOM:
				{
					delta=anchor->rect_parent.bottom-anchor->rect_ctrl.top;
					x=anchor->rect_ctrl.left-anchor->rect_parent.left;
					y=rparent.bottom-delta;
					flags=SWP_NOSIZE;
				}
				break;
			case ANCHOR_LEFT|ANCHOR_RIGHT|ANCHOR_TOP|ANCHOR_BOTTOM:
				{
					x=anchor->rect_ctrl.left-anchor->rect_parent.left;
					y=anchor->rect_ctrl.top-anchor->rect_parent.top;
					delta=anchor->rect_parent.right -  anchor->rect_ctrl.right;
					cx=(rparent.right-rparent.left) - delta - x;
					delta=anchor->rect_parent.bottom -  anchor->rect_ctrl.bottom;
					cy=(rparent.bottom-rparent.top) - delta - y;
					flags=SWP_NOMOVE;
				}
				break;
			case ANCHOR_LEFT|ANCHOR_RIGHT|ANCHOR_TOP:
			case ANCHOR_LEFT|ANCHOR_RIGHT:
				{
					x=anchor->rect_ctrl.left-anchor->rect_parent.left;
					//y=anchor->rect_ctrl.top-anchor->rect_parent.top;
					delta=anchor->rect_parent.right -  anchor->rect_ctrl.right;
					cx=(rparent.right-rparent.left) - delta - x;
					cy=anchor->rect_ctrl.bottom-anchor->rect_ctrl.top;
					flags=SWP_NOMOVE;
				}
				break;
			case ANCHOR_LEFT|ANCHOR_RIGHT|ANCHOR_BOTTOM:
				{
					x=anchor->rect_ctrl.left-anchor->rect_parent.left;
					delta=anchor->rect_parent.bottom-anchor->rect_ctrl.top;
					y=rparent.bottom-delta;
					delta=anchor->rect_parent.right -  anchor->rect_ctrl.right;
					cx=(rparent.right-rparent.left) - delta - x;
					cy=anchor->rect_ctrl.bottom-anchor->rect_ctrl.top;
					flags=SWP_SHOWWINDOW;
				}
				break;
			case ANCHOR_RIGHT|ANCHOR_BOTTOM:
				{
					delta=anchor->rect_parent.right-anchor->rect_ctrl.left;
					x=rparent.right-delta;
					delta=anchor->rect_parent.bottom-anchor->rect_ctrl.top;
					y=rparent.bottom-delta;
					flags=SWP_NOSIZE;
				}
				break;
			case ANCHOR_LEFT|ANCHOR_TOP|ANCHOR_BOTTOM:
			case ANCHOR_TOP|ANCHOR_BOTTOM:
				{
					//x=anchor->rect_ctrl.left-anchor->rect_parent.left;
					y=anchor->rect_ctrl.top-anchor->rect_parent.top;
					cx=anchor->rect_ctrl.right-anchor->rect_ctrl.left;
					delta=anchor->rect_parent.bottom -  anchor->rect_ctrl.bottom;
					cy=(rparent.bottom-rparent.top) - delta - y;
					flags=SWP_NOMOVE;
				}
				break;
			case ANCHOR_RIGHT|ANCHOR_TOP|ANCHOR_BOTTOM:
				{
					delta=anchor->rect_parent.right-anchor->rect_ctrl.left;
					x=rparent.right-delta;
					y=anchor->rect_ctrl.top-anchor->rect_parent.top;
					cx=anchor->rect_ctrl.right-anchor->rect_ctrl.left;
					delta=anchor->rect_parent.bottom -  anchor->rect_ctrl.bottom;
					cy=(rparent.bottom-rparent.top) - delta - y;
					flags=SWP_SHOWWINDOW;
				}
				break;
			case ANCHOR_BOTTOM|ANCHOR_HCENTER:
				{
					delta=rparent.right-rparent.left;
					x=delta/2;
					delta=anchor->rect_ctrl.right-anchor->rect_ctrl.left;
					delta/=2;
					x-=delta;
					delta=anchor->rect_parent.bottom-anchor->rect_ctrl.top;
					y=rparent.bottom-delta;
					flags=SWP_NOSIZE;
				}
				break;
			}
			if(flags){
				flags&=~SWP_SHOWWINDOW;
				flags|=SWP_NOZORDER|SWP_NOREPOSITION;
				SetWindowPos(hctrl,NULL,x,y,cx,cy,flags);
			}
		}
	}
	return 0;
}

int save_win_rel_position(HWND hparent,HWND hwin,struct WIN_REL_POS *relpos)
{
	int result=FALSE;
	memset(&relpos->parent,0,sizeof(relpos->parent));
	relpos->parent.length=sizeof(WINDOWPLACEMENT);
	if(GetWindowPlacement(hparent,&relpos->parent)){
		if(relpos->parent.showCmd==SW_SHOWMAXIMIZED)
			GetWindowRect(hparent,&relpos->parent.rcNormalPosition);
		memset(&relpos->win,0,sizeof(relpos->win));
		relpos->win.length=sizeof(WINDOWPLACEMENT);
		if(GetWindowPlacement(hwin,&relpos->win)){
			result=TRUE;
		}
	}
	relpos->initialized=result;
	return result;
}
int restore_win_rel_position(HWND hparent,HWND hwin,struct WIN_REL_POS *relpos)
{
	//clamp window to nearest monitor
	if(relpos->initialized){
		WINDOWPLACEMENT *wp_parent,*wp_win;
		RECT rparent={0};
		RECT orig_parent={0};
		wp_parent=&relpos->parent;
		wp_win=&relpos->win;
		orig_parent=relpos->parent.rcNormalPosition;
		if((!(SW_SHOWMAXIMIZED==wp_win->showCmd || SW_SHOWMINIMIZED==wp_win->showCmd)) 
			&& GetWindowRect(hparent,&rparent)){
			HMONITOR hmon;
			RECT rwin;
			int x,y,cx,cy;
			x=wp_win->rcNormalPosition.left-orig_parent.left;
			y=wp_win->rcNormalPosition.top-orig_parent.top;
			cx=wp_win->rcNormalPosition.right-wp_win->rcNormalPosition.left;
			cy=wp_win->rcNormalPosition.bottom-wp_win->rcNormalPosition.top;
			rwin.left=rparent.left+x;
			rwin.top=rparent.top+y;
			rwin.right=rwin.left+cx;
			rwin.bottom=rwin.top+cy;
			hmon=MonitorFromRect(&rwin,MONITOR_DEFAULTTONEAREST);
			if(hmon){
				MONITORINFO mi;
				mi.cbSize=sizeof(mi);
				if(GetMonitorInfo(hmon,&mi)){
					RECT rmon;
					rmon=mi.rcWork;
					x=rwin.left;
					y=rwin.top;
					if(x<rmon.left)
						x=rmon.left;
					if(y<rmon.top)
						y=rmon.top;
					if(cx>(rmon.right-rmon.left))
						cx=rmon.right-rmon.left;
					if(cy>(rmon.bottom-rmon.top))
						cy=rmon.bottom-rmon.top;
					if((x+cx)>rmon.right)
						x=rmon.right-cx;
					if((y+cy)>rmon.bottom)
						y=rmon.bottom-cy;
					SetWindowPos(hwin,NULL,x,y,cx,cy,SWP_NOZORDER);
				}
			}
			
		}
	}
	return 0;
}
int save_ini_win_rel_pos(char *section,struct WIN_REL_POS *relpos)
{
	if(relpos->initialized){
		int offsetx,offsety;
		int width,height;
		RECT *rparent,*rwin;
		rparent=&relpos->parent.rcNormalPosition;
		rwin=&relpos->win.rcNormalPosition;
		offsetx=rparent->left-rwin->left;
		offsety=rparent->top-rwin->top;
		width=rwin->right-rwin->left;
		height=rwin->bottom-rwin->top;
		write_ini_value(section,"width",width);
		write_ini_value(section,"height",height);
		write_ini_value(section,"offsetx",offsetx);
		write_ini_value(section,"offsety",offsety);
	}
	return 0;
}
int load_ini_win_rel_pos(char *section,struct WIN_REL_POS *relpos)
{
	int offsetx=0,offsety=0;
	int width=0,height=0;
	get_ini_value(section,"width",&width);
	get_ini_value(section,"height",&height);
	get_ini_value(section,"offsetx",&offsetx);
	get_ini_value(section,"offsety",&offsety);
	if(width>=50 && height>=50){
		RECT *rparent,*rwin;
		relpos->initialized=1;
		rparent=&relpos->parent.rcNormalPosition;
		rwin=&relpos->win.rcNormalPosition;
		rparent->left=offsetx;
		rparent->top=offsety;
		rwin->left=0;
		rwin->top=0;
		rwin->right=width;
		rwin->bottom=height;
	}
	return 0;
}
int snap_window(HWND hwnd,RECT *rect)
{
	if(hwnd && rect){
		HMONITOR hmon;
		MONITORINFO mi;
		hmon=MonitorFromRect(rect,MONITOR_DEFAULTTONEAREST);
		mi.cbSize=sizeof(mi);
		if(GetMonitorInfo(hmon,&mi)){
			long d_top,d_bottom,d_left,d_right;
			d_right=mi.rcWork.right-rect->right;
			if(d_right<=8 && d_right>=-4){
				rect->right=mi.rcWork.right;
				rect->left+=d_right;
			}
			d_left=rect->left-mi.rcWork.left;
			if(d_left<=8 && d_left>=-4){
				rect->left=mi.rcWork.left;
				rect->right-=d_left;
			}
			d_top=rect->top-mi.rcWork.top;
			if(d_top<=8 && d_top>=-4){
				rect->top=mi.rcWork.top;
				rect->bottom-=d_top;
			}
			d_bottom=mi.rcWork.bottom-rect->bottom;
			if(d_bottom<=8 && d_bottom>=-4){
				rect->bottom=mi.rcWork.bottom;
				rect->top+=d_bottom;
			}
		}
	}
	return 0;
}

int snap_sizing(HWND hwnd,RECT *rect,int side)
{
	int result=FALSE;
	if(hwnd && rect){
		HMONITOR hmon;
		MONITORINFO mi;
		hmon=MonitorFromRect(rect,MONITOR_DEFAULTTONEAREST);
		mi.cbSize=sizeof(mi);
		if(GetMonitorInfo(hmon,&mi)){
			RECT *rwork=&mi.rcWork;
			const int snap_size=10;
			if(side==WMSZ_TOP || side==WMSZ_TOPLEFT || side==WMSZ_TOPRIGHT){
				if(abs(rect->top - rwork->top)<snap_size){
					rect->top=rwork->top;
					result=TRUE;
				}
			}
			if(side==WMSZ_BOTTOM || side==WMSZ_BOTTOMLEFT || side==WMSZ_BOTTOMRIGHT){
				if(abs(rect->bottom - rwork->bottom)<snap_size){
					rect->bottom=rwork->bottom;
					result=TRUE;
				}
			}
			if(side==WMSZ_LEFT || side==WMSZ_TOPLEFT || side==WMSZ_BOTTOMLEFT){
				if(abs(rect->left - rwork->left)<snap_size){
					rect->left=rwork->left;
					result=TRUE;
				}
			}
			if(side==WMSZ_RIGHT || side==WMSZ_TOPRIGHT || side==WMSZ_BOTTOMRIGHT){
				if(abs(rect->right - rwork->right)<snap_size){
					rect->right=rwork->right;
					result=TRUE;
				}
			}
		}
	}
	return result;
}
#define GRIPPIE_SQUARE_SIZE 15
int create_grippy(HWND hwnd,HWND *hgrip)
{
	int result=FALSE;
	RECT client_rect;
	GetClientRect(hwnd,&client_rect);
	
	if(hgrip){
		*hgrip=CreateWindow("Scrollbar",NULL,WS_CHILD|WS_VISIBLE|SBS_SIZEGRIP,
			client_rect.right-GRIPPIE_SQUARE_SIZE,
			client_rect.bottom-GRIPPIE_SQUARE_SIZE,
			GRIPPIE_SQUARE_SIZE,GRIPPIE_SQUARE_SIZE,
			hwnd,NULL,NULL,NULL);
		if(*hgrip)
			result=TRUE;
	}
	return result;
}

int grippy_move(HWND hwnd,HWND grippy)
{
	RECT client_rect;
	GetClientRect(hwnd,&client_rect);
	if(grippy!=0)
	{
		SetWindowPos(grippy,NULL,
			client_rect.right-GRIPPIE_SQUARE_SIZE,
			client_rect.bottom-GRIPPIE_SQUARE_SIZE,
			GRIPPIE_SQUARE_SIZE,GRIPPIE_SQUARE_SIZE,
			SWP_NOZORDER|SWP_SHOWWINDOW);
	}
	return 0;
}

struct CONTROL_ANCHOR ini_win_anchor[]={
	{IDC_TXT_LOCAL,ANCHOR_LEFT|ANCHOR_RIGHT|ANCHOR_TOP,0,0,0,0,0},
	{IDC_TXT_APPDATA,ANCHOR_LEFT|ANCHOR_RIGHT|ANCHOR_TOP,0,0,0,0,0},
	{IDC_INSTALL_INFO,ANCHOR_LEFT|ANCHOR_RIGHT|ANCHOR_TOP,0,0,0,0,0}
};
int init_ini_win_anchor(HWND hwnd)
{
	return anchor_init_by_id(hwnd,ini_win_anchor,sizeof(ini_win_anchor)/sizeof(struct CONTROL_ANCHOR));
}
int resize_ini_win(HWND hwnd)
{
	return anchor_resize(hwnd,ini_win_anchor,sizeof(ini_win_anchor)/sizeof(struct CONTROL_ANCHOR));
}

