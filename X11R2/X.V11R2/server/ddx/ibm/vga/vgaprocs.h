/***********************************************************
		Copyright IBM Corporation 1988

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
/* $Header:vgaprocs.h 1.4$ */
/* $ACIS:vgaprocs.h 1.4$ */
/* $Source: /ibm/acis/usr/src/X11/server/ddx/ibm/vga/RCS/vgaprocs.h,v $ */

#if !defined(lint) && !defined(LOCORE) && defined(RCS_HDRS)
static char *rcsidvgaprocs = "$Header:vgaprocs.h 1.4$";
#endif

void vgaBitBlt() ;
Bool vgaScreenInit() ;
Bool vgaScreenClose() ;
/*
*/
void vgaFillSolid() ;
void vgaFillTile() ;
void vgaFillStipple() ;
void vgaOpaqueStipple() ;
Bool vgaCreateGC() ;
void vgaValidateGC() ;
int  vgaDisplayCursor() ;
void vgaTileRect() ;
void vgaBresLine() ;
void vgaHorzLine() ;
void vgaVertLine() ;
void vgaSetColor() ;
void vgaResolveColor() ;
void vgaResolveToGray() ;
void vgaReadColorImage() ;
void vgaDrawColorImage() ;
void vgaDrawGlyph() ;
