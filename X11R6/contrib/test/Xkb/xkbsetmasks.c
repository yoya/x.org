/* $XConsortium: xkbsetmasks.c,v 1.4 94/04/01 18:50:17 erik Exp $ */
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

#include <stdio.h>
#include <X11/Xproto.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/XKBlib.h>

#define	SERVER_INTERNAL	0
#define	KEY_ONLY	1

static	char		*dpyName = NULL;
static	unsigned	 device = XkbUseCoreKbd;
static	unsigned	 which = SERVER_INTERNAL;
static	unsigned	 mask = 0;
static	unsigned	 state = 0;

int
parseArgs(argc,argv)
    int argc;
    char *argv[];
{
int i;

    for (i=1;i<argc;i++) {
	if ( strcmp(argv[i],"-display")==0 ) {
	    if ( ++i<argc )	dpyName= argv[i];
	    else {
		fprintf(stderr,"Must specify a display with -display option\n");
		return 0;
	    }
	}
	else if ( strcmp(argv[i],"-device")==0 ) {
	    if ( ++i<argc ) {
		if (sscanf(argv[i],"%i",&device)!=1) {
		    fprintf(stderr,"Device specification must be an integer\n");
		    return 0;
		}
	    }
	    else {
		fprintf(stderr,"Must specify a device with -device option\n");
		return 0;
	    }
	}
	else if (strcmp(argv[i],"-internal")==0)
	    which = SERVER_INTERNAL;
	else if (strcmp(argv[i],"-keyonly")==0)
	    which = KEY_ONLY;
	else if (strlen(argv[i])==2) {
	    switch (argv[i][1]) {
		case 's': case 'S':
		    mask|= ShiftMask;
		    if ( argv[i][0]=='-' )	state&= ~ShiftMask;
		    else if (argv[i][0]=='+')	state|= ShiftMask;
		    else			return 0;
		    break;
		case 'C':
		    mask|= LockMask;
		    if ( argv[i][0]=='-' )	state&= ~LockMask;
		    else if (argv[i][0]=='+')	state|= LockMask;
		    else			return 0;
		    break;
		case 'c':
		    mask|= ControlMask;
		    if ( argv[i][0]=='-' )	state&= ~ControlMask;
		    else if (argv[i][0]=='+')	state|= ControlMask;
		    else			return 0;
		    break;
		case 'a': case 'A': case 'm': case 'M': case '1':
		    mask|= Mod1Mask;
		    if ( argv[i][0]=='-' )	state&= ~Mod1Mask;
		    else if (argv[i][0]=='+')	state|= Mod1Mask;
		    else			return 0;
		    break;
		case '2':
		    mask|= Mod2Mask;
		    if ( argv[i][0]=='-' )	state&= ~Mod2Mask;
		    else if (argv[i][0]=='+')	state|= Mod2Mask;
		    else			return 0;
		    break;
		case '3':
		    mask|= Mod3Mask;
		    if ( argv[i][0]=='-' )	state&= ~Mod3Mask;
		    else if (argv[i][0]=='+')	state|= Mod3Mask;
		    else			return 0;
		    break;
		case '4':
		    mask|= Mod4Mask;
		    if ( argv[i][0]=='-' )	state&= ~Mod4Mask;
		    else if (argv[i][0]=='+')	state|= Mod4Mask;
		    else			return 0;
		    break;
		case '5':
		    mask|= Mod5Mask;
		    if ( argv[i][0]=='-' )	state&= ~Mod5Mask;
		    else if (argv[i][0]=='+')	state|= Mod5Mask;
		    else			return 0;
		    break;
	    }
	}
	else {
	    fprintf(stderr,"Unknown option %s\n",argv[i]);
	    return 0;
	}
    }
    return 1;
}

int
main(argc,argv)
    int argc;
    char *argv[];
{
Display	*dpy;
int	i1,i2,i3,i4,i5;
unsigned	query;

  
    if (!parseArgs(argc,argv)) {
	fprintf(stderr,"Usage: %s <options>\n",argv[0]);
	fprintf(stderr,"Where legal options are:\n");
	fprintf(stderr,"-display <dpy>     specifies display to use\n");
	fprintf(stderr,"[+-][sS]           filter/report shift\n");
	fprintf(stderr,"[+-]C              filter/report caps\n");
	fprintf(stderr,"[+-]c              filter/report control\n");
	fprintf(stderr,"[+-][amAM1]        filter/report mod1 (alt/meta)\n");
	fprintf(stderr,"[+-]2              filter/report mod2\n");
	fprintf(stderr,"[+-]3              filter/report mod3\n");
	fprintf(stderr,"[+-]4              filter/report mod4\n");
	fprintf(stderr,"[+-]5              filter/report mod5\n");
	fprintf(stderr,"\nChanges the set of \"server internal\" modifiers.\n");
	fprintf(stderr,"Server internal modifiers are not reported to clients\n");
	fprintf(stderr,"in key, button or motion events.\n");
	return 1;
    }
    if ( mask==0 ) {
	mask = 0xff;
	state = 0;
    }
    dpy = XOpenDisplay(dpyName);
    if ( !dpy )
	return 1;
    if ( !XkbQueryExtension(dpy,&i1,&i2,&i3,&i4,&i5)>0 ) {
	fprintf(stderr,"query failed\n");
	goto BAIL;
    }
    if ( !XkbUseExtension(dpy) ) {
	fprintf(stderr,"use extension failed (%d,%d)\n",i4,i5);
	goto BAIL;
    }
    XSynchronize(dpy,1);
    if ( which==SERVER_INTERNAL ) {
	printf("Setting server internal modifiers\n");
	XkbSetServerInternalMods(dpy,device,mask,state);
    }
    else if ( which==KEY_ONLY ) {
	printf("Setting \"key event only\" modifiers\n");
	XkbSetIgnoreLockMods(dpy,device,mask,state);
    }
    XCloseDisplay(dpy);
    return 0;
BAIL:
    XCloseDisplay(dpy);
    return 0;
}
