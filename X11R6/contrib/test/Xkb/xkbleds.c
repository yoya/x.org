/* $XConsortium: xkbleds.c,v 1.4 94/04/01 18:50:14 erik Exp $ */
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

char *
atomText(dpy,atom)
   Display *	dpy;
   Atom		atom;
{
static char buf[256];
char	*name = XGetAtomName(dpy,atom);

    if ( atom==None ) {
	buf[0]= '\0';
    }
    else {
	if ( name ) {
	    strncpy(buf,name,255);
	    buf[255]= '\0';
	    XFree(name);
	}
	else {
	    sprintf(buf,"(unknown)");
	}
    }
    return buf;
}

char *
indComponentText(which)
    CARD8 which;
{
register int i;
static char buf[8];
char *str;

    if (which) {
	str= buf;
	if (which&XkbIM_UseBase)
	    *str++= 'b';
	if (which&XkbIM_UseLatched)
	    *str++= 'l';
	if (which&XkbIM_UseLocked)
	    *str++= 'L';
	if (which&XkbIM_UseEffective)
	    *str++= 'e';
	if (which&XkbIM_UseCompat)
	    *str++= 'c';
	*str++= '\0';
    }
    else strcpy(buf,"none");
    return buf;
}

/***====================================================================***/

static	char		*dpyName = NULL;
static	int		 showMapping;
static	int		 synch = 0;

static	CARD32			changed;
static	XkbIndicatorMapRec	map[XkbNumIndicators];

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
	else if ( strcmp(argv[i],"-synch")==0 ) {
	    synch= 1;
	}
	else if ( strcmp(argv[i],"-v")==0 ) {
	    showMapping = 1;
	}
	else if ( isdigit(argv[i][0]) ) {
	    char buf[20],*tmp;
	    int  led,mods,vmods,group,ctrls,which;
	    int	   got;
	    led= mods= group= ctrls= which= 0;
	    got= sscanf(argv[i],"%i %s %i %i %i %i",
				&led,buf,&mods,&vmods,&group,&ctrls);
	    if (got<6) {
		fprintf(stderr,"Couldn't parse map for indicator %d\n",led);
		return 0;
	    }
	    if ((led<1)||(led>XkbNumIndicators)) {
		fprintf(stderr,"Illegal led %d (must be 1..%d)\n",led,
							XkbNumIndicators);
		return 0;
	    }
	    led--;
	    tmp= buf;
	    while (*tmp) {
		if (*tmp=='b')		which|= XkbIM_UseBase;
		else if (*tmp=='l')	which|= XkbIM_UseLatched;
		else if (*tmp=='L')	which|= XkbIM_UseLocked;
		else if (*tmp=='e')	which|= XkbIM_UseEffective;
		else if (*tmp=='c')	which|= XkbIM_UseCompat;
		else if (*tmp=='x')	which= 0;
		else {
		    fprintf(stderr,"Unknown specifier '%c' ignored\n",*tmp);
		}
		tmp++;
	    }
	    changed|= (1<<led);
	    map[led].which_mods= which;
	    map[led].real_mods= mods;
	    map[led].vmods= vmods;
	    map[led].which_groups= which;
	    map[led].groups= group;
	    map[led].ctrls= ctrls;
	}
	else {
	    fprintf(stderr,"Unknown option %s\n",argv[i]);
	    return 0;
	}
    }
    if (argc==1)
	showMapping= 1;
    return 1;
}

void
showMaps(dpy,desc)
    Display *dpy;
    XkbDescRec *desc;
{
register int i;
CARD8	*action;
char	*name;
XkbIndicatorRec *leds;
XkbNamesRec	*names;
unsigned state;

    XkbGetIndicatorState(dpy,XkbUseCoreKbd,&state);
    printf("state: 0x%08x\n",state);
    leds= desc->indicators;
    names= desc->names;
    printf("%d physical indicators\n",leds->num_phys_indicators);
    for (i=0;i<XkbNumIndicators;i++) {
	if ((leds->maps[i].which_mods==0)&&(leds->maps[i].which_groups==0)&&
	    (leds->maps[i].ctrls==0)&&(leds->maps[i].flags==0))
	    continue;
	printf("Indicator %d",i+1);
	if (names&&(name=atomText(dpy,names->indicators[i]))&&(name[0]))
	    printf(" (%s)",name);
	printf(":\n");
	if (leds->maps[i].flags&XkbIM_NoExplicit)
	    printf("  explicit control disabled\n");
	if (leds->maps[i].flags&XkbIM_NoAutomatic)
	    printf("  automatic control disabled\n");
	printf("          mask: 0x%x (%s)\n",leds->maps[i].mask,
				indComponentText(leds->maps[i].which_mods));
	printf("     real mods: 0x%x\n",leds->maps[i].real_mods);
	printf("  virtual mods: 0x%x\n",leds->maps[i].vmods);
	printf("        groups: 0x%x (%s)\n",leds->maps[i].groups,
				indComponentText(leds->maps[i].which_groups));
	printf("      controls: 0x%x\n",leds->maps[i].ctrls);
    }
    return;
}

int
main(argc,argv)
    int argc;
    char *argv[];
{
Display	*dpy;
int	i1,i2,i3,i4,i5;
XkbDescRec	*desc;
unsigned	 	 query;

  
    if (!parseArgs(argc,argv)) {
	fprintf(stderr,"Usage: %s <options> [map [map...]]\n",argv[0]);
	fprintf(stderr,"Where legal options are:\n");
	fprintf(stderr,"-display <dpy>     specifies display to use\n");
	fprintf(stderr,"-v                 show current indicator mapping\n");
	fprintf(stderr,"-synch             turn on synchronization\n");
	fprintf(stderr,"Indicator maps have the format:\n");
	fprintf(stderr,"    led which mods vmods group ctrls\n");
	fprintf(stderr,"led                specifies led map to change\n");
	fprintf(stderr,"which              state components that affect map\n");
	fprintf(stderr,"                   [blLecx]+\n");
	fprintf(stderr,"mods               modifiers that affect the map\n");
	fprintf(stderr,"vmods              virtual mods that affect the map\n");
	fprintf(stderr,"group              groups that affect the map\n");
	fprintf(stderr,"controls           controls that affect the map\n");
	fprintf(stderr,"All fields except which have integer values that\n");
	fprintf(stderr,"are interpreted as bit masks.  If no indicator maps\n");
	fprintf(stderr,"are specified on the command line, all current maps\n");
	fprintf(stderr,"and the current state are reported.\n");
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
    desc = XkbGetMap(dpy,0,XkbUseCoreKbd);
    if (!desc) {
	fprintf(stderr,"XkbGetMap failed\n");
	goto BAIL;
    }

    if (!XkbGetIndicatorMap(dpy,0xFFFFFFFF,desc)) {
	fprintf(stderr,"GetIndicatorMapping failed\n");
	goto BAIL;
    }
    if ( showMapping )
	showMaps(dpy,desc);
    if (changed) {
	for (i1=0,i2=1;i1<XkbNumIndicators;i1++,i2<<=1) {
	    if (changed&i2)
		desc->indicators->maps[i1]= map[i1];
	}
	XkbSetIndicatorMap(dpy,changed,desc);
    }
    XCloseDisplay(dpy);
    return 0;
BAIL:
    XCloseDisplay(dpy);
    return 1;
}
