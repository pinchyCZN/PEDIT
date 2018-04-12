#include <windows.h>

#define ANCHOR_LEFT 1
#define ANCHOR_RIGHT 2
#define ANCHOR_TOP 4
#define ANCHOR_BOTTOM 8
#define ANCHOR_HCENTER 16

#define MOUSE_DIVIDER 1
#define MOUSE_DRAG 2

struct CONTROL_ANCHOR{
	int ctrl_id;
	int anchor_mask;
	int snap_bias;
	RECT rect_ctrl,rect_parent;
	HWND hwnd;
	char name[80];
	int initialized;
};

struct WIN_REL_POS{
	WINDOWPLACEMENT parent,win;
	int initialized;
};
