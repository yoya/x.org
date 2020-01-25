/* $XConsortium: xkbcompat.c,v 1.2 94/04/01 18:50:09 erik Exp $ */
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

static	char		*dpyName = NULL;
static	int		 changes = 0;
static	int		 synch   = 0;
static	int		 verbose = 0;

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
actText(sa)
    XkbAction sa;
{
static char buf[60];

    switch (sa.type) {
	case XkbSA_NoAction:
	    strcpy(buf,"NO_ACTION");
	    break;
	case XkbSA_SetMods:
	    sprintf(buf,"SET_MODS(%s)",stateText(sa.mods.mask));
	    break;
	case XkbSA_ISOLock:
	    sprintf(buf,"ISO_LOCK(%s)",stateText(sa.iso.mask));
	    break;
	case XkbSA_LockMods:
	    sprintf(buf,"LOCK_MODS(%s)",stateText(sa.mods.mask));
	    break;
	case XkbSA_LatchMods:
	    sprintf(buf,"LATCH_MODS(%s)",stateText(sa.mods.mask));
	    break;
	case XkbSA_SetGroup:
	    sprintf(buf,"SET_GROUP(%s=%d)",
		(sa.group.flags&XkbSA_GroupAbsolute?"absolute":"relative"),
		XkbSAGroup(&sa.group));
	    break;
	case XkbSA_LatchGroup:
	    sprintf(buf,"LATCH_GROUP(%s=%d)",
		(sa.group.flags&XkbSA_GroupAbsolute?"absolute":"relative"),
		XkbSAGroup(&sa.group));
	    break;
	case XkbSA_LockGroup:
	    sprintf(buf,"LOCK_GROUP(%s=%d)",
		(sa.group.flags&XkbSA_GroupAbsolute?"absolute":"relative"),
		XkbSAGroup(&sa.group));
	    break;
	case XkbSA_MovePtr:
	    sprintf(buf,"MOVE_POINTER=(%d,%d)",XkbPtrActionX(&sa.ptr),
						XkbPtrActionY(&sa.ptr));
	    break;
	case XkbSA_AccelPtr:
	    sprintf(buf,"ACCEL_POINTER=(%d,%d)",XkbPtrActionX(&sa.ptr),
						XkbPtrActionY(&sa.ptr));
	    break;
	case XkbSA_PtrBtn:
	    sprintf(buf,"POINTER_BUTTON(%d)",sa.btn.button);
	    break;
	case XkbSA_ClickPtrBtn:
	    sprintf(buf,"CLICK_POINTER_BUTTON(%d,%d)",
				sa.btn.count,sa.btn.button);
	    break;
	case XkbSA_LockPtrBtn:
	    sprintf(buf,"LOCK_POINTER_BUTTON(%d)",sa.btn.button);
	    break;
	case XkbSA_SetPtrDflt:
	    sprintf(buf,"SET_POINTER_DFLT(%d,%d)",sa.dflt.affect,
						  XkbSAPtrDfltValue(&sa.dflt));
	    break; 
	case XkbSA_Terminate:
	    sprintf(buf,"TERMINATE_SERVER");
	    break;
	case XkbSA_SwitchScreen:
	    sprintf(buf,"SWITCH_TO_SCREEN(0x%x,%d)",sa.screen.flags,
						XkbSAScreen(&sa.screen));
	    break;
	case XkbSA_SetControls:
	    sprintf(buf,"SET_CONTROLS(0x%x)",XkbActionCtrls(&sa.ctrls));
	    break;
	case XkbSA_LockControls:
	    sprintf(buf,"LOCK_CONTROLS(0x%x)",XkbActionCtrls(&sa.ctrls));
	    break;
	default:
	    sprintf(buf,"UNKNOWN(0x%x)",sa);
	    break;
    }
    return buf;
}

static char *
SIMatchTypeText(match)
    unsigned match;
{
static	char buf[20];

    buf[0]='\0';
    switch (match) {
	case XkbSI_NoneOf:
	    strcpy(buf,"NoneOf");
	    break;
	case XkbSI_AnyOfOrNone:
	    strcpy(buf,"AnyOfOrNone");
	    break;
	case XkbSI_AnyOf:
	    strcpy(buf,"AnyOf");
	    break;
	case XkbSI_AllOf:
	    strcpy(buf,"AllOf");
	    break;
	case XkbSI_Exactly:
	    strcpy(buf,"Exactly");
	    break;
    }
    return buf;
}

static void
PrintCompatMap(file,xkb)
    FILE *file;
    XkbDescPtr xkb;
{
register int i;
XkbSymInterpretPtr	si;

    fprintf(file,"%d symbol interpretations\n",xkb->compat->num_si);
    si= xkb->compat->sym_interpret;
    for (i=0;i<xkb->compat->num_si;i++,si++) {
	register char *name= XKeysymToString(si->sym);
	if (name)	fprintf(file,"%s+",name);
	else if (si->sym==NoSymbol)
			fprintf(file,"Any+");
	else		fprintf(file,"0x%x+",si->sym);
	fprintf(file,"%s(%s) = {\n",SIMatchTypeText(si->match),
							stateText(si->mods));
	fprintf(file,"    act    = %s\n",actText(si->act));
	if (si->flags&XkbSI_Autorepeat)
	    fprintf(file,"    repeats\n");
	if (si->virtual_mod!=XkbNoModifier)
	    fprintf(file,"   virtual modifier= %d\n",si->virtual_mod);
	fprintf(file,"}\n");
    }
    fprintf(file,"Modifier:    mods  groups\n");
    for (i=0;i<XkbNumModifiers;i++) {
	fprintf(file,"    %2d        0x%02x   0x%02x\n",
			i,
			xkb->compat->real_mod_compat[i].mods,
			xkb->compat->real_mod_compat[i].groups);
    }
    fprintf(file,"Virtual Mod  mods  groups\n");
    for (i=0;i<XkbNumVirtualMods;i++) {
	fprintf(file,"    %2d        0x%02x   0x%02x\n",
			i,
			xkb->compat->vmod_compat[i].mods,
			xkb->compat->vmod_compat[i].groups);
    }
}

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
	else if ((strcmp(argv[i],"-v")==0) || (strcmp(argv[i],"-verbose")==0)) {
	    verbose= 1;
	}
	else if (strcmp(argv[i],"-help")==0) {
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
unsigned	query;
unsigned int	rtrnFlags;
XkbDescPtr	xkb;

  
    if (!parseArgs(argc,argv)) {
	fprintf(stderr,"Usage: %s [<options>] \n",argv[0]);
	fprintf(stderr,"       %s {start,end} [ <flags> [ <message> ] ]\n",
								argv[0]);
	fprintf(stderr,"Where legal options are:\n");
	fprintf(stderr,"-display  <dpy>     specifies display to use\n");
	fprintf(stderr,"-synch              force synchronization\n");
	fprintf(stderr,"-verbose            display resulting compat map\n");
	fprintf(stderr,"If no changes are indicated, %s simply reports\n",
								argv[0]);
	fprintf(stderr,"the current state of all compatbility mappings.\n");
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
    xkb= XkbGetMap(dpy,0,XkbUseCoreKbd);
    if (!xkb) {
	fprintf(stderr,"Couldn't get keyboard map.\n");
	goto BAIL;
    }
    if (!XkbGetCompatMap(dpy,XkbAllCompatMask,xkb)) {
	fprintf(stderr,"Couldn't get compatibility map.\n");
	goto BAIL;
    }
    if (changes) {
	/* apply changes to compatibility map here */
    }
    if ((!changes)||verbose)
	PrintCompatMap(stdout,xkb);
    XCloseDisplay(dpy);
    return 0;
BAIL:
    XCloseDisplay(dpy);
    return 0;
}
