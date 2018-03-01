#include <windows.h>
#include "scintilla.h"
#include "scilexer.h"

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

#define INDIC_HIGHLIGHT INDIC_STRAIGHTBOX
int setup_highlight(HWND hscint)
{
	SendMessage(hscint,SCI_INDICSETSTYLE,INDIC_HIGHLIGHT,INDIC_HIGHLIGHT);
	SendMessage(hscint,SCI_INDICSETALPHA,INDIC_HIGHLIGHT,100);
	SendMessage(hscint,SCI_INDICSETUNDER,INDIC_HIGHLIGHT,TRUE);
	SendMessage(hscint,SCI_INDICSETFORE,INDIC_HIGHLIGHT,0xFF);
	return TRUE;
}
int highlight_section(HWND hscint,int pos_start,int pos_end,char *str,int str_len)
{
#define MAX_SECTION_HIGHLIGHT 0x100000
	int result=0;
	int i,section_len;
	section_len=pos_end-pos_start;
	if(section_len<=0)
		return result;
	if(str_len<=0)
		return result;
	if(section_len>MAX_SECTION_HIGHLIGHT)
		section_len=MAX_SECTION_HIGHLIGHT;
	SendMessage(hscint,SCI_SETSEARCHFLAGS,0,0);
	for(i=0;i<section_len;i++){
		int found;
		SendMessage(hscint,SCI_SETTARGETSTART,pos_start+i,0);
		SendMessage(hscint,SCI_SETTARGETEND,pos_end,0);
		found=SendMessage(hscint,SCI_SEARCHINTARGET,str_len,str);
		if(!(-1==found)){
			SendMessage(hscint,SCI_SETINDICATORCURRENT,INDIC_HIGHLIGHT,0);
			SendMessage(hscint,SCI_INDICATORFILLRANGE,found,str_len);
			i+=str_len;
			result++;
		}else{
			break;
		}
	}
	return result;
}
int clear_highlight(HWND hscint)
{
	int len;
	len=SendMessage(hscint,SCI_GETLENGTH,0,0);
	SendMessage(hscint,SCI_SETINDICATORCURRENT,INDIC_HIGHLIGHT,0);
	SendMessage(hscint,SCI_INDICATORCLEARRANGE,0,len);
	return TRUE;
}
int highlight_match(HWND hscint)
{
	int result=FALSE;
	long a,b,sel_len;
	char *buf=0;
	const int buf_size=4096;
	int tmp;
	a=SendMessage(hscint,SCI_GETSELECTIONSTART,0,0);
	b=SendMessage(hscint,SCI_GETSELECTIONEND,0,0);
	if(a==b)
		return result;
	if(a>b)
		return result;
	sel_len=b-a;
	if(sel_len>(buf_size-1))
		sel_len=buf_size-1;
	b=a+sel_len;
	buf=malloc(buf_size);
	if(0==buf)
		return result;

	tmp=SendMessage(hscint,SCI_GETCHARAT,b,0);
	printf("b=%c %02X\n",tmp,tmp);
	if(tmp && (is_wordchar(tmp)))
		goto EXIT;
	tmp=SendMessage(hscint,SCI_GETCHARAT,a-1,0);
	printf("a-1=%c %02X\n",tmp,tmp);
	if(tmp && (is_wordchar(tmp)))
		goto EXIT;
	{
		struct Sci_TextRange tr={0};
		tr.chrg.cpMin=a;
		tr.chrg.cpMax=b;
		tr.lpstrText=buf;
		buf[0]=0;
		SendMessage(hscint,SCI_GETTEXTRANGE,0,&tr);
	}
	if(is_word(buf,sel_len)){
		int line_start,line_count;
		int pos_start,pos_end;
		line_start=SendMessage(hscint,SCI_GETFIRSTVISIBLELINE,0,0);
		line_count=SendMessage(hscint,SCI_LINESONSCREEN,0,0);
		pos_start=SendMessage(hscint,SCI_POSITIONFROMLINE,line_start,0);
		pos_end=SendMessage(hscint,SCI_POSITIONFROMLINE,line_start+line_count+1,0);
		if(-1==pos_end)
			pos_end=SendMessage(hscint,SCI_GETLENGTH,0,0);
		highlight_section(hscint,pos_start,pos_end,buf,sel_len);
		SendMessage(hscint,SCI_INDICATORCLEARRANGE,a,sel_len);
	}
EXIT:
	if(buf)
		free(buf);
	return result;
}