#ifndef MPEL_PROCS_SEEN
#define	MPEL_PROCS_SEEN	1
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

/* $Header:mpelprocs.h 1.2$ */
/* $ACIS:mpelprocs.h 1.2$ */
/* $Source: /ibm/acis/usr/src/X11/server/ddx/ibm/mpel/RCS/mpelprocs.h,v $ */

#ifndef lint
static char *rcsidmpelprocs = "$Header:mpelprocs.h 1.2$";
#endif

extern mpelColorCursor();
extern mpelDisplayCursor();
extern mpelShowCursor();
extern mpelFillStipple();
extern mpelReadColorImage();
extern mpelDrawColorImage();
extern mpelFillSolid();
extern mpelBitBlt();
extern mpelResolveColor();
extern mpelSetColor();
extern void mpelValidateGC();
extern Bool mpelCreateGC();
extern Bool mpelScreenInit();
extern Bool mpelScreenClose();
extern void mpelInstallColormap();
extern void mpelUninstallColormap();
extern void mpelTile();
extern void mpelDrawMonoImage();
extern void mpelDrawMonoByteImage();
extern void mpelFillPolygon();
extern void mpelImageGlyphBlt();
extern void mpelZeroSolidLine();
extern void mpelPolySegment();
extern void mpelPolyText8();
extern void mpelImageText8();
extern Bool mpelRealizeFont();
extern Bool mpelUnrealizeFont();
extern unsigned long *mpelGetPlane();
#endif /* MPEL_PROCS_SEEN */
