/* @(#)Xfeoak.h      6.1 94/06/01 20:04:28 FUJITSU LIMITED. */
/*

Copyright (c) 1991, 1992, 1993, 1994  FUJITSU LIMITED

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
          Kiyoaki oya          FUJITSU LIMITED
	  Toshimitu Terazono   FUJITSU LIMITED
	  Kazuhiro Asakuma     FUJITSU LIMITED

*/

/*
 *  xfeoak : Input Server (kana-kanji covertion IM Server)
 */

#ifndef _XFEOAK_H_
#define _XFEOAK_H_

#include <X11/Xmd.h>
#include <X11/Xlib.h>
#include <X11/Xfuncs.h>
#include <X11/Xos.h>
#include <stdio.h>
#include "XimProto.h"

/* xfeoak implementation revision */ /* for XV11R6 */
#define XFEOAK_REVISION "xfeoak Revision 6.0"

#ifdef AIXV3
#include <sys/select.h>
#endif /* AIXV3 */

#ifdef XV11R4
/*
 * The following is specification defines a set of rutines
 *
 *	X Window System, Version 11
 *	 Input Method Specifications
 *	Post-Public Review Draft 1.1 - propsed
 */
#define XIMPreeditArea          	0x0001L
#define XIMPreeditCallbacks		0x0002L
#define XIMPreeditPosition		0x0004L
#define XIMPreeditNothing		0x0008L
#define XIMPreeditNone			0x0010L

#define XIMStatusArea			0x0100L
#define XIMStatusCallbacks		0x0200L
#define XIMStatusNothing		0x0400L
#define XIMStatusNone			0x0800L

typedef CARD32 XIMStyle;

typedef struct {
    CARD16	 count_styles;
    XIMStyle	*supported_styles;
} XIMStyles;

#endif /* XV11R4 */

#define	FPRT_XIM			0x01

#define	FIM_DESTROY			0x00
#define	FIM_CREATE			0x01
#define	FIM_BEGIN			0x02
#define	FIM_SETFOCUS			0x04

#define FS_HIRAGANA			0x01
#define FS_KATAKANA			0x02
#define FS_KANA				0x03
#define FS_UPPER_ASCII			0x04
#define FS_LOWER_ASCII			0x08
#define FS_ASCII			0x0C

#define FXIM_LINEBUFF_LENGTH		42
#define FXIM_MAX_CLIENT_NO  		32 

#define FINF_NONE			0	
#define FINF_ADD			1
#define FINF_MODY			2

#define FKEY_NONE			0
#define FKEY_G_OYA			1
#define FKEY_G_JIS			2
#define FKEY_DS_OYA			3
#define FKEY_DS_JIS			4

#define FF_FONT_LOAD			0x01
#define FF_CREATE_GC			0x10

#define FC_SAME_ROOT_PREEDIT_WINDOW	0x01
#define FC_SAME_ROOT_PREEDIT_GC		0x02
#define FC_SAME_ROOT_PREEDIT_FONT	0x04
#define FC_SAME_ROOT_PREEDIT		0x07

#define FC_SAME_ROOT_STATUS_WINDOW	0x10
#define FC_SAME_ROOT_STATUS_GC		0x20
#define FC_SAME_ROOT_STATUS_FONT	0x40
#define FC_SAME_ROOT_STATUS		0x70

/* mask (XIM_CREATE, XIM_SETVALUE, XIM_GETVALUE) */
#define XFE_QUERY_INPUT_STYLES_MASK	(1L <<  0)
#define XFE_QUERY_FUNCTIONS_MASK	(1L <<  1)

#define XFE_INPUT_STYLE_MASK		(1L <<  0)
#define XFE_CLIENT_WIN_MASK		(1L <<  1)
#define XFE_FOCUS_WIN_MASK		(1L <<  2)
#define XFE_FILTER_EVENTS_MASK		(1L <<  3)
#define XFE_RESET_STATE_MASK		(1L <<  4)
#ifdef SUPPORT_STRING_CONV
#define XFE_STRING_CONV_MASK		(1L <<  5)
#endif
#ifdef SUPPORT_HOTKEY
#define XFE_HOTKEY_MASK			(1L <<  6)
#define XFE_HOTKEY_STATE_MASK		(1L <<  7)
#endif
#define XFE_PREEDIT_MASK		(1L <<  8)
#define XFE_STATUS_MASK			(1L <<  9)

#define XFE_PRE_AREA_MASK		(1L <<  0)
#define XFE_PRE_AREANEED_MASK		(1L <<  1)
#define XFE_PRE_SPOTL_MASK		(1L <<  2)
#define XFE_PRE_COLORMAP_MASK		(1L <<  3)
#define XFE_PRE_STDCOLORMAP_MASK	(1L <<  4)
#define XFE_PRE_FG_MASK			(1L <<  5)
#define XFE_PRE_BG_MASK			(1L <<  6)
#define XFE_PRE_BGPIXMAP_MASK		(1L <<  7)
#define XFE_PRE_FONT_MASK		(1L <<  8)
#define XFE_PRE_LINESP_MASK		(1L <<  9)
#define XFE_PRE_CURSOR_MASK		(1L << 10)
#define XFE_PRE_PREEDITSTATE_MASK	(1L << 11)

#define XFE_STS_AREA_MASK		(1L <<  0)
#define XFE_STS_AREANEED_MASK		(1L <<  1)
#define XFE_STS_COLORMAP_MASK		(1L <<  2)
#define XFE_STS_STDCOLORMAP_MASK	(1L <<  3)
#define XFE_STS_FG_MASK			(1L <<  4)
#define XFE_STS_BG_MASK			(1L <<  5)
#define XFE_STS_BGPIXMAP_MASK		(1L <<  6)
#define XFE_STS_FONT_MASK		(1L <<  7)
#define XFE_STS_LINESP_MASK		(1L <<  8)
#define XFE_STS_CURSOR_MASK		(1L <<  9)

typedef struct	_Xfe_Bitmask {
    BITMASK32	 mask[4];
} Xfe_Bitmask;

/*
 * index of Xfe_Bitmask
 */
#define	XFE_TOP_MASK_IDX		 0
#define	XFE_PRE_MASK_IDX		 1
#define	XFE_STS_MASK_IDX		 2

typedef struct  _Xim_Preedit {
    XRectangle		 Area;
    XRectangle		 AreaNeeded;
    XPoint		 SpotLocation;
    Colormap		 colormap;
    Atom		 StdColormap;
    unsigned long	 Foreground;
    unsigned long	 Background;
    Pixmap		 Bg_Pixmap;
    char		*FontName;
    long		 LineSpacing;
    Cursor		 cursor;
    XIMPreeditState	 preeditState;
} Xim_PreeditPropRec;

typedef struct  _Xim_Status {
    XRectangle		 Area;
    XRectangle		 AreaNeeded;
    Colormap		 colormap;
    Atom		 StdColormap;
    unsigned long	 Foreground;
    unsigned long	 Background;
    Pixmap		 Bg_Pixmap;
    char		*FontName;
    long		 LineSpacing;
    Cursor		 cursor;
    Window		 window;
} Xim_StatusPropRec;

typedef struct	_Xfe_IMAttr {
    XIMStyle		*styles;
/*
    XIMOptions		*options;
*/
} Xfe_IMAttr;

typedef struct	_Xfe_ICAttr {
    XIMStyle			 style;
    CARD16			 unused;
    Window			 client_window;
    Window			 focus_window;
    BITMASK32			 filter_events;
    XIMResetState		 reset_state;
#ifdef SUPPORT_STRING_CONV
    XIMStringConversionText	*string_conv;
#endif
#ifdef SUPPORT_HOTKEY
    XIMHotKeyTriggers		*hotkey;
    XIMHotKeyState		 hotkey_state;
#endif
    Xim_PreeditPropRec		 preedit;
    Xim_StatusPropRec		 status;
} Xfe_ICAttr;

typedef struct	_Xfe_RecvRec {
    CARD16		 length;
    CARD16		 total_length;
    struct _Xfe_RecvRec *next;
} Xfe_RecvRec, *Xfe_Recv;

typedef struct _XimProtoHdr {
    CARD8		major_opcode;
    CARD8		minor_opcode;
    CARD16		length;
} XimProtoHdr;

typedef struct	_Xfe_TransMethodRec {
    void		(*event_proc)();
    Xfe_Recv		(*read_client)();
    Bool		(*send_client)();
    void		(*flush)();
    void		(*disconnect)();
} Xfe_TransMethodRec;

typedef struct	_Xfe_TransRec {
    char		*transport_name;
    Bool		 (*config)();
    void		 (*exit)();
    Bool		 (*get_name)();
    Bool		 config_flag;
    Xfe_TransMethodRec	 method;
    XPointer		 trans_data;
} Xfe_TransRec;

typedef CARD16	FXCNID;
typedef CARD16	FXIMID;
typedef CARD16	FXICID;

typedef struct	_FXim_Connect {
    Bool		 used;
    FXCNID		 connectID;
    CARD16		 dmy;
    CARD8		 byte_order;
    CARD8		 pad;
    CARD16		 (*Card16)();
    CARD32		 (*Card32)();
    CARD16		 major_protocol_version;
    CARD16		 minor_protocol_version;
    CARD16		 auth_wait;
    CARD16		 auth_names_num;
    CARD16		*auth_names;
    Bool		 sync;
    Xfe_TransRec	*trans;
    Xfe_Recv		 recv;
    Window		 client_window;	/* If Xproto (connect window) */
    Window		 server_window;	/* If Xproto (connect window) */
    int			 fd;		/* If Tcp (client fd) */
    CARD32		 major_transport_version;
    CARD32		 minor_transport_version;
    CARD32		 BoundarySize;
} FXim_Connect;

 /*
  * values for auth_wait of FXim_Connect
  */

#define	SERVER_WAIT1		1
#define	SERVER_WAIT2		2
#define	SERVER_CONNECT		3

typedef struct	_FXim_IMRec {
	Bool		 used;
	FXIMID		 imid;
	CARD16		 pad;
	XIMStyle	 style;
	CARD32		 encoding;
	BITMASK32	 extension_flag;
	FXim_Connect	*parent_connect;
} FXim_IMRec;

/*
 * bit mask for the extension_flag of Xim_IMRec
 */

#define	XIM_EXTENSION_SET_EVENT_MASK	(1L << 0)
#ifdef EXT_FORWARD
#define XIM_EXTENSION_FORWARD_KEYEVENT	(1L << 1)
#endif
#ifdef EXT_MOVE
#define XIM_EXTENSION_MOVE		(1L << 2)
#endif

#define	IS_SUPPORT_EXT_SET_EVENT_MASK(im) \
    (((FXim_IMRec *)im)->extension_flag & XIM_EXTENSION_SET_EVENT_MASK)
#ifdef EXT_FORWARD
#define	IS_SUPPORT_EXT_FORWARD_KEYEVENT(im) \
    (((FXim_IMRec *)im)->extension_flag & XIM_EXTENSION_FORWARD_KEYEVENT)
#endif
#ifdef EXT_MOVE
#define	IS_SUPPORT_EXT_MOVE(im) \
    (((FXim_IMRec *)im)->extension_flag & XIM_EXTENSION_MOVE)
#endif

typedef struct  _FXimFont {
	GC		 normal_gc;
	GC		 reverse_gc;
	GC		 invert_gc;
	XFontStruct 	*ascii_kana_font_struct;
	XFontStruct 	*kanji_font_struct;
	XFontStruct 	*gaiji_font_struct;
	short		 ascii_kana_font_width;
	short		 kanji_font_width;
	short		 max_font_height;
	short		 max_font_descent;
	short		 max_font_ascent;
	unsigned char	 fixe_width;
	unsigned char	 status;
} FXimFont;

#define	DEFAULT_FILTER_MASK		 (0L)
#define	DEFAULT_INTERCEPT_MASK		 (0L)
#define	DEFAULT_SELECT_MASK		 (KeyPressMask | KeyReleaseMask)
#define	DEFAULT_FORWARD_MASK		 (KeyPressMask | KeyReleaseMask)
#define	DEFAULT_SYNCRONOUS_MASK		~(KeyPressMask | KeyReleaseMask)

typedef struct  _FXim_Client {
	Bool			 used;
	FXIMID			 imid;
	FXICID			 icid;
	Bool			 sync;
	FXim_IMRec		*parent_imrec;
	Xfe_Recv		 pending;
	Window  		 window;        
	XIMStyle 		 style;
	EVENTMASK		 filter_event_mask;
	EVENTMASK		 intercept_event_mask;
	EVENTMASK		 select_event_mask;
	EVENTMASK		 forward_event_mask;
	EVENTMASK		 synchoronous_event_mask;
	EVENTMASK		 cv_filter_event_mask;
	EVENTMASK		 cv_intercept_event_mask;
	EVENTMASK		 cv_select_event_mask;
	EVENTMASK		 cv_forward_event_mask;
	EVENTMASK		 cv_synchoronous_event_mask;
	EVENTMASK		 saved_event_mask;
	unsigned char		 prot_type;
	unsigned char		 imp_mode;    
	unsigned char		 ext_ui_back_front;    
	unsigned char		 frontend_key_press_mask;    
	unsigned char		 ext_ui_statuswindow;
	unsigned char		 client_sync_reply;
	unsigned char		 window_type;
	unsigned char		 check_preedit_area_size;    
	Xim_PreeditPropRec 	*preedit;
	Xim_StatusPropRec 	*status;
	FXimFont   		*preedit_font;
	FXimFont   		*status_font;
	Window			 focus_window;
	Window  		 preedit_window;
	Window  		 preedit_window2;
	Window  		 status_window; 
	XRectangle 		 preedit_area[2];
	short			 used_preedit_window_no;
	short			 preedit_line_length[2];
	short			 preedit_line_no;
	unsigned char		 preedit_visible;
	unsigned char		 status_visible;
	unsigned char		 henkan_mode;
	unsigned char		 siftmode;
	unsigned char		 romaji_mode;
	unsigned char		 hankakumode;	
	unsigned char		 physical_kanashift;	
	unsigned char		 logical_kanashift;
	unsigned char		 logical_capslock;
	unsigned char		 stage;		/* reserved */
	unsigned long		 old_state;
	unsigned char		 select_kanji_no;	
	unsigned char		 select_kanji_mod;
	short			 last_colums;	
	CARD16			*output_buff;
	CARD16			*input_buff;
	short			 romaji_colums;
	short			 current_colums;
	short			 reverse_colums;
	short			 input_colums;
	short			 convert_colums;
	short			 convert_count;
	short			 input_flag;
	short			 pad;
	unsigned char		 slcrtok_nhtype;
	unsigned char		 slcrtok_input_no;
	unsigned char		 slcrtok_inbuf[4];
	unsigned char		 slcrtok_outbuf[4];
        EVENTMASK		 filter_events;
        XIMResetState		 reset_state;
#ifdef SUPPORT_STRING_CONV
	XIMStringConversionText	*string_conv;
#endif
#ifdef SUPPORT_HOTKEY
	XIMHotKeyTriggers	*hotkey;
	XIMHotKeyState		 hotkey_state;
#endif
} FXim_Client;

/*	<< New Window Form >>
 *	|<--------------------- big_width ------------------------------------>|
 *	 |<------------- preedit_width ---------------->| |<-- status_width ->|
 *	+----------------------------------------------------------------------+
 *	|+----------------------------------------------+ +-------------------+|
 *	||  Preedit Window	                        | |   Status Window   ||
 *	|+----------------------------------------------+ +-------------------+|
 *	+----------------------------------------------------------------------+
 *							 |<--- small_width --->|
 *	MINIMAM Window					  |<-- status_width ->|
 *	                                                 +---------------------+
 *	                                                 |+-------------------+|
 *	                                                 ||   Status Window   ||
 *	                                                 |+-------------------+|
 *	                                                 +---------------------+
 */
typedef struct  _FXim_Server {
	unsigned char	root_display_area;
	unsigned char	root_form;
	unsigned char	status_direction;
	unsigned char	rsv1;
	unsigned char	iconic;
	unsigned char	redirect;
	short		border_width;
	short		status_message_length;
	short		font_width;
	short		status_width;
	short		preedit_width;
	short		window_height;
	short		area_needed_height;
	int		gravity;
	XRectangle	*current;
	XRectangle	big;
	XRectangle	small;
	XRectangle	core;
	XPoint		big_status;
	XPoint		small_status;
	XPoint		pre_edit;
	XPoint		wm_offset;
	GC 		higlight_gc;
	GC 		shadow_gc;
	Pixmap		pixmap;
} FXim_Server;

#define UNDER_LINE_HEIGHT       2

#define ROOT_DISPLAY_STATUS     1
#define ROOT_DISPLAY_PREEDIT    2

#define ROOT_FORM_2D    2
#define ROOT_FORM_3D    3

#define ROOT_STATUS_RIGHT       0
#define ROOT_STATUS_LEFT        1

#define KS_TOP          1
#define KS_BOTTOM       2
#define KS_FOWARD       3
#define KS_BACKWARD     4
#define KS_NEXT         5
#define KS_PREV         6

#define Static static

#define	XfeAlloc(size)		((Xfe_Recv)malloc(size))
#define	XfeRealloc(size)	((Xfe_Recv)realloc(buf, size))
#define	XfeFree(ptr)		(free(ptr))

#define XfeCard16(val)		((*current_connect->Card16)((CARD16)(val)))
#define XfeCard32(val)		((*current_connect->Card32)((CARD32)(val)))

#define	XIM_SERIALNUMBER(serial)  \
		(CARD16)(((unsigned long)serial >> 16) & 0xffff)
#define	XIM_SET_SERIALNUMBER(serial) \
		(((unsigned long)serial & 0xffff) << 16)

#ifdef __sxg__
#define	fd_set		long

#ifndef FD_ZERO
#define	FD_ZERO(p)	(*(p) = 0)
#endif
#ifndef FD_SET
#define	FD_SET(n, p)	(*(p) |= (1 << (n)))
#endif
#ifndef FD_CLR
#define	FD_CLR(n, p)	(*(p) &= ~(1 << (n)))
#endif
#ifndef FD_ISSET
#define FD_ISSET(n, p)	(*(p) & (1 << (n)))
#endif

#endif
extern	CARD32		 server_major_transport_version;
extern	CARD32		 server_minor_transport_version;
extern	int		 boundary_size;

extern	fd_set		 server_fd_mask;
extern	int		 server_max_fd;

extern	FXim_Server	 fxim_server;

extern	Atom		 selection_atom;
extern	Atom		 locale_target;
extern	Atom		 transport_target;

extern	unsigned char	*support_locale_name;
extern	unsigned char	*support_transport_name;
extern	char     	*command_name;
extern	char		 vendor_name[];
extern	char		 server_name[];
extern	char		 server_version[];
extern	char		*transport_name;
extern	char		*xfeoak_locale;


extern	Display		*display;
extern	int      	 screen;
extern	Window   	 root;
extern	Window   	 preedit_root;

extern	int		 keybord_type;
extern	int		 bd_width;

extern	FXim_Client 	*root_client;
extern	FXim_Client 	*focus_in_client;	
extern	FXim_Client 	*current_client;
extern	FXim_IMRec	*current_imrec;
extern	FXim_Connect	*current_connect;
extern	FXimFont	*current_preedit_font;

extern	int		 num_of_preeditnothing;

extern	int		 event_flow;
extern	int		 back_front;

extern	int		 no_beep;
extern	int		 verbose_flag;

extern	Bool		 server_sync;
extern	Bool		 client_sync;

extern	Bool		 use_auth;
extern	char		 control_modifiers_flag;	

extern	Bool		 use_ext_set_event_mask;

#ifdef EXT_FORWARD
extern	Bool		 use_ext_forward_keyevent;
#endif /* EXT_FORWARD */

#ifdef EXT_MOVE
extern	Bool		 use_ext_move;
#endif /* EXT_MOVE */


extern	int		 Dic_Argv();
extern	int		 Dic_Help();
extern	int		 Dic_Open();
extern	int		 Dic_Close();

extern	void		 ErrorMessage();
extern	void		 WarningMessage();

extern	Bool		 InitialConnectTable();
extern	int		 InitialTranslationTable();
extern	Bool		 InitialIMRecTable();
extern	Bool		 InitialClientTable();
extern	int		 InitialKanjiBuff();
extern	Bool		 InitialTransport();
extern	void		 InitializeProtocol();
extern	void		 InitializeExtension();
extern	Bool		 InitialXProto();
extern	Bool		 InitialTcp();
extern	Bool		 InitialInternal();

extern	void		 ExitProcess();
extern	void		 ExitProtocol();
extern	void		 ExitTransport();
extern	void		 ExitClientTable();
extern	void		 ExitIMRecTable();
extern	void		 ExitConnectTable();
extern	void		 ExitXProto();
extern	void		 ExitTcp();
extern	void		 ExitInternal();

extern	Bool		 GetTransportName();
extern	Bool		 GetXProtoName();
extern	Bool		 GetTcpName();
extern	Bool		 GetInternalName();

extern	FXim_Client	*NewClient();
extern	FXim_Client	*GetClient();
extern	FXim_Client	*FindClientByIMIDandICID();
extern	FXim_Client	*FindClientByPreeditOrStatusWindow();
extern	void		 DestroyCurrentClient();
extern	void		 DeleteClient();
extern	FXim_IMRec	*NewIMRec();
extern	FXim_IMRec	*FindIMRecByIMID();
extern	FXim_Connect	*NewConnect();
extern	FXim_Connect	*FindConnectByServerWindow();
extern	FXim_Connect	*FindConnectByClientWindow();
extern	FXim_Connect	*FindConnectByFd();
extern	void		 DeleteConnect();

extern	int		 Fj_InitialMode();
extern	int		 fj_HenkanReset();
extern	unsigned short	 Hankaku_To_ZenkakuAsciiHiragana();
extern	int		 EUC_To_CompoundText();

extern	int		 FXLoadQueryBaseFont();
extern	int		 FXCreateGC();

extern	int		 GetMaxStatusMessageLength();
extern	int		 Redisplay();
extern	void		 DisplayRootWidow();
extern	void		 DisplayRootPreedit();
extern	int		 DisplayStatus();
extern	int		 CreateStatusWindow();
extern	int		 CreatePreeditWindow();
extern	INT16		 CreateIMAttrList();
extern	INT16		 CreateICAttrList();
extern	int		 DestroyPreeditWindow();
extern	int		 DestroyStatusWindow();
extern	int		 ChangeWindowAttributes();
extern	int		 ChangePreeditWindow();
extern	int		 ChangeStatusWindow();
extern	int		 ChangePreeditFont();
extern	int		 ChangeStatusFont();
extern	int		 ChangeHenkanMode();
extern	Bool		 SetCurrentClient();
extern	int		 SetFocusClient();
extern	int		 UnsetFocusClient();
extern	int		 CurrentKeyPress();
extern	int		 ResizePreeditArea();

extern	void		 XimTransportCheck();
extern	void		 XimCnversionEnd();
extern	Bool		 XimSendKeyEvent();
extern	Bool		 XimSyncReplySend();
extern	void		 XimSetEndian();
extern	Bool		 XimExtSetEventMaskSend();
extern	INT16		 XimEncodeIMValues();
extern	INT16		 XimEncodeICValues();
extern	void		 XimCheckICValues();
extern	void		 XimDecodeICValues();
extern	Xfe_Recv	 XimprotoKeypress();
extern	Xfe_Recv	 XimprotoNewRecv();
extern	void		 XimSetHeader();
extern	void		 XimHoldQueue();
extern	Bool		 XimEvent();
extern	Bool		 XimExtensionDispatch();
extern	int		 XimExtensionErrorno();
extern	INT16		 XimSetExtension();
extern	void		 XimGetExtension();
extern	Bool		 XimCheckExtension();
extern	void		 XimDestroyClient();

#ifdef EXT_FORWARD
extern Bool		 XimExtForwardEventSend();
#endif /* EXT_FORWARD */

#ifdef CONTROL_MODIFIERS_EXTENSION
extern	Bool		 CheckControlModifiers();
#endif /*CONTROL_MODIFIERS_EXTENSION*/

#ifdef FUJITSU_OAK
extern	int		 FXDefaultError();
#endif /* FUJITSU_OAK */

#endif /* _XFEOAK_H_ */
