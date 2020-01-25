/* $XConsortium: xkbwatch.c,v 1.4 94/04/01 18:50:21 erik Exp $ */
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
#define NEED_EVENTS
#include <X11/Xproto.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/XKBlib.h>

static	char		*dpyName = NULL;
static	unsigned	 device = XkbUseCoreKbd;
static	unsigned	 which = XkbAllStateComponentsMask;
static	int		 page_size = 24;
static	int		 synch= 0;

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
	else if ( strcmp(argv[i],"-synch")==0 ) {
	    synch= 1;
	}
	else if ( strcmp(argv[i],"-page")==0 ) {
	    if ( ++i<argc ) {
		if ((sscanf(argv[i],"%i",&page_size)!=1)||(i<1)) {
		    fprintf(stderr,"Device specification must be a positive integer\n");
		    return 0;
		}
	    }
	    else {
		fprintf(stderr,"Must specify a device with -device option\n");
		return 0;
	    }
	}
	else if (strlen(argv[i])==2) {
	    switch (argv[i][1]) {
		case 'a': case 'A':
		    if (argv[i][0]=='-')      which&=~XkbAllStateComponentsMask;
		    else if (argv[i][0]=='+') which|= XkbAllStateComponentsMask;
		    else			return 0;
		    break;
		case 's': case 'S':
		    if (argv[i][0]=='-')	which&= ~XkbModifierStateMask;
		    else if (argv[i][0]=='+')	which|= XkbModifierStateMask;
		    else			return 0;
		    break;
		case 'g': 
		    if (argv[i][0]=='-')	which&= ~XkbGroupStateMask;
		    else if (argv[i][0]=='+')	which|= XkbGroupStateMask;
		    else			return 0;
		    break;
		case 'G': 
		    if (argv[i][0]=='-')	which&= ~XkbGroupLatchMask;
		    else if (argv[i][0]=='+')	which|= XkbGroupLatchMask;
		    else			return 0;
		    break;
		case 'l':
		    if (argv[i][0]=='-')	which&= ~XkbModifierLockMask;
		    else if (argv[i][0]=='+')	which|= XkbModifierLockMask;
		    else			return 0;
		    break;
		case 'L':
		    if (argv[i][0]=='-')	which&= ~XkbModifierLatchMask;
		    else if (argv[i][0]=='+')	which|= XkbModifierLatchMask;
		    else			return 0;
		    break;
		case 'c': case 'C':
		    if (argv[i][0]=='-')	which&= ~XkbCompatStateMask;
		    else if (argv[i][0]=='+')	which|= XkbCompatStateMask;
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
int	i1,i2,i3,i4;
int		ev_base,num_out;
XkbEvent		xev;
XkbStateNotifyEvent	*sn;

  
    if (!parseArgs(argc,argv)) {
	fprintf(stderr,"Usage: %s <options>\n",argv[0]);
	fprintf(stderr,"Where legal options are:\n");
	fprintf(stderr,"-display <dpy>     specifies display to use\n");
	fprintf(stderr,"-synch             force synchronization\n");
	fprintf(stderr,"-device <id>       specifies device to use\n");
	fprintf(stderr,"-page              specifies lines per output page\n");
	fprintf(stderr,"[+-][aA]           watch/ignore all components\n");
	fprintf(stderr,"[+-][cC]           watch/ignore compatibility state\n");
	fprintf(stderr,"[+-]g              watch/ignore keyboard group\n");
	fprintf(stderr,"[+-]G              watch/ignore latched group\n");
	fprintf(stderr,"[+-]l              watch/ignore latched modifiers\n");
	fprintf(stderr,"[+-]L              watch/ignore locked modifiers\n");
	fprintf(stderr,"[+-][sS]           watch/ignore modifier state\n");
	return 1;
    }
    if ( which==0 ) {
	fprintf(stderr,"You must specify at least one component to watch\n");
	exit(1);
    }
    dpy = XOpenDisplay(dpyName);
    if ( !dpy )
	return 1;
    if ( !XkbQueryExtension(dpy,&i1,&ev_base,&i2,&i3,&i4)>0 ) {
	fprintf(stderr,"query failed\n");
	goto BAIL;
    }
    if ( !XkbUseExtension(dpy) ) {
	fprintf(stderr,"use extension failed (%d,%d)\n",i3,i4);
	goto BAIL;
    }
    if (synch)
	XSynchronize(dpy,1);
    XkbSelectEventDetails(dpy,device,XkbStateNotify,which,which);
    num_out = 0;
    printf("Watching the keyboard state...\n");
    while (1) {
	XNextEvent(dpy,&xev.core);
	if ((xev.type==ev_base+XkbEventCode)&&
	    (xev.any.xkb_type==XkbStateNotify)) {
	    if ((num_out%page_size)==0) {
		printf("                ------- group --------  ------- modifiers -------------\n");
		printf("id key   event  eff  base  latch  lock   eff  base  latch  lock  compat\n");
	    }
	    num_out++;
	    sn= &xev.state;
	    printf("%2d  %2d",sn->device, sn->keycode);
		if (sn->keycode!=0)
		     printf(" %7s",(sn->event_type==KeyPress?"down":"up"));
		else {
		    char buf[30];
		    sprintf(buf,"%d/%d",sn->req_major,sn->req_minor);
		    printf(" %7s",buf);
		}
		printf("   %2d%c   %2d%c    %2d%c   %2d%c",
				sn->group,
				(sn->changed&XkbGroupStateMask?'*':' '),
				sn->base_group,
				(sn->changed&XkbGroupBaseMask?'*':' '),
				sn->latched_group,
				(sn->changed&XkbGroupLatchMask?'*':' '),
				sn->locked_group,
				(sn->changed&XkbGroupLockMask?'*':' '));
		printf(" 0x%02x%c 0x%02x%c  0x%02x%c 0x%02x%c   0x%02x%c\n",
				sn->mods,
				(sn->changed&XkbModifierStateMask?'*':' '),
				sn->base_mods,
				(sn->changed&XkbModifierBaseMask?'*':' '),
				sn->latched_mods,
				(sn->changed&XkbModifierLatchMask?'*':' '),
				sn->locked_mods,
				(sn->changed&XkbModifierLockMask?'*':' '),
				sn->compat_state,
				(sn->changed&XkbCompatStateMask?'*':' '));
	}
	else {
	    fprintf(stderr,"Unknown event type %d\n",xev.type);
	}
    }
    XCloseDisplay(dpy);
    return 0;
BAIL:
    XCloseDisplay(dpy);
    return 0;
}
