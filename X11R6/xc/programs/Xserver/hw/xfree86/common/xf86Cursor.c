/* $XConsortium: xf86Cursor.c,v 1.3 95/01/06 20:57:31 kaleb Exp $ */
/* $XFree86: xc/programs/Xserver/hw/xfree86/common/xf86Cursor.c,v 3.3 1994/12/11 10:54:36 dawes Exp $ */
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
 */

#include "X.h"
#include "Xmd.h"
#include "input.h"
#include "mipointer.h"
#include "scrnintstr.h"

#include "compiler.h"

#include "xf86.h"
#include "xf86Procs.h"
/* #include "atKeynames.h" -hv- dont need that include here */


static Bool   xf86CursorOffScreen();
static void   xf86CrossScreen();
static void   xf86WrapCursor();

miPointerScreenFuncRec xf86PointerScreenFuncs = {
  xf86CursorOffScreen,
  xf86CrossScreen,
  xf86WrapCursor,
};



/*
 * xf86InitViewport --
 *      Initialize paning & zooming parameters, so that a driver must only
 *      check what resolutions are possible and whether the virtual area
 *      is valid if specifyed.
 */

void
xf86InitViewport(pScr)
     ScrnInfoPtr pScr;
{
  /*
   * Compute the initial Viewport if necessary
   */
  if (pScr->frameX0 < 0)
    {
      pScr->frameX0 = (pScr->virtualX - pScr->modes->HDisplay) / 2;
      pScr->frameY0 = (pScr->virtualY - pScr->modes->VDisplay) / 2;
    }
  pScr->frameX1 = pScr->frameX0 + pScr->modes->HDisplay - 1;
  pScr->frameY1 = pScr->frameY0 + pScr->modes->VDisplay - 1;

  /*
   * Now adjust the initial Viewport, so it lies within the virtual area
   */
  if (pScr->frameX1 >= pScr->virtualX)
    {
	pScr->frameX0 = pScr->virtualX - pScr->modes->HDisplay;
	pScr->frameX1 = pScr->frameX0 + pScr->modes->HDisplay - 1;
    }

  if (pScr->frameY1 >= pScr->virtualY)
    {
	pScr->frameY0 = pScr->virtualY - pScr->modes->VDisplay;
	pScr->frameY1 = pScr->frameY0 + pScr->modes->VDisplay - 1;
    }
}


/*
 * xf86SetViewport --
 *      Scroll the visual part of the screen so the pointer is visible.
 */

void
xf86SetViewport(pScreen, x, y)
     ScreenPtr   pScreen;
     int         x, y;
{
  Bool          frameChanged = FALSE;
  ScrnInfoPtr   pScr = XF86SCRNINFO(pScreen);

  /*
   * check wether (x,y) belongs to the visual part of the screen
   * if not, change the base of the displayed frame accoring
   */
  if ( pScr->frameX0 > x) { 
    pScr->frameX0 = x;
    pScr->frameX1 = x + pScr->modes->HDisplay - 1;
    frameChanged = TRUE ;
  }
  
  if ( pScr->frameX1 < x) { 
    pScr->frameX1 = x + 1;
    pScr->frameX0 = x - pScr->modes->HDisplay + 2;
    frameChanged = TRUE ;
  }
  
  if ( pScr->frameY0 > y) { 
    pScr->frameY0 = y;
    pScr->frameY1 = y + pScr->modes->VDisplay - 1;
    frameChanged = TRUE;
  }
  
  if ( pScr->frameY1 < y) { 
    pScr->frameY1 = y;
    pScr->frameY0 = y - pScr->modes->VDisplay + 1;
    frameChanged = TRUE; 
  }
  
  if (frameChanged) (pScr->AdjustFrame)(pScr->frameX0, pScr->frameY0);
}



/*
 * xf86ZoomViewport --
 *      Reinitialize the visual part of the screen for another modes->
 */

void
xf86ZoomViewport (pScreen, zoom)
     ScreenPtr   pScreen;
     int        zoom;
{
  ScrnInfoPtr   pScr = XF86SCRNINFO(pScreen);

  if (pScr->modes != pScr->modes->next)
  {
    pScr->modes = zoom > 0 ? pScr->modes->next : pScr->modes->prev;

    if ((pScr->SwitchMode)(pScr->modes))
    {
      /* 
       * adjust new frame for the displaysize
       */
      pScr->frameX0 = (pScr->frameX1 + pScr->frameX0 -pScr->modes->HDisplay)/2;
      pScr->frameX1 = pScr->frameX0 + pScr->modes->HDisplay - 1;

      if (pScr->frameX0 < 0)
	{
	  pScr->frameX0 = 0;
	  pScr->frameX1 = pScr->frameX0 + pScr->modes->HDisplay - 1;
	}
      else if (pScr->frameX1 >= pScr->virtualX)
	{
	  pScr->frameX0 = pScr->virtualX - pScr->modes->HDisplay;
	  pScr->frameX1 = pScr->frameX0 + pScr->modes->HDisplay - 1;
	}
      
      pScr->frameY0 = (pScr->frameY1 + pScr->frameY0 - pScr->modes->VDisplay)/2;
      pScr->frameY1 = pScr->frameY0 + pScr->modes->VDisplay - 1;

      if (pScr->frameY0 < 0)
	{
	  pScr->frameY0 = 0;
	  pScr->frameY1 = pScr->frameY0 + pScr->modes->VDisplay - 1;
	}
      else if (pScr->frameY1 >= pScr->virtualY)
	{
	  pScr->frameY0 = pScr->virtualY - pScr->modes->VDisplay;
	  pScr->frameY1 = pScr->frameY0 + pScr->modes->VDisplay - 1;
	}
    }
    else /* switch failed, so go back to old mode */
      pScr->modes = zoom > 0 ? pScr->modes->prev : pScr->modes->next;
  }

  (pScr->AdjustFrame)(pScr->frameX0, pScr->frameY0);
}



/*
 * xf86CursorOffScreen --
 *      Check whether it is necessary to switch to another screen
 */

/* ARGSUSED */
static Bool
xf86CursorOffScreen (pScreen, x, y)
     ScreenPtr   *pScreen;
     int         *x, *y;
{
  int		i;

  if ((screenInfo.numScreens > 1) && ((*x < 0) || ((*pScreen)->width <= *x))) {
    i = (*pScreen)->myNum;
    if (*x < 0) {
      i = (i ? i : screenInfo.numScreens) -1;
      *pScreen = screenInfo.screens[i];
      *x += (*pScreen)->width;
    }
    else {
      *x -= (*pScreen)->width;
      i = (i+1) % screenInfo.numScreens;
      *pScreen = screenInfo.screens[i];
    }
    return(TRUE);
  }
  return(FALSE);
}



/*
 * xf86CrossScreen --
 *      Switch to another screen
 */

/* ARGSUSED */
static void
xf86CrossScreen (pScreen, entering)
     ScreenPtr   pScreen;
     Bool        entering;
{
  if (xf86Info.sharedMonitor)
    (XF86SCRNINFO(pScreen)->EnterLeaveMonitor)(entering);
  (XF86SCRNINFO(pScreen)->EnterLeaveCursor)(entering);
}


/*
 * xf86WrapCursor --
 *      Wrap possible to another screen
 */

/* ARGSUSED */
static void
xf86WrapCursor (pScreen, x, y)
     ScreenPtr   pScreen;
     int         x,y;
{
  miPointerWarpCursor(pScreen,x,y);

  xf86Info.currentScreen = pScreen;
}
