#include <windows.h>
#include "scintilla.h"
#include "scilexer.h"

#define MAXLINEHIGHLIGHT 400	//prevent highlighter from doing too much work when a lot is visible

int is_wordchar(unsigned char a)
{
	const unsigned char list[]={
		' ',
		'\t',
		'\n',
		'\r',
		'.',
		',',
		'?',
		';',
		',',
		'!',
		'(',
		')',
		'[',
		']',
		'+',
		'-',
		'*',
		'/',
		'#',
		'@',
		'^',
		'%',
		'$',
		'"',
		'\'',
		'~',
		'&',
		'{',
		'}',
		'|',
		'=',
		'<',
		'>',
		'\\'
	};
	int i;
	if(a<' ')
		return FALSE;
	for(i=0;i<sizeof(list);i++){
		if(a==list[i])
			return FALSE;
	}
	return TRUE;
}
int is_word(unsigned char *buf,int len)
{
	int i;
	if(0==buf)
		return FALSE;
	for(i=0;i<len;i++){
		unsigned char a=buf[i];
		if(!is_wordchar(a))
			return FALSE;
	}
	return TRUE;
}
int highlight_lines(HWND hscint,int start,int count,char *str,int str_len)
{
	int i;
	for(i=0;i<count;i++){
		
	}
	return 0;
}
int highlight_match(HWND hscint)
{
	int result=FALSE;
	long a,b,sel_len;
	char *buf;
	const int buf_size=4096;
	int tmp,line_start,line_count;
	a=SendMessage(hscint,SCI_GETSELECTIONSTART,0,0);
	b=SendMessage(hscint,SCI_GETSELECTIONEND,0,0);
	if(a==b)
		return result;
	if(a>b)
		return result;
	sel_len=b-a;
	sel_len=b-a;
	if(sel_len>(buf_size-1))
		sel_len=buf_size-1;
	b=a+sel_len;
	buf=malloc(buf_size);
	if(0==buf)
		return result;

	tmp=SendMessage(hscint,SCI_GETCHARAT,b,0);
	if(tmp && (!is_wordchar(tmp)))
		return result;
	tmp=SendMessage(hscint,SCI_GETCHARAT,a-1,0);
	if(tmp && (!is_wordchar(tmp)))
		return result;
	{
		struct Sci_TextRange tr={0};
		tr.chrg.cpMin=a;
		tr.chrg.cpMax=b;
		tr.lpstrText=buf;
		buf[0]=0;
		SendMessage(hscint,SCI_GETTEXTRANGE,0,&tr);
	}
	if(is_word(buf,sel_len)){
		line_start=SendMessage(hscint,SCI_GETFIRSTVISIBLELINE,0,0);
		line_count=SendMessage(hscint,SCI_LINESONSCREEN,0,0);
		highlight_lines(hscint,line_start,line_count,buf,sel_len);
	}
	free(buf);
	return result;
}