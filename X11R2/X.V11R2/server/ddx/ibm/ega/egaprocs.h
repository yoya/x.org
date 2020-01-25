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
/* $Header:egaprocs.h 1.2$ */
/* $ACIS:egaprocs.h 1.2$ */
/* $Source: /ibm/acis/usr/src/X11/server/ddx/ibm/ega/RCS/egaprocs.h,v $ */

#if !defined(lint) && !defined(LOCORE) && defined(RCS_HDRS)
static char *rcsidegaprocs = "$Header:egaprocs.h 1.2$";
#endif

void egaBitBlt() ;
Bool egaScreenInit() ;
Bool egaScreenClose() ;
/*
*/
void egaFillSolid() ;
void egaFillTile() ;
void egaFillStipple() ;
void egaOpaqueStipple() ;
Bool egaCreateGC() ;
void egaValidateGC() ;
int  egaDisplayCursor() ;
void egaTileRect() ;
void egaBresLine() ;
void egaHorzLine() ;
void egaVertLine() ;
void egaSetColor() ;
void egaResolveColor() ;
void egaReadColorImage() ;
void egaDrawColorImage() ;
void egaDrawGlyph() ;
