/* $XConsortium: Visual.c,v 1.1 93/07/12 15:28:52 rws Exp $ */
/*

Copyright 1993 by Davor Matic

Permission to use, copy, modify, distribute, and sell this software
and its documentation for any purpose is hereby granted without fee,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation.  Davor Matic makes no representations about
the suitability of this software for any purpose.  It is provided "as
is" without express or implied warranty.

*/
#include "X.h"
#include "Xproto.h"
#include "scrnintstr.h"
#include "dix.h"
#include "mi.h"
#include "mibstore.h"
#define GC XlibGC
#include "Xlib.h"
#include "Xutil.h"
#undef GC

#include "Display.h"
#include "Visual.h"

Visual *xnestVisual(pVisual)
     VisualPtr pVisual;
{
  int i;

  for (i = 0; i < xnestNumVisuals; i++)
    if (pVisual->class == xnestVisuals[i].class &&
	pVisual->bitsPerRGBValue == xnestVisuals[i].bits_per_rgb &&
	pVisual->ColormapEntries == xnestVisuals[i].colormap_size &&
	pVisual->nplanes == xnestVisuals[i].depth &&
	pVisual->redMask == xnestVisuals[i].red_mask &&
	pVisual->greenMask == xnestVisuals[i].green_mask &&
	pVisual->blueMask == xnestVisuals[i].blue_mask)
      return xnestVisuals[i].visual;

  return NULL;
}

Visual *xnestVisualFromID(pScreen, visual)
     ScreenPtr pScreen;
     VisualID visual;
{
  int i;
  
  for (i = 0; i < pScreen->numVisuals; i++)
    if (pScreen->visuals[i].vid == visual)
      return xnestVisual(&pScreen->visuals[i]);

  return NULL;
}

Colormap xnestDefaultVisualColormap(visual)
     Visual *visual;
{
  int i;
  
  for (i = 0; i < xnestNumVisuals; i++)
    if (xnestVisuals[i].visual == visual)
      return xnestDefaultColormaps[i];
  
  return None;
}
