/* $SonyId: XimcpP.h,v 1.2 1994/06/03 06:33:04 makoto Exp $ */
/******************************************************************

Copyright (c) 1993, 1994  Sony Corporation

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL SONY CORPORATION BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of Sony Corporation shall
not be used in advertising or otherwise to promote the sale, use or ot
her dealings in this Software without prior written authorization from
Sony Corporation.

******************************************************************/
#ifndef _XimcpP_H
#define _XimcpP_H
#include	"sjxa.h"
#include	"Ximcp.h"

#define _XIM_SERVERS			"XIM_SERVERS"
#define _XIM_TRANSPORT			"TRANSPORT"
#define _XIM_LOCALES			"LOCALES"

#define	_XIM_XCONNECT			"_XIM_XCONNECT"
#define _XIM_PROTOCOL			"_XIM_PROTOCOL"

#define _XIM_OPEN			"_XIM_OPEN"
#define _XIM_REGISTER_TRIGGERKEYS	"_XIM_REGISTER_TRIGGERKEYS"
#define _XIM_QUERY_EXTENSION		"_XIM_QUERY_EXTENSION"
#define _XIM_IM_VALUES			"_XIM_IM_VALUES"
#define _XIM_IC_VALUES			"_XIM_IC_VALUES"
#define _XIM_COMMIT			"_XIM_COMMIT"
#define _XIM_RESET			"_XIM_RESET"
#define _XIM_FORWARD_EVENT		"_XIM_FORWARD_EVENT"


/* Protocol number */
#define XIM_CONNECT			1
#define XIM_CONNECT_REPLY		2
#define XIM_DISCONNECT			3
#define XIM_DISCONNECT_REPLY		4

#define XIM_AUTH_REQUIRED		10
#define XIM_AUTH_REPLY			11
#define XIM_AUTH_NEXT			12
#define XIM_AUTH_SETUP			13
#define XIM_AUTH_NG			14

#define XIM_ERROR			20

#define XIM_OPEN			30
#define XIM_OPEN_REPLY			31
#define XIM_CLOSE			32
#define XIM_CLOSE_REPLY			33
#define XIM_REGISTER_TRIGGERKEYS	34
#define XIM_TRIGGER_NOTIFY		35
#define XIM_TRIGGER_NOTIFY_REPLY	36
#define XIM_SET_EVENT_MASK		37
#define XIM_ENCODING_NEGOTIATION	38
#define XIM_ENCODING_NEGOTIATION_REPLY	39
#define XIM_QUERY_EXTENSION		40
#define XIM_QUERY_EXTENSION_REPLY	41
#define XIM_SET_IM_VALUES		42
#define XIM_SET_IM_VALUES_REPLY		43
#define XIM_GET_IM_VALUES		44
#define XIM_GET_IM_VALUES_REPLY		45

#define XIM_CREATE_IC			50
#define XIM_CREATE_IC_REPLY		51
#define XIM_DESTROY_IC			52
#define XIM_DESTROY_IC_REPLY		53
#define XIM_SET_IC_VALUES		54
#define XIM_SET_IC_VALUES_REPLY		55
#define XIM_GET_IC_VALUES		56
#define XIM_GET_IC_VALUES_REPLY		57
#define XIM_SET_IC_FOCUS		58
#define XIM_UNSET_IC_FOCUS		59
#define XIM_FORWARD_EVENT		60
#define XIM_SYNC			61
#define XIM_SYNC_REPLY			62
#define XIM_COMMIT			63
#define XIM_RESET_IC			64
#define XIM_RESET_IC_REPLY		65

#define XIM_GEOMETRY			70
#define XIM_STR_CONVERSION		71
#define XIM_STR_CONVERSION_REPLY	72
#define XIM_PREEDIT_START		73
#define XIM_PREEDIT_START_REPLY		74
#define XIM_PREEDIT_DRAW		75
#define XIM_PREEDIT_CARET		76
#define XIM_PREEDIT_CARET_REPLY		77
#define XIM_PREEDIT_DONE		78

#define XIM_STATUS_START		79
#define XIM_STATUS_DRAW			80
#define XIM_STATUS_DONE			81

#define XIM_EXT_SET_EVENT_MASK		0x81
#define XIM_EXT_FORWARD_KEYEVENT	0x82
#define XIM_EXT_MOVE			0x84


/* IM Values */
#define XIM_QUERY_EXTENSIONS_LIST	1
#define XIM_QUERY_FUNCTIONS_LIST	2
#define XIM_QUERY_IC_ATTRIBUTES_LIST	3
#define XIM_QUERY_INPUT_STYLE		4
#define XIM_DESTROY_CALLBACK		5


/* IC Values */
#define XIC_INPUT_STYLE			0x0001
#define XIC_CLIENT_WINDOW		0x0002
#define XIC_FOCUS_WINDOW		0x0003
#define XIC_RESOURCE_NAME		0x0004
#define XIC_RESOURCE_CLASS		0x0005
#define XIC_FILTER_EVENTS		0x0006
#define XIC_PREEDIT_STATE		0x0007
#define XIC_RESET_STATE			0x0008
#define XIC_HOT_KEY			0x0009
#define XIC_HOT_KEY_STATE		0x000a
#define XIC_STRING_CONVERSION		0x000b
#define XIC_PREEDIT_ATTRIBUTES		0x000c
#define XIC_STATUS_ATTRIBUTES		0x000d

#define XIC_AREA			0x0100
#define XIC_AREA_NEEDED			0x0101
#define XIC_SPOT_LOCATION		0x0102
#define XIC_COLORMAP			0x0103
#define XIC_STD_COLORMAP		0x0104
#define XIC_FOREGROUND			0x0105
#define XIC_BACKGROUND			0x0106
#define XIC_BACKGROUND_PIXMAP		0x0107
#define XIC_FONTSET			0x0108
#define XIC_LINE_SPACING		0x0109
#define XIC_CURSOR			0x010a
#define XIC_SEPARATOR_OF_NESTED_LIST	0x010b

#define XIC_GEOMETRY_CALLBACK		0x0200
#define XIC_DESTROY_CALLBACK		0x0201
#define XIC_STRING_CONVERSION_CALLBACK	0x0202

#define XIC_PREEDIT_START_CALLBACK	0x0300
#define XIC_PREEDIT_DONE_CALLBACK	0x0301
#define XIC_PREEDIT_DRAW_CALLBACK	0x0302
#define XIC_PREEDIT_CARET_CALLBACK	0x0303

#define XIC_STATUS_START_CALLBACK	0x0400
#define XIC_STATUS_DONE_CALLBACK	0x0401
#define XIC_STATUS_DRAW_CALLBACK	0x0402

/* mask of IC Values */
#define _XIC_INPUT_STYLE		0x0002
#define _XIC_CLIENT_WINDOW		0x0004
#define _XIC_FOCUS_WINDOW		0x0008
#define _XIC_RESOURCE_NAME		0x0010
#define _XIC_RESOURCE_CLASS		0x0020
#define _XIC_FILTER_EVENTS		0x0040
#define _XIC_PREEDIT_STATE		0x0080
#define _XIC_RESET_STATE		0x0100
#define _XIC_HOT_KEY			0x0200
#define _XIC_HOT_KEY_STATE		0x0400
#define _XIC_STRING_CONVERSION		0x0800
#define _XIC_PREEDIT_ATTRIBUTES		0x1000
#define _XIC_STATUS_ATTRIBUTES		0x2000

#define _XIC_AREA			0x0001
#define _XIC_AREA_NEEDED		0x0002
#define _XIC_SPOT_LOCATION		0x0004
#define _XIC_COLORMAP			0x0008
#define _XIC_STD_COLORMAP		0x0010
#define _XIC_FOREGROUND			0x0020
#define _XIC_BACKGROUND			0x0040
#define _XIC_BACKGROUND_PIXMAP		0x0080
#define _XIC_FONTSET			0x0100
#define _XIC_LINE_SPACING		0x0200
#define _XIC_CURSOR			0x0400
#define _XIC_SEPARATOR_OF_NESTED_LIST	0x0800


/* value types */
#define XimRseparatorOfNestedList	0
#define XimRbyteData			1
#define XimRwordData			2
#define XimRlongData			3
#define XimRcharData			4
#define XimRwindow			5
#define XimRimStyles			10
#define XimRrectangle			11
#define XimRpoint			12
#define XimRfontSet			13
#define XimRimOptions			14
#define XimRimHotKeyTriggers		15
#define XimRimHotKeyState		16
#define XimRimStringConversion		17
#define XimRimPreeditState		18
#define XimRimResetState		19
#define XimRnestedList			0x7fff


/* Error code */
#define XIM_NO_ERROR			0
#define XIM_ERROR_BAD_ALLOC		1
#define XIM_ERROR_BAD_STYLE		2
#define XIM_ERROR_BAD_CLIENT_WINDOW	3
#define XIM_ERROR_BAD_FOCUS_WINDOW	4
#define XIM_ERROR_BAD_AREA		5
#define XIM_ERROR_BAD_SPOT_LOCATION	6
#define XIM_ERROR_BAD_COLORMAP		7
#define XIM_ERROR_BAD_ATOM		8
#define XIM_ERROR_BAD_PIXEL		9
#define XIM_ERROR_BAD_PIXMAP		10
#define XIM_ERROR_BAD_NAME		11
#define XIM_ERROR_BAD_CURSOR		12
#define XIM_ERROR_BAD_PROTOCOL		13
#define XIM_ERROR_BAD_FOREGROUND	14
#define XIM_ERROR_BAD_BACKGROUND	15
#define XIM_ERROR_LOCALE_NOT_SUPPORTED	16
#define XIM_ERROR_BAD_SOMETHING		17

#if XlibSpecificationRelease == 5
#define XNStringConversion	"stringConversion"
#define XNResetState		"resetState"
#define XNHotKey		"hotKey"
#define XNHotKeyState		"hotKeyState"
#define XNPreeditState		"preeditState"
#define XNSeparatorofNestedList	"separatorofNestedList"

#define XIMPreeditEnable	1L
#define XIMPreeditDisable	(1L<<1)

#define XIMInitialState		1L
#define XIMPreserveState	(1L<<1)

#define XIMHotKeyStateON	(0x0001L)
#define XIMHotKeyStateOFF	(0x0002L)

typedef struct _XIMHotKeyTrigger {
    KeySym	 keysym;
    int		 modifier;
    int		 modifier_mask;
} XIMHotKeyTrigger;

typedef struct _XIMHotKeyTriggers {
    int			 num_hot_key;
    XIMHotKeyTrigger	*key;
} XIMHotKeyTriggers;
#endif

/* structure */
typedef union _CARD {
    CARD8	*b;
    CARD16	*s;
    CARD32	*l;
} CARD;

typedef struct	_XimcpConnection {
    struct _XimcpConnection	*next;
    struct _XimcpConnection	*previous;
    Window			 ims_window;
    Window			 client_window;
    Endian			 endian;
    Boolean			 bug;
    struct _XimcpInputMethod	*xim;
} XimcpConnection;

typedef struct	_XimcpInputMethod {
    struct _XimcpInputMethod	*next;
    struct _XimcpInputMethod	*previous;
    struct _XimcpConnection	*xc;
    struct _XimcpInputContext	*xic;
    unsigned short		 imid;
    CARD8			 initialize;
    CARD8			 extension;
} XimcpInputMethod;

typedef struct _XimcpPreeditRec {
    CARD32			 mask;
    XRectangle			 area;
    XRectangle			 area_needed;
    XPoint			 spot_location;
    Colormap			 colormap;
    unsigned long		 foreground;
    unsigned long		 background;
    Pixmap			 pixmap;
    char			*font_set;
    int				 ascent;
    int				 line_spacing;
    Cursor			 cursor;
    unsigned long		 state;
} XimcpPreeditRec;

typedef struct _XimcpStatusRec {
    CARD32			 mask;
    XRectangle			 area;
    XRectangle			 area_needed;
    Colormap			 colormap;
    unsigned long		 foreground;
    unsigned long		 background;
    Pixmap			 pixmap;
    char			*font_set;
    Cursor			 cursor;
} XimcpStatusRec;

typedef struct	_XimcpInputContext {
    struct _XimcpInputContext	*next;
    struct _XimcpInputContext	*previous;
    struct _XimcpInputMethod	*xim;
    unsigned short		 icid;
    CARD32			 mask;
    unsigned long		 input_style;
    Window			 client_window;
    Window			 focus_window;
    long			 filter_events;
    unsigned long		 reset_state;
    XIMHotKeyTriggers		 hot_key;
    unsigned long		 hot_key_state;
    XimcpPreeditRec		 preedit;
    XimcpStatusRec		 status;
    Boolean			 has_focus;
    Buffer			*buffer;
} XimcpInputContext;

typedef struct _XimcpAttrRec {
    String			 name;
    CARD16			 id;
    CARD16			 type;
    Cardinal			 size;
    Cardinal			 offset;
    struct _XimcpAttrRec	*nested_list;
} XimcpAttrRec;

typedef struct _XimcpExtRec {
    CARD8			 major_opcode;
    CARD8			 minor_opcode;
    char			*name;
} XimcpExtRec;
#endif /* _XimcpP_H */
