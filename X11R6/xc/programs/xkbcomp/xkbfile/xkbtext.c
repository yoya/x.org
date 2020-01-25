/* $XConsortium: xkbtext.c,v 1.1 94/04/08 15:50:11 erik Exp $ */
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
#ifndef X_NOT_STDC_ENV
#include <stdlib.h>
#endif
#include <X11/Xlib.h>
#include <X11/XKBlib.h>

#define	DEBUG_VAR_NOT_LOCAL
#define	DEBUG_VAR debugFlags
#include "utils.h"
#include "strtbl.h"

#include "XKM.h"
#include "xkbfile.h"

/***====================================================================***/

#define	BUFFER_SIZE	512

static char textBuffer[BUFFER_SIZE];
static int  tbNext= 0;

char *
tbGetBuffer(size)
    int	size;
{
char *rtrn;

    if (size>=BUFFER_SIZE)
	return NULL;
    if ((BUFFER_SIZE-tbNext)<=size)
	tbNext= 0;
    rtrn= &textBuffer[tbNext];
    tbNext+= size;
    return rtrn;
}

/***====================================================================***/

char *
XkbAtomText(dpy,atm,format)
    Display *	dpy;
    Atom 	atm;
    unsigned	format;
{
char	*rtrn,*tmp;

    tmp= XkbAtomGetString(dpy,atm);
    if (tmp!=NULL) {
	int	len;
	len= strlen(tmp)+1;
	if (len>BUFFER_SIZE)
	    len= BUFFER_SIZE-2;
	rtrn= tbGetBuffer(len);
	strncpy(rtrn,tmp,len);
	rtrn[len]= '\0';
    }
    else {
	rtrn= tbGetBuffer(1);
	rtrn[0]= '\0';
    }
    if (format==XkbCFile) {
	for (tmp=rtrn;*tmp!='\0';tmp++) {
	    if ((tmp==rtrn)&&(!isalpha(*tmp)))
		*tmp= '_';
	    else if (!isalnum(*tmp))
		*tmp= '_';
	}
    }
    return rtrn;
}

/***====================================================================***/

char *
XkbVModIndexText(dpy,xkb,ndx,format)
    Display *	dpy;
    XkbDescPtr	xkb;
    unsigned	ndx;
    unsigned	format;
{
register int i,len;
register Atom *vmodNames;
char *rtrn,*tmp;
char  numBuf[20];

    if (xkb && xkb->names)
	 vmodNames= xkb->names->vmods;
    else vmodNames= NULL;

    tmp= NULL;
    if (ndx>=XkbNumVirtualMods)
	 tmp= "illegal";
    else if (vmodNames&&(vmodNames[ndx]!=None))
	 tmp= XkbAtomGetString(dpy,vmodNames[ndx]);
    if (tmp==NULL)
	sprintf(tmp=numBuf,"%d",ndx);

    len= strlen(tmp)+1;
    if (format==XkbCFile)
	len+= 4;
    if (len>=BUFFER_SIZE)
	len= BUFFER_SIZE-1;
    rtrn= tbGetBuffer(len);
    if (format==XkbCFile) {
	 strcpy(rtrn,"vmod_");
	 strncpy(&rtrn[5],tmp,len-4);
    }
    else strncpy(rtrn,tmp,len);
    return rtrn;
}

char *
XkbVModMaskText(dpy,xkb,modMask,mask,format)
    Display *	dpy;
    XkbDescPtr	xkb;
    unsigned	modMask;
    unsigned	mask;
    unsigned	format;
{
register int i,bit;
int	 len;
char *mm,*rtrn;
char *str,buf[BUFFER_SIZE];

    if ((modMask==0)&&(mask==0)) {
	rtrn= tbGetBuffer(5);
	if (format==XkbCFile)
	     sprintf(rtrn,"0");
	else sprintf(rtrn,"None");
	return rtrn;
    }
    if (modMask!=0)
	 mm= XkbModMaskText(modMask,format);
    else mm= NULL;

    str= buf;
    buf[0]= '\0';
    if (mask) {
	char *tmp;
	for (i=0,bit=1;i<XkbNumVirtualMods;i++,bit<<=1) {
	    if (mask&bit) {
		tmp= XkbVModIndexText(dpy,xkb,i,format);
		len= strlen(tmp)+1+(str==buf?0:1);
		if (format==XkbCFile)
		    len+= 4;
		if ((str-(buf+len))<=BUFFER_SIZE) {
		    if (str!=buf) {
			if (format==XkbCFile)	*str++= '|';
			else			*str++= '+';
			len--;
		    }
		}
		if (format==XkbCFile)
		     sprintf(str,"%sMask",tmp);
		else strcpy(str,tmp);
		str= &str[len-1];
	    }
	}
	str= buf;
    }
    else str= NULL;
    if (mm)	
	len= strlen(mm);
    else	len= 0;
    if (str)
	len+= strlen(str)+(mm==NULL?0:1);
    if (len>=BUFFER_SIZE)
	len= BUFFER_SIZE-1;
    rtrn= tbGetBuffer(len+1);
    rtrn[0]= '\0';

    if (mm!=NULL) {
	i= strlen(mm);
	if (i>len)
	    i= len;
	strncpy(rtrn,mm,i);
    }
    else {
	i=0;
    }
    if (str!=NULL) {
	if (mm!=NULL) {
	    if (format==XkbCFile)	strcat(rtrn,"|");
	    else			strcat(rtrn,"+");
	}
	strncat(rtrn,str,len-i);
    }
    rtrn[len]= '\0';
    return rtrn;
}

static char *modNames[XkbNumModifiers] = {
    "Shift", "Lock", "Control", "Mod1", "Mod2", "Mod3", "Mod4", "Mod5"
};

char *
XkbModIndexText(ndx,format)
    unsigned	ndx;
    unsigned	format;
{
static char *rtrn,*tmp;

    if (ndx<XkbNumModifiers) {
	tmp= modNames[ndx];
	if (format==XkbCFile) {
	    rtrn= tbGetBuffer(strlen(tmp)+9);
	    sprintf(rtrn,"%sMapIndex",modNames[ndx]);
	    return rtrn;
	}
	rtrn= tbGetBuffer(strlen(tmp)+1);
	strcpy(rtrn,tmp);
	return rtrn;
    }
    rtrn= tbGetBuffer(strlen("ILLEGAL")+1);
    strcpy(rtrn,"ILLEGAL");
    return rtrn;
}

char *
XkbModMaskText(mask,format)
    unsigned	mask;
    unsigned	format;
{
register int i,bit;
char buf[64],*rtrn;

    if ((mask&0xff)==0xff) {
	if (format==XkbCFile) 		strcpy(buf,"0xff");
	else				strcpy(buf,"all");
    }
    else if ((mask&0xff)==0) {
	if (format==XkbCFile)		strcpy(buf,"0");
	else				strcpy(buf,"none");
    }
    else {
	char *str= buf;
	buf[0]= '\0';
	for (i=0,bit=1;i<XkbNumModifiers;i++,bit<<=1) {
	    if (mask&bit) {
		if (str!=buf) {
		    if (format==XkbCFile)	*str++= '|';
		    else			*str++= '+';
		}
		strcpy(str,modNames[i]);
		str= &str[strlen(str)];
		if (format==XkbCFile) {
		    strcpy(str,"Mask");
		    str+= 4;
		}
	    }
	}
    }
    rtrn= tbGetBuffer(strlen(buf)+1);
    strcpy(rtrn,buf);
    return rtrn;
}

/***====================================================================***/

char *
XkbConfigText(config)
    unsigned config;
{
static char *buf;

    buf= tbGetBuffer(32);
    switch (config) {
	case XkmSemanticsFile:
	    strcpy(buf,"semantics");
	    break;
	case XkmLayoutFile:
	    strcpy(buf,"layout");
	    break;
	case XkmKeymapFile:
	    strcpy(buf,"keymap");
	    break;
	case XkmGeometryFile:
	case XkmGeometryIndex:
	    strcpy(buf,"geometry");
	    break;
	case XkmAlternateSymsFile:
	    strcpy(buf,"alternate symbols");
	    break;
	case XkmTypesIndex:
	    strcpy(buf,"types");
	    break;
	case XkmCompatMapIndex:
	    strcpy(buf,"compatibility map");
	    break;
	case XkmSymbolsIndex:
	    strcpy(buf,"symbols");
	    break;
	case XkmIndicatorsIndex:
	    strcpy(buf,"indicators");
	    break;
	case XkmKeyNamesIndex:
	    strcpy(buf,"key names");
	    break;
	case XkmVirtualModsIndex:
	    strcpy(buf,"virtual modifiers");
	    break;
	default:
	    sprintf(buf,"unknown(%d)",config);
	    break;
    }
    return buf;
}

/***====================================================================***/

char *
XkbKeysymText(sym,format)
    KeySym	sym;
    unsigned	format;
{
static char buf[32],*rtrn;

    if (sym==NoSymbol)
	strcpy(rtrn=buf,"NoSymbol");
    else if ((rtrn=XKeysymToString(sym))==NULL)
	sprintf(rtrn=buf,"0x%x",sym);
    else if (format==XkbCFile) {
	sprintf(buf,"XK_%s",rtrn);
	rtrn= buf;
    }
    return rtrn;
}

/***====================================================================***/

static char *imWhichNames[]= {
	"base",
	"latched",
	"locked",
	"effective",
	"compat"
};

char *
XkbIMWhichStateMask(use_which,format)
    unsigned	use_which;
    unsigned	format;
{
int		len;
unsigned	i,bit,tmp;
char *		buf;

    if (use_which==0) {
	buf= tbGetBuffer(2);
	strcpy(buf,"0");
	return buf;
    }
    tmp= use_which&XkbIM_UseAnyState;
    for (len=i=0,bit=1;tmp!=0;i++,bit<<=1) {
	if (tmp&bit) {
	    tmp&= ~bit;
	    len+= strlen(imWhichNames[i])+1;
	    if (format==XkbCFile)
		len+= 9;
	}
    }
    buf= tbGetBuffer(len+1);
    tmp= use_which&XkbIM_UseAnyState;
    for (len=i=0,bit=1;tmp!=0;i++,bit<<=1) {
	if (tmp&bit) {
	    tmp&= ~bit;
	    if (format==XkbCFile) {
		if (len!=0)
		    buf[len++]= '|';
		sprintf(&buf[len],"XkbIM_Use%s",imWhichNames[i]);
		buf[len+9]= toupper(buf[len+9]);
	    }
	    else {
		if (len!=0)
		    buf[len++]= '+';
		sprintf(&buf[len],"%s",imWhichNames[i]);
	    }
	    len+= strlen(&buf[len]);
	}
    }
    return buf;
}

static char *ctrlNames[] = {
	"repeatKeys",
	"slowKeys",
	"bounceKeys",
	"stickyKeys",
	"mouseKeys",
	"mouseKeysAccel",
	"accessXKeys",
	"accessXTimeout",
	"accessXFeedback",
	"groupsWrap",
	"audibleBell",
	"autoAutorepeat"
};

char *
XkbControlsMaskText(ctrls,format)
    unsigned	ctrls;
    unsigned	format;
{
int		len;
unsigned	i,bit,tmp;
char *		buf;

    if (ctrls==0) {
	buf= tbGetBuffer(5);
	if (format==XkbCFile)
	     strcpy(buf,"0");
	else strcpy(buf,"none");
	return buf;
    }
    tmp= ctrls&XkbAllAccessXMask;
    for (len=i=0,bit=1;tmp!=0;i++,bit<<=1) {
	if (tmp&bit) {
	    tmp&= ~bit;
	    len+= strlen(ctrlNames[i])+1;
	    if (format==XkbCFile)
		len+= 7;
	}
    }
    buf= tbGetBuffer(len+1);
    tmp= ctrls&XkbAllAccessXMask;
    for (len=i=0,bit=1;tmp!=0;i++,bit<<=1) {
	if (tmp&bit) {
	    tmp&= ~bit;
	    if (format==XkbCFile) {
		if (len!=0)
		    buf[len++]= '|';
		sprintf(&buf[len],"Xkb%sMask",ctrlNames[i]);
		buf[len+4]= toupper(buf[len+4]);
	    }
	    else {
		if (len!=0)
		    buf[len++]= '+';
		sprintf(&buf[len],"%s",ctrlNames[i]);
	    }
	    len+= strlen(&buf[len]);
	}
    }
    return buf;
}
