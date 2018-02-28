#include <windows.h>
#include "scintilla.h"
#include "scilexer.h"


int highlight_match(HWND hscint)
{
	int result=FALSE;
	long a,b;
	char *buf;
	a=SendMessage(hscint,SCI_GETSELECTIONSTART,0,0);
	b=SendMessage(hscint,SCI_GETSELECTIONEND,0,0);
	if(a==b)
		return result;
	//SendMessage(hscint,SCI
}