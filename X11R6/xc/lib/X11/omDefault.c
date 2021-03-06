/* $XConsortium: omDefault.c,v 1.2 94/03/28 21:38:48 rws Exp $ */
/*
 * Copyright 1992, 1993 by TOSHIBA Corp.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of TOSHIBA not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission. TOSHIBA make no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * TOSHIBA DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
 * TOSHIBA BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
 * ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 *
 * Author: Katsuhisa Yano	TOSHIBA Corp.
 *			   	mopi@osa.ilab.toshiba.co.jp
 */

#include "Xlibint.h"
#include "XomGeneric.h"
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <stdio.h>

#define DefineLocalBuf		char local_buf[BUFSIZ]
#define AllocLocalBuf(length)	(length > BUFSIZ ? (char *)Xmalloc(length) : local_buf)
#define FreeLocalBuf(ptr)	if (ptr != local_buf) Xfree(ptr)

static Bool
wcs_to_mbs(oc, to, from, length)
    XOC oc;
    char *to;
    wchar_t *from;
    int length;
{
    XlcConv conv = XOC_GENERIC(oc)->wcs_to_cs;
    XLCd lcd;
    int ret, to_left = length;

    if (conv == NULL) {
	lcd = oc->core.om->core.lcd;
	conv = _XlcOpenConverter(lcd, XlcNWideChar, lcd, XlcNMultiByte);
	if (conv == NULL)
	    return False;
	XOC_GENERIC(oc)->wcs_to_cs = conv;
    } else
	_XlcResetConverter(conv);

    ret = _XlcConvert(conv, (XPointer *) &from, &length, (XPointer *) &to,
		      &to_left, NULL, 0);
    if (ret != 0 || length > 0)
	return False;
    
    return True;
}

int
#if NeedFunctionPrototypes
_XmbDefalutTextEscapement(XOC oc, _Xconst char *text, int length)
#else
_XmbDefalutTextEscapement(oc, text, length)
    XOC oc;
    char *text;
    int length;
#endif
{
    return XTextWidth(*oc->core.font_info.font_struct_list, text, length);
}

int
#if NeedFunctionPrototypes
_XwcDefalutTextEscapement(XOC oc, _Xconst wchar_t *text, int length)
#else
_XwcDefalutTextEscapement(oc, text, length)
    XOC oc;
    wchar_t *text;
    int length;
#endif
{
    DefineLocalBuf;
    char *buf = AllocLocalBuf(length);
    int ret;

    if (buf == NULL)
	return 0;

    if (wcs_to_mbs(oc, buf, text, length) == False)
	goto err;

    ret = _XmbDefalutTextEscapement(oc, buf, length);

err:
    FreeLocalBuf(buf);

    return ret;
}

int
#if NeedFunctionPrototypes
_XmbDefalutTextExtents(XOC oc, _Xconst char *text, int length,
		       XRectangle *overall_ink, XRectangle *overall_logical)
#else
_XmbDefalutTextExtents(oc, text, length, overall_ink, overall_logical)
    XOC oc;
    char *text;
    int length;
    XRectangle *overall_ink;
    XRectangle *overall_logical;
#endif
{
    int direction, logical_ascent, logical_descent;
    XCharStruct overall;

    XTextExtents(*oc->core.font_info.font_struct_list, text, length, &direction,
		 &logical_ascent, &logical_descent, &overall);

    if (overall_ink) {
	overall_ink->x = overall.lbearing;
	overall_ink->y = -(overall.ascent);
	overall_ink->width = overall.rbearing - overall.lbearing;
	overall_ink->height = overall.ascent + overall.descent;
    }

    if (overall_logical) {
	overall_logical->x = 0;
        overall_logical->y = -(logical_ascent);
	overall_logical->width = overall.width;
        overall_logical->height = logical_ascent + logical_descent;
    }

    return overall.width;
}

int
#if NeedFunctionPrototypes
_XwcDefalutTextExtents(XOC oc, _Xconst wchar_t *text, int length,
		       XRectangle *overall_ink, XRectangle *overall_logical)
#else
_XwcDefalutTextExtents(oc, text, length, overall_ink, overall_logical)
    XOC oc;
    wchar_t *text;
    int length;
    XRectangle *overall_ink;
    XRectangle *overall_logical;
#endif
{
    DefineLocalBuf;
    char *buf = AllocLocalBuf(length);
    int ret;

    if (buf == NULL)
	return 0;

    if (wcs_to_mbs(oc, buf, text, length) == False)
	goto err;

    ret = _XmbDefalutTextExtents(oc, buf, length, overall_ink, overall_logical);

err:
    FreeLocalBuf(buf);

    return ret;
}

Status
#if NeedFunctionPrototypes
_XmbDefalutTextPerCharExtents(XOC oc, _Xconst char *text, int length,
			      XRectangle *ink_buf, XRectangle *logical_buf,
			      int buf_size, int *num_chars,
			      XRectangle *overall_ink,
			      XRectangle *overall_logical)
#else
_XmbDefalutTextPerCharExtents(oc, text, length, ink_buf, logical_buf, buf_size,
			      num_chars, overall_ink, overall_logical)
    XOC oc;
    char *text;	
    int length;
    XRectangle *ink_buf;
    XRectangle *logical_buf;
    int buf_size;
    int *num_chars;
    XRectangle *overall_ink;
    XRectangle *overall_logical;
#endif
{
    XFontStruct *font = *oc->core.font_info.font_struct_list;
    XCharStruct *def, *cs, overall;
    Bool first = True;

    *num_chars = length;

    if (buf_size < length)
	return 0;

    bzero((char *) &overall, sizeof(XCharStruct));

    CI_GET_DEFAULT_INFO_1D(font, def)

    while (length-- > 0) {
	CI_GET_CHAR_INFO_1D(font, *text, def, cs)
	text++;
	if (cs == NULL)
	    continue;

	ink_buf->x = overall.width + cs->lbearing;
	ink_buf->y = -(cs->ascent);
	ink_buf->width = cs->rbearing - cs->lbearing;
	ink_buf->height = cs->ascent + cs->descent;
	ink_buf++;

	logical_buf->x = overall.width;
	logical_buf->y = -(font->ascent);
	logical_buf->width = cs->width;
	logical_buf->height = font->ascent + font->descent;
	logical_buf++;

	if (first) {
	    overall = *cs;
	    first = False;
	} else {
	    overall.ascent = max(overall.ascent, cs->ascent);
	    overall.descent = max(overall.descent, cs->descent);
	    overall.lbearing = min(overall.lbearing, overall.width +
				   cs->lbearing);
	    overall.rbearing = max(overall.rbearing, overall.width +
				   cs->rbearing);
	    overall.width += cs->width;
	}
    }

    if (overall_ink) {
	overall_ink->x = overall.lbearing;
	overall_ink->y = -(overall.ascent);
	overall_ink->width = overall.rbearing - overall.lbearing;
	overall_ink->height = overall.ascent + overall.descent;
    }

    if (overall_logical) {
	overall_logical->x = 0;
	overall_logical->y = -(font->ascent);
	overall_logical->width = overall.width;
	overall_logical->height = font->ascent + font->descent;
    }

    return 1;
}

Status
#if NeedFunctionPrototypes
_XwcDefalutTextPerCharExtents(XOC oc, _Xconst wchar_t *text, int length,
			      XRectangle *ink_buf, XRectangle *logical_buf,
			      int buf_size, int *num_chars,
			      XRectangle *overall_ink,
			      XRectangle *overall_logical)
#else
_XwcDefalutTextPerCharExtents(oc, text, length, ink_buf, logical_buf, buf_size,
			      num_chars, overall_ink, overall_logical)
    XOC oc;
    wchar_t *text;
    int length;
    XRectangle *ink_buf;
    XRectangle *logical_buf;
    int buf_size;
    int *num_chars;
    XRectangle *overall_ink;
    XRectangle *overall_logical;
#endif
{
    DefineLocalBuf;
    char *buf = AllocLocalBuf(length);
    Status ret;

    if (buf == NULL)
	return 0;

    if (wcs_to_mbs(oc, buf, text, length) == False)
	goto err;

    ret = _XmbDefalutTextPerCharExtents(oc, buf, length, ink_buf, logical_buf,
					buf_size, num_chars, overall_ink,
					overall_logical);

err:
    FreeLocalBuf(buf);

    return ret;
}

int
#if NeedFunctionPrototypes
_XmbDefalutDrawString(Display *dpy, Drawable d, XOC oc, GC gc, int x, int y,
		      _Xconst char *text, int length)
#else
_XmbDefalutDrawString(dpy, d, oc, gc, x, y, text, length)
    Display *dpy;
    Drawable d;
    XOC oc;
    GC gc;
    int x, y;
    char *text;
    int length;
#endif
{
    XFontStruct *font = *oc->core.font_info.font_struct_list;

    XSetFont(dpy, gc, font->fid);
    XDrawString(dpy, d, gc, x, y, text, length);

    return XTextWidth(font, text, length);
}

int
#if NeedFunctionPrototypes
_XwcDefalutDrawString(Display *dpy, Drawable d, XOC oc, GC gc, int x, int y,
		      _Xconst wchar_t *text, int length)
#else
_XwcDefalutDrawString(dpy, d, oc, gc, x, y, text, length)
    Display *dpy;
    Drawable d;
    XOC oc;
    GC gc;
    int x, y;
    wchar_t *text;
    int length;
#endif
{
    DefineLocalBuf;
    char *buf = AllocLocalBuf(length);
    int ret;

    if (buf == NULL)
	return 0;

    if (wcs_to_mbs(oc, buf, text, length) == False)
	goto err;

    ret = _XmbDefalutDrawString(dpy, d, oc, gc, x, y, buf, length);

err:
    FreeLocalBuf(buf);

    return ret;
}

void
#if NeedFunctionPrototypes
_XmbDefalutDrawImageString(Display *dpy, Drawable d, XOC oc, GC gc, int x,
			   int y, _Xconst char *text, int length)
#else
_XmbDefalutDrawImageString(dpy, d, oc, gc, x, y, text, length)
    Display *dpy;
    Drawable d;
    XOC oc;
    GC gc;
    int x, y;
    char *text;
    int length;
#endif
{
    XSetFont(dpy, gc, (*oc->core.font_info.font_struct_list)->fid);
    XDrawImageString(dpy, d, gc, x, y, text, length);
}

void
#if NeedFunctionPrototypes
_XwcDefalutDrawImageString(Display *dpy, Drawable d, XOC oc, GC gc, int x,
			   int y, _Xconst wchar_t *text, int length)
#else
_XwcDefalutDrawImageString(dpy, d, oc, gc, x, y, text, length)
    Display *dpy;
    Drawable d;
    XOC oc;
    GC gc;
    int x, y;
    wchar_t *text;
    int length;
#endif
{
    DefineLocalBuf;
    char *buf = AllocLocalBuf(length);

    if (buf == NULL)
	return;

    if (wcs_to_mbs(oc, buf, text, length) == False)
	goto err;

    _XmbDefalutDrawImageString(dpy, d, oc, gc, x, y, buf, length);

err:
    FreeLocalBuf(buf);
}
