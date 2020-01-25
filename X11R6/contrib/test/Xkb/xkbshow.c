/* $XConsortium: xkbshow.c,v 1.7 94/04/08 17:04:51 erik Exp $ */
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

#ifdef sgi
#include <malloc.h>
#endif

void
printSyms(map,sym,num)
    XkbKeyTypeRec *map;
    KeySym *sym;
    int num;
{
int	i,nsPending;
char	*name;

    nsPending= 0;
    for (i=0;i<num;i++) {
	if ( sym[i]==NoSymbol )
	    nsPending++;
	else {
	    while (nsPending>0) {
		printf("NoSymbol ");
		nsPending--;
	    }
	    name = XKeysymToString(sym[i]);
	    if ( name )	printf("%s ",name);
	    else	printf("unknown (0x%x)",sym[i]);
	}
    }
}

char *
atomText(dpy,atom)
    Display *dpy;
    Atom atom;
{
static char buf[256];
char	*name;

    if (atom==None) {
	sprintf(buf,"(None)");
	return buf;
    }
    name= XGetAtomName(dpy,atom);
    if ( name ) {
	strncpy(buf,name,255);
	buf[255]= '\0';
	XFree(name);
    }
    else {
	sprintf(buf,"(unknown)");
    }
    return buf;
}

char *
behaviorText(behavior)
    XkbBehavior behavior;
{
static char buf[30],*str;

    str= buf;
    if (behavior.type&XkbKB_Permanent) {
	strcpy(buf,"permanent ");
	str= &buf[strlen(buf)];
    }
    switch (behavior.type&XkbKB_OpMask) {
	case XkbKB_RadioGroup:
		sprintf(str,"radio group %d",behavior.data);
		break;
	case XkbKB_Default:	  
		strcpy(str,"default");
		break;
	case XkbKB_Lock:	 
		strcpy(str,"lock");
		break;
	default:		 
		sprintf(str,"(unknown 0x%x)",behavior.type);
		break;
    }
    return buf;
}

char *
modIndexText(ndx)
    CARD8 ndx;
{
static char buf[12];

    switch (ndx) {
	case ShiftMapIndex:
	    strcpy(buf,"Shift");
	    break;
	case LockMapIndex:
	    strcpy(buf,"Lock");
	    break;
	case ControlMapIndex:
	    strcpy(buf,"Control");
	    break;
	case Mod1MapIndex:
	    strcpy(buf,"Mod1");
	    break;
	case Mod2MapIndex:
	    strcpy(buf,"Mod2");
	    break;
	case Mod3MapIndex:
	    strcpy(buf,"Mod3");
	    break;
	case Mod4MapIndex:
	    strcpy(buf,"Mod4");
	    break;
	case Mod5MapIndex:
	    strcpy(buf,"Mod5");
	    break;
	case XkbNoModifier:
	    strcpy(buf,"None");
	    break;
	default:
	    strcpy(buf,"ERROR");
	    break;
    }
    return buf;
}

char *
stateText(state)
    CARD8 state;
{
static char buf[12];
char	*str;

    if ( state == 0 ) {
	strcpy(buf,"none");
	return buf;
    }

    str= buf;
    if ( state&ShiftMask )	*str++ = 'S';
    if ( state&ControlMask )	*str++ = 'C';
    if ( state&LockMask )	*str++ = 'L';
    if ( state&Mod1Mask )	*str++ = '1';
    if ( state&Mod2Mask )	*str++ = '2';
    if ( state&Mod3Mask )	*str++ = '3';
    if ( state&Mod4Mask )	*str++ = '4';
    if ( state&Mod5Mask )	*str++ = '5';
    *str++ = '\0';
    return buf;
}

char *
modsFlagsText(flags)
    CARD8 flags;
{
static char buf[48];
char *str= buf;

    if (flags&XkbSA_ClearLocks) {
	if (str!=buf)	*str++= '+';
	strcpy(str,"clear");
	str+= strlen("clear");
    }
    if (flags&XkbSA_LatchToLock) {
	if (str!=buf)	*str++= '+';
	strcpy(str,"latch->lock");
	str+= strlen("latch->lock");
    }
    *str++= '\0';
    return buf;
}
char *

isoAffectText(flags)
    CARD8 flags;
{
static char buf[48];
char *str= buf;

    if (flags&XkbSA_ISONoAffectMods) {
	if (str==buf)	*str++= '!';
	*str++= 'M';
    }
    if (flags&XkbSA_ISONoAffectGroup) {
	if (str==buf)	*str++= '!';
	*str++= 'G';
    }
    if (flags&XkbSA_ISONoAffectPtr) {
	if (str==buf)	*str++= '!';
	*str++= 'P';
    }
    if (flags&XkbSA_ISONoAffectCtrls) {
	if (str==buf)	*str++= '!';
	*str++= 'C';
    }
    if (str==buf)
	 strcpy(str,"all");
    else *str++= '\0';
    return buf;
}

char *
actText(sa)
    XkbAction sa;
{
static char buf[100];
char	*str1,*str2;

    switch (sa.type) {
	case XkbSA_NoAction:
	    strcpy(buf,"NoAction");
	    break;
	case XkbSA_SetMods:
	    str1= (sa.mods.flags?modsFlagsText(sa.mods.flags):NULL);
	    sprintf(buf,"SetMods(%s%s%s)",stateText(sa.mods.mask),
					(str1?",":""),(str1?str1:""));
	    break;
	case XkbSA_ISOLock:
	    str1= isoAffectText(sa.iso.affect);
	    if (sa.iso.flags&XkbSA_ISODfltIsGroup)
		 sprintf(buf,"ISOLock(group=%d,affect=%s)",
				XkbSAGroup(&sa.iso),str1);
	    else sprintf(buf,"ISOLock(mods=%s,affect=%s)",
				stateText(sa.iso.mask),str1);
	    break;
	case XkbSA_LockMods:
	    sprintf(buf,"LockMods(%s)",stateText(sa.mods.mask));
	    break;
	case XkbSA_LatchMods:
	    str1= (sa.mods.flags?modsFlagsText(sa.mods.flags):NULL);
	    sprintf(buf,"LatchMods(%s%s%s)",stateText(sa.mods.mask),
					(str1?",":""),(str1?str1:""));
	    break;
	case XkbSA_SetGroup:
	    str1= (sa.group.flags?modsFlagsText(sa.group.flags):NULL);
	    sprintf(buf,"SetGroup(%d(%s)%s)",XkbSAGroup(&sa.group),
		(sa.group.flags&XkbSA_GroupAbsolute?"absolute":"relative"),
		(str1?",":""),(str1?str1:""));
	    break;
	case XkbSA_LatchGroup:
	    str1= (sa.group.flags?modsFlagsText(sa.group.flags):NULL);
	    sprintf(buf,"LatchGroup(%d(%s)%s)",XkbSAGroup(&sa.group),
		(sa.group.flags&XkbSA_GroupAbsolute?"absolute":"relative"),
		(str1?",":""),(str1?str1:""));
	    break;
	case XkbSA_LockGroup:
	    sprintf(buf,"LockGroup(%d(%s))",XkbSAGroup(&sa.group),
		(sa.group.flags&XkbSA_GroupAbsolute?"absolute":"relative"));
	    break;
	case XkbSA_MovePtr:
	    sprintf(buf,"MovePtr=(%d,%d)",XkbPtrActionX(&sa.ptr),
						XkbPtrActionY(&sa.ptr));
	    break;
	case XkbSA_AccelPtr:
	    sprintf(buf,"AccelPtr=(%d,%d)",XkbPtrActionX(&sa.ptr),
						XkbPtrActionY(&sa.ptr));
	    break;
	case XkbSA_PtrBtn:
	    if (sa.btn.button==XkbSA_UseDfltButton)
		 sprintf(buf,"PtrBtn(dflt)");
	    else sprintf(buf,"PtrBtn(%d)",sa.btn.button);
	    break;
	case XkbSA_ClickPtrBtn:
	    if (sa.btn.button==XkbSA_UseDfltButton)
		 sprintf(buf,"ClickPtrBtn(%d,default)",sa.btn.count);
	    else sprintf(buf,"ClickPtrBtn(%d,%d)",sa.btn.count,sa.btn.button);
	    break;
	case XkbSA_LockPtrBtn:
	    if (sa.btn.button==XkbSA_UseDfltButton)
		 sprintf(buf,"LockPtrBtn(default)");
	    else sprintf(buf,"LockPtrBtn(%d)",sa.btn.button);
	    break;
	case XkbSA_SetPtrDflt:
	    if (sa.dflt.affect==XkbSA_AffectDfltBtn) {
		int value;
		value= XkbSAPtrDfltValue(&sa.dflt);
		if ((sa.dflt.flags&XkbSA_DfltBtnAbsolute)||(value<0))
		     sprintf(buf,"SetPtrDflt(SetDfltBtn,%d)",value);
		else sprintf(buf,"SetPtrDflt(SetDfltBtn,+%d)",value);
	    }
	    else sprintf(buf,"SetPtrDflt(Unknown(%d),%d)",sa.dflt.affect,
						 XkbSAPtrDfltValue(&sa.dflt));
	    break; 
	case XkbSA_Terminate:
	    sprintf(buf,"Terminate");
	    break;
	case XkbSA_SwitchScreen:
	    sprintf(buf,"SwitchScreen(0x%x,%d)",sa.screen.flags,
						XkbSAScreen(&sa.screen));
	    break;
	case XkbSA_SetControls:
	    sprintf(buf,"SetControls(0x%x)",XkbActionCtrls(&sa.ctrls));
	    break;
	case XkbSA_LockControls:
	    sprintf(buf,"LockControls(0x%x)",XkbActionCtrls(&sa.ctrls));
	    break;
	case XkbSA_ActionMessage:
	    sprintf(buf,"ActionMessage(0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)\n",
			sa.msg.flags,
			sa.msg.message[0],sa.msg.message[1],sa.msg.message[2],
			sa.msg.message[3],sa.msg.message[4],sa.msg.message[5]);
	    break;
	default:
	    sprintf(buf,"Unknown(0x%x)",sa);
	    break;
    }
    return buf;
}

char *
vmodsText(dpy,xkb,vmods)
    Display *	dpy;
    XkbDescPtr	xkb;
    unsigned	vmods;
{
register int i,bit,nVMods;
static char buf[256],*tmp;

    buf[0]= '\0';
    if (vmods==0)
	strcpy(buf,"none");
    else {
	Atom atm;
	for (i=nVMods=0,bit=1;i<XkbNumVirtualMods;i++,bit<<=1) {
	    if ((vmods&bit)==0)
		continue;
	    if (xkb->names&&(xkb->names->vmods[i]!=None)) {
		tmp= atomText(dpy,xkb->names->vmods[i]);
		if (nVMods>0)
		    strcat(buf,"+");
		strcat(buf,tmp);
	    }
	    else {
		tmp= &buf[strlen(buf)];
		if (nVMods>0)
		    *tmp++= '+';
		sprintf(tmp,"VMod%d",i);
	    }
	}
    }
    return buf;
}

/***====================================================================***/

void
showKeys(dpy,xkb,which)
    Display *dpy;
    XkbDescRec *xkb;
    unsigned which;
{
int	i,key,nKeys;

    if ( !(which & (XkbKeySymsMask|XkbKeyBehaviorsMask|XkbKeyActionsMask)) )
	return;
    nKeys = xkb->max_key_code-xkb->min_key_code+1;
    for (i=0,key=xkb->min_key_code;i<nKeys;i++,key++) {
	int n = XkbKeyNumSyms(xkb,key);
	if (n==0)
	    continue;
	printf("Key %d",key);
	if (xkb->map && xkb->map->types) {
	    Atom name;
	    name= xkb->map->types[xkb->map->key_sym_map[key].kt_index].name;
	    if (name==None)
		 printf(" (%d",xkb->map->key_sym_map[key].kt_index);
	    else printf(" (%s",atomText(dpy,name));
	    if (xkb->names && xkb->names->keys) {
		char buf[5];
		memcpy(buf,xkb->names->keys[key].name,4);
		buf[4]= '\0';
		printf("/<%s>",buf);
	    }
	    printf(")");
	}
	printf(":\n");
	if ( which & XkbKeySymsMask ) {
	    int n = XkbKeyNumSyms(xkb,key);
	    int ng= XkbKeyNumGroups(xkb,key);
	    int tmp;
	    if (XkbKeyGroupsWrap(xkb,key))
		printf("    Groups wrap\n");
	    for (tmp=0;tmp<ng;tmp++) {
		if (tmp==0) {
		     printf("    Symbols: [ ");
		}
		else printf("             [ ");
		printSyms(XkbKeyKeyType(xkb,key),
				XkbKeySymsPtr(xkb,key)+(tmp*n/ng),n/ng);
		printf("]");
	    }
	    printf("\n");
	}
	if ( which & XkbKeyActionsMask ) {
	    int nActs = XkbKeyNumActions(xkb,key);
	    XkbAction *acts=XkbKeyActionsPtr(xkb,key);
	    if ((nActs>1)||(acts[0].type!=XkbSA_NoAction)) {
		int nGroups= XkbKeyNumGroups(xkb,key);
		int nLevels= nActs/nGroups;
		int g,l;
		for (g=0;g<nGroups;g++) {
		    if (g==0) {
			 printf("    Actions: [ ");
		    }
		    else printf("             [ ");
		    for (l=0;l<nLevels;l++) {
			printf("%s ",actText(acts[(g*nGroups)+l]));
		    }
		    printf("]\n");
		}
	    }
	}
	if ( which & XkbKeyBehaviorsMask ) {
	    if (xkb->server->behaviors[key].type!=XkbKB_Default) {
		printf("    Behavior: %s\n",
				behaviorText(xkb->server->behaviors[key]));
	    }
	}
    }
}

void
showKeyTypes(dpy,desc)
    Display *dpy;
    XkbDescRec *desc;
{
XkbClientMapRec *map = desc->map;
int	i,m;

    for (i=0;i<map->num_types;i++) {
	XkbKeyTypeRec *type= &map->types[i];
	printf("Key Type %d (%s):\n",i,atomText(dpy,type->name));
	printf("    mask:         %s\n",stateText(type->mask));
	printf("    real mods:    %s\n",stateText(type->real_mods));
	printf("    virtual mods: %s\n",
				vmodsText(dpy,desc,type->vmods));
	printf("    group width:  %d\n",type->group_width);
	if (type->map_count>0) {
	    printf("    map:          ");
	    for (m=0;m<type->map_count;m++) {
		if (m!=0)
		    printf("                  ");
		if (type->map[m].real_mods==0) {
		    printf("%s= %d",
			vmodsText(dpy,desc,type->map[m].vmods),
			type->map[m].level);
		}
		else if (type->map[m].vmods==0) {
		    printf("%s= %d",
			stateText(type->map[m].real_mods),type->map[m].level);
		}
		else {
		    printf("%s+%s= %d",
			vmodsText(dpy,desc,type->map[m].vmods),
			stateText(type->map[m].real_mods),type->map[m].level);
		}
		if (type->preserve) {
		    unsigned rmods= type->preserve[m].real_mods;
		    unsigned vmods= type->preserve[m].vmods;
		    unsigned mask=  type->preserve[m].mask;
		    if (rmods!=0) {
			printf(" [preserve %s (%s",stateText(mask),
						   stateText(rmods));
			if (type->preserve[m].vmods!=0) {
			     printf("+%s)]",vmodsText(dpy,desc,vmods));
			}
			else printf(")]");
		    }
		    else if (vmods!=0) {
			printf(" [preserve %s (%s)]",stateText(mask),
						vmodsText(dpy,desc,vmods));
		    }
		}
		printf("\n");
	    }
	}
	if (type->lvl_names!=NULL) {
	    for (m=0;m<type->group_width;m++) {
		printf("        level %d:  %s\n",m,
					atomText(dpy,type->lvl_names[m]));
	    }
	}
	else printf("     level names: none\n");
    }
    return;
}

/***====================================================================***/

static	char		*dpyName = NULL;
static	int		 getState = -1;
static	int		 getMap = -1;
static	unsigned	 which = XkbAllMapComponentsMask;
static	int		 usePartialQueries = 0;
static	int		 synch = 0;
static	int		 debug = 0;

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
	else if ( strcmp(argv[i],"-m")==0 ) {
	    if ((i+1<argc) && (argv[i+1][0]!='-')) {
		char *subsets = argv[++i];
		which = 0;
		while (*subsets) {
		    switch (*subsets) {
			case 't':	which|= XkbKeyTypesMask; break;
			case 's':	which|= XkbKeySymsMask; break;
			case 'a':	which|= XkbKeyActionsMask; break;
			case 'b':	which|= XkbKeyBehaviorsMask; break;
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
	    }
	    else getMap = 1;
	}
	else if ( strcmp(argv[i],"-s")==0 ) {
	    getState = 1;
	}
	else if ( strcmp(argv[i],"-1")==0 ) {
	    usePartialQueries = 1;
	}
	else if ( strcmp(argv[i],"-synch")==0 ) {
	    synch= 1;
	}
	else if ( strcmp(argv[i],"-debug")==0 ) {
	    debug= 1;
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
XkbClientMapRec	*map;
XkbStateRec	 state;
XkbControlsRec	*ctrls;
XKeyboardState	 coreKbdState;
unsigned	 query;

  
    if (!parseArgs(argc,argv)) {
	fprintf(stderr,"Usage: %s <options>\n",argv[0]);
	fprintf(stderr,"Where legal options are:\n");
	fprintf(stderr,"-display <dpy>     specifies display to use\n");
	fprintf(stderr,"-debug             turn on extra debugging\n");
	fprintf(stderr,"-s                 report keyboard state\n");
	fprintf(stderr,"-m <subsets>       specifies subset of keyboard mapping to display\n");
	fprintf(stderr,"                   Legal subsets are:\n");
	fprintf(stderr,"		   t:  Key Types\n");
	fprintf(stderr,"                   s:  Key Syms\n");
	fprintf(stderr,"                   a:  Key Actions\n");
	fprintf(stderr,"                   b:  Key Behaviors\n");
	fprintf(stderr,"                   r:  Radio Groups\n");
	fprintf(stderr,"-1                 specifies use of partial queries\n");
	return 1;
    }
#ifdef sgi
    if (debug)
	mallopt(M_DEBUG,~0);
#endif
    dpy = XOpenDisplay(dpyName);
    if ( !dpy )
	return 1;
    if (synch)
	XSynchronize(dpy,1);
    if ( !XkbQueryExtension(dpy,&i1,&i2,&i3,&i4,&i5)>0 ) {
	fprintf(stderr,"query failed\n");
	goto BAIL;
    }
    if ((getState>0) && (getMap<0))		getMap = 0;
    else if ((getMap>0) && (getState<0))	getState = 0;
    else if ((getMap<0) && (getState<0))	getMap = getState = 1;

    if ( getState ) {
	XGetKeyboardControl(dpy,&coreKbdState);
	if (!XkbGetState(dpy,XkbUseCoreKbd,&state)) {
	    fprintf(stderr,"get keyboard state request failed\n");
	    goto BAIL;
	}
        printf("group:         %d\n",state.group);
        printf("latched group: %d\n",state.latched_group);
        printf("locked group:  %d\n",state.locked_group);
	printf("mods:          %s\n",stateText(state.mods));
	printf("latched:       %s\n",stateText(state.latched_mods));
	printf("locked:        %s\n",stateText(state.locked_mods));
	printf("compatibility state: %s\n",stateText(state.compat_state));
    }
    if ( getMap ) {
	query = which;
	if (query&(XkbKeySymsMask|XkbKeyActionsMask))
	    query|= XkbKeyTypesMask;
	desc = XkbGetMap(dpy,0,XkbUseCoreKbd);
	map= desc->map;
	if ( !usePartialQueries ) {
	    if ( !XkbGetUpdatedMap(dpy,query,desc) ) {
		fprintf(stderr,"get keyboard desc request failed\n");
		goto BAIL;
	    }
	}
	else {
	    if ( query & XkbKeyTypesMask )  {
		if ( !XkbGetKeyTypes(dpy,0,3,map) ) {
		    fprintf(stderr,"XkbGetKeyTypes failed\n");
		    goto BAIL;
		}
	    }
	}

 	if (!XkbGetControls(dpy,XkbAllControlsMask,desc)) {
	    fprintf(stderr,"XkbGetControls failed\n");
	    goto BAIL;
	}
	if (!XkbGetNames(dpy,XkbAllNamesMask,desc)) {
	    fprintf(stderr,"XkbGetNames failed\n");
	    goto BAIL;
	}
	ctrls= desc->ctrls;
	printf("Device ID:     %d\n",desc->device_spec);
	printf("keycodes type: %s\n",atomText(dpy,desc->names->keycodes));
	printf("geometry type: %s ",atomText(dpy,desc->names->geometry));
	printf("(%s)\n",atomText(dpy,desc->names->phys_geometry));
	printf("symbols type:  %s ",atomText(dpy,desc->names->symbols));
	printf("(%s)\n",atomText(dpy,desc->names->phys_symbols));
	printf("semantics:     %s\n",atomText(dpy,desc->names->semantics));
	printf("keycode range: %d-%d\n",desc->min_key_code,desc->max_key_code);
	printf("audible bell:      %s\n",
		((ctrls->enabled_ctrls&XkbAudibleBellMask)?"on":"off"));
	printf("auto autorepeat:   %s\n",
		((ctrls->enabled_ctrls&XkbAutoAutorepeatMask)?"on":"off"));
	printf("internal mods:     %s ",stateText(ctrls->internal_mask));
	printf("(%s) ",stateText(ctrls->internal_real_mods));
	printf("(%s)\n",vmodsText(dpy,desc,ctrls->internal_vmods));
	printf("ignore lock mods:  %s ",stateText(ctrls->ignore_lock_mask));
	printf("(%s) ",stateText(ctrls->ignore_lock_real_mods));
	printf("(%s)\n",vmodsText(dpy,desc,ctrls->ignore_lock_vmods));
	printf("repeat keys:       %s (%d/%d -- %s)\n",
		(coreKbdState.global_auto_repeat?"on":"off"),
		ctrls->repeat_delay,ctrls->repeat_interval,
		((ctrls->enabled_ctrls&XkbRepeatKeysMask)?"sw":"hw"));
	printf("slow keys:         %s (%d)\n",
		(ctrls->enabled_ctrls&XkbSlowKeysMask?"on":"off"),
		ctrls->slow_keys_delay);
	printf("bounce keys:       %s (%d)\n",
		(ctrls->enabled_ctrls&XkbBounceKeysMask?"on":"off"),
		ctrls->debounce_delay);
	printf("sticky keys:       %s\n",
		(ctrls->enabled_ctrls&XkbStickyKeysMask?"on":"off"));
	printf("mouse keys:        %s (btn=%d,accel=%d/%d/%d/%d)\n",
		(ctrls->enabled_ctrls&XkbMouseKeysMask?"on":"off"),
		ctrls->mouse_keys_dflt_btn,
		ctrls->mouse_keys_delay, ctrls->mouse_keys_interval,
		ctrls->mouse_keys_time_to_max, ctrls->mouse_keys_curve);
	printf("access X keys:     %s (timeout=%d,mask=0x%x)\n",
		(ctrls->enabled_ctrls&XkbAccessXKeysMask?"on":"off"),
		ctrls->accessx_timeout,ctrls->accessx_timeout_mask);
	printf("modifier names:\n");
	for (i1=0;i1<XkbNumModifiers;i1++) {
	    if (desc->names->mods[i1]!=None)
		printf("%2d: %s\n",i1,atomText(dpy,desc->names->mods[i1]));
	}
	printf("virtual modifier names:\n");
	for (i1=0;i1<XkbNumVirtualMods;i1++) {
	    XkbNamesPtr names= desc->names;
	    if (desc->names->vmods[i1]!=None)
		 printf("%2d: %12s",i1,atomText(dpy,names->vmods[i1]));
	    else if (desc->server->vmods[i1]!=XkbNoModifier)
		 printf("%2d: %12s",i1,"(no name)");
	    else continue;
	    printf(" = %s\n",modIndexText(desc->server->vmods[i1]));
	}
	for (i1=0;i1<XkbNumIndicators;i1++) {
	    if (desc->names->indicators[i1]!=None) {
		static int been_here=0;
		if (!been_here) {
		    been_here=1;
		    printf("indicator names:\n");
		}
		printf("%2d: %s\n",i1+1,
				atomText(dpy,desc->names->indicators[i1]));
	    }
	}
	if (desc->names->keys) {
	    register int nKeys= desc->max_key_code-desc->min_key_code+1;
	    char buf[5];
	    buf[4]= '\0';
	    printf("keys:\n");
	    for (i1=0;i1<nKeys;i1++) {
		memcpy(buf,desc->names->keys[i1+desc->min_key_code].name,4);
		printf("%-4s ",buf);
		if ((i1&0x7)==7)
		    printf("\n");
	    }
	    if ((i1&0x7)!=0)
		printf("\n");
	}
	if (desc->names->num_char_sets) {
	    printf("character sets: ");
	    for (i1=0;i1<desc->names->num_char_sets;i1++) {
		if (i1==0)
		     printf("%s",atomText(dpy,desc->names->char_sets[i1]));
		else printf(", %s",atomText(dpy,desc->names->char_sets[i1]));
	    }
	    printf("\n");
	}
	else printf("No character sets defined\n");
	printf("%d keyboard groups\n",ctrls->num_groups);
	printf("keyboard groups %s\n",
		(ctrls->enabled_ctrls&XkbGroupsWrapMask)?"wrap":"don't wrap");
	showKeys(dpy,desc,which);
	if ( which & XkbKeyTypesMask )
	    showKeyTypes(dpy,desc);
	XCloseDisplay(dpy);
    }
    return 0;
BAIL:
    XCloseDisplay(dpy);
    return 0;
}
