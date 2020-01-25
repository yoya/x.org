/* $XConsortium: xkbatom.c,v 1.1 94/04/08 15:47:49 erik Exp $ */
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

char *
XkbAtomGetString(dpy,atm)
    Display *	dpy;
    Atom 	atm;
{
    if (atm==None)
	return NULL;
    if (dpy==NULL)
	return stGetString((StringToken)atm);
    return XGetAtomName(dpy,atm);
}

/***====================================================================***/

Atom
XkbInternAtom(dpy,name,onlyIfExists)
    Display *	dpy;
    char *	name;
    Bool	onlyIfExists;
{
    if (name==NULL)
	return None;
    if (dpy==NULL) {
	if (onlyIfExists) {
	    StringToken tok;
	    if (stGetTokenIfExists(name,&tok))
		 return (Atom)tok;
	    return None;
	}
	return (Atom)stGetToken(name);
    }
    return XInternAtom(dpy,name,onlyIfExists);
}

/***====================================================================***/

Atom
XkbChangeAtomDisplay(oldDpy,newDpy,atm)
    Display *	oldDpy;
    Display *	newDpy;
    Atom	atm;
{
char *tmp;

    if (atm!=None) {
	tmp= XkbAtomGetString(oldDpy,atm);
	if (tmp!=NULL)
	    return XkbInternAtom(newDpy,tmp,False);
    }
    return None;
}

/***====================================================================***/

void
XkbInitAtoms(dpy)
    Display *dpy;
{
    if (dpy==NULL) {
	stInit(40,20,False,NULL);
    }
    return;
}
