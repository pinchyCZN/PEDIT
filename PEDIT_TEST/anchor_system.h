#include <windows.h>

#define ANCHOR_LEFT 1
#define ANCHOR_RIGHT 2
#define ANCHOR_TOP 4
#define ANCHOR_BOTTOM 8
#define ANCHOR_HCENTER 16

struct CONTROL_ANCHOR{
	int ctrl_id;
	int anchor_mask;
	RECT rect_ctrl,rect_parent;
	HWND hwnd;
	void *child;
	int initialized;
};

struct WIN_REL_POS{
	WINDOWPLACEMENT parent,win;
	int initialized;
};
