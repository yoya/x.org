/*
 * $XConsortium: aixKeybd.c,v 1.5 94/02/23 15:55:05 dpw Exp $
 *
 * Copyright IBM Corporation 1987,1988,1989,1990,1991
 *
 * All Rights Reserved
 *
 * License to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of IBM not be
 * used in advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.
 *
 * IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS, AND 
 * NONINFRINGEMENT OF THIRD PARTY RIGHTS, IN NO EVENT SHALL
 * IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
 * ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 *
*/

#include <stdio.h>
#include <sys/hft.h>

#include "X.h"
#include "aixInput.h"
#include "Xmd.h"
#include "input.h"
#include "miscstruct.h"
#include "scrnintstr.h"
#include "keysym.h"
#include "cursorstr.h"

#include "aixKeymap.h"

#include "ibmIO.h"
#include "ibmKeybd.h"
#include "ibmScreen.h"

#include "hftUtils.h"

#include "ibmTrace.h"

/***============================================================***/

static void
rtChangeKeyboardControl(pDevice,ctrl)
    DevicePtr pDevice;
    KeybdCtrl *ctrl;
{
    int volume;

    TRACE(("rtChangeKeyboardControl(pDev=0x%x,ctrl=0x%x)\n",pDevice,ctrl));

    ibmKeyClick =   ctrl->click;
    hftSetKeyClick(ctrl->click);

    /* X specifies duration in milliseconds, RT in 1/128th's of a second */
    ibmBellPitch=	ctrl->bell_pitch;
    ibmBellDuration= 	((double)ctrl->bell_duration)*(128.0/1000.0);

    hftSetLEDS(0x7,(ibmLEDState=ctrl->leds));

    if (ibmKeyRepeat= ctrl->autoRepeat)
	hftSetTypematicDelay( 400 );
    else
	hftSetTypematicDelay( 600 );

    return;
}

/***============================================================***/

static void
rtBell(loud, pDevice, ctrl, what)
    int loud;
    DeviceIntPtr pDevice;
    pointer ctrl;
    int what;
{

    TRACE(("rtBell(loud= %d, pDev= 0x%x)\n",loud,pDevice));
    hftSound(loud,ibmBellDuration,ibmBellPitch);
    return;
}

/***============================================================***/

Bool LegalModifier (key, pDev)
    unsigned int key;
    DevicePtr pDev;
{
    TRACE(("LegalModifier(key= 0x%x)\n",key));
    if ( (key==Aix_Control_L) || (key==Aix_Shift_L) || (key==Aix_Shift_R) ||
	(key==Aix_Caps_Lock) || (key==Aix_Alt_L) || (key==Aix_Alt_R) ||
        (key==Aix_Num_Lock) )
      return TRUE ;
    else
      return FALSE;
}

/***============================================================***/

extern int ibmUsePCKeys;
int kbdType ;

rtGetKbdMappings(pKeySyms, pModMap)
KeySymsPtr pKeySyms;
CARD8 *pModMap;
{
    register int i;
    TRACE(("rtGetKbdMappings( pKeySyms= 0x%x, pModMap= 0x%x )\n",
							pKeySyms,pModMap));
    for (i = 0; i < MAP_LENGTH; i++)
       pModMap[i] = NoSymbol;

    if (ibmUsePCKeys) {
	pModMap[ Aix_Control_L ] = LockMask;
	pModMap[ Aix_Caps_Lock ] = ControlMask;
    }
    else {
	pModMap[ Aix_Caps_Lock ] = LockMask;
	pModMap[ Aix_Control_L ] = ControlMask;
    }

    pModMap[ Aix_Shift_L ] = ShiftMask;
    pModMap[ Aix_Shift_R ] = ShiftMask;
    pModMap[ Aix_Num_Lock ] = NumLockMask;

    switch ( kbdType = hftGetKeyboardID() )
    {
    case HF101KBD :
       pModMap[ Aix_Alt_L ] = Mod1Mask;
       pModMap[ Aix_Alt_R ] = Mod1Mask;
	break;
    case HF102KBD :
       pModMap[ Aix_Alt_L ] = Mod1Mask;
       pModMap[ Aix_Alt_R ] = Mod2Mask;
	break;
    case HF106KBD :
       pModMap[ Aix_Alt_R ] = Mod1Mask;
    case HFT_ILLEGAL_KEYBOARD:
    default :
       pModMap[ Aix_Alt_L ] = Mod1Mask;
       pModMap[ Aix_Alt_R ] = Mod1Mask;
	break;
    }

    pKeySyms->minKeyCode=       AIX_MIN_KEY;
    pKeySyms->maxKeyCode=       AIX_MAX_KEY;
    pKeySyms->mapWidth=         AIX_GLYPHS_PER_KEY;
    pKeySyms->map=              aixmap;
}

/***============================================================***/

int
AIXKeybdProc(pDev, onoff)
    DevicePtr 	 pDev;
    int 	 onoff;
{
    KeySymsRec		keySyms;
    CARD8 		modMap[MAP_LENGTH];

    TRACE(("AIXKeybdProc( pDev= 0x%x, onoff= 0x%x )\n",pDev,onoff));

    switch (onoff)
    {
	case DEVICE_INIT: 
	    ibmKeybd = pDev;
	    rtGetKbdMappings( &keySyms, modMap );
	    InitKeyboardDeviceStruct(
			ibmKeybd, &keySyms, modMap, rtBell,
			rtChangeKeyboardControl);
	    break;
	case DEVICE_ON: 
	    pDev->on = TRUE;
	    break;
	case DEVICE_OFF: 
	    pDev->on = FALSE;
	    break;
	case DEVICE_CLOSE:
	    break;
    }
    return Success;
}
