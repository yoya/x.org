/* $XConsortium: xkbrefresh.c,v 1.3 94/04/01 18:50:16 erik Exp $ */
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

/***====================================================================***/

static	char		*dpyName = NULL;
static	int		 changeAll = 0;
static	int		 synch = 0;
static	unsigned	 full;
static	XkbMapChangesRec changes;
static	unsigned	whichNames = XkbAllNamesMask;
static	CARD8		firstNamesKeyType = 0;
static	CARD8		nNamesKeyTypes = 0;

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
	else if ((strcmp(argv[i],"-t")==0) || (strcmp(argv[i],"-types")==0)){
	    if ((i+1<argc)&&(argv[i+1][0]!='-')) {
		int low,high;
		if (sscanf(argv[++i],"%i-%i",&low,&high)==2) {
		    full&= ~XkbKeyTypesMask;
		    changes.changed|= XkbKeyTypesMask;
		    changes.first_type = low;
		    changes.num_types = high-low+1;
		    continue;
		}
		fprintf(stderr,"must specify a range of key types\n");
		return 0;
	    }
	    else full|= XkbKeyTypesMask;
	}
	else if ((strcmp(argv[i],"-s")==0) || (strcmp(argv[i],"-syms")==0)){
	    if ((i+1<argc)&&(argv[i+1][0]!='-')) {
		int low,high;
		if (sscanf(argv[++i],"%i-%i",&low,&high)!=2) {
		    full&= ~XkbKeySymsMask;
		    changes.changed|= XkbKeySymsMask;
		    changes.first_key_sym = low;
		    changes.num_key_syms = high-low+1;
		    continue;
		}
		fprintf(stderr,"must specify a range of key syms\n");
		return 0;
	    }
	    else full|= XkbKeySymsMask;
	}
	else if ((strcmp(argv[i],"-a")==0) || (strcmp(argv[i],"-acts")==0)){
	    if ((i+1<argc)&&(argv[i+1][0]!='-')) {
		int low,high;
		if (sscanf(argv[++i],"%i-%i",&low,&high)!=2) {
		    full&= ~XkbKeyActionsMask;
		    changes.changed|= XkbKeyActionsMask;
		    changes.first_key_act = low;
		    changes.num_key_acts = high-low+1;
		    continue;
		}
		fprintf(stderr,"must specify a range of acts\n");
		return 0;
	    }
	    else full|= XkbKeyActionsMask;
	}
	else if ((strcmp(argv[i],"-b")==0)||(strcmp(argv[i],"-behaviors")==0)){
	    if ((i+1<argc)&&(argv[i+1][0]!='-')) {
		int low,high;
		if (sscanf(argv[++i],"%i-%i",&low,&high)!=2) {
		    full&= ~XkbKeyBehaviorsMask;
		    changes.changed|= XkbKeyBehaviorsMask;
		    changes.first_key_behavior = low;
		    changes.num_key_behaviors = high-low+1;
		    continue;
		}
		fprintf(stderr,"must specify a range of key behaviors\n");
		return 0;
	    }
	    else full|= XkbKeyBehaviorsMask;
	}
	else if ((strcmp(argv[i],"-v")==0)||(strcmp(argv[i],"-vmods")==0)){
	    if ((i+1<argc)&&(argv[i+1][0]!='-')) {
		int mods;
		if (sscanf(argv[++i],"%i",&mods)!=2) {
		    full&= ~XkbVirtualModsMask;
		    changes.changed|= XkbVirtualModsMask;
		    changes.vmods = mods;
		    continue;
		}
		fprintf(stderr,"must specify virtual modifiers\n");
		return 0;
	    }
	    else full|= XkbKeyBehaviorsMask;
	}
	else if ((strcmp(argv[i],"-e")==0)||(strcmp(argv[i],"-explicit")==0)){
	    if ((i+1<argc)&&(argv[i+1][0]!='-')) {
		int low,high;
		if (sscanf(argv[++i],"%i-%i",&low,&high)!=2) {
		    full&= ~XkbExplicitComponentsMask;
		    changes.changed|= XkbExplicitComponentsMask;
		    changes.first_key_explicit = low;
		    changes.num_key_explicit = high-low+1;
		    continue;
		}
		fprintf(stderr,"must specify a range of keys for explicit components \n");
		return 0;
	    }
	    else full|= XkbKeyBehaviorsMask;
	}
	else if ((strcmp(argv[i],"-x")==0)||(strcmp(argv[i],"-exclude")==0)) {
	    if ((i+1<argc) && (argv[i+1][0]!='-')) {
		char *subsets = argv[++i];
		unsigned which = 0;
		while (*subsets) {
		    switch (*subsets) {
			case 't':	which|= XkbKeyTypesMask; break;
			case 'a':	which|= XkbKeyActionsMask; break;
			case 's':	which|= XkbKeySymsMask; break;
			case 'b':	which|= XkbKeyBehaviorsMask; break;
			case 'v':	which|= XkbVirtualModsMask; break;
			case 'e':	which|= XkbExplicitComponentsMask;break;
			default:
			    fprintf(stderr,"Unknown subset %c\n",*subsets);
			    return 0;
		    }
		    subsets++;
		}
		if ( !which ) {
		    fprintf(stderr,"no subsets specified\n");
		    return 0;
		}
		else {
		    changes.changed&= ~which;
		}
	    }
	    else {
		fprintf(stderr,"%s option requires an argument\n",argv[i]);
		return 0;
	    }
	}
	else if ((!strcmp(argv[i],"-nx"))||(!strcmp(argv[i],"-excludenames"))) {
	    if ((i+1<argc) && (argv[i+1][0]!='-')) {
		char *subsets = argv[++i];
		unsigned which = 0;
		while (*subsets) {
		    switch (*subsets) {
			case 's':	which|= XkbSymbolsNameMask; break;
			case 'c':	which|= XkbKeycodesNameMask; break;
			case 'g':	which|= XkbGeometryNameMask; break;
			case 'm':	which|= XkbModifierNamesMask; break;
			case 'l':	which|= XkbKTLevelNamesMask; break;
			case 'v':	which|= XkbVirtualModNamesMask; break;
			default:
			    fprintf(stderr,"Unknown subset %c\n",*subsets);
			    return 0;
		    }
		    subsets++;
		} 
		whichNames&= which;
	    }
	    else {
		fprintf(stderr,"%s option requires an argument\n",argv[i]);
		return 0;
	    }
	}
	else if ((!strcmp(argv[i],"-ktn")) || (!strcmp(argv[i],"-KTlnames"))){
	    if ((i+1<argc)&&(argv[i+1][0]!='-')) {
		int low,high;
		if (sscanf(argv[++i],"%i-%i",&low,&high)!=2) {
		    firstNamesKeyType = low;
		    nNamesKeyTypes = high-low+1;
		    if (nNamesKeyTypes>0)
			continue;
		}
		fprintf(stderr,"must specify a legal range of key types\n");
		return 0;
	    }
	    else full|= XkbKeyActionsMask;
	}
	else if ( strcmp(argv[i],"-synch")==0 ) {
	    synch= 1;
	}
	else if ( strcmp(argv[i],"-all")==0 ) {
	    changeAll= 1;
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
XkbDescRec	*map;
XkbStateRec	 state;
unsigned	 query;

  
    if (!parseArgs(argc,argv)) {
	fprintf(stderr,"Usage: %s <options>\n",argv[0]);
	fprintf(stderr,"Where legal options are:\n");
	fprintf(stderr,"-display <dpy>         specifies display to use\n");
	fprintf(stderr,"-t[ypes] [range]       specifies a range of key types to refresh\n");
	fprintf(stderr,"-s[yms] [range]        specifies a range of key syms to refresh\n");
	fprintf(stderr,"-a[ctions] [range]     specifies a range of acts to refresh\n");
	fprintf(stderr,"-b[ehaviors] [range]   specifies a range of key behaviors to refresh\n");
	fprintf(stderr,"-v[mods] [mask]        specifies a mask of virtual modifiers to refresh\n");
	fprintf(stderr,"-e[xplicit] [range]    specifies a mask of key explicit components to refresh\n");
	fprintf(stderr,"-x <subsets>           specifies unchanged subset of keyboard mapping\n");
	fprintf(stderr,"                       Legal subsets are:\n");
	fprintf(stderr,"		       t:  Key Types\n");
	fprintf(stderr,"                       a:  Key Actions\n");
	fprintf(stderr,"                       s:  Key Syms\n");
	fprintf(stderr,"                       b:  Key Behavior\n");
	fprintf(stderr,"                       v:  Virtual Modifiers\n");
	fprintf(stderr,"                       e:  Explicit Components\n");
	fprintf(stderr,"-xn <name subsets>     specifies unchanges subset of keyboard names\n");
	fprintf(stderr,"                       legal name subsets are:\n");
	fprintf(stderr,"                       c:  Keycode name\n");
	fprintf(stderr,"                       s:  Symbols name\n");
	fprintf(stderr,"                       g:  Geometry name\n");
	fprintf(stderr,"                       i:  Indicator names\n");
	fprintf(stderr,"                       m:  Modifier names\n");
	fprintf(stderr,"                       l:  Key type level names\n");
	fprintf(stderr,"                       v:  Virtual modifier names\n");
	fprintf(stderr,"-cmn [range]           specifies a range of column map names to refresh\n");
	return 1;
    }
    dpy = XOpenDisplay(dpyName);
    if ( !dpy )
	return 1;
    XSynchronize(dpy,1);
    if ( !XkbQueryExtension(dpy,&i1,&i2,&i3,&i4,&i5)>0 ) {
	fprintf(stderr,"query failed\n");
	goto BAIL;
    }
    if ( !XkbUseExtension(dpy) ) {
	fprintf(stderr,"use extension failed (%d,%d)\n",i4,i5);
	goto BAIL;
    }

    map = XkbGetMap(dpy,XkbAllMapComponentsMask,XkbUseCoreKbd);
    if (!map) {
	fprintf(stderr,"XkbGetMap failed\n");
	goto BAIL;
    }
    if (!XkbGetNames(dpy,XkbAllNamesMask,map)) {

	fprintf(stderr,"XkbGetNames failed\n");
	goto BAIL;
    }
    if ((!changeAll)&&(!changes.changed)) {
	changeAll = 1;
	full = XkbAllMapComponentsMask;
    }
    if (changeAll) {
	printf("Refreshing entire mapping...");
	if (XkbSetMap(dpy,full,map))
	     printf("done\n");
	else printf("XkbSetMap failed!\n");
    }
    else {
	printf("Refreshing:");
	if (changes.changed&XkbKeyTypesMask) {
	    if ((changes.first_type==0)&&(changes.num_types==0))
	    	changes.num_types = map->map->num_types;
	    printf("     key types:      %d-%d\n",changes.first_type,
	    						changes.num_types);
	}
	if (changes.changed&XkbKeySymsMask) {
	    if ((changes.first_key_sym==0)&&(changes.num_key_syms==0)) {
		changes.first_key_sym = map->min_key_code;
	    	changes.num_key_syms = map->max_key_code-map->min_key_code+1;
	    }
	    printf("   key syms:         %d-%d\n",changes.first_key_sym,
	    						changes.num_key_syms);
	}
	if (changes.changed&XkbKeyActionsMask) {
	    if ((changes.first_key_act==0)&&(changes.num_key_acts==0)) {
		changes.first_key_act = map->min_key_code;
	    	changes.num_key_acts = map->max_key_code-map->min_key_code+1;
	    }
	    printf("   key acts:      %d-%d\n",changes.first_key_act,
    						changes.num_key_acts);
	}
	if (changes.changed&XkbKeyBehaviorsMask) {
	    if ((!changes.first_key_behavior)&&(!changes.num_key_behaviors)) {
		changes.first_key_behavior = map->min_key_code;
	    	changes.num_key_behaviors = map->max_key_code-map->min_key_code+1;
	    }
	    printf("   key behaviors:    %d-%d\n",changes.first_key_behavior,
	    					changes.num_key_behaviors);
	}
	if (XkbChangeMap(dpy,map,&changes))
	     printf("Map refreshed\n");
	else printf("XkbChangeMap failed!\n");
    }
    if (whichNames) {
	printf("Refreshing names...");
	if (nNamesKeyTypes==0) {
	    firstNamesKeyType = 0;
	    nNamesKeyTypes= map->map->num_types;
	}
	XkbSetNames(dpy,whichNames,firstNamesKeyType,nNamesKeyTypes,map);
	printf("done\n");
    }
    return 0;
BAIL:
    XCloseDisplay(dpy);
    return 0;
}
