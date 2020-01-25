/* $XConsortium: xkbdebug.c,v 1.4 94/04/01 18:50:11 erik Exp $ */
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
#include <ctype.h>
#include <X11/Xproto.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/XKBlib.h>
#include <X11/extensions/XKBproto.h>

static	char		*dpyName = NULL;
static	unsigned	 flags = 0;
static	unsigned	 mask = 0;
static 	char		*msg= NULL;
static	Bool		 disableLocks = XkbLeaveLocks;

int
parseArgs(argc,argv)
    int argc;
    char *argv[];
{
int i;
unsigned on= 0;
unsigned off= 0;

    if ((argc>1)&&(isdigit(argv[1][0]))) {
	unsigned int tmp;
	if (sscanf(argv[1],"%i",&tmp)!=1) {
	    fprintf(stderr,"Flag specification must be an integer\n");
	    return 0;
	}
	mask= ~0;
	flags= tmp;
	if (argc>2)
	    msg= argv[2];
	if (argc>3) {
	    fprintf(stderr,"Warning! Extra options on command line\n");
	    for (tmp=3;tmp<argc;tmp++) {
		fprintf(stderr,"         %s ignored\n",argv[tmp]);
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
	else if ( strcmp(argv[i],"-on")==0 ) {
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
	else if ( strcmp(argv[i],"-off")==0 ) {
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
	else if ( strcmp(argv[i],"-nolock")==0 ) {
	    disableLocks= True;
	}
	else {
	    fprintf(stderr,"Unknown option %s\n",argv[i]);
	    return 0;
	}
    }
    if (on&off) {
	fprintf(stderr,"Warning! On and off (0x%x and 0x%x) overlap\n",on,off);
	fprintf(stderr,"         Conflicting flags will be turned on\n");
    }
    flags= on;
    mask= (on|off);
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
unsigned int	rtrnFlags;

  
    if (!parseArgs(argc,argv)) {
	fprintf(stderr,"Usage: %s [<options>] or %s <flags> [ <message> ]\n",
								argv[0]);
	fprintf(stderr,"Where legal options are:\n");
	fprintf(stderr,"-display <dpy>     specifies display to use\n");
	fprintf(stderr,"-on      <flags>   debugging flags to turn on\n");
	fprintf(stderr,"-off     <flags>   debugging flags to turn off\n");
	fprintf(stderr,"-msg     <message> message to print in log\n");
	fprintf(stderr,"-nolock            disable locking and latching\n");
	fprintf(stderr,"If no options are specified, %s prints the current\n",
								argv[0]);
	fprintf(stderr,"value of the server debugging flags and returns.\n");
	fprintf(stderr,"The second form sets the server debugging flags to\n");
	fprintf(stderr,"exactly <flags> and prints <message>, if specified,\n");
	fprintf(stderr,"to the log file.\n");
	return 1;
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
    XkbSetDebuggingFlags(dpy,mask,flags,msg,&rtrnFlags,&disableLocks);
    if (mask)
	 fprintf(stderr,"Debugging flags set to 0x%x\n",rtrnFlags);
    else fprintf(stderr,"Current debuggging flags: 0x%x\n",rtrnFlags);
    fprintf(stderr,"Locks are %sabled\n",
				(disableLocks==XkbDisableLocks?"dis":"en"));
    XCloseDisplay(dpy);
    return 0;
BAIL:
    XCloseDisplay(dpy);
    return 0;
}
