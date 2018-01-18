#define WINVER 0x500
#define _WIN32_WINNT 0x500
#include <windows.h>

int set_window_pos(HWND hwnd,int x,int y,int w,int h,int max)
{
	int result=FALSE;
	HMONITOR hmon;
	MONITORINFO mi;
	RECT rect;
	rect.left=x;
	rect.top=y;
	rect.right=x+w;
	rect.bottom=y+h;
	hmon=MonitorFromRect(&rect,MONITOR_DEFAULTTONEAREST);
	mi.cbSize=sizeof(mi);
	if(GetMonitorInfo(hmon,&mi)){
		int cw,ch;
		rect=mi.rcWork;
		cw=w;
		ch=h;
		if(0==cw)
			cw=100;
		if(0==ch)
			ch=100;
		if((x+cw)>rect.right)
			x=rect.right-cw;
		if(x<rect.left)
			x=rect.left;
		if((y+ch)>rect.bottom)
			y=rect.bottom-ch;
		if(y<rect.top)
			y=rect.top;
		if(w>0 && h>0){
			int rw,rh;
			rw=rect.right-rect.left;
			rh=rect.bottom-rect.top;
			if(w>rw)
				w=rw;
			if(h>rh)
				h=rh;
			if(w<25)
				w=25;
			if(h<25)
				h=25;
		}
		{
			int flags=SWP_NOZORDER;
			if(max)
				flags|=SW_MAXIMIZE;
			if(w==0 || h==0)
				flags|=SWP_NOSIZE;
			SetWindowPos(hwnd,NULL,x,y,w,h,flags);
		}
		result=TRUE;
	}
	return result;
}
