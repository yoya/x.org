/* $XConsortium: xkbscan.c,v 1.2 94/04/04 15:28:49 rws Exp $ */
/************************************************************
 Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.

 Permission to use, copy, modify, and distribute this
 software and its documentation for any purpose and without
 fee is hereby granted, provided that the above copyright
 notice appear in all copies and that both that copyright
 notice and this permission notice appear in supporting
 documentation, and that the name of Silicon Graphics not be 
 used in advertising or publicity pertaining to distribution 
 of the software without specific prior written permission.
 Silicon Graphics makes no representation about the suitability 
 of this software for any purpose. It is provided "as is"
 without any express or implied warranty.
 
 SILICON GRAPHICS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS 
 SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY 
 AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL SILICON
 GRAPHICS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL 
 DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, 
 DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE 
 OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION  WITH
 THE USE OR PERFORMANCE OF THIS SOFTWARE.

 ********************************************************/

#include <stdio.h>
#include <ctype.h>
#include <X11/Xos.h>
#include <X11/X.h>
#include <X11/extensions/XKB.h>

#include "tokens.h"

FILE	*yyin = stdin;

static char scanFileBuf[1024];
char *	 scanFile= scanFileBuf;
int	 lineNum=	0;

int	 scanInt;
char	*scanIntStr;
int	 scanIntClass;

char	*scanStr = NULL;
int	 scanStrLine=	0;

#define	BUFSIZE	512
static	int	nInBuf = 0;
static	char	buf[BUFSIZE];

#ifdef DEBUG

extern	unsigned debugFlags;

static char *
tokText(tok)
    int tok;
{
static char buf[32];

    switch (tok) {
	case END_OF_FILE:	sprintf(buf, "END_OF_FILE");break;
	case ERROR:		sprintf(buf, "ERROR");	break;

	case XKB_KEYMAP:	sprintf(buf, "XKB_KEYMAP"); break;
	case XKB_KEYCODES:	sprintf(buf, "XKB_KEYCODES"); break;
	case XKB_TYPES:		sprintf(buf, "XKB_TYPES"); break;
	case XKB_SYMBOLS:	sprintf(buf, "XKB_SYMBOLS"); break;
	case XKB_COMPATMAP:	sprintf(buf, "XKB_COMPATMAP"); break;
	case XKB_GEOMETRY:	sprintf(buf, "XKB_GEOMETRY"); break;
	case XKB_SEMANTICS:	sprintf(buf, "XKB_SEMANTICS"); break;
	case XKB_LAYOUT:	sprintf(buf, "XKB_LAYOUT"); break;

	case INCLUDE:		sprintf(buf, "INCLUDE"); break;
	case OVERRIDE:		sprintf(buf, "OVERRIDE"); break;
	case AUGMENT:		sprintf(buf, "AUGMENT"); break;
	case REPLACE:		sprintf(buf, "REPLACE"); break;

	case VIRTUAL_MODS:	sprintf(buf, "VIRTUAL_MODS"); break;
	case TYPE:		sprintf(buf, "TYPE"); break;
	case INTERPRET:		sprintf(buf, "INTERPRET"); break;
	case ACTION:		sprintf(buf, "ACTION"); break;
	case KEY:		sprintf(buf, "KEY"); break;
	case MODIFIER:		sprintf(buf, "MODIFIER"); break;
	case MODIFIER_MAP:	sprintf(buf, "MODIFIER_MAP"); break;
	case INDICATOR:		sprintf(buf, "INDICATOR"); break;

	case EQUALS:		sprintf(buf, "EQUALS");	break;
	case PLUS:		sprintf(buf, "PLUS");	break;
	case MINUS:		sprintf(buf, "MINUS");	break;
	case DIVIDE:		sprintf(buf, "DIVIDE");	break;
	case TIMES:		sprintf(buf, "TIMES");	break;
	case OBRACE:		sprintf(buf, "OBRACE");	break;
	case CBRACE:		sprintf(buf, "CBRACE");	break;
	case OPAREN:		sprintf(buf, "OPAREN");	break;
	case CPAREN:		sprintf(buf, "CPAREN");	break;
	case OBRACKET:		sprintf(buf, "OBRACKET");break;
	case CBRACKET:		sprintf(buf, "CBRACKET");break;
	case DOT:		sprintf(buf, "DOT");	break;
	case COMMA:		sprintf(buf, "COMMA");	break;
	case SEMI:		sprintf(buf, "SEMI");	break;
	case EXCLAM:		sprintf(buf, "EXCLAM");	break;
	case INVERT:		sprintf(buf, "INVERT");	break;

	case STRING:		sprintf(buf, "STRING (%s)",scanStr);	break;
	case NUMBER:		sprintf(buf, "NUMBER (0x%x)",scanInt);	break;
	case IDENT:		sprintf(buf, "IDENT (%s)",scanStr);	break;
	case KEYNAME:		sprintf(buf, "KEYNAME (%s)",scanStr);	break;
	default:		sprintf(buf, "UNKNOWN");	break;
    }
    return buf;
}
#endif

int
setScanState(file,line)
    char *	file;
    int 	line;
{
    if (file!=NULL)
	strncpy(scanFile,file,1024);
    if (line>=0)
	lineNum= line;
    return 1;
}

int
yyGetString()
{
int ch;

    nInBuf = 0;
    while ( ((ch=getc(yyin))!=EOF) && (ch!='"') ) {
	if ( ch == '\\' ) {
	    if ((ch = getc(yyin))!=EOF) {
		if ( ch=='n' )		ch = '\n';
		else if ( ch == 't' )	ch = '\t';
		else if ( ch == 'v' )	ch = '\v';
		else if ( ch == 'b' )	ch = '\b';
		else if ( ch == 'r' )	ch = '\r';
		else if ( ch == 'f' )	ch = '\f';
		else if ( ch == 'e' )	ch = '\033';
		else if ( ch == '0' ) {
		    int tmp,stop;
		    ch = stop = 0;
		    if (((tmp=getc(yyin))!=EOF) && (isdigit(tmp)) && 
						(tmp!='8') && (tmp!='9')) {
			ch= (ch*8)+(tmp-'0');
		    }
		    else {
			stop= 1;
			ungetc(tmp,yyin);
		    }
		    if ((!stop) && ((tmp=getc(yyin))!=EOF) && (isdigit(tmp)) && 
						(tmp!='8') && (tmp!='9')) {
			ch= (ch*8)+(tmp-'0');
		    }
		    else {
			stop= 1;
			ungetc(tmp,yyin);
		    }
		    if ((!stop) && ((tmp=getc(yyin))!=EOF) && (isdigit(tmp)) && 
						(tmp!='8') && (tmp!='9')) {
			ch= (ch*8)+(tmp-'0');
		    }
		    else {
			stop= 1;
			ungetc(tmp,yyin);
		    }
		}
	    }
	    else return ERROR;
	}

	if ( nInBuf < BUFSIZE-1 ) 
	    buf[nInBuf++] = ch;
    }
    if ( ch == '"' ) {
	buf[nInBuf++] = '\0';
	if  ( scanStr )
	    free( scanStr );
	scanStr = (char *)uStringDup(buf);
	scanStrLine = lineNum;
	return STRING;
    }
    return ERROR;
}

int
yyGetKeyName()
{
int ch;

    nInBuf = 0;
    while ( ((ch=getc(yyin))!=EOF) && (ch!='>') ) {
	if ( ch == '\\' ) {
	    if ((ch = getc(yyin))!=EOF) {
		if ( ch=='n' )		ch = '\n';
		else if ( ch == 't' )	ch = '\t';
		else if ( ch == 'v' )	ch = '\v';
		else if ( ch == 'b' )	ch = '\b';
		else if ( ch == 'r' )	ch = '\r';
		else if ( ch == 'f' )	ch = '\f';
		else if ( ch == 'e' )	ch = '\033';
		else if ( ch == '0' ) {
		    int tmp,stop;
		    ch = stop = 0;
		    if (((tmp=getc(yyin))!=EOF) && (isdigit(tmp)) && 
						(tmp!='8') && (tmp!='9')) {
			ch= (ch*8)+(tmp-'0');
		    }
		    else {
			stop= 1;
			ungetc(tmp,yyin);
		    }
		    if ((!stop) && ((tmp=getc(yyin))!=EOF) && (isdigit(tmp)) && 
						(tmp!='8') && (tmp!='9')) {
			ch= (ch*8)+(tmp-'0');
		    }
		    else {
			stop= 1;
			ungetc(tmp,yyin);
		    }
		    if ((!stop) && ((tmp=getc(yyin))!=EOF) && (isdigit(tmp)) && 
						(tmp!='8') && (tmp!='9')) {
			ch= (ch*8)+(tmp-'0');
		    }
		    else {
			stop= 1;
			ungetc(tmp,yyin);
		    }
		}
	    }
	    else return ERROR;
	}

	if ( nInBuf < BUFSIZE-1 ) 
	    buf[nInBuf++] = ch;
    }
    if (( ch == '>' )&&(nInBuf<5)) {
	buf[nInBuf++] = '\0';
	if  ( scanStr )
	    free( scanStr );
	scanStr = (char *)uStringDup(buf);
	scanStrLine = lineNum;
	return KEYNAME;
    }
    return ERROR;
}

struct _Keyword {
	char	*keyword;
	int	 token;
} keywords[] = {
    { "xkb_keymap",		XKB_KEYMAP		},
    { "xkb_keycodes",		XKB_KEYCODES		},
    { "xkb_types",		XKB_TYPES		},
    { "xkb_symbols",		XKB_SYMBOLS		},
    { "xkb_compat",		XKB_COMPATMAP		},
    { "xkb_compat_map",		XKB_COMPATMAP		},
    { "xkb_compatibility",	XKB_COMPATMAP		},
    { "xkb_compatibility_map",	XKB_COMPATMAP		},
    { "xkb_geometry",		XKB_GEOMETRY		},
    { "xkb_semantics",		XKB_SEMANTICS		},
    { "xkb_layout",		XKB_LAYOUT		},
    { "include",		INCLUDE			},
    { "override",		OVERRIDE		},
    { "augment",		AUGMENT			},
    { "replace",		REPLACE			},
    { "virtual_modifiers",	VIRTUAL_MODS		},
    { "type",			TYPE			},
    { "interpret",		INTERPRET		},
    { "action",			ACTION			},
    { "key",			KEY			},
    { "modifier",		MODIFIER		},
    { "modmap",			MODIFIER_MAP		},
    { "mod_map",		MODIFIER_MAP		},
    { "modifier_map",		MODIFIER_MAP		},
    { "indicator",		INDICATOR		}
};
int	numKeywords = sizeof(keywords)/sizeof(struct _Keyword);

int
yyGetIdent(first)
    int first;
{
int ch,i,found;
int	rtrn;

    buf[0] = first; nInBuf = 1;
    while ( ((ch=getc(yyin))!=EOF) && (isalnum(ch)||(ch=='_')) ) {
	if ( nInBuf < BUFSIZE - 1 )
	    buf[nInBuf++] = ch;
    }
    buf[nInBuf++] = '\0';
    found= 0;

    for (i=0;(!found)&&(i<numKeywords);i++) {
	if (uStrCaseCmp(buf,keywords[i].keyword)==0) {
	    rtrn= keywords[i].token;
	    found= 1;
	}
    }
    if (!found) {
	if  ( scanStr )
	    free( scanStr );
	scanStr = (char *)uStringDup(buf);
	scanStrLine = lineNum;
	rtrn = IDENT;
    }

    if ( (ch!=EOF) && (!isspace(ch)) )
	ungetc( ch, yyin );
    else if ( ch=='\n' )
	lineNum++;

    return rtrn;
}

int
yyGetNumber(ch)
    int ch;
{
    while ( isspace(ch) && (ch!=EOF)) {
	ch = getc(yyin);
	if ( ch=='\n' )
	    lineNum++;
    }
    if (ch!=EOF)
	ungetc(ch,yyin);
    if ( fscanf(yyin,"%i",&scanInt)==1 )
	return NUMBER;
    return ERROR;
}

int
yylex()
{
int	ch;
int	rtrn;
    
    do {
	ch = getc(yyin);
	if ( ch == '\n' ) {
	    lineNum++;
	}
	else if ( ch == '#' ) {
	    if (fscanf(yyin," %d %s",&lineNum,scanFile)!=2)
		fprintf(stderr,"bad #line directive\n");
	    else {
		ch = getc(yyin);
	    }
	}
	else if ( ch=='/' ) {	/* handle C++ style double-/ comments */
	    int newch= getc(yyin);
	    if (newch=='/') {
		do {
		    ch= getc(yyin);
		} while ((ch!='\n')&&(ch!=EOF));
		lineNum++;
	    }
	    else if (newch!=EOF) {
		ungetc(newch,yyin);
	    }
	}
    } while ((ch!=EOF)&&(isspace(ch)));
    if ( ch == '=' )			rtrn = EQUALS;
    else if ( ch == '+' )		rtrn = PLUS;
    else if ( ch == '-' )		rtrn = MINUS;
    else if ( ch == '/' )		rtrn = DIVIDE;
    else if ( ch == '*' )		rtrn = TIMES;
    else if ( ch == '{' )		rtrn = OBRACE;
    else if ( ch == '}' )		rtrn = CBRACE;
    else if ( ch == '(' )		rtrn = OPAREN;
    else if ( ch == ')' )		rtrn = CPAREN;
    else if ( ch == '[' )		rtrn = OBRACKET;
    else if ( ch == ']' )		rtrn = CBRACKET;
    else if ( ch == '.' )		rtrn = DOT;
    else if ( ch == ',' )		rtrn = COMMA;
    else if ( ch == ';' )		rtrn = SEMI;
    else if ( ch == '!' )		rtrn = EXCLAM;
    else if ( ch == '~' )		rtrn = INVERT;
    else if ( ch == '"' )		rtrn = yyGetString();
    else if ( ch == '<' )		rtrn = yyGetKeyName();
    else if ( isalpha(ch) || (ch=='_')) rtrn = yyGetIdent(ch);
    else if ( isdigit(ch) )		rtrn = yyGetNumber(ch);
    else if ( ch == EOF )		rtrn = END_OF_FILE;
    else {
	fprintf(stderr,"Unexpected character %c (%d) in input stream\n",ch,ch);
	rtrn = ERROR;
    }
#ifdef DEBUG
    if (debugFlags&0x2)
	fprintf(stderr,"scan: %s\n",tokText(rtrn));
#endif
    return rtrn;
}
