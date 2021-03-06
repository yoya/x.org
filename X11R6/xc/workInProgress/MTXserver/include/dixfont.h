/* $XConsortium: dixfont.h,v 1.2 94/04/17 21:16:39 dpw Exp $ */
/***********************************************************
Copyright 1987 by Digital Equipment Corporation, Maynard, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the name of Digital not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

#ifndef DIXFONT_H
#define DIXFONT_H 1

#include <dix.h>
#include <font.h>
#include <closure.h>

#define NullDIXFontProp ((DIXFontPropPtr)0)

typedef struct _DIXFontProp *DIXFontPropPtr;

extern int FontToXError(
#if NeedFunctionPrototypes
    int /*err*/
#endif
);

extern Bool SetDefaultFont(
#if NeedFunctionPrototypes
    char * /*defaultfontname*/
#endif
);

extern void QueueFontWakeup(
#if NeedFunctionPrototypes
    FontPathElementPtr /*fpe*/
#endif
);

extern void RemoveFontWakeup(
#if NeedFunctionPrototypes
    FontPathElementPtr /*fpe*/
#endif
);

extern void FontWakeup(
#if NeedFunctionPrototypes
    pointer /*data*/,
    int /*count*/,
    pointer /*LastSelectMask*/
#endif
);

extern int OpenFont(
#if NeedFunctionPrototypes
    ClientPtr /*client*/,
    XID /*fid*/,
    Mask /*flags*/,
    unsigned /*lenfname*/,
    char * /*pfontname*/
#endif
);

extern int CloseFont(
#if NeedFunctionPrototypes
    pointer /*pfont*/,
    XID /*fid*/
#endif
);

typedef struct _xQueryFontReply *xQueryFontReplyPtr;

extern void QueryFont(
#if NeedFunctionPrototypes
    FontPtr /*pFont*/,
    xQueryFontReplyPtr /*pReply*/,
    int /*nProtoCCIStructs*/
#endif
);

extern int ListFonts(
#if NeedFunctionPrototypes
    ClientPtr /*client*/,
    unsigned char * /*pattern*/,
    unsigned int /*length*/,
    unsigned int /*max_names*/
#endif
);

extern int doPolyText(
#if NeedFunctionPrototypes
    ClientPtr /*client*/,
    PTclosurePtr /*c*/
#endif
);

extern int PolyText(
#if NeedFunctionPrototypes
    ClientPtr /*client*/,
    DrawablePtr /*pDraw*/,
    GCPtr /*pGC*/,
    unsigned char * /*pElt*/,
    unsigned char * /*endReq*/,
    int /*xorg*/,
    int /*yorg*/,
    int /*reqType*/,
    XID /*did*/
#endif
);

extern int doImageText(
#if NeedFunctionPrototypes
    ClientPtr /*client*/,
    ITclosurePtr /*c*/
#endif
);

extern int ImageText(
#if NeedFunctionPrototypes
    ClientPtr /*client*/,
    DrawablePtr /*pDraw*/,
    GCPtr /*pGC*/,
    int /*nChars*/,
    unsigned char * /*data*/,
    int /*xorg*/,
    int /*yorg*/,
    int /*reqType*/,
    XID /*did*/
#endif
);

extern int SetFontPath(
#if NeedFunctionPrototypes
    ClientPtr /*client*/,
    int /*npaths*/,
    unsigned char * /*paths*/,
    int * /*error*/
#endif
);

extern int SetDefaultFontPath(
#if NeedFunctionPrototypes
    char * /*path*/
#endif
);

extern unsigned char *GetFontPath(
#if NeedFunctionPrototypes
    int * /*count*/,
    int * /*length*/
#endif
);

extern int LoadGlyphs(
#if NeedFunctionPrototypes
    ClientPtr /*client*/,
    FontPtr /*pfont*/,
    unsigned /*nchars*/,
    int /*item_size*/,
    unsigned char * /*data*/
#endif
);

extern void DeleteClientFontStuff(
#if NeedFunctionPrototypes
    ClientPtr /*client*/
#endif
);

extern void InitFonts(
#if NeedFunctionPrototypes
    void
#endif
);

extern int GetDefaultPointSize(
#if NeedFunctionPrototypes
    void
#endif
);

extern struct resolution *GetClientResolutions(
#if NeedFunctionPrototypes
    int * /*num*/
#endif
);

/* XXX leave these unprototyped for now -- need to do groundwork in 
 * fonts/include/fontstruct.h before we do these
 */
extern int RegisterFPEFunctions(
#if NeedFunctionPrototypes
    Bool (* /*name_func*/)(),
    int (* /*init_func*/)(),
    int (* /*free_func*/)(),
    int (* /*reset_func*/)(),
    int (* /*open_func*/)(),
    int (* /*close_func*/)(),
    int (* /*list_func*/)(),
    int (* /*start_lfwi_func*/)(),
    int (* /*next_lfwi_func*/)(),
#ifndef XTHREADS
    int (* /*wakeup_func*/)(),
#endif /* XTHREADS */
    int (* /*client_died*/)(),
    int (* /*load_glyphs*/)(),
    int (* /*start_list_alias_func*/)(),
    int (* /*next_list_alias_func*/)(),
    void (* /* set_path_func*/) ()
#endif
);

extern void FreeFonts(
#if NeedFunctionPrototypes
    void
#endif
);

extern FontPtr find_old_font(
#if NeedFunctionPrototypes
    XID /*id*/
#endif
);

extern Font GetNewFontClientID(
#if NeedFunctionPrototypes
    void
#endif
);

extern int StoreFontClientFont(
#if NeedFunctionPrototypes
    FontPtr /*pfont*/,
    Font /*id*/
#endif
);

extern void DeleteFontClientID(
#if NeedFunctionPrototypes
    Font /*id*/
#endif
);

extern int client_auth_generation(
#if NeedFunctionPrototypes
    ClientPtr /*client*/
#endif
);

extern int init_fs_handlers(
#if NeedFunctionPrototypes
    FontPathElementPtr /*fpe*/,
    BlockHandlerProcPtr /*block_handler*/
#endif
);

extern void remove_fs_handlers(
#if NeedFunctionPrototypes
    FontPathElementPtr /*fpe*/,
    BlockHandlerProcPtr /*block_handler*/,
    Bool /*all*/
#endif
);

extern void GetGlyphs(
#if NeedFunctionPrototypes
    FontPtr     /*font*/,
    unsigned long /*count*/,
    unsigned char * /*chars*/,
    FontEncoding /*fontEncoding*/,
    unsigned long * /*glyphcount*/,
    CharInfoPtr * /*glyphs*/
#endif
);

extern void QueryGlyphExtents(
#if NeedFunctionPrototypes
    FontPtr     /*pFont*/,
    CharInfoPtr * /*charinfo*/,
    unsigned long /*count*/,
    ExtentInfoPtr /*info*/
#endif
);

extern Bool QueryTextExtents(
#if NeedFunctionPrototypes
    FontPtr     /*pFont*/,
    unsigned long /*count*/,
    unsigned char * /*chars*/,
    ExtentInfoPtr /*info*/
#endif
);

extern Bool ParseGlyphCachingMode(
#if NeedFunctionPrototypes
    char * /*str*/
#endif
);

extern void InitGlyphCaching(
#if NeedFunctionPrototypes
    void
#endif
);

extern void SetGlyphCachingMode(
#if NeedFunctionPrototypes
    int /*newmode*/
#endif
);

void
ResetFontPrivateIndex(
#if NeedFunctionPrototypes
    void
#endif
);

#endif				/* DIXFONT_H */
