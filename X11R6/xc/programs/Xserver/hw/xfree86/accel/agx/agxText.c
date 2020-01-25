/* $XConsortium: agxText.c,v 1.3 95/01/05 20:30:52 kaleb Exp $ */
/* $XFree86: xc/programs/Xserver/hw/xfree86/accel/agx/agxText.c,v 3.2 1994/11/19 07:50:20 dawes Exp $ */
/*
 * Copyright 1992 by Kevin E. Martin, Chapel Hill, North Carolina.
 * Copyright 1994 by Henry A. Worth, Sunnyvale, California.
 * 
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Kevin E. Martin not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  Kevin E. Martin makes no
 * representations about the suitability of this software for any purpose. It
 * is provided "as is" without express or implied warranty.
 * 
 * KEVIN E. MARTIN AND HENRY A. WORTH DISCLAIM ALL WARRANTIES WITH 
 * REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL THE AUTHORS BE LIABLE 
 * FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES 
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN 
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING 
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * Modified by Amancio Hasty and Jon Tombs
 * Modified for the AGX by Henry A. Worth (haw30@eng.amdahl.com)
 * 
 * Id: s3text.c,v 2.3 1993/08/09 06:17:57 jon Exp jon
 */

#include	"X.h"
#include	"Xmd.h"
#include	"Xproto.h"
#include	"cfb.h"
#include	"misc.h"
#include	"xf86.h"
#include	"gcstruct.h"
#include	"windowstr.h"
#include	"fontstruct.h"
#include	"dixfontstr.h"
#include	"mi.h"
#include	"agx.h"
#include	"regagx.h"
#include	"agxBCach.h"

extern CacheFont8Ptr agxCacheFont8();

int
agxPolyText8(pDraw, pGC, x, y, count, chars)
     DrawablePtr pDraw;
     GCPtr pGC;
     int   x, y;
     int   count;
     char *chars;
{
   CacheFont8Ptr ret;

   if (!xf86VTSema)
   {
      return(miPolyText8(pDraw, pGC, x, y, count, chars));
   }

   if ((ret = agxCacheFont8(pGC->font)) == NULL)
       return miPolyText8(pDraw, pGC, x, y, count, chars);
   else
       return agxCPolyText8(pDraw, pGC, x, y, count, chars, ret, FALSE);
}

void
agxImageText8(pDraw, pGC, x, y, count, chars)
     DrawablePtr pDraw;
     GCPtr pGC;
     int   x, y;
     int   count;
     char *chars;
{
   CacheFont8Ptr ret;

   if (!xf86VTSema)
   {
      miImageText8(pDraw, pGC, x, y, count, chars);
      return;
   } 

   if ((ret = agxCacheFont8(pGC->font)) == NULL)
      miImageText8(pDraw, pGC, x, y, count, chars);
   else
#if 0
      agxCImageText8(pDraw, pGC, x, y, count, chars, ret);
#else
      agxCPolyText8(pDraw, pGC, x, y, count, chars, ret, TRUE);
#endif 
}
