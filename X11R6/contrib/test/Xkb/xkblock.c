/* $XConsortium: xkblock.c,v 1.4 94/04/01 18:50:15 erik Exp $ */
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
#include <X11/Xfuncs.h>

static	char		*dpyName = NULL;
static	unsigned	 device = XkbUseCoreKbd;
static	unsigned	 mask = 0;
static	unsigned	 value = 0;
static	unsigned	 latch = 0;

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
	else if (strcmp(argv[i],"-latch")==0) {
	    latch= 1;
	}
	else if (strcmp(argv[i],"-latch")==0) {
	    latch= 0;
	}
	else if (strlen(argv[i])==2) {
	    switch (argv[i][1]) {
		case 's': case 'S':
		    mask|= ShiftMask;
		    if ( argv[i][0]=='-' )	value&= ~ShiftMask;
		    else if (argv[i][0]=='+')	value|= ShiftMask;
		    else			return 0;
		    break;
		case 'C':
		    mask|= LockMask;
		    if ( argv[i][0]=='-' )	value&= ~LockMask;
		    else if (argv[i][0]=='+')	value|= LockMask;
		    else			return 0;
		    break;
		case 'c':
		    mask|= ControlMask;
		    if ( argv[i][0]=='-' )	value&= ~ControlMask;
		    else if (argv[i][0]=='+')	value|= ControlMask;
		    else			return 0;
		    break;
		case 'a': case 'A': case 'm': case 'M': case '1':
		    mask|= Mod1Mask;
		    if ( argv[i][0]=='-' )	value&= ~Mod1Mask;
		    else if (argv[i][0]=='+')	value|= Mod1Mask;
		    else			return 0;
		    break;
		case '2':
		    mask|= Mod2Mask;
		    if ( argv[i][0]=='-' )	value&= ~Mod2Mask;
		    else if (argv[i][0]=='+')	value|= Mod2Mask;
		    else			return 0;
		    break;
		case '3':
		    mask|= Mod3Mask;
		    if ( argv[i][0]=='-' )	value&= ~Mod3Mask;
		    else if (argv[i][0]=='+')	value|= Mod3Mask;
		    else			return 0;
		    break;
		case '4':
		    mask|= Mod4Mask;
		    if ( argv[i][0]=='-' )	value&= ~Mod4Mask;
		    else if (argv[i][0]=='+')	value|= Mod4Mask;
		    else			return 0;
		    break;
		case '5':
		    mask|= Mod5Mask;
		    if ( argv[i][0]=='-' )	value&= ~Mod5Mask;
		    else if (argv[i][0]=='+')	value|= Mod5Mask;
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
XkbStateRec	state;

  
    if (!parseArgs(argc,argv)) {
	fprintf(stderr,"Usage: %s <options>\n",argv[0]);
	fprintf(stderr,"Where legal options are:\n");
	fprintf(stderr,"-display <dpy>  specifies display to use\n");
	fprintf(stderr,"-latch          latch specified modifiers\n");
	fprintf(stderr,"-lock           lock specified modifiers (default)\n");
	fprintf(stderr,"[+-][sS]        set/clear shift\n");
	fprintf(stderr,"[+-]C           set/clear caps\n");
	fprintf(stderr,"[+-]c           set/clear control\n");
	fprintf(stderr,"[+-][amAM1]     set/clear mod1 (alt/meta)\n");
	fprintf(stderr,"[+-]2           set/clear mod2\n");
	fprintf(stderr,"[+-]3           set/clear mod3\n");
	fprintf(stderr,"[+-]4           set/clear mod4\n");
	fprintf(stderr,"[+-]5           set/clear mod5\n");
	return 1;
    }
    if ( mask==0 ) {
	mask = LockMask;
	value = LockMask;
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
    if (!XkbGetState(dpy,XkbUseCoreKbd,&state)) {
	fprintf(stderr,"XkbGetState failed\n");
	exit(1);
    }
    printf("       ------- group --------    ------- modifiers -------------\n");
    printf("       eff  base  latch  lock     eff  base  latch  lock  compat\n");
    printf("before: %2d    %2d     %2d    %2d    0x%02x  0x%02x   0x%02x  0x%02x    0x%02x\n",
       state.group, state.base_group, state.latched_group, state.locked_group,
       state.mods, state.base_mods, state.latched_mods, state.locked_mods,
       state.compat_state);
    if (latch)		XkbLatchModifiers(dpy,device,mask,value);
    else 		XkbLockModifiers(dpy,device,mask,value);
    bzero(&state,sizeof(state));
    if (!XkbGetState(dpy,XkbUseCoreKbd,&state)) {
	fprintf(stderr,"XkbGetState failed\n");
	exit(1);
    }
    printf("after:  %2d    %2d     %2d    %2d    0x%02x  0x%02x   0x%02x  0x%02x    0x%02x\n",
       state.group, state.base_group, state.latched_group, state.locked_group,
       state.mods, state.base_mods, state.latched_mods, state.locked_mods,
       state.compat_state);
    XCloseDisplay(dpy);
    return 0;
BAIL:
    XCloseDisplay(dpy);
    return 0;
}
