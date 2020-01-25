/* $XConsortium: xkbctrl.c,v 1.5 94/04/02 01:07:06 erik Exp $ */
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
#include <string.h>
#include <ctype.h>
#include <X11/Xproto.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/XKBlib.h>

#define	ON	 1
#define	OFF	 0
#define	IGNORE	-1

static	char		*dpyName = NULL;
static	unsigned	 which;
static	XkbControlsRec	 newControls;
static	unsigned	 changeInternal;
static	unsigned	 internal;
static	unsigned	 changeVInternal;
static	unsigned	 vInternal;
static	unsigned	 changeIgnoreLocks;
static	unsigned	 ignoreLocks;
static	unsigned	 changeVIgnoreLocks;
static	unsigned	 vIgnoreLocks;
static	unsigned	 changeEnabled;
static	unsigned	 enabled;
static	int		 synch = 0;
static	int		 device = XkbUseCoreKbd;

int
parseMods(modText,onOff,changes,mods)
    char *modText;
    int   onOff;
    unsigned *changes;
    unsigned *mods;
{
register char *tmp;

    for (tmp=modText;*tmp;tmp++) {
	if ((*tmp=='s')||(*tmp=='S')) {
	    *changes|= ShiftMask;
	    if (onOff)	*mods|= ShiftMask;
	    else	*mods&= ~ShiftMask;
	}
	if ((*tmp=='l')||(*tmp=='L')) {
	    *changes|= LockMask;
	    if (onOff)	*mods|= LockMask;
	    else	*mods&= ~LockMask;
	}
	if ((*tmp=='c')||(*tmp=='C')) {
	    *changes|= ControlMask;
	    if (onOff)	*mods|= ControlMask;
	    else	*mods&= ~ControlMask;
	}
	if ((*tmp=='a')||(*tmp=='A')||(*tmp=='m')||(*tmp=='M')||(*tmp=='1')) {
	    *changes|= Mod1Mask;
	    if (onOff)	*mods|= Mod1Mask;
	    else	*mods&= ~Mod1Mask;
	}
	if (*tmp=='2') {
	    *changes|= Mod2Mask;
	    if (onOff)	*mods|= Mod2Mask;
	    else	*mods&= ~Mod2Mask;
	}
	if (*tmp=='3') {
	    *changes|= Mod3Mask;
	    if (onOff)	*mods|= Mod3Mask;
	    else	*mods&= ~Mod3Mask;
	}
	if (*tmp=='4') {
	    *changes|= Mod4Mask;
	    if (onOff)	*mods|= Mod4Mask;
	    else	*mods&= ~Mod2Mask;
	}
	if (*tmp=='5') {
	    *changes|= Mod5Mask;
	    if (onOff)	*mods|= Mod5Mask;
	    else	*mods&= ~Mod5Mask;
	}
    }
    return 1;
}

int
parseVMods(modText,onOff,changes,mods)
    char *modText;
    int   onOff;
    unsigned *changes;
    unsigned *mods;
{
register char *tmp;

    for (tmp=modText;*tmp;tmp++) {
	if (isdigit(*tmp)) {
	    int ndx= *tmp-'0';
	    *changes|= (1<<ndx);
	    if (onOff)	*mods|= (1<<ndx);
	    else	*mods&= ~(1<<ndx);
	}
	else if ( (*tmp>='a') && (*tmp<='f') ) {
	    int ndx= 10+(*tmp-'a');
	    *changes|= (1<<ndx);
	    if (onOff)	*mods|= (1<<ndx);
	    else	*mods&= ~(1<<ndx);
	}
	else if ( (*tmp>='A') && (*tmp<='F') ) {
	    int ndx= 10+(*tmp-'F');
	    *changes|= (1<<ndx);
	    if (onOff)	*mods|= (1<<ndx);
	    else	*mods&= ~(1<<ndx);
	}
    }
    return 1;
}

#define	E(m)	fprintf(stderr,m)
void
usage(argc,argv)
    int argc;
    char *argv[];
{
    fprintf(stderr,"Usage: %s <options>\n",argv[0]);
    E("Where legal options are:\n");
    E("-display <dpy>                 specifies display to use\n");
    E("[+-]synch                      synchonize on/off\n");
    E("-device  <id>                  specifies device to use\n");
    E("[+-]wrap                       group wrap on/off\n");
    E("[+-]bell                       audible bell on/off\n");
    E("[+-]autoautorepeat             automatic determination of repeating keys on/off\n");
    E("[+-]internal <mods>            set/clear internal modifiers\n");
    E("[+-]ignorelock <mods>          set/clear ignore locks modifiers\n");
    E("                               <mods> can contain one or more of:\n");
    E("                               s:         Shift\n");
    E("                               l:         Lock\n");
    E("                               c:         Control\n");
    E("                               [am]:      Alt/Meta (mod1)\n");
    E("                               [1-5]:     Mod1-Mod5\n");
    E("[+-]vinternal <vmods>          set/clear internal virtual modifiers\n");
    E("[+-]vignorelock <vmods>        set/clear ignore locks virtual mods\n");
    E("                               virtual modifiers are specified by\n");
    E("                               using one hexadecimal digit per vmod\n");
    E("-repeat                        disable RepeatKeys\n");
    E("+repeat [ delay [ ival ] ]     enable RepeatKeys with specified delay\n");
    E("                               and interval\n");
    E("-slow                          disable SlowKeys\n");
    E("+slow [ delay ]                enable SlowKeys with the specified delay\n");
    E("-bounce                        disable BounceKeys\n");
    E("+bounce [timeout]              enable BounceKeys with the specified timeout\n");
    E("[+-]sticky                     enable/disable sticky keys\n");
    E("-mouse                         disable MouseKeys\n");
    E("+mouse [ delay [ interval [ time-to-max [ dflt-button ] ] ] ]\n");
    E("                               enable mouse keys with the specified behavior\n");
    E("-accessx                       disable AccessX hotkeys and timeout\n");
    E("+accessx [timeout]             enable AccessX hotkeys with the specified\n");
    E("                               timeout (0 indicates no timeout)\n");
}

int
parseArgs(argc,argv)
    int argc;
    char *argv[];
{
int i;
int onoff;

    for (i=1;i<argc;i++) {
	 if (argv[i][0]=='-')		onoff= 0;
	 else if (argv[i][0]=='+')	onoff= 1;
	 else {
	     fprintf(stderr,"Options must start with '+' or '-'\n");
	     return 0;
	 }
	 if ( strcmp(argv[i],"-display")==0 ) {
	    if ( ++i<argc )	dpyName= argv[i];
	    else {
		fprintf(stderr,"Must specify a display with -display option\n");
		return 0;
	    }
	}
	else if ( strcmp(argv[i],"-device")==0 ) {
	    if (( (i+1)<argc ) && isdigit(argv[i+1][0]))
		device= atoi(argv[++i]);
	    else {
		fprintf(stderr,"Must specify an id with -device option\n");
		return 0;
	    }
	}
	else if ( strcmp(&argv[i][1],"synch")==0 ) {
	    synch= onoff;
	}
	else if ( strcmp(&argv[i][1],"wrap")==0 ) {
	    which|= XkbControlsEnabledMask;
	    if (onoff)	enabled|= XkbGroupsWrapMask;
	    else	enabled&= ~XkbGroupsWrapMask;
	}
	else if ( strcmp(&argv[i][1],"bell")==0 ) {
	    which|= XkbControlsEnabledMask;
	    if (onoff)	enabled|= XkbAudibleBellMask;
	    else	enabled&= ~XkbAudibleBellMask;
	}
	else if ( strcmp(&argv[i][1],"autoautorepeat")==0 ) {
	    which|= XkbAutoAutorepeatMask;
	    if (onoff)	enabled|= XkbAutoAutorepeatMask;
	    else	enabled&= ~XkbAutoAutorepeatMask;
	}
	else if ( strcmp(&argv[i][1],"internal")==0 ) {
	    which|= XkbInternalModsMask;
	    if (((i+1)>=argc) ||
		(!parseMods(argv[i+1],onoff,&changeInternal,&internal))) {
		fprintf(stderr,"Must specify the internal modifiers\n");
		return 0;
	    }
	    i++;
	}
	else if ( strcmp(&argv[i][1],"ignorelock")==0 ) {
	    which|= XkbIgnoreLockModsMask;
	    if (((i+1)>=argc) || (!parseMods(argv[i+1],onoff,&changeIgnoreLocks,
							&ignoreLocks))) {
		fprintf(stderr,"Must specify the ignore lock modifiers\n");
		return 0;
	    }
	    i++;
	}
	else if ( strcmp(&argv[i][1],"vinternal")==0 ) {
	    which|= XkbInternalModsMask;
	    if (((i+1)>=argc) ||
		(!parseVMods(argv[i+1],onoff,&changeVInternal,&vInternal))) {
		fprintf(stderr,"Must specify the internal virtual modifiers\n");
		return 0;
	    }
	    i++;
	}
	else if ( strcmp(&argv[i][1],"vignorelock")==0 ) {
	    which|= XkbIgnoreLockModsMask;
	    if (((i+1)>=argc) || (!parseVMods(argv[i+1],onoff,
					&changeVIgnoreLocks,&vIgnoreLocks))) {
		fprintf(stderr,"Must specify the ignore lock virtual mods\n");
		return 0;
	    }
	    i++;
	}
	else if ( strcmp(&argv[i][1],"repeat")==0 ) {
	    which|= XkbControlsEnabledMask;
	    changeEnabled|= XkbRepeatKeysMask;
	    enabled|= (onoff?XkbRepeatKeysMask:0);
	    if (onoff) {
		int ok= 0;
		if ( ((i+1)<argc) && (isdigit(argv[i+1][0])) ) {
		    which|= XkbRepeatKeysMask;
		    newControls.repeat_delay= atoi(argv[++i]);
		    ok= 1;
		}
		if (ok && ((i+1)<argc) && (isdigit(argv[i+1][0]))) {
		    newControls.repeat_interval= atoi(argv[++i]);
		}
	    }
	}
	else if ( strcmp(&argv[i][1],"slow")==0 ) {
	    which|= XkbControlsEnabledMask;
	    changeEnabled|= XkbSlowKeysMask;
	    enabled|= (onoff?XkbSlowKeysMask:0);
	    if (onoff) {
		if ( ((i+1)<argc) && (isdigit(argv[i+1][0])) ) {
		    which|= XkbSlowKeysMask;
		    newControls.slow_keys_delay= atoi(argv[++i]);
		}
	    }
	}
	else if ( strcmp(&argv[i][1],"bounce")==0 ) {
	    which|= XkbControlsEnabledMask;
	    changeEnabled|= XkbBounceKeysMask;
	    enabled|= (onoff?XkbBounceKeysMask:0);
	    if (onoff) {
		if ( ((i+1)<argc) && (isdigit(argv[i+1][0])) ) {
		    which|= XkbBounceKeysMask;
		    newControls.debounce_delay= atoi(argv[++i]);
		}
	    }
	}
	else if ( strcmp(&argv[i][1],"sticky")==0 ) {
	    which|= XkbControlsEnabledMask;
	    changeEnabled|= XkbStickyKeysMask;
	    enabled|= (onoff?XkbStickyKeysMask:0);
	}
	else if ( strcmp(&argv[i][1],"mouse")==0 ) {
	    which|= XkbControlsEnabledMask;
	    changeEnabled|= XkbMouseKeysMask;
	    enabled|= (onoff?XkbMouseKeysMask:0);
	    if (onoff) {
		int ok= 0;
		if ( ((i+1)<argc) && (isdigit(argv[i+1][0])) ) {
		    which|= XkbMouseKeysMask;
		    newControls.mouse_keys_delay= atoi(argv[++i]);
		    ok= 1;
		}
		if (ok && ((i+1)<argc) && (isdigit(argv[i+1][0]))) {
		    newControls.mouse_keys_interval= atoi(argv[++i]);
		}
		if (ok && ((i+1)<argc) && (isdigit(argv[i+1][0]))) {
		    newControls.mouse_keys_time_to_max= atoi(argv[++i]);
		}
		if (ok && ((i+1)<argc) && (isdigit(argv[i+1][0]))) {
		    newControls.mouse_keys_curve= atoi(argv[++i]);
		}
		if (ok && ((i+1)<argc) && (isdigit(argv[i+1][0]))) {
		    newControls.mouse_keys_dflt_btn= atoi(argv[++i]);
		}
	    }
	}
	else if ( strcmp(&argv[i][1],"accessx")==0 ) {
	    which|= XkbControlsEnabledMask;
	    changeEnabled|= XkbAccessXKeysMask;
	    enabled|= (onoff?XkbAccessXKeysMask:0);
	    if (onoff) {
		if ( ((i+1)<argc) && (isdigit(argv[i+1][0])) ) {
		    which|= XkbAccessXKeysMask;
		    newControls.accessx_timeout= atoi(argv[++i]);
		}
	    }
	}
	else if ( strcmp(argv[i],"-help") ) {
	    return 0;
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
XkbDescRec	*desc;
XkbControlsRec	*ctrls;
unsigned	 query;

  
    if (!parseArgs(argc,argv)) {
	usage(argc,argv);
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
    desc = XkbGetMap(dpy,0,device);
    if (desc) {
	if (!XkbGetControls(dpy,XkbAllControlsMask,desc)) {
	    fprintf(stderr,"XkbGetControls failed\n");
	    goto BAIL;
	}
	if (which) {
	    if (which&XkbRepeatKeysMask) {
		fprintf(stderr,"Changing RepeatKeys delay%s",
			newControls.repeat_interval>0?" and interval\n":"\n");
		desc->ctrls->repeat_delay= newControls.repeat_delay;
		if (newControls.repeat_interval>0)
		    desc->ctrls->repeat_interval= newControls.repeat_interval;
	    }
	    if (which&XkbSlowKeysMask) {
		fprintf(stderr,"Changing SlowKeys delay\n");
		desc->ctrls->slow_keys_delay= newControls.slow_keys_delay;
	    }
	    if (which&XkbBounceKeysMask) {
		fprintf(stderr,"Changing BounceKeys delay\n");
		desc->ctrls->debounce_delay= newControls.debounce_delay;
	    }
	    if (which&XkbMouseKeysMask) {
		fprintf(stderr,"Changing MouseKeys delay");
		desc->ctrls->mouse_keys_delay= newControls.mouse_keys_delay;
		if (newControls.mouse_keys_interval>0) {
		    fprintf(stderr,", interval");
		    desc->ctrls->mouse_keys_interval=
					newControls.mouse_keys_interval;
		}
		if (newControls.mouse_keys_time_to_max) {
		    fprintf(stderr,", time-to-max");
		    desc->ctrls->mouse_keys_time_to_max=
					newControls.mouse_keys_time_to_max;
		}
		if (newControls.mouse_keys_time_to_max) {
		    fprintf(stderr,", curve");
		    desc->ctrls->mouse_keys_curve= newControls.mouse_keys_curve;
		}
		if (newControls.mouse_keys_dflt_btn) {
		    fprintf(stderr,", default button");
		    desc->ctrls->mouse_keys_dflt_btn=
					newControls.mouse_keys_dflt_btn;
		}
		fprintf(stderr,"\n");
	    }
	    if (which&XkbAccessXKeysMask) {
		fprintf(stderr,"Changing AccessX timeout\n");
		desc->ctrls->accessx_timeout= 
					newControls.accessx_timeout;
	    }
	    if (which&XkbControlsEnabledMask) {
		fprintf(stderr,"Changing enabled ctrls\n");
		desc->ctrls->enabled_ctrls&= ~changeEnabled;
		desc->ctrls->enabled_ctrls|= (changeEnabled&enabled);
	    }
	    if (which&XkbInternalModsMask) {
		fprintf(stderr,"Changing internal modifiers\n");
		desc->ctrls->internal_real_mods&= ~changeInternal;
		desc->ctrls->internal_real_mods|= (changeInternal|internal);
		desc->ctrls->internal_vmods&= ~changeVInternal;
		desc->ctrls->internal_vmods|= 
					(changeVInternal&vInternal);
	    }
	    if (which&XkbIgnoreLockModsMask) {
		fprintf(stderr,"Changing ignore locks modifiers\n");
		desc->ctrls->ignore_lock_real_mods&= ~changeIgnoreLocks;
		desc->ctrls->ignore_lock_real_mods|=
					(changeIgnoreLocks|ignoreLocks);
		desc->ctrls->ignore_lock_vmods&= ~changeVIgnoreLocks;
		desc->ctrls->ignore_lock_vmods|=
					(changeVIgnoreLocks&vIgnoreLocks);
	    }
	    XkbSetControls(dpy,which,desc);
	}
    }
    else {
	fprintf(stderr,"Get keyboard description request failed\n");
	return 1;
    }
    ctrls= desc->ctrls;
    printf("Device ID:         %d\n",desc->device_spec);
    printf("Groups:            %swrap\n",
		(ctrls->enabled_ctrls&XkbGroupsWrapMask)?"":"don't ");
    printf("audible bell:      %s\n",
		(ctrls->enabled_ctrls&XkbAudibleBellMask)?"on ":"off");
    printf("auto autorepeat:   %s\n",
		(ctrls->enabled_ctrls&XkbAutoAutorepeatMask)?"on ":"off");
    printf("internal mask:     0x%x\n",ctrls->internal_mask);
    printf("ignore lock mask:  0x%x\n",ctrls->ignore_lock_mask);
    printf("internal mods:     0x%x\n",ctrls->internal_real_mods);
    printf("ignore lock mods:  0x%x\n",ctrls->ignore_lock_real_mods);
    printf("internal vmods:    0x%x\n",ctrls->internal_vmods);
    printf("ignore lock vmods: 0x%x\n",ctrls->ignore_lock_vmods);
    printf("repeat keys:       %s (%d/%d)\n",
		(ctrls->enabled_ctrls&XkbRepeatKeysMask?"on ":"off"),
		ctrls->repeat_delay,ctrls->repeat_interval);
    printf("slow keys:         %s (%d)\n",
		(ctrls->enabled_ctrls&XkbSlowKeysMask?"on ":"off"),
		ctrls->slow_keys_delay);
    printf("bounce keys:       %s (%d)\n",
		(ctrls->enabled_ctrls&XkbBounceKeysMask?"on ":"off"),
		ctrls->debounce_delay);
    printf("sticky keys:       %s\n",
		(ctrls->enabled_ctrls&XkbStickyKeysMask?"on ":"off"));
    printf("mouse keys:        %s (btn=%d,accel=%d/%d/%d/%d)\n",
		(ctrls->enabled_ctrls&XkbMouseKeysMask?"on ":"off"),
		ctrls->mouse_keys_dflt_btn,
		ctrls->mouse_keys_delay,
		ctrls->mouse_keys_interval,
		ctrls->mouse_keys_time_to_max,
		ctrls->mouse_keys_curve);
    printf("access X keys:     %s (timeout=%d, mask= 0x%x)\n",
		(ctrls->enabled_ctrls&XkbAccessXKeysMask?"on ":"off"),
		ctrls->accessx_timeout,ctrls->accessx_timeout_mask);
    XCloseDisplay(dpy);
    return 0;
BAIL:
    XCloseDisplay(dpy);
    return 0;
}
