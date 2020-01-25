/* $XConsortium: xkbtest.c,v 1.2 94/04/01 18:50:19 erik Exp $ */
/************************************************************
Copyright (c) 1993 by Silicon Graphics Computer Systems, Inc.

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

#include <sys/types.h>
#include <stdio.h>
#include <X11/Xproto.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>

#ifdef sgi
#include <malloc.h>
#endif

static	char		*dpyName = NULL;
static	int		 synch = 0;
static	int		 wantDebug= 0;
static	unsigned	 flags = 0;
static	unsigned	 mask = 0;
static 	char		*msg= NULL;
static	int		 startTest= -1;

int
parseArgs(argc,argv)
    int argc;
    char *argv[];
{
int i;
unsigned on= 0;
unsigned off= 0;

    if (argc<2)
	return 0;
    if (argv[1][0]!='-') {
	if (strcasecmp(argv[1],"start")==0)	startTest= 1;
	else if (strcasecmp(argv[1],"end")==0)	startTest= 0;
	if (argc>2) {
	    unsigned int tmp;
	    wantDebug= 1;
	    if (sscanf(argv[2],"%i",&tmp)!=1) {
		fprintf(stderr,"Flag specification must be an integer\n");
		return 0;
	    }
	    mask= ~0;
	    flags= tmp;
	    if (argc>3)
		msg= argv[3];
	    if (argc>4) {
		fprintf(stderr,"Warning! Extra options on command line\n");
		for (tmp=4;tmp<argc;tmp++) {
		    fprintf(stderr,"         %s ignored\n",argv[tmp]);
		}
	    }
	}
	return 1;
    }
    for (i=1;i<argc;i++) {
	if ( strcmp(argv[i],"-display")==0 ) {
	    if ( ++i<argc )	dpyName= argv[i];
	    else {
		fprintf(stderr,"Must specify a display with -display option\n");
		return 0;
	    }
	}
	else if ( strcmp(argv[i],"-synch")==0 ) {
	    synch= 1;
	}
	else if ( strcmp(argv[i],"-start")==0 ) {
	    startTest= 1;
	}
	else if ( strcmp(argv[i],"-end")==0 ) {
	    startTest= 0;
	}
	else if ( strcmp(argv[i],"-debugon")==0 ) {
	    if ( ++i<argc ) {
		if (sscanf(argv[i],"%i",&on)!=1) {
		    fprintf(stderr,"Flag specification must be an integer\n");
		    return 0;
		}
	    }
	    else {
		fprintf(stderr,"Must specify flags with -on option\n");
		return 0;
	    }
	}
	else if ( strcmp(argv[i],"-debugoff")==0 ) {
	    if ( ++i<argc ) {
		if (sscanf(argv[i],"%i",&off)!=1) {
		    fprintf(stderr,"Flag specification must be an integer\n");
		    return 0;
		}
	    }
	    else {
		fprintf(stderr,"Must specify flags with -off option\n");
		return 0;
	    }
	}
	else if ( strcmp(argv[i],"-msg")==0 ) {
	    if ( ++i<argc )
		msg= argv[i];
	}
#ifdef sgi
	else if ( strcmp(argv[i],"-mdebug")==0 ) {
	    mallopt(M_DEBUG,0xff);
	}
#endif
	else {
	    fprintf(stderr,"Unknown option %s\n",argv[i]);
	    return 0;
	}
    }
    if (startTest<0) {
	fprintf(stderr,"Must specify whether to start or end testing\n");
	return 0;
    }
    if (on&off) {
	fprintf(stderr,"Warning! On and off (0x%x and 0x%x) overlap\n",on,off);
	fprintf(stderr,"         Conflicting flags will be turned on\n");
    }
    flags= on;
    mask= (on|off);
    if (mask || (msg!=NULL))
	wantDebug= 1;
    return 1;
}

static void
ChangeCompatMap(dpy,xkb,start)
    Display *dpy;
    XkbDescPtr xkb;
    int	start;
{
register int i;
XkbSymInterpretPtr si;

    si= xkb->compat->sym_interpret;
    for (i=0;i<xkb->compat->num_si;i++,si++) {
	if (start) {
	    switch(si->act.type) {
		case XkbSA_ISOLock:
		case XkbSA_LockMods:
		case XkbSA_LatchMods:
		    si->act.type= XkbSA_SetMods;
		    break;
		case XkbSA_LatchGroup:
		case XkbSA_LockGroup:
		    si->act.type= XkbSA_SetGroup;
		    break;
	    }
	}
	else if (si->act.type==XkbSA_SetMods) {
	    switch (si->sym) {
		case XK_Num_Lock:
		case XK_Scroll_Lock:
		case XK_Caps_Lock:
		case XK_Shift_Lock:
		    si->act.type= XkbSA_LockMods;
		    break;
		case XK_ISO_Lock:
		    si->act.type= XkbSA_ISOLock;
		    break;
		case XK_ISO_Level2_Latch:
		case XK_ISO_Level3_Latch:
		    si->act.type= XkbSA_LatchMods;
		    break;
	    }
	}
	else if (si->act.type==XkbSA_SetGroup) {
	    switch (si->sym) {
		case XK_Mode_switch:
		    si->act.type= XkbSA_SetGroup;
		    break;
		case XK_ISO_Group_Latch:
		    si->act.type= XkbSA_LatchGroup;
		    break;
		case XK_ISO_Next_Group:
		case XK_ISO_Prev_Group:
		    si->act.type= XkbSA_LockGroup;
		    break;
	    }
	}
    }
}

int
main(argc,argv)
    int argc;
    char *argv[];
{
Display	*dpy;
int	i1,i2,i3,i4,i5;
unsigned	query;
unsigned int	rtrnFlags;
XkbDescPtr	xkb;

  
    if (!parseArgs(argc,argv)) {
	fprintf(stderr,"Usage: %s [<options>] or %s {start,end} [ <flags> [ <message> ] ]\n",
								argv[0]);
	fprintf(stderr,"Where legal options are:\n");
	fprintf(stderr,"-display  <dpy>     specifies display to use\n");
	fprintf(stderr,"-synch              force synchronization\n");
	fprintf(stderr,"-start              indicates start of testing\n");
	fprintf(stderr,"-end                indicates end of testing\n");
	fprintf(stderr,"-debugon  <flags>   debugging flags to turn on\n");
	fprintf(stderr,"-debugoff <flags>   debugging flags to turn off\n");
	fprintf(stderr,"-msg      <message> message to print in log\n");
#ifdef sgi
	fprintf(stderr,"-mdebug             turn on allocator debugging\n");
#endif
	fprintf(stderr,"At the start of testing, disables all XKB controls\n");
	fprintf(stderr,"and changes any LockMods or LatchMods key acts\n");
	fprintf(stderr,"to SetMods, which makes the key map conform to some\n");
	fprintf(stderr,"assumptions made by the X test suite.\n");
	fprintf(stderr,"When testing ends, returns XKB controls and acts\n");
	fprintf(stderr,"to their defaults, which does not necessarily\n");
	fprintf(stderr,"correspond to their state at the start of testing\n");
	return 1;
    }
    dpy = XOpenDisplay(dpyName);
    if ( !dpy )
	return 1;
    if (synch)
	XSynchronize(dpy,1);
    if ( !XkbQueryExtension(dpy,&i1,&i2,&i3,&i4,&i5)>0 ) {
	fprintf(stderr,"query failed\n");
	goto BAIL;
    }
    if ( !XkbUseExtension(dpy) ) {
	fprintf(stderr,"use extension failed (%d,%d)\n",i4,i5);
	goto BAIL;
    }
    if (wantDebug) {
	XkbSetDebuggingFlags(dpy,mask,flags,msg,&rtrnFlags,NULL);
	fprintf(stderr,"Debugging flags set to 0x%x\n",rtrnFlags);
    }
    xkb= XkbGetMap(dpy,0,XkbUseCoreKbd);
    if (!xkb) {
	fprintf(stderr,"Couldn't get keyboard map.\n");
	goto BAIL;
    }
    if (!XkbGetControls(dpy,XkbAllControlsMask,xkb)) {
	fprintf(stderr,"Couldn't get keyboard controls.\n");
	goto BAIL;
    }
    if (!XkbGetCompatMap(dpy,XkbAllCompatMask,xkb)) {
	fprintf(stderr,"Couldn't get compatibility map.\n");
	goto BAIL;
    }
    ChangeCompatMap(dpy,xkb,startTest);
    XkbSetCompatMap(dpy,XkbAllCompatMask,xkb,True);
    XCloseDisplay(dpy);
    return 0;
BAIL:
    XCloseDisplay(dpy);
    return 0;
}
