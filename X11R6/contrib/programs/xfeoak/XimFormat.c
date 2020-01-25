/* @(#)XimFormat.c      6.1 94/06/01 20:04:28 FUJITSU LIMITED. */
/*

Copyright (c) 1992, 1993, 1994  FUJITSU LIMITED

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE FUJITSU LIMITED BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the FUJITSU LIMITED shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from the FUJITSU LIMITED.

  Author: Takashi Fujiwara     FUJITSU LIMITED 
                               fujiwara@a80.tech.yk.fujitsu.co.jp
          Riki Kawaguchi       FUJITSU LIMITED
	  Toshimitu Terazono   FUJITSU LIMITED

*/

/*
 *  xfeoak : Input Server (kana-kanji covertion IM Server)
 */

#include <string.h>
#include <malloc.h>
#include "Xfeoak.h"
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#ifdef XV11R4
#define	XNQueryInputStyle		"queryInputStyle"
#define	XNInputStyle			"inputStyle"
#define	XNClientWindow			"clientWindow"
#define	XNFocusWindow			"focusWindow"
#define	XNFontSet			"fontSet"
#define	XNFilterEvents			"filterEvents"
#define	XNArea				"area"
#define	XNAreaNeeded			"areaNeeded"
#define	XNSpotLocation			"spotLocation"
#define	XNColormap			"colormap"
#define	XNStdColormap			"stdColormap"
#define	XNForeground			"foreground"
#define	XNBackground			"background"
#define	XNBackgroundPixmap		"backgroundPixmap"
#define	XNLineSpace			"lineSpace"
#define	XNCursor			"cursor"
#define	XNPreeditAttributes		"preeditAttributes"
#define	XNStatusAttributes		"statusAttributes"
#endif	/* XV11R4 */

#define	LOC(type, member)	((int)(&(((type *)0)->member)))

#define	XFE_NONAME		(Xfe_AttrName *)NULL
#define	XFE_NOMASK		(0L)
#define	XFE_NONEST		(Xfe_Attr)NULL
#define	XFE_NOENCODE		NULL
#define	XFE_NODECODE		NULL
#define	XFE_ATTR_END \
    {XFE_NONAME, 0, 0, 0, XFE_NOMASK, 0, XFE_NONEST, XFE_NOENCODE, XFE_NODECODE}

typedef struct	_Xfe_Attr	*Xfe_Attr;
Static INT16	 EncodeIMStyles(
#if NeedFunctionPrototypes
    Xfe_Attr, XPointer, XPointer
#endif
);
Static INT16	 EncodeIMOptions(
#if NeedFunctionPrototypes
    Xfe_Attr, XPointer, XPointer
#endif
);

Static INT16	 EncodeStyle(
#if NeedFunctionPrototypes
    Xfe_Attr, XPointer, XPointer
#endif
);
Static INT16	 Encode8(
#if NeedFunctionPrototypes
    Xfe_Attr, XPointer, XPointer
#endif
);
Static INT16	 Encode16(
#if NeedFunctionPrototypes
    Xfe_Attr, XPointer, XPointer
#endif
);
Static INT16	 Encode32(
#if NeedFunctionPrototypes
    Xfe_Attr, XPointer, XPointer
#endif
);
Static INT16	 EncodeStr(
#if NeedFunctionPrototypes
    Xfe_Attr, XPointer, XPointer
#endif
);
Static INT16	 EncodeWindow(
#if NeedFunctionPrototypes
    Xfe_Attr, XPointer, XPointer
#endif
);
Static INT16	 EncodeRect(
#if NeedFunctionPrototypes
    Xfe_Attr, XPointer, XPointer
#endif
);
Static INT16	 EncodePoint(
#if NeedFunctionPrototypes
    Xfe_Attr, XPointer, XPointer
#endif
);
Static INT16	 EncodeFontSet(
#if NeedFunctionPrototypes
    Xfe_Attr, XPointer, XPointer
#endif
);
#ifdef SUPPORT_STRING_CONV
Static INT16	 EncodeStringConv(
#if NeedFunctionPrototypes
    Xfe_Attr, XPointer, XPointer
#endif
);
#endif
#ifdef SUPPORT_HOTKEY
Static INT16	 EncodeHotKeyTriggers(
#if NeedFunctionPrototypes
    Xfe_Attr, XPointer, XPointer
#endif
);
Static INT16	 EncodeHotKeyState(
#if NeedFunctionPrototypes
    Xfe_Attr, XPointer, XPointer
#endif
);
#endif

Static Bool	 DecodeStyle(
#if NeedFunctionPrototypes
    Xfe_Attr, XPointer, INT16, XPointer, Xfe_Bitmask *, Xfe_Bitmask *, Bool
#endif
);
Static Bool	 Decode8(
#if NeedFunctionPrototypes
    Xfe_Attr, XPointer, INT16, XPointer, Xfe_Bitmask *, Xfe_Bitmask *, Bool
#endif
);
Static Bool	 Decode16(
#if NeedFunctionPrototypes
    Xfe_Attr, XPointer, INT16, XPointer, Xfe_Bitmask *, Xfe_Bitmask *, Bool
#endif
);
Static Bool	 Decode32(
#if NeedFunctionPrototypes
    Xfe_Attr, XPointer, INT16, XPointer, Xfe_Bitmask *, Xfe_Bitmask *, Bool
#endif
);
Static Bool	 DecodeStr(
#if NeedFunctionPrototypes
    Xfe_Attr, XPointer, INT16, XPointer, Xfe_Bitmask *, Xfe_Bitmask *, Bool
#endif
);
Static Bool	 DecodeWindow(
#if NeedFunctionPrototypes
    Xfe_Attr, XPointer, INT16, XPointer, Xfe_Bitmask *, Xfe_Bitmask *, Bool
#endif
);
Static Bool	 DecodeRect(
#if NeedFunctionPrototypes
    Xfe_Attr, XPointer, INT16, XPointer, Xfe_Bitmask *, Xfe_Bitmask *, Bool
#endif
);
Static Bool	 DecodePoint(
#if NeedFunctionPrototypes
    Xfe_Attr, XPointer, INT16, XPointer, Xfe_Bitmask *, Xfe_Bitmask *, Bool
#endif
);
Static Bool	 DecodeFontSet(
#if NeedFunctionPrototypes
    Xfe_Attr, XPointer, INT16, XPointer, Xfe_Bitmask *, Xfe_Bitmask *, Bool
#endif
);
#ifdef SUPPORT_STRING_CONV
Static Bool	 DecodeStringConv(
#if NeedFunctionPrototypes
    Xfe_Attr, XPointer, INT16, XPointer, Xfe_Bitmask *, Xfe_Bitmask *, Bool
#endif
);
#endif
#ifdef SUPPORT_HOTKEY
Static Bool	 DecodeHotKeyTriggers(
#if NeedFunctionPrototypes
    Xfe_Attr, XPointer, INT16, XPointer, Xfe_Bitmask *, Xfe_Bitmask *, Bool
#endif
);
Static Bool	 DecodeHotKeyState(
#if NeedFunctionPrototypes
    Xfe_Attr, XPointer, INT16, XPointer, Xfe_Bitmask *, Xfe_Bitmask *, Bool
#endif
);
#endif
Static Bool	 DecodeNest(
#if NeedFunctionPrototypes
    Xfe_Attr, XPointer, INT16, XPointer, Xfe_Bitmask *, Xfe_Bitmask *, Bool
#endif
);

typedef struct	_Xfe_AttrName {
    CARD16	attr_id;
    CARD16	pad;
    char	*name;
} Xfe_AttrName;

typedef struct	_Xfe_Attr {
    Xfe_AttrName	*name;
    CARD16		 value_type;
    CARD16		 pad;
    int			 loc;
    BITMASK32		 mask;
    int			 mask_idx;
    Xfe_Attr		 nest;
    INT16		(*encode)(
#if NeedFunctionPrototypes
    Xfe_Attr, XPointer, XPointer
#endif
				  );
    Bool		(*decode)(
#if NeedFunctionPrototypes
    Xfe_Attr, XPointer, INT16, XPointer, Xfe_Bitmask *, Xfe_Bitmask *, Bool
#endif
				  );
} Xfe_AttrRec;

#define XFE_IS_NEST(tbl)	((tbl)->value_type == XimType_NEST)

static Xfe_AttrName	XANQueryInputStyle    = {0, 0, XNQueryInputStyle};

static Xfe_AttrName	XANArea		      = {0, 0, XNArea};
static Xfe_AttrName	XANAreaNeeded	      = {0, 0, XNAreaNeeded};
static Xfe_AttrName	XANSpotLocation	      = {0, 0, XNSpotLocation};
static Xfe_AttrName	XANColormap	      = {0, 0, XNColormap};
static Xfe_AttrName	XANStdColormap	      = {0, 0, XNStdColormap};
static Xfe_AttrName	XANForeground	      = {0, 0, XNForeground};
static Xfe_AttrName	XANBackground	      = {0, 0, XNBackground};
static Xfe_AttrName	XANBackgroundPixmap   = {0, 0, XNBackgroundPixmap};
static Xfe_AttrName	XANFontSet	      = {0, 0, XNFontSet};
static Xfe_AttrName	XANLineSpace	      = {0, 0, XNLineSpace};
static Xfe_AttrName	XANCursor	      = {0, 0, XNCursor};
static Xfe_AttrName	XANPreeditState	      = {0, 0, XNPreeditState};

static Xfe_AttrName	XANInputStyle	      = {0, 0, XNInputStyle};
static Xfe_AttrName	XANClientWindow	      = {0, 0, XNClientWindow};
static Xfe_AttrName	XANFocusWindow	      = {0, 0, XNFocusWindow};
static Xfe_AttrName	XANFilterEvents	      = {0, 0, XNFilterEvents};
static Xfe_AttrName	XANResetState	      = {0, 0, XNResetState};
#ifdef SUPPORT_STRING_CONV
static Xfe_AttrName	XANStringConversion   = {0, 0, XNStringConversion};
#endif
#ifdef SUPPORT_HOTKEY
static Xfe_AttrName	XANHotKey	      = {0, 0, XNHotKey};
static Xfe_AttrName	XANHotKeyState	      = {0, 0, XNHotKeyState};
#endif

static Xfe_AttrName	XANPreeditAttributes  = {0, 0, XNPreeditAttributes};
static Xfe_AttrName	XANStatusAttributes   = {0, 0, XNStatusAttributes};
static Xfe_AttrName	XANSeparatorofNestedList
					      = {0, 0, XNSeparatorofNestedList};

static Xfe_AttrRec		im_attr[] = {
    {&XANQueryInputStyle, XimType_XIMStyles, 0, LOC(Xfe_IMAttr, styles),
		XFE_QUERY_INPUT_STYLES_MASK, XFE_TOP_MASK_IDX, XFE_NONEST,
		EncodeIMStyles, XFE_NODECODE},

    XFE_ATTR_END
};

static Xfe_AttrRec		ic_attr_pre_area[] = {
   {&XANArea, XimType_XRectangle, 0, LOC(Xfe_ICAttr, preedit.Area),
		XFE_PRE_AREA_MASK, XFE_PRE_MASK_IDX, XFE_NONEST,
		EncodeRect, DecodeRect},

   {&XANAreaNeeded, XimType_XRectangle, 0, LOC(Xfe_ICAttr, preedit.AreaNeeded),
		XFE_PRE_AREANEED_MASK, XFE_PRE_MASK_IDX, XFE_NONEST,
		EncodeRect, DecodeRect},

   {&XANSpotLocation, XimType_XPoint, 0, LOC(Xfe_ICAttr, preedit.SpotLocation),
		XFE_PRE_SPOTL_MASK, XFE_PRE_MASK_IDX, XFE_NONEST,
		EncodePoint, DecodePoint},

   {&XANColormap, XimType_CARD32, 0, LOC(Xfe_ICAttr, preedit.colormap),
		XFE_PRE_COLORMAP_MASK, XFE_PRE_MASK_IDX, XFE_NONEST,
		Encode32, Decode32},

   {&XANStdColormap, XimType_CARD32, 0, LOC(Xfe_ICAttr, preedit.StdColormap),
		XFE_PRE_STDCOLORMAP_MASK, XFE_PRE_MASK_IDX, XFE_NONEST,
		Encode32, Decode32},

   {&XANForeground, XimType_CARD32, 0, LOC(Xfe_ICAttr, preedit.Foreground),
		XFE_PRE_FG_MASK, XFE_PRE_MASK_IDX, XFE_NONEST,
		Encode32, Decode32},

   {&XANBackground, XimType_CARD32, 0, LOC(Xfe_ICAttr, preedit.Background),
		XFE_PRE_BG_MASK, XFE_PRE_MASK_IDX, XFE_NONEST,
		Encode32, Decode32},

   {&XANBackgroundPixmap, XimType_CARD32, 0, LOC(Xfe_ICAttr, preedit.Bg_Pixmap),
		XFE_PRE_BGPIXMAP_MASK, XFE_PRE_MASK_IDX, XFE_NONEST,
		Encode32, Decode32},

   {&XANFontSet, XimType_XFontSet, 0, LOC(Xfe_ICAttr, preedit.FontName),
		XFE_PRE_FONT_MASK, XFE_PRE_MASK_IDX, XFE_NONEST,
		EncodeFontSet, DecodeFontSet},

   {&XANLineSpace, XimType_CARD32, 0, LOC(Xfe_ICAttr, preedit.LineSpacing),
		XFE_PRE_LINESP_MASK, XFE_PRE_MASK_IDX, XFE_NONEST,
		Encode32, Decode32},

   {&XANCursor, XimType_CARD32, 0, LOC(Xfe_ICAttr, preedit.cursor),
		XFE_PRE_CURSOR_MASK, XFE_PRE_MASK_IDX, XFE_NONEST,
		Encode32, Decode32},

   {&XANPreeditState, XimType_CARD32, 0, LOC(Xfe_ICAttr, preedit.preeditState),
		XFE_PRE_PREEDITSTATE_MASK, XFE_PRE_MASK_IDX, XFE_NONEST,
		Encode32, Decode32},

   {&XANSeparatorofNestedList, XimType_SeparatorOfNestedList, 0, 0,
		XFE_NOMASK, XFE_TOP_MASK_IDX, XFE_NONEST,
		XFE_NOENCODE, XFE_NODECODE},

    XFE_ATTR_END
};

static Xfe_AttrRec		ic_attr_sts_area[] = {
   {&XANArea, XimType_XRectangle, 0, LOC(Xfe_ICAttr, status.Area),
		XFE_STS_AREA_MASK, XFE_STS_MASK_IDX, XFE_NONEST,
		EncodeRect, DecodeRect},

   {&XANAreaNeeded, XimType_XRectangle, 0, LOC(Xfe_ICAttr, status.AreaNeeded),
		XFE_STS_AREANEED_MASK, XFE_STS_MASK_IDX, XFE_NONEST,
		EncodeRect, DecodeRect},

   {&XANColormap, XimType_CARD32, 0, LOC(Xfe_ICAttr, status.colormap),
		XFE_STS_COLORMAP_MASK, XFE_STS_MASK_IDX, XFE_NONEST,
		Encode32, Decode32},

   {&XANStdColormap, XimType_CARD32, 0, LOC(Xfe_ICAttr, status.StdColormap),
		XFE_STS_STDCOLORMAP_MASK, XFE_STS_MASK_IDX, XFE_NONEST,
		Encode32, Decode32},

   {&XANForeground, XimType_CARD32, 0, LOC(Xfe_ICAttr, status.Foreground),
		XFE_STS_FG_MASK, XFE_STS_MASK_IDX, XFE_NONEST,
		Encode32, Decode32},

   {&XANBackground, XimType_CARD32, 0, LOC(Xfe_ICAttr, status.Background),
		XFE_STS_BG_MASK, XFE_STS_MASK_IDX, XFE_NONEST,
		Encode32, Decode32},

   {&XANBackgroundPixmap, XimType_CARD32, 0, LOC(Xfe_ICAttr, status.Bg_Pixmap),
		XFE_STS_BGPIXMAP_MASK, XFE_STS_MASK_IDX, XFE_NONEST,
		Encode32, Decode32},

   {&XANFontSet, XimType_XFontSet, 0, LOC(Xfe_ICAttr, status.FontName),
		XFE_STS_FONT_MASK, XFE_STS_MASK_IDX, XFE_NONEST,
		EncodeFontSet, DecodeFontSet},

   {&XANLineSpace, XimType_CARD32, 0, LOC(Xfe_ICAttr, status.LineSpacing),
		XFE_STS_LINESP_MASK, XFE_STS_MASK_IDX, XFE_NONEST,
		Encode32, Decode32},

   {&XANCursor, XimType_CARD32, 0, LOC(Xfe_ICAttr, status.cursor),
		XFE_STS_CURSOR_MASK, XFE_STS_MASK_IDX, XFE_NONEST,
		Encode32, Decode32},

   {&XANSeparatorofNestedList, XimType_SeparatorOfNestedList, 0, 0,
		XFE_NOMASK, XFE_TOP_MASK_IDX, XFE_NONEST,
		XFE_NOENCODE, XFE_NODECODE},

    XFE_ATTR_END
};

static Xfe_AttrRec		ic_attr[] = {
   {&XANInputStyle, XimType_CARD32, 0, LOC(Xfe_ICAttr, style),
		XFE_INPUT_STYLE_MASK, XFE_TOP_MASK_IDX, XFE_NONEST,
		EncodeStyle, DecodeStyle},

   {&XANClientWindow, XimType_Window, 0, LOC(Xfe_ICAttr, client_window),
		XFE_CLIENT_WIN_MASK, XFE_TOP_MASK_IDX, XFE_NONEST,
		EncodeWindow, DecodeWindow},

   {&XANFocusWindow, XimType_Window, 0, LOC(Xfe_ICAttr, focus_window),
		XFE_FOCUS_WIN_MASK, XFE_TOP_MASK_IDX, XFE_NONEST,
		EncodeWindow, DecodeWindow},

   {&XANFilterEvents, XimType_CARD32, 0, LOC(Xfe_ICAttr, filter_events),
		XFE_FILTER_EVENTS_MASK, XFE_TOP_MASK_IDX, XFE_NONEST,
		Encode32, Decode32},

   {&XANResetState, XimType_CARD32, 0, LOC(Xfe_ICAttr, reset_state),
		XFE_RESET_STATE_MASK, XFE_TOP_MASK_IDX, XFE_NONEST,
		Encode32, Decode32},

#ifdef SUPPORT_STRING_CONV
   {&XANStringConversion, XimType_XIMStringConversion, 0,
						LOC(Xfe_ICAttr, string_conv),
		XFE_STRING_CONV_MASK, XFE_TOP_MASK_IDX, XFE_NONEST,
		EncodeStringConv, DecodeStringConv},
#endif

#ifdef SUPPORT_HOTKEY
   {&XANHotKey, XimType_HotKetTriggers, 0, LOC(Xfe_ICAttr, hotkey),
		XFE_HOTKEY_MASK, XFE_TOP_MASK_IDX, XFE_NONEST,
		EncodeHotKeyTriggers, DecodeHotKeyTriggers},

   {&XANHotKey, XimType_XIMHotKeyState, 0, LOC(Xfe_ICAttr, hotkey_state),
		XFE_HOTKEY_STATE_MASK, XFE_TOP_MASK_IDX, XFE_NONEST,
		EncodeHotKeyState, DecodeHotKeyState},
#endif

   {&XANPreeditAttributes, XimType_NEST, 0, LOC(Xfe_ICAttr, preedit),
		XFE_PREEDIT_MASK, XFE_TOP_MASK_IDX, ic_attr_pre_area,
		XFE_NOENCODE, DecodeNest},

   {&XANStatusAttributes, XimType_NEST, 0, LOC(Xfe_ICAttr, status),
		XFE_STATUS_MASK, XFE_TOP_MASK_IDX, ic_attr_sts_area,
		XFE_NOENCODE, DecodeNest},

    XFE_ATTR_END
};

Static INT16
CreateAttrList(table, n, list, num)
    Xfe_Attr		 table;
    CARD16		 n;
    CARD16		*list;
    INT16		*num;
{
    INT16		 length;
    INT16		 total = 0;
    INT16		 new_num;
    register int	 i;

    *num = 0;
    for (i = 0; table[i].name; i++) {
	if (table[i].name->attr_id)
		continue;

	table[i].name->attr_id = n;
	length = strlen(table[i].name->name);
	list[0] = n;					/* attribute_id */
	list[1] = table[i].value_type;			/* type of value */
	list[2] = length;				/* length of attr */
	(void)strcpy((char *)&list[3], table[i].name->name);
							/* name of attribute */
	length += sizeof(CARD16);			/* sizeof length */
	XIM_SET_PAD(&list[2], length);			/* pad */

	length	+= sizeof(CARD16)			/* sizeof type */
		 + sizeof(CARD16);			/* sizeof length */

	list = (CARD16 *)((char *)list + length);
	(*num)++;
	n++;
	total += length;
	if (XFE_IS_NEST(&table[i])) {
	    length = CreateAttrList(table[i].nest, n, list, &new_num);
	    list = (CARD16 *)((char *)list + length);
	    (*num) += new_num;
	    n += new_num;
	    total += length;
	}
    }
    return total;
}

INT16
CreateIMAttrList(list, num)
    CARD16		*list;
    INT16		*num;
{
    return CreateAttrList(im_attr, 100, list, num);
}

INT16
CreateICAttrList(list, num)
    CARD16		*list;
    INT16		*num;
{
    return CreateAttrList(ic_attr, 200, list, num);
}

Static INT16
EncodeTagByAttr(attribute, buf, top)
    Xfe_Attr		 attribute;
    CARD16		*buf;
    XPointer		 top;
{
    INT16		 length;

    if (attribute->value_type == XimType_SeparatorOfNestedList)
	return -1;

    if (!attribute->encode) {
	WarningMessage(131, attribute->name->attr_id);
	return 0;
    }

    length = (*attribute->encode)(attribute, (XPointer)&buf[2], top);
						/* value */
    buf[0] = XfeCard16(attribute->name->attr_id);
						/* attribute id */
    buf[1] = XfeCard16(length);			/* value length */
    XIM_SET_PAD(&buf[2], length);		/* pad */
    length += sizeof(CARD16)			/* sizeof attribute id */
	    + sizeof(CARD16);			/* sizeof value length */

    return length;
}

Static Xfe_Attr
GetTagByID(attribute, id)
    Xfe_Attr		 attribute;
    CARD16		 id;
{
    register int	 i;

    for (i = 0; attribute[i].name; i++) {
	if (attribute[i].name->attr_id == id)
	    return &attribute[i];
    }
    return (Xfe_Attr)NULL;
}

Static INT16
EncodeTagByIDList(attribute, id_list, len, num_ret, buf, top)
    Xfe_Attr		 attribute;
    CARD16		*id_list;
    INT16		 len;
    INT16		*num_ret;
    CARD16		*buf;
    XPointer		 top;
{
    CARD16		 id;
    Xfe_Attr		 attr;
    INT16		 length;
    CARD16		*buffer = buf;
    INT16		 n = 0;
    INT16		 new_num_ret;
    INT16		 total = 0;

    while (len > 0) {
	id = XfeCard16(id_list[n]);			/* attribute-id */
	if (!(attr = GetTagByID(attribute, id))) {
	    WarningMessage(130, id);
	    n++;
	    len -= sizeof(CARD16);
	    continue;
	}

	if (XFE_IS_NEST(attr)) {
	    length = EncodeTagByIDList(attr->nest, &id_list[n + 1], len,
				&new_num_ret, &buffer[2], top);
						/* value */
	    buffer[0] = XfeCard16(id);		/* attribute-id */
	    buffer[1] = XfeCard16(length);	/* value length */
	    XIM_SET_PAD(&buffer[2], length);	/* pad */

	    length += sizeof(CARD16)		/* sizeof attribute */
		    + sizeof(INT16);		/* sizeof value length */

	    len -= sizeof(CARD16) * new_num_ret;
	    n += new_num_ret;
	} else {
	    if ((length = EncodeTagByAttr(attr, buffer, top)) < 0) {
		len -= sizeof(CARD16);
		n++;
		break;
	    }
	}
	len -= sizeof(CARD16);
	n++;
	buffer = (CARD16 *)((char *)buffer + length);
	total += length;
    }
    *num_ret = n;
    return total;
}

INT16
XimEncodeIMValues(id_list, len, buf, im_area)
    CARD16		*id_list;
    INT16		 len;
    CARD16		*buf;
    Xfe_IMAttr		*im_area;
{
    INT16		 num_ret;

    return EncodeTagByIDList(im_attr, id_list, len, &num_ret,
						buf, (XPointer)im_area);
}

INT16
XimEncodeICValues(id_list, len, buf, ic_area)
    CARD16		*id_list;
    INT16		 len;
    CARD16		*buf;
    Xfe_ICAttr		*ic_area;
{
    INT16		 num_ret;

    return EncodeTagByIDList(ic_attr, id_list, len, &num_ret,
						buf, (XPointer)ic_area);
}

Static Bool
DecodeTag(attribute, buf, max_size, top, mask, check_mask, is_decode)
    Xfe_Attr		 attribute;
    CARD16		*buf;
    INT16		 max_size;
    XPointer		 top;
    Xfe_Bitmask		*mask;
    Xfe_Bitmask		*check_mask;
    Bool		 is_decode;
{
    CARD16		 id;
    INT16		 length;
    INT16		 size;
    Xfe_Attr		 attr;

    for (size = 0; size < max_size; ) {
	id     = XfeCard16(buf[0]);		/* attribute ID */
	length = XfeCard16(buf[1]);		/* value length */

	if (!(attr = GetTagByID(attribute, id)))
	    WarningMessage(132, id);
	else {
	    if (!attr->decode)
		WarningMessage(133, id);
	    else
		if (!((*attr->decode)(attr, (XPointer)&buf[2], length, top,
						mask, check_mask, is_decode)))
		    WarningMessage(134, id);
	}

	length += XIM_PAD(length);
	length += sizeof(CARD16)		/* sizeof attribute ID */
		+ sizeof(INT16);		/* sizeof value length */
	size += length;
	buf = (CARD16 *)((char *)buf + length);
    }
    return True;
}

void
XimCheckIMValues(buf, max_size, style, mask)
    CARD16		*buf;
    INT16		 max_size;
    XIMStyle		*style;
    Xfe_Bitmask		*mask;
{
    bzero(mask, sizeof(Xfe_Bitmask));
    (void)DecodeTag(im_attr, buf, max_size, (XPointer)style, mask, False);
    return;
}

void
XimDecodeIMValues(buf, max_size, style, mask)
    CARD16		*buf;
    INT16		 max_size;
    XIMStyle		*style;
    Xfe_Bitmask		*mask;
{
    bzero(mask, sizeof(Xfe_Bitmask));
    (void)DecodeTag(im_attr, buf, max_size, (XPointer)style, mask, True);
    return;
}

void
XimCheckICValues(buf, max_size, ic_area, mask)
    CARD16		*buf;
    INT16		 max_size;
    Xfe_ICAttr		*ic_area;
    Xfe_Bitmask		*mask;
{
    bzero(mask, sizeof(Xfe_Bitmask));
    (void)DecodeTag(ic_attr, buf, max_size, (XPointer)ic_area,
					mask, (Xfe_Bitmask *)NULL, False);
    return;
}

void
XimDecodeICValues(buf, max_size, ic_area, check_mask)
    CARD16		*buf;
    INT16		 max_size;
    Xfe_ICAttr		*ic_area;
    Xfe_Bitmask		*check_mask;
{
    Xfe_Bitmask		 mask;

    bzero(&mask, sizeof(Xfe_Bitmask));
    (void)DecodeTag(ic_attr, buf, max_size, (XPointer)ic_area,
						&mask, check_mask, True);
    if (check_mask)
	*check_mask = mask;
    return;
}

/*
 * encode functions
 */

Static INT16
#if NeedFunctionPrototypes
EncodeIMStyles(
    Xfe_Attr		 attribute,
    XPointer		 buf,
    XPointer		 top)
#else
EncodeIMStyles(attribute, buf, top)
    Xfe_Attr		 attribute;
    XPointer		 buf;
    XPointer		 top;
#endif
{
    XIMStyle		**in;
    CARD16		*out = (CARD16 *)buf;
    CARD32		*outStyle = (CARD32 *)&out[2];
    INT16		 len;
    register INT16	 i;

    in = (XIMStyle **)((char *)top + attribute->loc);
    for (i = 0; (*in)[i]; i++ )
	outStyle[i] = XfeCard32((*in)[i]);

    out[0] = XfeCard16(i);	/* number of XIMStyle list */
    out[1] = 0;			/* unused */
    len = sizeof(INT16)		/* sizeof number */
	+ sizeof(CARD16)	/* sizeof unused */
	+ sizeof(CARD32)	/* sizeof XIMStyle */
	* i;
    return len;
}

Static INT16
#if NeedFunctionPrototypes
EncodeStyle(
    Xfe_Attr		 attribute,
    XPointer		 buf,
    XPointer		 top)
#else
EncodeStyle(attribute, buf, top)
    Xfe_Attr		 attribute;
    XPointer		 buf;
    XPointer		 top;
#endif
{
    XIMStyle		*in;
    CARD32		*out = (CARD32 *)buf;

    in = (XIMStyle *)((char *)top + attribute->loc);
    *out = XfeCard32(*in);
    return sizeof(CARD32);
}

Static INT16
#if NeedFunctionPrototypes
Encode8(
    Xfe_Attr		 attribute,
    XPointer		 buf,
    XPointer		 top)
#else
Encode8(attribute, buf, top)
    Xfe_Attr		 attribute;
    XPointer		 buf;
    XPointer		 top;
#endif
{
    CARD8		*in;
    CARD8		*out = (CARD8 *)buf;

    in = (CARD8 *)((char *)top + attribute->loc);
    *out = *in;
    return sizeof(CARD8);
}

Static INT16
#if NeedFunctionPrototypes
Encode16(
    Xfe_Attr		 attribute,
    XPointer		 buf,
    XPointer		 top)
#else
Encode16(attribute, buf, top)
    Xfe_Attr		 attribute;
    XPointer		 buf;
    XPointer		 top;
#endif
{
    CARD16		*in;
    CARD16		*out = (CARD16 *)buf;

    in = (CARD16 *)((char *)top + attribute->loc);
    *out = XfeCard16(*in);
    return sizeof(CARD16);
}

Static INT16
#if NeedFunctionPrototypes
Encode32(
    Xfe_Attr		 attribute,
    XPointer		 buf,
    XPointer		 top)
#else
Encode32(attribute, buf, top)
    Xfe_Attr		 attribute;
    XPointer		 buf;
    XPointer		 top;
#endif
{
    CARD32		*in;
    CARD32		*out = (CARD32 *)buf;

    in = (CARD32 *)((char *)top + attribute->loc);
    *out = XfeCard32(*in);
    return sizeof(CARD32);
}

Static INT16
#if NeedFunctionPrototypes
EncodeStr(
    Xfe_Attr		 attribute,
    XPointer		 buf,
    XPointer		 top)
#else
EncodeStr(attribute, buf, top)
    Xfe_Attr		 attribute;
    XPointer		 buf;
    XPointer		 top;
#endif
{
    char		**in;
    char		*out = (char *)buf;
    INT16		 len;

    in = (char **)((int)top + attribute->loc);
    len = strlen(*in);
    (void)strcpy(out, *in);
    return  len;
}

Static INT16
#if NeedFunctionPrototypes
EncodeWindow(
    Xfe_Attr		 attribute,
    XPointer		 buf,
    XPointer		 top)
#else
EncodeWindow(attribute, buf, top)
    Xfe_Attr		 attribute;
    XPointer		 buf;
    XPointer		 top;
#endif
{
    Window		*in;
    CARD32		*out = (CARD32 *)buf;

    in = (Window *)((char *)top + attribute->loc);
    *out = XfeCard32(*in);
    return sizeof(CARD32);
}

Static INT16
#if NeedFunctionPrototypes
EncodeRect(
    Xfe_Attr		 attribute,
    XPointer		 buf,
    XPointer		 top)
#else
EncodeRect(attribute, buf, top)
    Xfe_Attr		 attribute;
    XPointer		 buf;
    XPointer		 top;
#endif
{
    XRectangle		*in;
    CARD16		*out = (CARD16 *)buf;
    int			 len;

    in = (XRectangle *)((char *)top + attribute->loc);
    out[0] = XfeCard16(in->x);		/* X */
    out[1] = XfeCard16(in->y);		/* Y */
    out[2] = XfeCard16(in->width);	/* width  */
    out[3] = XfeCard16(in->height);	/* height */
    len = sizeof(INT16)			/* sizeof X */
	+ sizeof(INT16)			/* sizeof Y */
	+ sizeof(CARD16)		/* sizeog width */
	+ sizeof(CARD16);		/* sizeog height */
    return len;
}

Static INT16
#if NeedFunctionPrototypes
EncodePoint(
    Xfe_Attr		 attribute,
    XPointer		 buf,
    XPointer		 top)
#else
EncodePoint(attribute, buf, top)
    Xfe_Attr		 attribute;
    XPointer		 buf;
    XPointer		 top;
#endif
{
    XPoint		*in;
    CARD16		*out = (CARD16 *)buf;
    INT16		 len;

    in = (XPoint *)((char *)top + attribute->loc);
    out[0] = XfeCard16(in->x);		/* X */
    out[1] = XfeCard16(in->y);		/* Y */
    len = sizeof(INT16)			/* sizeof X */
	+ sizeof(INT16);		/* sizeof Y */
    return len;
}

Static INT16
#if NeedFunctionPrototypes
EncodeFontSet(
    Xfe_Attr		 attribute,
    XPointer		 buf,
    XPointer		 top)
#else
EncodeFontSet(attribute, buf, top)
    Xfe_Attr		 attribute;
    XPointer		 buf;
    XPointer		 top;
#endif
{
    char		**in;
    CARD16		*out = (CARD16 *)buf;
    INT16		 len;

    in = (char **)((char *)top + attribute->loc);
    len = strlen(*in);
    (void)strcpy((char *)&out[1], *in);
				/* Base font name list */
    out[0] = XfeCard16(len);	/* length of Base font name list */
    len += sizeof(INT16);	/* sizeof length */
    return len;
}

#ifdef SUPPORT_STRING_CONV
Static INT16
#if NeedFunctionPrototypes
EncodeStringConv(
    Xfe_Attr		 attribute,
    XPointer		 buf,
    XPointer		 top)
#else
EncodeStringConv(attribute, buf, top)
    Xfe_Attr		 attribute;
    XPointer		 buf;
    XPointer		 top;
#endif
{
    /*
     * Not yet
     */
    return True;
}
#endif

#ifdef SUPPORT_HOTKEY
Static INT16
#if NeedFunctionPrototypes
EncodeHotKeyTriggers(
    Xfe_Attr		 attribute,
    XPointer		 buf,
    XPointer		 top)
#else
EncodeHotKeyTriggers(attribute, buf, top)
    Xfe_Attr		 attribute;
    XPointer		 buf;
    XPointer		 top;
#endif
{
    XIMHotKeyTriggers	*in;
    CARD32		*out = (CARD32 *)buf;
    CARD32		*key;
    INT32		 num;
    INT16		 len;
    register int	 i;

    in = (XIMHotKeyTriggers *)((char *)top + attribute->loc);
    num = (INT32)in->num_hot_key;
    key = (CARD32 *)&out[1];
    for (i = 0; i < num; i++, key += 3) {
	key[0] = XfeCard32(in->key[i].keysym);		/* keysym */
	key[1] = XfeCard32(in->key[i].modifier);	/* modifier */
	key[2] = XfeCard32(in->key[i].modifier_mask);	/* modifier mask */
    }
    out[0] = XfeCard16(num);				/* number of Options */

    len = sizeof(INT32)			/* sizeof number */
	+ (sizeof(CARD32)		/* sizeof keysym */
	+  sizeof(CARD32)		/* sizeof modifier */
	+  sizeof(CARD32))		/* sizeof modifier mask */
	* num;
    return len;
}

Static INT16
#if NeedFunctionPrototypes
EncodeHotKeyState(
    Xfe_Attr		 attribute,
    XPointer		 buf,
    XPointer		 top)
#else
EncodeHotKeyState(attribute, buf, top)
    Xfe_Attr		 attribute;
    XPointer		 buf;
    XPointer		 top;
#endif
{
    XIMHotKeyState	*in;
    CARD32		*out = (CARD32 *)buf;

    in = (XIMHotKeyState *)((char *)top + attribute->loc);
    *out = XfeCard32(*in);
    return sizeof(CARD32);
}
#endif

/*
 * decode functions
 */

Static Bool
#if NeedFunctionPrototypes
DecodeStyle(
    Xfe_Attr		 attribute,
    XPointer		 buf,
    INT16		 len,
    XPointer		 top,
    Xfe_Bitmask		*mask,
    Xfe_Bitmask		*check_mask,
    Bool		 is_decode)
#else
DecodeStyle(attribute, buf, len, top, mask, check_mask, is_decode)
    Xfe_Attr		 attribute;
    XPointer		 buf;
    INT16		 len;
    XPointer		 top;
    Xfe_Bitmask		*mask;
    Xfe_Bitmask		*check_mask;
    Bool		 is_decode;
#endif
{
    CARD32		*in = (CARD32 *)buf;
    XIMStyle	 	*out;
    INT16		 min_len = sizeof(CARD32);

    if (len < min_len)
	return False;

    mask->mask[attribute->mask_idx] |= attribute->mask;

    if (check_mask
     && (!(check_mask->mask[attribute->mask_idx] & attribute->mask)))
	return False;

    out = (XIMStyle *)((char *)top + attribute->loc);

    *out = (XIMStyle)XfeCard32(*in);
    return True;
}

Static Bool
#if NeedFunctionPrototypes
Decode8(
    Xfe_Attr		 attribute,
    XPointer		 buf,
    INT16		 len,
    XPointer		 top,
    Xfe_Bitmask		*mask,
    Xfe_Bitmask		*check_mask,
    Bool		 is_decode)
#else
Decode8(attribute, buf, len, top, mask, check_mask, is_decode)
    Xfe_Attr		 attribute;
    XPointer		 buf;
    INT16		 len;
    XPointer		 top;
    Xfe_Bitmask		*mask;
    Xfe_Bitmask		*check_mask;
    Bool		 is_decode;
#endif
{
    CARD8		*in = (CARD8 *)buf;
    CARD8		*out;
    INT16		 min_len = sizeof(CARD8);

    if (len < min_len)
	return False;

    mask->mask[attribute->mask_idx] |= attribute->mask;
    if (!is_decode)
	return True;

    if (!(check_mask->mask[attribute->mask_idx] & attribute->mask))
	return False;

    out = (CARD8 *)((char *)top + attribute->loc);
    *out = *in;
    return True;
}

Static Bool
#if NeedFunctionPrototypes
Decode16(
    Xfe_Attr		 attribute,
    XPointer		 buf,
    INT16		 len,
    XPointer		 top,
    Xfe_Bitmask		*mask,
    Xfe_Bitmask		*check_mask,
    Bool		 is_decode)
#else
Decode16(attribute, buf, len, top, mask, check_mask, is_decode)
    Xfe_Attr		 attribute;
    XPointer		 buf;
    INT16		 len;
    XPointer		 top;
    Xfe_Bitmask		*mask;
    Xfe_Bitmask		*check_mask;
    Bool		 is_decode;
#endif
{
    CARD16		*in = (CARD16 *)buf;
    CARD16		*out;
    INT16		 min_len = sizeof(CARD16);

    if (len < min_len)
	return False;

    mask->mask[attribute->mask_idx] |= attribute->mask;
    if (!is_decode)
	return True;

    if (!(check_mask->mask[attribute->mask_idx] & attribute->mask))
	return False;

    out = (CARD16 *)((char *)top + attribute->loc);
    *out = XfeCard16(*in);
    return True;
}

Static Bool
#if NeedFunctionPrototypes
Decode32(
    Xfe_Attr		 attribute,
    XPointer		 buf,
    INT16		 len,
    XPointer		 top,
    Xfe_Bitmask		*mask,
    Xfe_Bitmask		*check_mask,
    Bool		 is_decode)
#else
Decode32(attribute, buf, len, top, mask, check_mask, is_decode)
    Xfe_Attr		 attribute;
    XPointer		 buf;
    INT16		 len;
    XPointer		 top;
    Xfe_Bitmask		*mask;
    Xfe_Bitmask		*check_mask;
    Bool		 is_decode;
#endif
{
    CARD32		*in = (CARD32 *)buf;
    CARD32	 	*out;
    INT16		 min_len = sizeof(CARD32);

    if (len < min_len)
	return False;

    mask->mask[attribute->mask_idx] |= attribute->mask;
    if (!is_decode)
	return True;

    if (!(check_mask->mask[attribute->mask_idx] & attribute->mask))
	return False;

    out = (CARD32 *)((char *)top + attribute->loc);
    *out = XfeCard32(*in);
    return True;
}

Static Bool
#if NeedFunctionPrototypes
DecodeStr(
    Xfe_Attr		 attribute,
    XPointer		 buf,
    INT16		 len,
    XPointer		 top,
    Xfe_Bitmask		*mask,
    Xfe_Bitmask		*check_mask,
    Bool		 is_decode)
#else
DecodeStr(attribute, buf, len, top, mask, check_mask, is_decode)
    Xfe_Attr		 attribute;
    XPointer		 buf;
    INT16		 len;
    XPointer		 top;
    Xfe_Bitmask		*mask;
    Xfe_Bitmask		*check_mask;
    Bool		 is_decode;
#endif
{
    CARD16		*in = (CARD16 *)buf;
    unsigned char	**out;
    unsigned char	 *p;

    mask->mask[attribute->mask_idx] |= attribute->mask;
    if (!is_decode)
	return True;

    if (!(check_mask->mask[attribute->mask_idx] & attribute->mask))
	return False;

    out = (unsigned char **)((char *)top + attribute->loc);
    if (!*out)
	p = (unsigned char *)malloc(len + 1);
    else
	p = (unsigned char *)realloc(*out, (len + 1));
    if (!p)
	return False;
    (void)memcpy(p, in, len);
    p[len] = '\0';
    *out = p;
    return True;
}

Static Bool
#if NeedFunctionPrototypes
DecodeWindow(
    Xfe_Attr		 attribute,
    XPointer		 buf,
    INT16		 len,
    XPointer		 top,
    Xfe_Bitmask		*mask,
    Xfe_Bitmask		*check_mask,
    Bool		 is_decode)
#else
DecodeWindow(attribute, buf, len, top, mask, check_mask, is_decode)
    Xfe_Attr		 attribute;
    XPointer		 buf;
    INT16		 len;
    XPointer		 top;
    Xfe_Bitmask		*mask;
    Xfe_Bitmask		*check_mask;
    Bool		 is_decode;
#endif
{
    CARD32		*in = (CARD32 *)buf;
    Window	 	*out;
    INT16		 min_len = sizeof(CARD32);
    Xfe_ICAttr		*ic_area = (Xfe_ICAttr *)top;

    if (len < min_len)
	return False;

    mask->mask[attribute->mask_idx] |= attribute->mask;
    if (!is_decode)
	return True;

    if (!(check_mask->mask[attribute->mask_idx] & attribute->mask))
	return False;

    out = (Window *)((char *)top + attribute->loc);
    if ((out == &ic_area->client_window) && (*out))
	return False;	/* Don't change ClientWindow */

    *out = (Window)XfeCard32(*in);
    return True;
}

Static Bool
#if NeedFunctionPrototypes
DecodeRect(
    Xfe_Attr		 attribute,
    XPointer		 buf,
    INT16		 len,
    XPointer		 top,
    Xfe_Bitmask		*mask,
    Xfe_Bitmask		*check_mask,
    Bool		 is_decode)
#else
DecodeRect(attribute, buf, len, top, mask, check_mask, is_decode)
    Xfe_Attr		 attribute;
    XPointer		 buf;
    INT16		 len;
    XPointer		 top;
    Xfe_Bitmask		*mask;
    Xfe_Bitmask		*check_mask;
    Bool		 is_decode;
#endif
{
    CARD16		*in = (CARD16 *)buf;
    XRectangle		*out;
    INT16		 min_len = sizeof(INT16)	/* sizeof X */
				 + sizeof(INT16)	/* sizeof Y */
				 + sizeof(CARD16)	/* sizeof width */
				 + sizeof(CARD16);	/* sizeof height */

    if (len < min_len)
	return False;

    mask->mask[attribute->mask_idx] |= attribute->mask;
    if (!is_decode)
	return True;

    if (!(check_mask->mask[attribute->mask_idx] & attribute->mask))
	return False;

    out = (XRectangle *)((char *)top + attribute->loc);
    out->x	= (INT16)XfeCard16(in[0]);	/* X */
    out->y	= (INT16)XfeCard16(in[1]);	/* Y */
    out->width	= XfeCard16(in[2]);		/* width */
    out->height	= XfeCard16(in[3]);		/* height */
    return True;
}

Static Bool
#if NeedFunctionPrototypes
DecodePoint(
    Xfe_Attr		 attribute,
    XPointer		 buf,
    INT16		 len,
    XPointer		 top,
    Xfe_Bitmask		*mask,
    Xfe_Bitmask		*check_mask,
    Bool		 is_decode)
#else
DecodePoint(attribute, buf, len, top, mask, check_mask, is_decode)
    Xfe_Attr		 attribute;
    XPointer		 buf;
    INT16		 len;
    XPointer		 top;
    Xfe_Bitmask		*mask;
    Xfe_Bitmask		*check_mask;
    Bool		 is_decode;
#endif
{
    CARD16		*in = (CARD16 *)buf;
    XPoint		*out;
    INT16		 min_len = sizeof(INT16)	/* sizeof X */
				 + sizeof(INT16);	/* sizeof Y */

    if (len < min_len)
	return False;

    mask->mask[attribute->mask_idx] |= attribute->mask;
    if (!is_decode)
	return True;

    if (!(check_mask->mask[attribute->mask_idx] & attribute->mask))
	return False;

    out = (XPoint *)((char *)top + attribute->loc);
    out->x = (INT16)XfeCard16(in[0]);		/* X */
    out->y = (INT16)XfeCard16(in[1]);		/* Y */
    return True;
}

Static Bool
#if NeedFunctionPrototypes
DecodeFontSet(
    Xfe_Attr		 attribute,
    XPointer		 buf,
    INT16		 len,
    XPointer		 top,
    Xfe_Bitmask		*mask,
    Xfe_Bitmask		*check_mask,
    Bool		 is_decode)
#else
DecodeFontSet(attribute, buf, len, top, mask, check_mask, is_decode)
    Xfe_Attr		 attribute;
    XPointer		 buf;
    INT16		 len;
    XPointer		 top;
    Xfe_Bitmask		*mask;
    Xfe_Bitmask		*check_mask;
    Bool		 is_decode;
#endif
{
    CARD16		*in = (CARD16 *)buf;
    unsigned char	**out;
    unsigned char	 *p;
    INT16		 min_len = XfeCard16(in[0]);   /* length of font name */

    mask->mask[attribute->mask_idx] |= attribute->mask;
    if (!is_decode)
	return True;

    if (!(check_mask->mask[attribute->mask_idx] & attribute->mask))
	return False;

    out = (unsigned char **)((char *)top + attribute->loc);
    if (!*out)
	p = (unsigned char *)malloc(min_len + 1);
    else
	p = (unsigned char *)realloc(*out, (min_len + 1));
    if (!p)
	return False;
    (void)memcpy(p, &in[1], min_len);
    p[min_len] = '\0';
    *out = p;
    return True;
}

#ifdef SUPPORT_STRING_CONV
Static Bool
#if NeedFunctionPrototypes
DecodeStringConv(
    Xfe_Attr		 attribute,
    XPointer		 buf,
    INT16		 len,
    XPointer		 top,
    Xfe_Bitmask		*mask,
    Xfe_Bitmask		*check_mask,
    Bool		 is_decode)
#else
DecodeStringConv(attribute, buf, len, top, mask, check_mask, is_decode)
    Xfe_Attr		 attribute;
    XPointer		 buf;
    INT16		 len;
    XPointer		 top;
    Xfe_Bitmask		*mask;
    Xfe_Bitmask		*check_mask;
    Bool		 is_decode;
#endif
{
    /*
     * Not yet
     */
     return True;
}
#endif

#ifdef SUPPORT_HOTKEY
Static Bool
#if NeedFunctionPrototypes
DecodeHotKeyTriggers(
    Xfe_Attr		 attribute,
    XPointer		 buf,
    INT16		 len,
    XPointer		 top,
    Xfe_Bitmask		*mask,
    Xfe_Bitmask		*check_mask,
    Bool		 is_decode)
#else
DecodeHotKeyTriggers(attribute, buf, len, top, mask, check_mask, is_decode)
    Xfe_Attr		 attribute;
    XPointer		 buf;
    INT16		 len;
    XPointer		 top;
    Xfe_Bitmask		*mask;
    Xfe_Bitmask		*check_mask;
    Bool		 is_decode;
#endif
{
    CARD32		*in = (CARD32 *)buf;
    XIMHotKeyTriggers	*out;
    CARD32		*key;
    XIMHotKeyTrigger	*p;
    register int	 i;
    INT32		 num = XfeCard32(in[0]);     /* number of list */
    INT16		 min_len = sizeof(CARD32)    /* sizeof number */
				+ (sizeof(CARD32)    /* sizeof keysym */
				+  sizeof(CARD32)    /* sizeof modifier */
				+  sizeof(CARD32))   /* sizeof modifier mask */
				* num;

    if (len < min_len)
	return False;

    mask->mask[attribute->mask_idx] |= attribute->mask;
    if (!is_decode)
	return True;

    if (!(check_mask->mask[attribute->mask_idx] & attribute->mask))
	return False;

    out = (XIMHotKeyTriggers *)((char *)top + attribute->loc);

    if (!(p = (XIMHotKeyTrigger *)malloc(num * sizeof(XIMHotKeyTrigger))))
	return False;

    key = &in[1];
    for (i = 0; i < num; i++, key += 3) {
	p[i].keysym	   = XfeCard32(key[0]);	/* keysym */
	p[i].modifier	   = XfeCard32(key[1]);	/* modifier */
	p[i].modifier_mask = XfeCard32(key[2]);	/* modifier mask */
    }

    out->num_hot_key = (int)num;
    out->key	     = p;
    return True;
}

Static Bool
#if NeedFunctionPrototypes
DecodeHotKeyState(
    Xfe_Attr		 attribute,
    XPointer		 buf,
    INT16		 len,
    XPointer		 top,
    Xfe_Bitmask		*mask,
    Xfe_Bitmask		*check_mask,
    Bool		 is_decode)
#else
DecodeHotKeyState(attribute, buf, len, top, mask, check_mask, is_decode)
    Xfe_Attr		 attribute;
    XPointer		 buf;
    INT16		 len;
    XPointer		 top;
    Xfe_Bitmask		*mask;
    Xfe_Bitmask		*check_mask;
    Bool		 is_decode;
#endif
{
    CARD32		*in = (CARD32 *)buf;
    XIMHotKeyState	*out;
    INT16		 min_len = sizeof(CARD32);

    if (len < min_len)
	return False;

    mask->mask[attribute->mask_idx] |= attribute->mask;

    if (check_mask
     && (!(check_mask->mask[attribute->mask_idx] & attribute->mask)))
	return False;

    out = (XIMHotKeyState *)((char *)top + attribute->loc);

    *out = (XIMHotKeyState)XfeCard32(*in);
    return True;
}
#endif

Static Bool
#if NeedFunctionPrototypes
DecodeNest(
    Xfe_Attr		 attribute,
    XPointer		 buf,
    INT16		 len,
    XPointer		 top,
    Xfe_Bitmask		*mask,
    Xfe_Bitmask		*check_mask,
    Bool		 is_decode)
#else
DecodeNest(attribute, buf, len, top, mask, check_mask, is_decode)
    Xfe_Attr		 attribute;
    XPointer		 buf;
    INT16		 len;
    XPointer		 top;
    Xfe_Bitmask		*mask;
    Xfe_Bitmask		*check_mask;
    Bool		 is_decode;
#endif
{
    return DecodeTag(attribute->nest, (CARD16 *)buf, len, top, mask,
						check_mask, is_decode);
}
