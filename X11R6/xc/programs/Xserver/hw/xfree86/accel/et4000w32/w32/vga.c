/* $XConsortium: vga.c,v 1.6 95/01/23 15:33:48 kaleb Exp $ */
/* $XFree86: xc/programs/Xserver/hw/xfree86/accel/et4000w32/w32/vga.c,v 3.10 1995/01/20 05:18:58 dawes Exp $ */
/*
 * Copyright 1990,91 by Thomas Roell, Dinkelscherben, Germany.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Thomas Roell not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  Thomas Roell makes no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 *
 * THOMAS ROELL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THOMAS ROELL BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Thomas Roell, roell@@informatik.tu-muenchen.de
 *
 */


#include "X.h"
#include "Xmd.h"
#include "input.h"
#include "pixmapstr.h"
#include "regionstr.h"
#include "mipointer.h"
#include "cursorstr.h"
#include "gcstruct.h"

#include "compiler.h"

#include "xf86.h"
#include "xf86Priv.h"
#include "xf86Procs.h"
#include "xf86_OSlib.h"
#include "xf86_Config.h"
#include "vga.h"
#include "cfb.h"
#include "cfbmskbits.h"
#include "w32version.h"
#include "w32box.h"

extern Bool xf86Exiting, xf86Resetting, xf86ProbeFailed;
extern Bool miDCInitialize();
extern Bool W32SaveScreen();

ScrnInfoRec vga256InfoRec = {
  FALSE,		/* Bool configured */
  -1,			/* int tmpIndex */
  -1,			/* int scrnIndex */
  vgaProbe,		/* Bool (* Probe)() */
  vgaScreenInit,	/* Bool (* Init)() */
  vgaValidMode,		/* Bool (* ValidMode)() */
  vgaEnterLeaveVT,	/* void (* EnterLeaveVT)() */
  (void (*)())NoopDDA,		/* void (* EnterLeaveMonitor)() */
  (void (*)())NoopDDA,		/* void (* EnterLeaveCursor)() */
  (void (*)())NoopDDA,	/* void (* AdjustFrame)() */
  vgaSwitchMode,	/* Bool (* SwitchMode)() */
  vgaPrintIdent,        /* void (* PrintIdent)() */
  8,			/* int depth */
  {0, 0, 0},            /* xrgb weight */
  8,			/* int bitsPerPixel */
  PseudoColor,		/* int defaultVisual */
  -1, -1,		/* int virtualX,virtualY */
  -1,                   /* int displayWidth */
  -1, -1, -1, -1,	/* int frameX0, frameY0, frameX1, frameY1 */
  {0, },		/* OFlagSet options */
  {0, },		/* OFlagSet clockOptions */
  {0, },                /* OFlagSet xconfigFlag */
  NULL,			/* char *chipset */
  NULL,			/* char *ramdac */
  0,			/* int dacSpeed */
  0,			/* int clocks */
  {0, },		/* int clock[MAXCLOCKS] */
  MAX_W32_CLOCK,	/* int maxClock */
  0,			/* int videoRam */
  0xC0000,		/* int BIOSbase */
  0,			/* unsigned long MemBase, unused for this driver */
  240, 180,		/* int width, height */
  0,			/* unsigned long speedup */
  NULL,			/* DisplayModePtr modes */
  NULL,			/* DisplayModePtr pModes */
  NULL,			/* char *clockprog */
  -1,                   /* int textclock */
  FALSE,		/* Bool bankedMono */
  "ET4000W32",		/* char *name */
  {0, 0, 0},		/* RgbRec blackColour */ 
  {0xFF, 0xFF, 0xFF},	/* RgbRec whiteColour */ 
  vga256ValidTokens,	/* int *validTokens */
  ET4000W32_PATCH_LEVEL,/* char *patchLevel */
  0,			/* int IObase */
  0,			/* int PALbase */
  0,			/* int COPbase */
  0,			/* int POSbase */
  0,			/* int instance */
  0,			/* int s3Madjust */
  0,			/* int s3Nadjust */
  0,			/* int s3MClk */
};

pointer vgaOrigVideoState = NULL;
pointer vgaNewVideoState = NULL;
pointer vgaBase = NULL;
pointer vgaVirtBase = NULL;

void (* vgaEnterLeaveFunc)(
#if NeedFunctionPrototypes
    Bool
#endif
) = (void (*)())NoopDDA;
Bool (* vgaInitFunc)(
#if NeedFunctionPrototypes
    DisplayModePtr
#endif
) = (Bool (*)())NoopDDA;
Bool (* vgaValidModeFunc)(
#if NeedFunctionPrototypes
    DisplayModePtr
#endif
) = (Bool (*)())NoopDDA;
void * (* vgaSaveFunc)(
#if NeedFunctionPrototypes
    void *
#endif
) = (void *(*)())NoopDDA;
void (* vgaRestoreFunc)(
#if NeedFunctionPrototypes
    void *
#endif
) = (void (*)())NoopDDA;
void (* vgaAdjustFunc)(
#if NeedFunctionPrototypes
    int,
    int
#endif
) = (void (*)())NoopDDA;
void (* vgaSaveScreenFunc)() = (void (*)())NoopDDA;
int vgaMapSize;
void *vgaWriteBottom;
void *vgaWriteTop; 

int  vgaInterlaceType;
OFlagSet vgaOptionFlags;
extern Bool vgaPowerSaver;

int vgaIOBase;

static ScreenPtr savepScreen = NULL;
static PixmapPtr ppix = NULL;

vgaHWCursorRec vgaHWCursor;

extern miPointerScreenFuncRec xf86PointerScreenFuncs;

#define Drivers W32Drivers

extern vgaVideoChipPtr Drivers[];

/*
 *	Wrap the chip-level restore function in a protect/unprotect.
 */
static w32_mode = FALSE;
void
vgaRestore(mode)
     pointer mode;
{
    vgaProtect(TRUE);
    (vgaRestoreFunc)(mode);
    vgaProtect(FALSE);
    if (w32_mode)
    {
	/*
	 *  Temporary
	 *  This somewhat contradicts Tseng's Tehnical Note #24--GGL
	 */
	if (W32p)
	{
	    int tmp;
	    outb(vgaIOBase + 4, 0x34);
	    GlennsIODelay();
	    tmp = inb(vgaIOBase + 5) | 0x10;
	    GlennsIODelay();
	    outb(vgaIOBase + 5, tmp);
	    GlennsIODelay();
	}
	(*vgaSaveScreenFunc)(SS_FINISH);
    }
}


/*
 *     Print out identifying strings for drivers included in the server
 */
void
vgaPrintIdent()
{
  int            i, j, n, c;
  char		 *id;

  ErrorF("  %s: accelerated server for ET4000W32 graphics adaptors (Patchlevel %s):\n      ",
         vga256InfoRec.name, vga256InfoRec.patchLevel);

  n = 0;
  c = 0;
  for (i=0; Drivers[i]; i++)
    for (j = 0; id = (Drivers[i]->ChipIdent)(j); j++, n++)
    {
      if (n)
      {
        ErrorF(",");
        c++;
        if (c + 1 + strlen(id) < 70)
        {
          ErrorF(" ");
          c++;
        }
        else
        {
          ErrorF("\n      ");
          c = 0;
        }
      }
      ErrorF("%s", id);
      c += strlen(id);
    }
  ErrorF("\n");
}


/*
 *     probe and initialize the hardware driver
 */
Bool
vgaProbe()
{
  int            i, j, k;
  DisplayModePtr pMode, pEnd, pmaxX = NULL, pmaxY = NULL;
  int            maxX, maxY;
  int            needmem, rounding;
  int            tx,ty;

  /* Only supports 8bpp.  Check if someone is trying a different depth */
  if (vga256InfoRec.depth != 8)
    {
      ErrorF("\n%s %s: Unsupported depth (%d)\n",
	     XCONFIG_GIVEN, vga256InfoRec.name, vga256InfoRec.depth);
      return(FALSE);
    }

  for (i=0; Drivers[i]; i++)

    if ((Drivers[i]->ChipProbe)())
      {
        xf86ProbeFailed = FALSE;

	needmem = vga256InfoRec.videoRam * 1024;
	rounding = Drivers[i]->ChipRounding;

        if (xf86Verbose)
        {
	  ErrorF("%s %s: chipset:  %s\n",
             OFLG_ISSET(XCONFIG_CHIPSET,&vga256InfoRec.xconfigFlag) ? 
                    XCONFIG_GIVEN : XCONFIG_PROBED ,
             vga256InfoRec.name,
             vga256InfoRec.chipset);
	  ErrorF("%s %s: videoram: %dk",
		 OFLG_ISSET(XCONFIG_VIDEORAM,&vga256InfoRec.xconfigFlag) ? 
                    XCONFIG_GIVEN : XCONFIG_PROBED ,
                 vga256InfoRec.name,
	         vga256InfoRec.videoRam
	         );
	

	  for (j=0; j < vga256InfoRec.clocks; j++)
	  {
	    if ((j % 8) == 0)
	      ErrorF("\n%s %s: clocks:", 
                OFLG_ISSET(XCONFIG_CLOCKS,&vga256InfoRec.xconfigFlag) ? 
                    XCONFIG_GIVEN : XCONFIG_PROBED ,
                vga256InfoRec.name);
	    ErrorF(" %6.2f", (double)vga256InfoRec.clock[j]/1000.0);
	  }
	  ErrorF("\n");
        }

	vgaEnterLeaveFunc = Drivers[i]->ChipEnterLeave;
	vgaInitFunc = Drivers[i]->ChipInit;
	vgaValidModeFunc = Drivers[i]->ChipValidMode;
	vgaSaveFunc = Drivers[i]->ChipSave;
	vgaRestoreFunc = Drivers[i]->ChipRestore;
	vgaWriteBottom = (pointer)Drivers[i]->ChipWriteBottom;
        vgaWriteTop = (pointer)Drivers[i]->ChipWriteTop;

	vga256InfoRec.AdjustFrame = vgaAdjustFunc = Drivers[i]->ChipAdjust;

	vgaSaveScreenFunc = Drivers[i]->ChipSaveScreen;
	vgaMapSize = Drivers[i]->ChipMapSize;
	vgaInterlaceType = Drivers[i]->ChipInterlaceType;
	vgaOptionFlags = Drivers[i]->ChipOptionFlags;
	OFLG_SET(OPTION_POWER_SAVER, &vgaOptionFlags);

	xf86VerifyOptions(&vgaOptionFlags, &vga256InfoRec);

	if (OFLG_ISSET(OPTION_POWER_SAVER, &vga256InfoRec.options))
	    vgaPowerSaver = TRUE;

	/* if Virtual given: is the virtual size too big? */
	if (vga256InfoRec.virtualX > 0 &&
	    vga256InfoRec.virtualX * vga256InfoRec.virtualY > needmem)
	  {
	    ErrorF("%s: Too little memory for virtual resolution %d %d\n",
                   vga256InfoRec.name, vga256InfoRec.virtualX,
                   vga256InfoRec.virtualY);
            vgaEnterLeaveFunc(LEAVE);
	    return(FALSE);
	  }

        maxX = maxY = -1;
	pMode = pEnd = vga256InfoRec.modes;
	if (pMode == NULL)
	  {
	    ErrorF("No modes supplied in XF86Config\n");
            vgaEnterLeaveFunc(LEAVE);
	    return(FALSE);
	  }
	do {
	  if (!xf86LookupMode(pMode, &vga256InfoRec))
	    {
              vgaEnterLeaveFunc(LEAVE);
	      return(FALSE);
	    }
	  if (pMode->HDisplay * pMode->VDisplay > needmem)
	    {
	      ErrorF("%s: Too little memory for mode %s\n", vga256InfoRec.name,
                     pMode->name);
              vgaEnterLeaveFunc(LEAVE);
	      return(FALSE);
	    }
          if (pMode->HDisplay > maxX)
            {
              maxX = pMode->HDisplay;
              pmaxX = pMode;
            }
          if (pMode->VDisplay > maxY)
            {
              maxY = pMode->VDisplay;
              pmaxY = pMode;
            }
	  pMode = pMode->next;
	}
	while (pMode != pEnd);

	tx = vga256InfoRec.virtualX;
	ty = vga256InfoRec.virtualY;

        vga256InfoRec.virtualX = max(maxX, vga256InfoRec.virtualX);
        vga256InfoRec.virtualY = max(maxY, vga256InfoRec.virtualY);

	if (vga256InfoRec.virtualX % rounding)
	  {
	    vga256InfoRec.virtualX -= vga256InfoRec.virtualX % rounding;
	    ErrorF(
	     "%s %s: Virtual width rounded down to a multiple of %d (%d)\n",
	     XCONFIG_PROBED, vga256InfoRec.name, rounding,
	     vga256InfoRec.virtualX);
            if (vga256InfoRec.virtualX < maxX)
            {
              ErrorF(
               "%s: Rounded down virtual width (%d) is too small for mode %s",
	       vga256InfoRec.name, vga256InfoRec.virtualX, pmaxX->name);
              vgaEnterLeaveFunc(LEAVE);
              return(FALSE);
            }
	  }

	if ( vga256InfoRec.virtualX * vga256InfoRec.virtualY > needmem)
	{
          if (vga256InfoRec.virtualX != maxX ||
              vga256InfoRec.virtualY != maxY)
	    ErrorF(
              "%s: Too little memory to accomodate virtual size and mode %s\n",
               vga256InfoRec.name,
               (vga256InfoRec.virtualX == maxX) ? pmaxX->name : pmaxY->name);
          else
	    ErrorF("%s: Too little memory to accomodate modes %s and %s\n",
                   vga256InfoRec.name, pmaxX->name, pmaxY->name);
          vgaEnterLeaveFunc(LEAVE);
	  return(FALSE);
	}
	if ((tx != vga256InfoRec.virtualX) || (ty != vga256InfoRec.virtualY))
            OFLG_CLR(XCONFIG_VIRTUAL,&vga256InfoRec.xconfigFlag);

        vga256InfoRec.displayWidth = vga256InfoRec.virtualX;
        if (xf86Verbose)
          ErrorF("%s %s: Virtual resolution set to %dx%d\n",
                 OFLG_ISSET(XCONFIG_VIRTUAL,&vga256InfoRec.xconfigFlag) ? 
                    XCONFIG_GIVEN : XCONFIG_PROBED ,
                 vga256InfoRec.name,
                 vga256InfoRec.virtualX, vga256InfoRec.virtualY);

        vgaHWCursor.Initialized = FALSE;
	return TRUE;
      }
  
  if (vga256InfoRec.chipset)
    ErrorF("%s: '%s' is an invalid chipset", vga256InfoRec.name,
	       vga256InfoRec.chipset);
  return FALSE;
}


/*
 *      Attempt to find and initialize a VGA framebuffer
 *      Most of the elements of the ScreenRec are filled in.  The
 *      video is enabled for the frame buffer...
 */
Bool
vgaScreenInit (scr_index, pScreen, argc, argv)
    int            scr_index;    /* The index of pScreen in the ScreenInfo */
    ScreenPtr      pScreen;      /* The Screen to initialize */
    int            argc;         /* The number of the Server's arguments. */
    char           **argv;       /* The arguments themselves. Don't change! */
{
  int displayResolution = 75;    /* default to 75dpi */
  extern int monitorResolution;

  if (serverGeneration == 1) {
    vgaBase = xf86MapVidMem(scr_index, VGA_REGION, (pointer)0xA0000,
			    vgaMapSize);
    vgaVirtBase = (pointer)VGABASE;
    vgaWriteBottom = (void *)((unsigned int)vgaWriteBottom +
		     (unsigned int)vgaBase); 
    vgaWriteTop    = (void *)((unsigned int)vgaWriteTop +
		     (unsigned int)vgaBase);
  }

  if (!(vgaInitFunc)(vga256InfoRec.modes))
    FatalError("%s: hardware initialization failed\n", vga256InfoRec.name);

  /*
   * This function gets called while in graphics mode during a server
   * reset, and this causes the original video state to be corrupted.
   * So, only initialise vgaOrigVideoState if it hasn't previously been done
   * DHD Dec 1991.
   */
  if (!vgaOrigVideoState)
    vgaOrigVideoState = (pointer)(vgaSaveFunc)(vgaOrigVideoState);
  vgaRestore(vgaNewVideoState);

#ifndef DIRTY_STARTUP
  W32SaveScreen(NULL, FALSE); /* blank the screen */
#endif
  (vgaAdjustFunc)(vga256InfoRec.frameX0, vga256InfoRec.frameY0);

  /*
   * Take display resolution from the -dpi flag if specified
   */

  if (monitorResolution)
    displayResolution = monitorResolution;

  /*
   * Inititalize the dragon to color display
   */
  if (!cfbScreenInit(pScreen,
		     (pointer) vgaVirtBase,
		     vga256InfoRec.virtualX,
		     vga256InfoRec.virtualY,
		     displayResolution, displayResolution,
		     vga256InfoRec.displayWidth))
    return(FALSE);

  pScreen->CloseScreen = vgaCloseScreen;
  pScreen->SaveScreen = W32SaveScreen;
  pScreen->InstallColormap = vgaInstallColormap;
  pScreen->UninstallColormap = vgaUninstallColormap;
  pScreen->ListInstalledColormaps = vgaListInstalledColormaps;
  pScreen->StoreColors = vgaStoreColors;
  
  if (vgaHWCursor.Initialized)
  {
    xf86PointerScreenFuncs.WarpCursor = vgaHWCursor.Warp;
    pScreen->QueryBestSize = vgaHWCursor.QueryBestSize;
    vgaHWCursor.Init(0, pScreen);
  }
  else
  {
    miDCInitialize (pScreen, &xf86PointerScreenFuncs);
  }

  if (!cfbCreateDefColormap(pScreen))
    return(FALSE);


#ifndef DIRTY_STARTUP
    W32SaveScreen(NULL, TRUE); /* unblank the screen */

  /* Fill the screen with black pixels */
    if (serverGeneration == 1)
    {
	/* Depends on pixel size--GGLGGL*/
	W32_INIT_BOX(GXcopy, ~0, PFILL(pScreen->blackPixel), vga256InfoRec.virtualX - 1)
	W32_BOX(0, vga256InfoRec.virtualX, vga256InfoRec.virtualY)
	WAIT_XY
    }
#endif /* ! DIRTY_STARTUP */

  savepScreen = pScreen;

  return(TRUE);
}


/*
 * vgaEnterLeaveVT -- 
 *      grab/ungrab the current VT completely.
 */
void
vgaEnterLeaveVT(enter, screen_idx)
     Bool enter;
     int screen_idx;
{
  BoxRec  pixBox;
  RegionRec pixReg;
  DDXPointRec pixPt;
  PixmapPtr   pspix;
  ScreenPtr   pScreen = savepScreen;

  if (!xf86Resetting && !xf86Exiting)
    {
      pixBox.x1 = 0; pixBox.x2 = pScreen->width;
      pixBox.y1 = 0; pixBox.y2 = pScreen->height;
      pixPt.x = 0; pixPt.y = 0;
      (pScreen->RegionInit)(&pixReg, &pixBox, 1);
      pspix = (PixmapPtr)pScreen->devPrivate;
    }

  if (enter)
    {
      w32_mode = TRUE;
      xf86MapDisplay(screen_idx, VGA_REGION);

      (vgaEnterLeaveFunc)(ENTER);
      vgaOrigVideoState = (pointer)(vgaSaveFunc)(vgaOrigVideoState);
      XRamdac();
      vgaRestore(vgaNewVideoState);

#ifdef SCO 
      /*
       * This is a temporary fix for et4000's, it shouldn't affect the other
       * drivers and the user doesn't notice it so, here it is.  What does it
       * fix, et4000 driver leaves the screen blank when you switch back to it.
       * A mode toggle, even on the same mode, fixes it.
       */
      vgaSwitchMode(vga256InfoRec.modes);
#endif

      /*
       * point pspix back to vgaVirtBase, and copy the dummy buffer to the
       * real screen.
       */
      if (!xf86Resetting)
      {
	ScrnInfoPtr pScr = XF86SCRNINFO(pScreen);

	if (vgaHWCursor.Initialized)
	{
	  vgaHWCursor.Init(0, pScreen);
	  vgaHWCursor.Restore(pScreen);
	  vgaAdjustFunc(pScr->frameX0, pScr->frameY0);
	}

        if ((pointer)pspix->devPrivate.ptr != (pointer)vgaVirtBase && ppix)
        {
	  pspix->devPrivate.ptr = (pointer)vgaVirtBase;
	  cfbDoBitbltCopy(&ppix->drawable, &pspix->drawable, GXcopy, &pixReg,
			  &pixPt, 0xFF);

        }
      }
      if (ppix) {
        (pScreen->DestroyPixmap)(ppix);
        ppix = NULL;
      }
    }
  else
    {
      w32_mode = FALSE;
      /* Make sure IO isn't disabled (by other drivers) */    
      xf86MapDisplay(screen_idx, VGA_REGION);

      (vgaEnterLeaveFunc)(ENTER);

      /*
       * Create a dummy pixmap to write to while VT is switched out.
       * Copy the screen to that pixmap
       */
      if (!xf86Exiting)
      {
        ppix = (pScreen->CreatePixmap)(pScreen, pScreen->width,
                                        pScreen->height, pScreen->rootDepth);
        if (ppix)
        {
	  cfbDoBitblt(&pspix->drawable, &ppix->drawable, GXcopy, &pixReg,
                      &pixPt, 0xFF);
	  pspix->devPrivate.ptr = ppix->devPrivate.ptr;
        }
        (vgaSaveFunc)(vgaNewVideoState);
      }
      /*
       * We come here in many cases, but one is special: When the server aborts
       * abnormaly. Therefore there MUST be a check whether vgaOrigVideoState
       * is valid or not.
       */
      if (vgaOrigVideoState)
	vgaRestore(vgaOrigVideoState);
      VGARamdac();

      (vgaEnterLeaveFunc)(LEAVE);

      xf86UnMapDisplay(screen_idx, VGA_REGION);
    }
}


/*
 *  Ensure video is enabled when the server exits
 */
Bool
vgaCloseScreen(screen_idx, pScreen)
    int		screen_idx;
    ScreenPtr	pScreen;
{
  /*
   * Hmm... The server may shut down even if it is not running on the
   * current vt. Let's catch this case here.
   */
  xf86Exiting = TRUE;
  if (xf86VTSema)
    vgaEnterLeaveVT(LEAVE, screen_idx);
  else if (ppix) {
    /*
     * 7-Jan-94 CEG: The server is not running on the current vt.
     * Free the screen snapshot taken when the server vt was left.
     */
    (savepScreen->DestroyPixmap)(ppix);
    ppix = NULL;
  }
  return(TRUE);
}


/*
 *  Set a new display mode
 */
Bool
vgaSwitchMode(mode)
     DisplayModePtr mode;
{
  if ((vgaInitFunc)(mode))
  {
    vgaRestore(vgaNewVideoState);
    return(TRUE);
  }
  else
  {
    ErrorF("Mode switch failed because of hardware initialisation error\n");
    return(FALSE);
  }
}

/*
 * vgaValidMode --
 *     Validate a mode for VGA architecture. Also checks the chip driver
 *     to see if the mode can be supported.
 */
Bool
vgaValidMode(mode)
     DisplayModePtr mode;
{
  if ((vgaValidModeFunc)(mode))
  {
    return(TRUE);
  }
  else
  {
    return(FALSE);
  }
}
