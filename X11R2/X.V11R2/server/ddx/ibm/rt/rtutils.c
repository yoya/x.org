/***********************************************************
		Copyright IBM Corporation 1987

                      All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the name of IBM not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/
/* $Header:rtutils.c 6.0$ */
/* $ACIS:rtutils.c 6.0$ */
/* $Source: /vice/X11/src/server/ddx/ibm/rt/RCS/rtutils.c,v $ */

#ifndef lint
static char *rcsid = "$Header:rtutils.c 6.0$";
#endif

#include <machinecons/qevent.h>

#include "X.h"
#include "cursorstr.h"
#include "miscstruct.h"
#include "scrnintstr.h"
#include "pixmapstr.h"

#include "rtscreen.h"
#include "rtutils.h"

int	rtTrace;

#ifdef NOT_X
#define ErrorF printf
#endif

#define PUT_BIT(d,b)   (((d)&(((unsigned)1)<<(b)))?ErrorF("*"):ErrorF("."))

print_pattern(width,height,data)
int	width,height;
char	*data;
{
char	*tmp=data;
int	row,bit;
unsigned data_byte;
int	bits_left;

    TRACE(("print_pattern( width= %d, height= %d, data= 0x%x )\n",
							width,height,data));

    for (row=0;row<height;row++) {
	ErrorF("0x");
	for (bit=0;bit<(width+7)/8;bit++) {
	   ErrorF("%02x",*tmp++);
	}
	ErrorF("\n");
    }
    for (row=0;row<height;row++) {
	for (bits_left=width;bits_left>0;bits_left-=8) {
	    data_byte= *data++;
	    for (bit=7;bit>=(bits_left>8?0:8-bits_left);bit--) {
		PUT_BIT(data_byte,bit);
	    }
	}
	ErrorF("\n");
    }
}

/***==================================================================***/

print_event(xE)
XEvent	*xE;
{

    TRACE(("print_event( xE= 0x%x )\n",xE));

    ErrorF("Event(%d,%d): ",xE->xe_x,xE->xe_y);
    switch (xE->xe_device) {
	case XE_MOUSE:		ErrorF("mouse "); break;
	case XE_DKB:		ErrorF("keyboard "); break;
	case XE_TABLET:		ErrorF("tablet "); break;
	case XE_AUX:		ErrorF("aux "); break;
	case XE_CONSOLE:	ErrorF("console "); break;
	default:		ErrorF("unknown(%d) ",xE->xe_device); break;
    }
    if (xE->xe_type==XE_BUTTON) {
	ErrorF("button ");
	if	(xE->xe_direction==XE_KBTUP)	ErrorF("up ");
	else if	(xE->xe_direction==XE_KBTDOWN)	ErrorF("down ");
	else if	(xE->xe_direction==XE_KBTRAW)	ErrorF("raw ");
	else 			ErrorF("unknown(%d) ",xE->xe_direction);
	ErrorF("(key= %d)",xE->xe_key);
    }
    else if (xE->xe_type==XE_MMOTION)	ErrorF("MMOTION");
    else if (xE->xe_type==XE_TMOTION)	ErrorF("TMOTION");
    ErrorF("\n");
}

/***==================================================================***/

rtPrintUsage(name,error)
char	*name;
char	*error;
{
int	i;

    TRACE(("rtPrintUsage(%s,%s)\n",name,error));
    if (error)	ErrorF("%s\n",error);
    ErrorF("Usage: %s <display> [option] <tty>\n",name);
    ErrorF("Recognized screens are:\n");
    ErrorF("    -all		opens all attached, supported screens\n");
    for (i=0;(rtPossibleScreens[i].rt_ScreenFlag!=NULL);i++) {
	ErrorF("    %s\n",rtPossibleScreens[i].rt_ScreenFlag);
    }
    ErrorF("Other options are:\n");
    ErrorF("    -a #		set mouse acceleration (pixels)\n");
    ErrorF("    -c		turns off key-click\n");
    ErrorF("     c #		key-click volume (0-8)\n");
    ErrorF("    -co string	color database file\n");
    ErrorF("    -fc string	cursor font\n");
    ErrorF("    -fn string	default text font name\n");
    ErrorF("    -fp string	default text font path\n");
#ifdef RT_SPECIAL_MALLOC
    ErrorF("    -malloc #	set malloc check level (0-5)\n");
#endif
#ifndef RT_MUST_USE_HDWR
    ErrorF("    -nohdwr		use generic functions where applicable\n");
#endif
    ErrorF("    -p #		screen-saver pattern duration (seconds)\n");
    ErrorF("    -pckeys		swap CAPS LOCK and CTRL (for touch typists)\n");
#ifdef RT_SPECIAL_MALLOC
    ErrorF("    -plumber string	dump malloc arena to named file\n");
#endif
    ErrorF("    -r		turn off auto-repeat\n");
    ErrorF("     r		turn on auto-repeat\n");
    ErrorF("    -rtkeys		use CAPS LOCK and CTRL as labelled\n");
    ErrorF("    -f #		bell (feep) volume (0-100)\n");
    ErrorF("    -x string	load extension on startup (not implemented)\n");
    ErrorF("    -help		print help message\n");
    ErrorF("    -s #		screen-saver timeout (seconds)\n");
    ErrorF("    -t #		set mouse motion threshold (pixels)\n");
#ifdef TRACE_X
    ErrorF("    -trace		trace execution of RT specific functions\n");
#endif
    ErrorF("    -to #		connection time out\n");
    ErrorF("     v		video blanking for screen-saver\n");
    ErrorF("    -v		screen-saver without video blanking\n");
    ErrorF("    -wrap		wrap mouse in both dimensions\n");
    ErrorF("    -wrapx		wrap mouse in X only\n");
    ErrorF("    -wrapy		wrap mouse in Y only\n");
    ErrorF("See Xibm(1) for a more complete description\n");
    exit(1);
}

/***==================================================================***/

rtDIXArgument(arg)
char	*arg;
{
    TRACE(("rtDIXArgument(%s)\n",arg));
    if (arg[0]==':')		return(1);	/* Display */
    if (strcmp(arg,"-a")==0)	return(2);	/* Pointer control */
    if (strcmp(arg,"-c")==0)	return(2);	/* Click off */
    if (strcmp(arg,"c")==0)	return(1);	/* Click on */
    if (strcmp(arg,"-co")==0)	return(2);	/* Color database path */
    if (strcmp(arg,"-f")==0)	return(2);	/* Bell (feep) volume */
    if (strcmp(arg,"-fc")==0)	return(2);	/* Cursor font */
    if (strcmp(arg,"-fn")==0)	return(2);	/* Text font */
    if (strcmp(arg,"-fp")==0)	return(2);	/* Font path */
    if (strcmp(arg,"-help")==0)	return(1);	/* Help */
    if (strcmp(arg,"-p")==0)	return(2);	/* Screen saver interval */
    if (strcmp(arg,"r")==0)	return(1);	/* Auto-repeat on */
    if (strcmp(arg,"-r")==0)	return(1);	/* Auto-repeat off */
    if (strcmp(arg,"-s")==0)	return(2);	/* Screen saver time */
    if (strcmp(arg,"-t")==0)	return(2);	/* Pointer motion threshold */
    if (strcmp(arg,"-to")==0)	return(2);	/* Time out */
    if (strcmp(arg,"v")==0)	return(1);	/* Prefer blanking */
    if (strcmp(arg,"-v")==0)	return(1);	/* Don't prefer blanking */
    if (strcmp(arg,"-x")==0)	return(2);	/* Add extension */
    return(0);
}

/***==================================================================***/

#ifdef RT_SPECIAL_MALLOC
#include <stdio.h>
#include <signal.h>

int	rtShouldDumpArena= 0;
static	char	*rtArenaFile= 0;

static	
rtMarkDumpArena()
{
    rtShouldDumpArena= 1;
    return 0;
}


rtDumpArena()
{
FILE  *mfil;
static	char	fileName[100];
static	int	dumpNum= 0;

   sprintf(fileName,rtArenaFile,dumpNum++);
   mfil= fopen(fileName,"a");
   if (!mfil) {
	ErrorF("Couldn't open %s to dump arena, ignored\n",fileName);
	return(0);
   }
   else {
	ErrorF("Dumping malloc arena to %s\n",fileName);
	plumber(mfil);
	fflush(mfil);
	fclose(mfil);
   }
   rtShouldDumpArena= 0;
   return(1);
}

rtNoteHit()
{
static int old= 4;

   ErrorF("received SIGTERM\n");
   old= SetMallocCheckLevel(old);
   return(1);
}

rtSetupPlumber(name)
char	*name;
{
struct sigvec sv;
extern exit();

    ErrorF("Setting up plumber to dump to %s\n",name);
    rtArenaFile= name;
    unlink(rtArenaFile);
    sv.sv_handler= rtMarkDumpArena;
    sv.sv_mask= 0;
    sv.sv_onstack= 0;
    sigvec(SIGUSR1,&sv,NULL);
    sv.sv_handler= rtDumpArena;
    sigvec(SIGEMT,&sv,NULL);
    sv.sv_handler= rtNoteHit;
    sigvec(SIGTERM,&sv, NULL);
    sv.sv_handler= exit;
    sigvec(SIGUSR2, &sv, NULL);
    return(1);
}
#endif /* RT_SPECIAL_MALLOC */
