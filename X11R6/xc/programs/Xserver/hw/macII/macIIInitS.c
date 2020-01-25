/*****************************************************************************
Copyright 1988-1993 by Apple Computer, Inc., Cupertino, CA
			All Rights Reserved

Permission to use, copy, modify, and distribute this software
for any purpose and without fee is hereby granted, provided
that the above copyright notice appear in all copies.

APPLE MAKES NO WARRANTY OR REPRESENTATION, EITHER EXPRESS,
OR IMPLIED, WITH RESPECT TO THIS SOFTWARE, ITS QUALITY,
PERFORMANCE, MERCHANABILITY, OR FITNESS FOR A PARTICULAR
PURPOSE. AS A RESULT, THIS SOFTWARE IS PROVIDED "AS IS,"
AND YOU THE USER ARE ASSUMING THE ENTIRE RISK AS TO ITS
QUALITY AND PERFORMANCE. IN NO EVENT WILL APPLE BE LIABLE 
FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
DAMAGES RESULTING FROM ANY DEFECT IN THE SOFTWARE.

THE WARRANTY AND REMEDIES SET FORTH ABOVE ARE EXCLUSIVE
AND IN LIEU OF ALL OTHERS, ORAL OR WRITTEN, EXPRESS OR
IMPLIED.

*****************************************************************************/
/*-
 * macIIInitS.c --
 *	Initialization functions for screen/keyboard/mouse, etc.
 *
 * Copyright (c) 1987 by the Regents of the University of California
 *
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 *
 *
 */

/************************************************************
Copyright 1987 by Sun Microsystems, Inc. Mountain View, CA.

                    All Rights Reserved

Permission  to  use,  copy,  modify,  and  distribute   this
software  and  its documentation for any purpose and without
fee is hereby granted, provided that the above copyright no-
tice  appear  in all copies and that both that copyright no-
tice and this permission notice appear in  supporting  docu-
mentation,  and  that the names of Sun or X Consortium
not be used in advertising or publicity pertaining to 
distribution  of  the software  without specific prior 
written permission. Sun and X Consortium make no 
representations about the suitability of this software for 
any purpose. It is provided "as is" without any express or 
implied warranty.

SUN DISCLAIMS ALL WARRANTIES WITH REGARD TO  THIS  SOFTWARE,
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FIT-
NESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL SUN BE  LI-
ABLE  FOR  ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,  DATA  OR
PROFITS,  WHETHER  IN  AN  ACTION OF CONTRACT, NEGLIGENCE OR
OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION  WITH
THE USE OR PERFORMANCE OF THIS SOFTWARE.

********************************************************/

#include    "macII.h"
#include    <servermd.h>
#include    "dixstruct.h"
#include    "dix.h"
#include    "opaque.h"
#include    "mipointer.h"
#include    <compat.h>
#include    <sys/slotmgr.h>
#ifdef XTESTEXT1
#include "keysym.h"
#endif /* XTESTEXT1 */

extern int macIIMouseProc();
extern void macIIKbdProc();
extern int macIIKbdSetUp();

extern Bool macIIMonoInit();

extern Bool macIIColorInit();

extern Bool macIISlotProbe();

extern void SetInputCheck();

static int GetVideoData();
#ifdef __STDC__
static long GetDevBase (int);
#endif

DevicePtr pPointerDevice, pKeyboardDevice;

static int autoRepeatHandlersInstalled; /* FALSE each time InitOutput called */

/*-
 *-----------------------------------------------------------------------
 * SigIOHandler --
 *	Signal handler for SIGIO - input is available.
 *
 * Results:
 *	SIGIO is set - ProcessInputEvents() will be called soon.
 *
 * Side Effects:
 *	None
 *
 *-----------------------------------------------------------------------
 */
/*ARGSUSED*/
static void
SigIOHandler(sig, code, scp)
    int		code;
    int		sig;
    struct sigcontext *scp;
{
    macIIEnqueueEvents ();
}

macIIFbDataRec macIIFbData[] = {
    macIISlotProbe,  	"slot 9",	    neverProbed,
    macIISlotProbe,  	"slot A",	    neverProbed,
    macIISlotProbe,  	"slot B",	    neverProbed,
    macIISlotProbe,  	"slot C",	    neverProbed,
    macIISlotProbe,  	"slot D",	    neverProbed,
    macIISlotProbe,  	"slot E",	    neverProbed,
};

/*
 * NUMSCREENS is the number of supported frame buffers (i.e. the number of
 * structures in macIIFbData which have an actual probeProc).
 */
#define NUMSCREENS (sizeof(macIIFbData)/sizeof(macIIFbData[0]))

fbFd	macIIFbs[NUMSCREENS];  /* Space for descriptors of open frame buffers */

static PixmapFormatRec	formats[] = {
    1, 1, BITMAP_SCANLINE_PAD,		/* 1-bit deep */
/*    2, 2, BITMAP_SCANLINE_PAD,	/* 2-bit deep */
/*    4, 4, BITMAP_SCANLINE_PAD,		/* 4-bit deep */
    8, 8, BITMAP_SCANLINE_PAD,		/* 8-bit deep */
    16, 16, BITMAP_SCANLINE_PAD,	/* 16-bit deep */
    24, 32, BITMAP_SCANLINE_PAD,	/* 24-bit deep */
/*    32, 32, BITMAP_SCANLINE_PAD,	/* 32-bit deep */
};
#define NUMFORMATS	(sizeof formats)/(sizeof formats[0])

#define NUMSLOTS 6
#define NUMMODES	7		/* 1,2,4,8,16,24,32 */
static struct legalModeStruct {	/* only 1 and 8 bits are supported */
    int depth;
    int legal;
} legalModes[NUMMODES] = {{1,1},{2,0},{4,0},{8,1},{16,1},{24,1},{32,1}};

static int screenDepth[MAXSCREENS] = {0,0,0,0,0,0};

static struct videoModeStruct{
	int depth[NUMMODES];
        int mode[NUMMODES];   
	VPBlock info[NUMMODES];
} videoModes[NUMSLOTS] = {
	{{0,0,0,0,0,0,0}, {0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{0,0,0,0,0,0,0}, {0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{0,0,0,0,0,0,0}, {0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{0,0,0,0,0,0,0}, {0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{0,0,0,0,0,0,0}, {0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{0,0,0,0,0,0,0}, {0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}};

static char*	vAddrs[NUMSLOTS];

/*-
 *-----------------------------------------------------------------------
 * InitOutput --
 *	Initialize screenInfo for all actually accessible framebuffers.
 *
 * Results:
 *	screenInfo init proc field set
 *
 * Side Effects:
 *	None
 *
 *-----------------------------------------------------------------------
 */

void
InitOutput(pScreenInfo, argc, argv)
    ScreenInfo 	  *pScreenInfo;
    int     	  argc;
    char    	  **argv;
{
    int     	  i, index, ac = argc;
    char	  **av = argv;

    pScreenInfo->imageByteOrder = IMAGE_BYTE_ORDER;
    pScreenInfo->bitmapScanlineUnit = BITMAP_SCANLINE_UNIT;
    pScreenInfo->bitmapScanlinePad = BITMAP_SCANLINE_PAD;
    pScreenInfo->bitmapBitOrder = BITMAP_BIT_ORDER;

    pScreenInfo->numPixmapFormats = NUMFORMATS;
    for (i=0; i< NUMFORMATS; i++)
    {
        pScreenInfo->formats[i] = formats[i];
    }

    autoRepeatHandlersInstalled = FALSE;

    ParseDepths(argc, argv);

    for (i = 0, index = 0; i < NUMSLOTS; i++) {
	if ((* macIIFbData[i].probeProc) (pScreenInfo, index, i, argc, argv)) {
	    /* This display exists OK */
	    index++;
	} 
    }

    if (index == 0)
	FatalError("Can't find any displays\n");

    pScreenInfo->numScreens = index;

    /* NOTE: This should actually be done individually for each screen,
       before the SetEntries call to load the color table. mjb */
    /* set selected gamma table entries */
    macIISetGamma();
}

static Bool
IsLegalDepth(depth)
int depth;
{
    int i;

    for (i=0; i< NUMMODES; i++) {
	if ((legalModes[i].legal) && (legalModes[i].depth == depth))
	    return TRUE;
    }
    return FALSE;
}

ParseDepths(argc, argv)
    int		argc;
    char 	**argv;
{
#define SCREEN_LO 	0
#define SCREEN_HI 	(MAXSCREENS - 1)
#define EQSTRING(s1,s2)	(!(strcmp((s1), (s2))))

    int i;
    int screen = -1;
    int depth = -1;
    int supportedDepth = 0;

    i = 0;
    while (i < argc) {
	if (EQSTRING(argv[i], "-screen")) {
	    if (i <= argc - 4) {
	        screen = atoi(argv[i+1]);
                if ((screen < SCREEN_LO) || (screen > SCREEN_HI)) {
		    ErrorF("Invalid screen number %d; a legal number is %d < screen < %d\n", 
		        screen, SCREEN_LO, SCREEN_HI);
                }

		if (!EQSTRING(argv[i+2], "-depth")) {
		    goto usage;
		}

	        /* is this a depth supported by the server? */
		depth = atoi(argv[i+3]);
		if (IsLegalDepth(depth)) {
		    screenDepth[screen] = depth;
		    supportedDepth = 1;
		}
		if (!supportedDepth) {
		    ErrorF("Invalid screen depth specified for screen %d: %d. Using default.\n", screen, depth);
		}
	    }
	    else {
usage:
		ErrorF("Usage: -screen screennum -depth depthnum\n");
	    }
	    i += 4;
	}
	else {
            i++;
	}
    }
}

/*-
 *-----------------------------------------------------------------------
 * InitInput --
 *	Initialize all supported input devices...what else is there
 *	besides pointer and keyboard?
 *
 * Results:
 *	None.
 *
 * Side Effects:
 *	Two DeviceRec's are allocated and registered as the system pointer
 *	and keyboard devices.
 *
 *-----------------------------------------------------------------------
 */
/*ARGSUSED*/
void
InitInput(argc, argv)
    int     	  argc;
    char    	  **argv;
{
    DevicePtr p, k;
    static int  zero = 0;
    
    p = AddInputDevice(macIIMouseProc, TRUE);
    k = AddInputDevice(macIIKbdProc, TRUE);
#ifdef XTESTEXT1
    xtest_command_key = XK_Clear + MIN_KEYCODE; /* Had to set my own key to
						   ensure it was available
						   on our keyboards. mjb */
#endif

    pPointerDevice = p;
    pKeyboardDevice = k;

    RegisterPointerDevice(p);
    RegisterKeyboardDevice(k);
    miRegisterPointerDevice(screenInfo.screens[0], p);

    setcompat (getcompat() | COMPAT_BSDSIGNALS);
    if (!mieqInit (k, p))
	FatalError ("could not initialize event queue");
    OsSignal(SIGIO, SigIOHandler);
    SetTimeSinceLastInputEvent ();
}

/*-
 *-----------------------------------------------------------------------
 * macIISlotProbe --
 *	Attempt to find and initialize a framebuffer. 
 *
 * Results:
 *	TRUE if everything went ok. FALSE if not.
 *
 * Side Effects:
 *	Memory is allocated for the frame buffer and the buffer is mapped.
 *
 *-----------------------------------------------------------------------
 */
Bool
macIISlotProbe (pScreenInfo, index, fbNum, argc, argv)
    ScreenInfo	  *pScreenInfo;	/* The screenInfo struct */
    int	    	  index;    	/* The index of pScreen in the ScreenInfo */
    int	    	  fbNum;    	/* Index into the macIIFbData array */
    int	    	  argc;	    	/* The number of the Server's arguments. */
    char    	  **argv;   	/* The arguments themselves. Don't change! */
{
    int         i, oldNumScreens;
    int		depth;
    int		depthSupported;
    static struct video defVideo;
    static char *videoVirtaddr = (char *)(112 * 1024 * 1024);
    char	*devBase, *devOffset;

    static ColorSpec csTable[2] =
	{
	    0, {0xffff, 0xffff, 0xffff},	/* white */
	    0, {0x0000, 0x0000, 0x0000}		/* black */
	};

    static struct VDSetEntryRecord vde =
	{
#ifdef	SUPPORT_2_0
	    (char *)
#endif
	    csTable,
	    0,              /* start = 0 */
	    1               /* count = 1 (2 entries) */
	};

    struct VDPageInfo vdp;
    struct CntrlParam pb;
    struct strioctl ctl; /* Streams ioctl control structure */
    int mode_index;
    int fd;
#ifndef	SLOT_LO
#define SLOT_LO 0x09
#endif
    int slotNum = fbNum + SLOT_LO;


    if (macIIFbData[fbNum].probeStatus == probedAndFailed) {
	return FALSE;
    }

    if (macIIFbData[fbNum].probeStatus == neverProbed) {

	if ((depth = GetVideoData(&defVideo, slotNum)) < 0) {
		macIIFbData[fbNum].probeStatus = probedAndFailed;
		return FALSE;
	}

	/* this slot is a video screen */

	/*
	 * we need to ensure that the video board supports a depth that the 
	 * server supports before physing in the board.
	 */
	depthSupported = 0;
	for (i = 0; i < NUMMODES; i++) {
	    if (IsLegalDepth(videoModes[fbNum].depth[i])) {
		depthSupported = 1;
		break;
	    }
	}
	if (!depthSupported) {
	    macIIFbData[fbNum].probeStatus = probedAndFailed;
	    ErrorF("Video board in slot %d does not offer a supported depth\n",
		(slotNum - SLOT_LO));
	    return FALSE;
	}

	{
		struct video_map_slot vMap;
		struct strioctl ctl; /* Streams ioctl control structure */

		/* map frame buffer to next 16MB segment boundary at or above 128M */
		videoVirtaddr = videoVirtaddr + (16 * 1024 * 1024); 
		vAddrs[index] = videoVirtaddr;
		vMap.map_slotnum = slotNum;
	        vMap.map_physnum = index;
        	vMap.map_virtaddr = videoVirtaddr;

		ctl.ic_cmd = VIDEO_MAP_SLOT;
		ctl.ic_timout = -1;
		ctl.ic_len = sizeof(vMap);
		ctl.ic_dp = (char *)&vMap;
		fd = open("/dev/console", O_RDWR, 0);
		if (fd < 0) {
		    FatalError ("could not open /dev/console");
		} 
		if (ioctl(fd, I_STR, &ctl) == -1) {
			FatalError ("ioctl I_STR VIDEO_MAP_SLOT failed");
			(void) close (fd);
			return (FALSE);
		}
		(void) close(fd);
	}

	macIIFbs[index].slot = slotNum;
	macIIFbs[index].installedMap = NULL;

	/* set things up for default mode */
	macIIFbs[index].defaultDepth = depth;
	macIIFbs[index].info = defVideo.vpBlock;

	/* check if user asks for a specific depth */
	if (screenDepth[index]) {
	    int i;
	    for (i = 0; i < NUMMODES; i++) {
		if (videoModes[fbNum].depth[i] == screenDepth[index]) {
		    macIIFbs[index].defaultDepth = screenDepth[index];
		    macIIFbs[index].info = videoModes[fbNum].info[i];
		    break;
		}
	    }
#ifdef SEPARATE_24
	    if (i == NUMMODES && screenDepth[index] == 24) {
		int j;
		for (j = 0; j < NUMMODES; j++) {
		    if (videoModes[fbNum].depth[j] == 32) {
			macIIFbs[index].defaultDepth = 24;
			macIIFbs[index].info = videoModes[fbNum].info[j];
			break;
		    }
		}
	    } else
#endif
	    if (i == NUMMODES)
		ErrorF("Screen %d does not support %d bits per pixel\n",
		       index, screenDepth[index]);
	}

	macIIFbData[fbNum].probeStatus = probedAndSucceeded;

    }

    /*
     * If we've ever successfully probed this device, do the following. 
     */

    oldNumScreens = pScreenInfo->numScreens;

    {
    /* poke the video board */
	ctl.ic_cmd = VIDEO_CONTROL;
	ctl.ic_timout = -1;
	ctl.ic_len = sizeof(pb);
	ctl.ic_dp = (char *)&pb;

	fd = open("/dev/console", O_RDWR, 0);
	if (fd < 0) {
	    FatalError ("could not open /dev/console");
	} 

	if (macIIFbs[index].defaultDepth == 32)
	    macIIFbs[index].defaultDepth = 24;

	for (mode_index = 0; mode_index < NUMMODES; ) {
	    if (videoModes[fbNum].depth[mode_index] == 
		    macIIFbs[index].defaultDepth) {
		break;
	    }
	    mode_index++;
	}

	vdp.csMode = videoModes[fbNum].mode[mode_index];
	vdp.csData = 0;
	vdp.csPage = 0;
	vdp.csBaseAddr = (char *) NULL;

	pb.qType = slotNum;
	pb.ioTrap = noQueueBit;
	pb.ioCmdAddr = (char *) -1;
	pb.csCode = cscSetMode;
	* (char **) pb.csParam = (char *) &vdp;

	if (ioctl(fd, I_STR, &ctl) == -1) {
		FatalError ("ioctl I_STR SetMode failed");
		(void) close (fd);
		return (FALSE);
	}

	(void) close (fd);
	if (pb.qType != 0) {
		FatalError ("ioctl I_STR VIDEO_CONTROL CMD failed");
		return (FALSE);
	}

	if ((devBase = (char *)GetDevBase(slotNum)) < (char *)0) {
		macIIFbData[fbNum].probeStatus = probedAndFailed;
		return FALSE;
	}
	devOffset = (char *)(devBase - (char *)((long)devBase & 0xfff00000));
	
    /*
     * This offset is the result of intensive research to determine where
     * the frame buffer memory should start. On-board and RAM-based video
     * don't behave exactly like NuBus video cards, because their base
     * address isn't necessarily at the slot (0xfs000000) boundary.
     */
	macIIFbs[index].fb = (pointer)(vAddrs[index] +
				       (long)(videoModes[fbNum].info[mode_index].vpBaseOffset) +
				       (long)devOffset);

	if (devBase == (char *)0xfbb08000 || devBase == (char *)0xfee08000)
	    macIIFbs[index].fb = (pointer)((long)macIIFbs[index].fb & 0xffff7fff);
	    /* Subtract the offset in the IIci and IIcx onboard video address */
    }

    /* install the black & white color map */
    fd = open("/dev/console", O_RDWR, 0);
    if (fd < 0) {
	FatalError ("could not open /dev/console");
    } 
    pb.qType = slotNum;
    pb.ioTrap = noQueueBit;
    pb.ioCmdAddr = (char *) -1;
    pb.csCode = CTB_SET_ENTRIES(macIIFbs[index]);
    * (char **) pb.csParam = (char *) &vde;

    if (ioctl(fd, I_STR, &ctl) == -1) {
	FatalError ("ioctl I_STR SetEntries failed");
	(void) close (fd);
	return(FALSE);
    }  
    (void) close (fd);

    macIIBlackScreen(index);

    switch (macIIFbs[index].defaultDepth) {
	case 1:
    	    i = AddScreen(macIIMonoInit, argc, argv);
	    break;

	case 8:
	case 16:
	case 24:
	case 32:
    	    i = AddScreen(macIIColorInit, argc, argv);
	    break;

	default:
	    FatalError("Encountered bogus depth: %d", fbNum);
	    break;
    }

    return (i >= oldNumScreens);
}

/*
 * This routine uses the Slot Manager to search through the list of video
 * parameter blocks.  The VPBlock structures are filled in.  Returns slot 
 * manager error, or 0 for success.
 */

static int noSlotMgr;

static void
SigSYSHandler(sig, code, scp)
    int		code;
    int		sig;
    struct sigcontext *scp;
{
    noSlotMgr++;
}

static int GetVideoData(vp, slot)
register struct video *vp;
int slot;
{
	int curMode;		/* current slotmanager video mode */
	int depth = 0;		/* start with bad depth */
	int defaultMode = 0x80;	/* video modes normally default to 0x80 */
	int err;		/* last slot manager result */
	int success = 0;	/* assume failure */
	struct SpBlock pb;
	VPBlock *vd;
	caddr_t slotModesPointer;
	int fbNum = slot - SLOT_LO;
	int i, got24 = 0;

	vp->dce.dCtlExtDev = 0;
	vp->dce.dCtlSlot = slot;
	pb.spSlot = vp->dce.dCtlSlot;
	pb.spID = 0;
	pb.spCategory = catDisplay;
	pb.spCType = typeVideo;
	pb.spDrvrSW = drSwApple;
	pb.spTBMask = 1;

	noSlotMgr = 0;
	OsSignal(SIGSYS, SigSYSHandler);
	err = slotmanager(_sNextTypesRsrc,&pb);
	OsSignal(SIGSYS, SIG_DFL);
	if (noSlotMgr) return(-1);

	if (err == 0 && pb.spSlot != vp->dce.dCtlSlot)
	    err = smNoMoresRsrcs;
	else if (err == 0) {
	    vp->dce.dCtlSlotId = pb.spID;
	    slotModesPointer = pb.spsPointer;
	    for (curMode = 0x80, i = 0; ((!err) && (i < NUMMODES)); curMode++) {
		pb.spID = curMode;
		pb.spsPointer = slotModesPointer;
		err = slotmanager(_sFindStruct,&pb);
		if (err == 0) {
		    pb.spID = mVidParams;
		    pb.spResult = (long)(&videoModes[fbNum].info[i]);
		    err = slotmanager(_sGetBlock,&pb);
		    if (err == 0) {
			vd = (VPBlock *) pb.spResult;
			videoModes[fbNum].depth[i] = vd->vpPixelSize;
			if (vd->vpPixelSize == 24) got24 = 1;
			videoModes[fbNum].mode[i] = curMode;
			i++;
			if (IsLegalDepth(vd->vpPixelSize) && vd->vpPixelSize > depth) {
			    depth = vd->vpPixelSize;
			    defaultMode = curMode;
			    vp->vpBlock = *vd;
			    success = 1;
			}
		    }
		}
	    }
	}
	if (depth == 32 && !got24) {
	    int i;

	    for (i = 0; i < NUMMODES; i++) {
		struct videoModeStruct *pvm = &videoModes[fbNum];

		if (pvm->depth[i] == 32) {
		    pvm->depth[NUMMODES-1] = 24;
		    defaultMode = pvm->mode[NUMMODES-1] = pvm->mode[i];
		    pvm->info[NUMMODES-1] = pvm->info[i];
		    depth = 24;
		    break;
		}
	    }
	}
	vp->video_def_mode = defaultMode;
	return success? depth: -abs(err);
}

/*
 * This routine uses the slot manager to return the actual base address
 * of the video device. This is not necessarily the 0xfs000000 boundary,
 * particularly for onboard and RAM-based video.
 */

long GetDevBase (slot)
    int slot;
{
    struct SpBlock spb, *sp;

    sp = &spb;
    sp->spID = 0;
    sp->spSlot = slot;
    sp->spCategory = catDisplay;
    sp->spCType = typeVideo;
    sp->spDrvrSW = drSwApple;
    sp->spDrvrHW = 0;
    sp->spTBMask = 1;
    sp->spExtDev = 0;

    if (slotmanager (_sNextTypesRsrc, sp) == 0) {
        if (sp->spSlot == slot)
            if (slotmanager (_sFindDevBase, sp) == 0)
                return (sp->spResult);
    }

    return (-1);
}

/*-
 *-------------------------------------------------------------%---------
 * macIIScreenInit --
 *	Things which must be done for all types of frame buffers...
 *	Should be called last of all.
 *
 * Results:
 *	None.
 *
 * Side Effects:
 *	The graphics context for the screen is created. The CreateGC,
 *	CreateWindow and ChangeWindowAttributes vectors are changed in
 *	the screen structure.
 *
 *	Both a BlockHandler and a WakeupHandler are installed for the
 *	first screen.  Together, these handlers implement autorepeat
 *	keystrokes on the macII.
 *
 *-----------------------------------------------------------------------
 */
Bool
macIIScreenInit (pScreen)
    ScreenPtr	  pScreen;
{
    extern void   macIIBlockHandler();
    extern void   macIIWakeupHandler();
    static ScreenPtr autoRepeatScreen;
    extern miPointerScreenFuncRec   macIIPointerCursorFuncs;

    /*
     *	Block/Unblock handlers
     */
    if (autoRepeatHandlersInstalled == FALSE) {
	autoRepeatScreen = pScreen;
	autoRepeatHandlersInstalled = TRUE;
    }

    if (pScreen == autoRepeatScreen) {
        pScreen->BlockHandler = macIIBlockHandler;
        pScreen->WakeupHandler = macIIWakeupHandler;
    }

    miDCInitialize (pScreen, &macIIPointerCursorFuncs);

    return TRUE; 
}

/*-
 *-----------------------------------------------------------------------
 * macIIBlackScreen --
 *    Fill a frame buffer with the black pixel.
 *
 * Results:
 *    None
 *
 * Side Effects:
 *
 *-----------------------------------------------------------------------
 */
int
macIIBlackScreen(index)
      int index;
{
    fbFd *pf;
    register unsigned char* fb;
    register int fbinc, line, lw;
    register unsigned int *fbt;
    short pad, shift, success;
    long pixFill;

    pf = &macIIFbs[index];
    fb = pf->fb; /* Assumed longword aligned! */

    switch (pf->info.vpPixelSize) {
    case 1:
	pad = 31;
	shift = 5;
	pixFill = 0xffffffff;
	success = 1;
	break;
    case 8:
	pad = 3;
	shift = 2;
	pixFill = 0x01010101;
	success = 1;
	break;
    case 16:
	pad = 1;
	shift = 1;
	pixFill = 0x8210;
	success = 1;
	break;
    case 24:
    case 32:
	pad = 0;
	shift = 0;
	pixFill = 0x010101;
	success = 1;
	break;
    default:
	ErrorF("Bad depth in macIIBlackScreen.\n");
	break;
    }
    if (success) {
	fbinc = pf->info.vpRowBytes;
#ifdef	SUPPORT_2_0
	for (line = pf->info.v_top; line < pf->info.v_bottom; line++) {
#else
	for (line = pf->info.vpBounds.top; line < pf->info.vpBounds.bottom; line++) {
#endif	SUPPORT_2_0
	    fbt = (unsigned int *)fb;
#ifdef	SUPPORT_2_0
	    lw = ((pf->info.v_right - pf->info.v_left) + pad) >> shift;
#else
	    lw = ((pf->info.vpBounds.right - pf->info.vpBounds.left) + pad) >> shift;
#endif	SUPPORT_2_0
	    do {
		*fbt++ = pixFill;
	    } while (--lw);
	    fb += fbinc;
	}
    }
}

void
AbortDDX()
{
    extern int consoleFd, devosmFd;

    if (devosmFd > 0) close(devosmFd);
    if (consoleFd > 0) {
	macIIKbdSetUp(consoleFd, FALSE); /* Must NOT FatalError() anywhere! */
        close(consoleFd);
	consoleFd = 0;
    }
}

/* Called by GiveUp(). */
void
ddxGiveUp()
{
}

int
ddxProcessArgument (argc, argv, i)
    int       argc;
    char *argv[];
    int       i;
{
    extern int optionMouse, noOsm;

    if ( strcmp ( argv[i], "-screen" ) == 0)
	return 4;
    else if ( strcmp ( argv[i], "-optionmouse" ) == 0) {
	optionMouse = TRUE;
	return 1;
    }
    else if ( strcmp ( argv[i], "-noosm" ) == 0) {
	noOsm = TRUE;
	return 1;
    }
    else return 0;
}

void
ddxUseMsg()
{
    ErrorF("-screen # -depth #		run screen (0-5) at depth {1,8,24}\n");
    ErrorF("-optionmouse		option-arrows for mouse buttons\n");
    ErrorF("-noosm			don't open /dev/osm\n");
}

void
MessageF(s)
char *s;
{
	ErrorF(s);
}

/* AUX version of ffs does bits in the wrong order, silly them */

int
ffs(mask)
unsigned int    mask;
{
    register i;

    if ( ! mask ) return 0;
    i = 1;
    while (! (mask & 1)) {
	i++;
	mask = mask >> 1;
    }
    return i;
}
