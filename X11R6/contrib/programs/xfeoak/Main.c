/* @(#)Main.c      6.1 94/06/01 20:04:28 FUJITSU LIMITED. */
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
          Riki Kawaguchi       FUJITSU LIMITED
	  Toshimitu Terazono   FUJITSU LIMITED
	  Kazuhiro Asakuma     FUJITSU LIMITED

*/

/*
 *  xfeoak : Input Server (kana-kanji covertion IM Server)
 */

#include <X11/Xlocale.h>
#include "Xfeoak.h"
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#ifdef CONTROL_MODIFIERS_EXTENSION
#include "cmod.h"
#endif /*CONTROL_MODIFIERS_EXTENSION*/

#ifdef USE_SIGNAL
#include <signal.h>
#endif /*USE_SIGNAL*/

extern	int		fsm_free;

#define gray_width		16
#define gray_height		16
static unsigned char	gray_bits[] = {
   0xff, 0xff, 0x55, 0x55, 0xff, 0xff, 0x55, 0x55, 0xff, 0xff, 0x55, 0x55,
   0xff, 0xff, 0x55, 0x55, 0xff, 0xff, 0x55, 0x55, 0xff, 0xff, 0x55, 0x55,
   0xff, 0xff, 0x55, 0x55, 0xff, 0xff, 0x55, 0x55};

/***************************************************************************
 * The bitmap pattern below is a registered trade mark of FUJITSU LIMITED. *
 ***************************************************************************/
#define icon_width		48
#define icon_height		48
#define icon_x_hot		24
#define icon_y_hot		15
#ifdef DEC
static char		icon_bits[] = {
#else
static unsigned char	icon_bits[] = {
#endif 
 0x00,0x00,0xa8,0x02,0x00,0x00,0x00,0x00,0x76,0x1d,0x00,0x00,0x00,0x00,0xfb,
 0x3e,0x00,0x00,0x00,0x80,0x51,0x7f,0x00,0x00,0x00,0x80,0xef,0xfe,0x00,0x00,
 0x00,0x40,0x59,0xdd,0x03,0x00,0x00,0x80,0xae,0xba,0x0f,0x00,0x00,0x70,0xdd,
 0x75,0x0d,0x00,0x00,0xa8,0xbb,0xea,0x0b,0x00,0xc0,0x5e,0x57,0x55,0x3f,0x00,
 0xa0,0xae,0xa8,0xea,0x2b,0x00,0x80,0x55,0x55,0xd7,0xf7,0x00,0xe0,0xe2,0xaa,
 0xaa,0xec,0x02,0x60,0x41,0x55,0xdd,0xbd,0x05,0x38,0xca,0xaa,0x7a,0xba,0x0f,
 0x56,0x11,0x5c,0x6d,0xad,0x1f,0xba,0x2e,0xaa,0xe2,0xaf,0x3f,0xc4,0x45,0x55,
 0xd5,0xf7,0x25,0xac,0xa2,0xea,0xba,0xfb,0x2b,0x54,0x57,0x56,0xd5,0x7d,0x3f,
 0xaa,0xa8,0xae,0xeb,0xa3,0x3e,0xdd,0x55,0x57,0x75,0x55,0x7d,0xaf,0xaa,0xee,
 0xea,0xaa,0x7e,0xd7,0xd7,0x55,0x17,0xd7,0x7d,0xaa,0xaa,0xaa,0xaa,0xbe,0x7a,
 0xd4,0xdf,0x55,0x55,0x57,0x7f,0xad,0xea,0xae,0xee,0xab,0xbe,0x5f,0x7f,0x5d,
 0x15,0x55,0xff,0xfe,0xcd,0xba,0xba,0xef,0x7f,0xe8,0x7e,0x5d,0x4d,0xff,0x7f,
 0x00,0xff,0xff,0xea,0xff,0x3f,0x00,0xd8,0x7f,0x7d,0xe7,0x07,0x00,0x80,0xfa,
 0x7e,0x00,0x00,0x00,0x00,0xf2,0x3f,0x00,0x00,0x00,0x00,0xd0,0x07,0x00,0x00,
 0x00,0x00,0xf0,0x07,0x00,0x00,0x00,0x00,0xd0,0x07,0x00,0x00,0x00,0x00,0xb0,
 0x07,0x00,0x00,0x00,0x00,0xd8,0x0f,0x00,0x00,0x00,0x40,0xff,0x7f,0x01,0x00,
 0x00,0xea,0xef,0xff,0x2a,0x00,0x00,0x40,0xfd,0xd7,0x03,0x00,0x80,0xaa,0xbe,
 0xaf,0x2a,0x00,0x48,0xd5,0x57,0x7d,0x55,0x01,0x00,0x80,0xa8,0xaa,0x00,0x00,
 0x00,0x55,0x55,0x55,0x55,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x55,
 0x55,0x05,0x00};

#ifdef NOTXPM
#include "icon.xpm"
#endif /* NOTXPM */

#define KANA14  "-*-*-medium-r-normal--16-*-*-*-*-*-jisx0201.1976-0"
#define K14     "-*-*-medium-r-normal--16-*-*-*-*-*-jisx0208.1983-0"
static char	*default_ascii_kana_font_name = KANA14;
static char	*default_kanji_font_name = K14;

#define MIN_PREEDIT_LENGTH	 40
#define DRAW_BW			 2
#define DRAW_YDW		 4
#define DRAW_XDW		 4
#define DRAW_YBDW		(DRAW_BW + DRAW_YDW)
#define DRAW_XBDW		(DRAW_BW + DRAW_XDW)
#define DRAW_OFFSET		 4
#define DRAW_YOFFSET1		(DRAW_BW + DRAW_OFFSET)
#define DRAW_XOFFSET1		(DRAW_BW + DRAW_OFFSET)
#define DRAW_YOFFSET		(DRAW_BW*2 + DRAW_OFFSET)
#define DRAW_XOFFSET		(DRAW_BW*2 + DRAW_XDW + DRAW_OFFSET)
#define DRAW_FIRST_OFFSET	 DRAW_XOFFSET
#define DRAW_SECOND_OFFSET	((DRAW_XOFFSET * 2) + DRAW_OFFSET)
#define DRAW_SMALL_OFFSET	(DRAW_XOFFSET * 2)
#define DRAW_BIG_OFFSET		((DRAW_XOFFSET * 3) + DRAW_OFFSET)
#define DELTA30			 30

typedef struct {
    char	*foreground;
    char	*root_background;
    char	*text_background;
    char	*text_font_color;
} FColors;
static FColors	*default_color;
static FColors	default_3D_color = { "black", "#D4C9B1", "white", "black"  };
static FColors	default_2D_color = { "black", "white", "white", "black" };

static char	*foreground_color = NULL;
static char	*root_background_color = NULL;
static char	*text_background_color = NULL;
static char	*text_font_color = NULL;

static unsigned long	 foreground_pixel;
static unsigned long	 root_background_pixel;
static unsigned long	 text_font_pixel;
static unsigned long	 text_background_pixel;

static char	*default_border_color ="black";
static char	*default_background_pixmap ="";
static char	*default2_background_pixmap = ".";

static int	 default_verbose = 0;

static int	 default_server_sync = 1;
static int	 default_client_sync = 0;
static int	 default_extension = 1;
static int	 default_forward_keyevent = 0;
static int	 default_ext_move = 0;

static int	 default_event_flow = 0;
static int	 default_back_front = 0;

static char	*default_transport_name = "X";
static char	*default_xfeoak_locale = "ja,ja_JP,ja_JP.eucJP";

static int	 default_title_on = 1;
static char	*default_title = "xfeoak";

static int	 default_trans_version = 1;
static int	 default_boundary_size = 400;

static Bool	 default_use_auth = False;

static int   	 default_keybord = FKEY_NONE;
static int   	 default_beep = 1;
static int	 default_icon_status = 0;

static char	*display_name = NULL;
static char	*geometry = NULL;

static char	*background_pixmap = NULL;
static char	*border_color = NULL;

static int	 default_border_width = 4;
int		 bd_width = -1;

static unsigned long	 border_pixel;

static char	*ascii_kana_font_name = NULL;
static char	*kanji_font_name = NULL;

static int	 icon_status = -1;
static int	 title_on = -1;
static char	*title = NULL;
static char	*resource_name = NULL;

static int	 max_client = -1;
static int	 kanji_buff_size = -1;

static int	 trans_version = -1;
CARD32		 server_major_transport_version = 3;
CARD32		 server_minor_transport_version = 3;
int		 boundary_size = -1;

fd_set		 server_fd_mask;
int		 server_max_fd;

char 	       	*command_name;
char		 vendor_name[]	  = "Fujitsu";
char		 server_name[]	  = "xfeoak";

char		 server_version[] = "V10L10";

Display		*display;
int              screen;
Window           root; 
Window           preedit_root; 

int	   	 keybord_type = -1;

FXim_Client	*root_client;
FXim_Client	*focus_in_client;
FXim_Client	*current_client;
FXimFont	*current_preedit_font;

FXim_Server	 fxim_server = {
                        0, 0, 0, 0, 0, 0
                        };

int		 fxim_server_icon_pixmap;

XWMHints	 wm_hints;

int		 event_flow = -1;		/* 0:Static  1:Dynamic */
int		 back_front = -1;		/* 0:BackEnd 1:FrontEnd */

int	   	 no_beep = -1;
int		 verbose_flag = -1;

Bool		 server_sync = -1;
Bool		 client_sync = -1;

char		*transport_name = NULL;		/* default protocol */
char		*xfeoak_locale = NULL;

Bool		 use_ext_set_event_mask = -1;
#ifdef EXT_FORWARD
Bool		 use_ext_forward_keyevent = -1;
#endif /* EXT_FORWARD */
#ifdef EXT_MOVE
Bool		 use_ext_move = -1;
#endif /* EXT_MOVE */
Bool		 use_auth = -1;

int 		 control_modifiers_grab = -1;

char		 control_modifiers_flag = 0;
static Bool	 control_modifiers_error_flag = False;

#ifdef CONTROL_MODIFIERS_EXTENSION
static int		 control_modifiers_event;
static int		 control_modifiers_error;
static unsigned long 	 control_modifiers_state = 0;
static unsigned long 	 old_state = 0;
static int		 def_key_grab = 1;
#else /*CONTROL_MODIFIERS_EXTENSION*/
static int		 def_key_grab = 0;
#endif /*CONTROL_MODIFIERS_EXTENSION*/

Static void
Usage()
{
    (void)fprintf(stderr, "usage: %s ", command_name);
    (void)fprintf(stderr,
	"[-display <host:dpy>] [-geometry <WxH+X+Y>] [-nobeep] [-v[erbose]]\n");
    (void)fprintf(stderr,
	"\t[{-none|-jis|-oya[yubi]|-jisG|-jisDS|-oyaG|-oyaDS}]\n");
    (void)fprintf(stderr,
	"\t[-text_fg <color>] [-text_bg <color>]\n");
    (void)fprintf(stderr,
	"\t[-fg <color>] [-bg <color>] [-bd <color>]\n");
    (void)fprintf(stderr,
	"\t[-bw <pixels>] [-bp <pixmap>]\n");
    (void)fprintf(stderr,
	"\t[-fn <ascii kana font_name>] [-fk <kanji font_name>]\n");
    (void)fprintf(stderr,
	"\t[{-serverSync|-serverSyncNone}] [{-clientSync|-clientSyncNone}]\n");
    (void)fprintf(stderr,
	"\t[{-ExtSetEventMask|-noExtSetEventMask}]\n");
#ifdef EXT_FORWARD
    (void)fprintf(stderr,
	"\t[{-ExtForwardKey|-noExtForwardKey}]\n");
#endif /* EXT_FORWARD */
#ifdef EXT_MOVE
    (void)fprintf(stderr,
	"\t[{-ExtMove|-noExtMove}]\n");
#endif /* EXT_MOVE */
    (void)fprintf(stderr, "\t[{-Ext|-noExt}]\n");
    (void)fprintf(stderr,
	"\t[{-static|-dynamic}] [{-backend|-frontend}]\n");
    (void)fprintf(stderr,
	"\t[-proto <protocol_name>] [-locale <locale_name>]\n");

    (void)fprintf(stderr,
	"\t[{-title [<string>]|-notitle}] [-name <string>] [-iconic]\n");
    (void)fprintf(stderr,
	"\t[-auth] [-maxClient <number>] [-workSize <number>]\n");
    (void)fprintf(stderr,
	"\t[-transVersion <number/number>] [-divideSize <number>]\n");

    (void)Dic_Help(stderr);
    return;
}

void
ExitProcess()
{
#ifdef CONTROL_MODIFIERS_EXTENSION
    if (control_modifiers_flag && control_modifiers_grab) {
        ControlModifiersUngrab(display);
    }
#endif /*CONTROL_MODIFIERS_EXTENSION*/

     (void)Dic_Close();
     ExitProtocol();
     ExitTransport();
     ExitClientTable();
     ExitIMRecTable();
     ExitConnectTable();
     XDestroyWindow(display, root_client->window);
     XCloseDisplay(display);
     exit(0);
}

Static int
X_ErrorHandler(display, event)
    Display	*display;
    XErrorEvent *event;
{
#ifdef CONTROL_MODIFIERS_EXTENSION
    if (control_modifiers_error_flag) {
	if (event->error_code == BadRequest) {
	    control_modifiers_error_flag = False;
	    if (control_modifiers_grab) {
     		unsigned long  mask;
     		mask = (ExposureMask    | KeyPressMask
		      | KeyReleaseMask  | FocusChangeMask
		      | EnterWindowMask | LeaveWindowMask);
     		XSelectInput(display, root_client->window, mask);
	    }
	    control_modifiers_grab = 0;
	    return 0;
	}
    }
#endif /*CONTROL_MODIFIERS_EXTENSION*/

#ifdef FUJITSU_OAK
    (void)FXDefaultError(display, event);
#endif /*FUJITSU_OAK*/

    return 0;
}

Static int
X_IOErrorHandler(display)
    Display	*display;
{
    WarningMessage(9);
    ExitProcess();
    return 0;
}

Static char *
GetStringDefault(inst_name, class_name, Default)
    char	*inst_name;
    char	*class_name;
    char	*Default;
{
    char	*tmp = XGetDefault(display, resource_name, inst_name);

    if (!tmp)
	tmp = XGetDefault(display, resource_name, class_name);
    if (!tmp)
	tmp = Default;
    return tmp;
}

Static int
GetIntDefault(inst_name, class_name, Default)
    char	*inst_name, *class_name;
    int		 Default;
{
    int		 flag = Default;
    char	*tmp = XGetDefault(display, resource_name, inst_name);

    if(!tmp)
	tmp = XGetDefault(display, resource_name, class_name);
    if(tmp)
	flag = atoi(tmp);;
    return flag;
}

Static int
GetKeybordDefault(inst_name, class_name, Default)
    char	*inst_name, *class_name;
    int		 Default;
{
    int	         flag = -1;
    char	*tmp = XGetDefault(display, resource_name, inst_name);

    if(!tmp)
	tmp = XGetDefault(display, resource_name, class_name);

    if(tmp) {
	if(!strcmp(tmp, "none"))	flag = FKEY_NONE;
	else
	if(!strcmp(tmp, "jis"))		flag = FKEY_G_JIS;
	else
	if(!strncmp(tmp, "oyaubi",3))	flag = FKEY_G_OYA;
    }
    if(flag == -1)			flag = Default;

    return flag;
}

Static int
GetYesNoDefault(inst_name, class_name, Default)
    char	*inst_name, *class_name;
    int		 Default;
{
    int		 flag = -1;
    char	*tmp = XGetDefault(display, resource_name, inst_name);

    if(!tmp)
	tmp = XGetDefault(display, resource_name, class_name);
    if(tmp) {
	if (!strcmp(tmp, "on"))		flag = 1;
	else
	if (!strcmp(tmp, "off"))	flag = 0;
	else
	if (!strcmp(tmp, "On"))		flag = 1;
	else
	if (!strcmp(tmp, "Off"))	flag = 0;
	else
	if (!strcmp(tmp, "True"))	flag = 1;
	else
	if (!strcmp(tmp, "False"))	flag = 0;
	else
	if (!strcmp(tmp, "true"))	flag = 1;
	else
	if (!strcmp(tmp, "false"))	flag = 0;
    }
    if (flag == -1)			flag = Default;

    return flag;
}

Static int
GetEventFlowDefault(inst_name, class_name, Default)
    char	*inst_name, *class_name;
    int		 Default;
{
    int		 flag = -1;
    char	*tmp = XGetDefault(display, resource_name, inst_name);

    if(!tmp)
	tmp = XGetDefault(display, resource_name, class_name);
    if(tmp) {
	if(!strcmp(tmp, "static"))	flag = 0;
	else
	if(!strcmp(tmp, "Static"))	flag = 0;
	else
	if(!strcmp(tmp, "STATIC"))	flag = 0;
	else
	if(!strcmp(tmp, "dynamic"))	flag = 1;
	else
	if(!strcmp(tmp, "Dynamic"))	flag = 1;
	else
	if(!strcmp(tmp, "DYNAMIC"))	flag = 1;
    }
    if(flag == -1)			flag = Default;

    return flag;
}

Static int
GetEventMethodDefault(inst_name, class_name, Default)
    char	*inst_name, *class_name;
    int		 Default;
{
    int		 flag = -1;
    char	*tmp = XGetDefault(display, resource_name, inst_name);

    if(!tmp)
	tmp = XGetDefault(display, resource_name, class_name);
    if(tmp) {
	if(!strcmp(tmp, "backend"))	flag = 0;
	else
	if(!strcmp(tmp, "Backend"))	flag = 0;
	else
	if(!strcmp(tmp, "backEnd"))	flag = 0;
	else
	if(!strcmp(tmp, "BackEnd"))	flag = 0;
	else
	if(!strcmp(tmp, "BACKEND"))	flag = 0;
	else
	if(!strcmp(tmp, "frontend"))	flag = 1;
	else
	if(!strcmp(tmp, "Frontend"))	flag = 1;
	else
	if(!strcmp(tmp, "frontEnd"))	flag = 1;
	else
	if(!strcmp(tmp, "FrontEnd"))	flag = 1;
	else
	if(!strcmp(tmp, "FRONTEND"))	flag = 1;
    }
    if(flag == -1)			flag = Default;

    return flag;
}

Static int
GetTransVersionDefault(inst_name, class_name, Default)
    char        *inst_name, *class_name;
    int          Default;
{
    int          flag = -1;
    char        *tmp = XGetDefault(display, resource_name, inst_name);
    char	 major, minor;


    if(!tmp)
        tmp = XGetDefault(display, resource_name, class_name);
    if(tmp) {
	if(strlen(tmp) == 3) {
	    major = *tmp;
	    minor = *(tmp + 2);

	    if((major == '0' && (minor >= '0' && minor <= '2')) ||
	       (major == '1' && minor == '0') ||
	       (major == '2' && (minor >= '0' && minor <= '1'))) {
		flag = (major - '0') * 10 + (minor - '0');
	    }
	}
    }
    if(flag == -1)		flag = Default;

    return flag;
}

Static void
SetColor()
{
    Colormap	  default_colormap;
    int		  default_depth;
    unsigned long white_pixel, black_pixel;
    unsigned long default_foreground_pixel, default_background_pixel;
    XColor        color_val;

    default_depth = DefaultDepth( display, screen);
    if( default_depth == 1 ) {
	fxim_server.root_form = ROOT_FORM_2D;
	default_color = &default_2D_color;
    } else {
	fxim_server.root_form = ROOT_FORM_3D;
	default_color = &default_3D_color;
    }

    if( text_background_color == NULL ) {
        if( root_background_color == NULL )
	    text_background_color = default_color->text_background;
	else
	    text_background_color = root_background_color;
    }
    if( text_font_color == NULL ) {
        if( foreground_color == NULL )
	    text_font_color = default_color->text_font_color;
	else
	    text_font_color = foreground_color;
    }
    if( foreground_color == NULL )
	foreground_color = default_color->foreground;
    if( root_background_color == NULL )
	root_background_color = default_color->root_background;

    default_colormap = DefaultColormap( display, screen);
    white_pixel = WhitePixel( display, screen);
    black_pixel = BlackPixel( display, screen);

#define FXColor( return_pixel, color, default_pixel ) \
    if( XParseColor( display, default_colormap, color, &color_val) \
    &&  XAllocColor( display, default_colormap, &color_val) ) { \
	return_pixel = color_val.pixel; \
    } else { \
	WarningMessage( 10, color); \
        return_pixel  = default_pixel; \
    }

    if( fxim_server.root_form == ROOT_FORM_2D ) {
	default_foreground_pixel = white_pixel;
	default_background_pixel = black_pixel;
    } else {
	default_foreground_pixel = black_pixel;
	default_background_pixel = white_pixel;
    }
    FXColor(foreground_pixel,
			foreground_color, default_foreground_pixel);
    FXColor(root_background_pixel,
			root_background_color, default_background_pixel);
    FXColor(text_background_pixel,
			text_background_color, default_background_pixel);
    FXColor(text_font_pixel,
			text_font_color, default_foreground_pixel);
    FXColor(border_pixel, border_color, black_pixel);

    if( text_font_pixel == text_background_pixel ) {
	WarningMessage( 12, text_font_color );
        text_font_pixel       = default_foreground_pixel;
        text_background_pixel = default_background_pixel;
    }
    if( foreground_pixel == root_background_pixel) {
	WarningMessage( 11, foreground_color );
        root_background_pixel = default_background_pixel;
        foreground_pixel      = default_foreground_pixel;
    }
    return;
}

Static void
GetDefaults()
{
    if (!geometry)
         geometry = GetStringDefault("geometry", "Geometry", NULL);

    if (no_beep < 0) 
	no_beep = GetYesNoDefault("beep", "Beep", default_beep) ? 0: 1;

    if (verbose_flag < 0) 
	verbose_flag = GetYesNoDefault("verbose", "Verbose", default_verbose);

    if (keybord_type < 0)
	keybord_type = GetKeybordDefault("keyBord", "KeyBord", default_keybord);

    if (!foreground_color)
	 foreground_color = GetStringDefault("foreground", "Foreground", 
						foreground_color);
    if (!root_background_color)
	 root_background_color = GetStringDefault("background", "Background", 
						foreground_color);
    if (!border_color)
   	 border_color = GetStringDefault("borderColor", "BorderColor",
						default_border_color);
    if (bd_width < 0)
	bd_width = GetIntDefault("borderWidth", "BorderWidth",
						default_border_width);
    if (!background_pixmap)
   	 background_pixmap = GetStringDefault("backgroundPixmap", 
				"BackgroundPixmap", default_background_pixmap);
    if (!ascii_kana_font_name)
	 ascii_kana_font_name = GetStringDefault("font", "Font", 
						default_ascii_kana_font_name);
    if (!kanji_font_name)
	 kanji_font_name = GetStringDefault("kanjiFont", "KanjiFont",
						default_kanji_font_name);
    if (server_sync < 0)
	 server_sync = GetYesNoDefault("serverSync", "ServerSync",
						default_server_sync);
    if (client_sync < 0)
	 client_sync = GetYesNoDefault("clientSync", "ClientSync",
						default_client_sync);
    if (use_ext_set_event_mask < 0)
	 use_ext_set_event_mask = GetYesNoDefault("extSetEventMask",
				"ExtSetEventMask", default_extension);
    if (use_ext_set_event_mask < 0)
	 use_ext_set_event_mask = GetYesNoDefault("extension", "Extension",
						default_extension);
#ifdef EXT_FORWARD
    if (use_ext_forward_keyevent < 0)
	 use_ext_forward_keyevent = GetYesNoDefault("extForwardKey",
				"ExtForwardKey", default_forward_keyevent);
#endif
#ifdef EXT_MOVE
    if (use_ext_move < 0)
	 use_ext_move = GetYesNoDefault("extMove", "ExtMove",
						default_ext_move);
#endif
    if (event_flow < 0)
	 event_flow = GetEventFlowDefault("eventFlow", "EventFlow",
						default_event_flow);
    if (back_front < 0)
	 back_front = GetEventMethodDefault("eventMethod", "EventMethod",
						default_back_front);
    if (!transport_name)
	 transport_name = GetStringDefault("protocol", "Protocol",
						default_transport_name);
    if (!xfeoak_locale)
	 xfeoak_locale = GetStringDefault("locale", "Locale",
						default_xfeoak_locale);
    if (title_on < 0)
	 title_on = GetYesNoDefault("title", "Title",
						default_title_on);
    if (!title)
	 title = GetStringDefault("titleName", "TitleName",
						default_title);
    if (icon_status < 0)
	icon_status = GetYesNoDefault("iconic", "Iconic", default_icon_status);

    if (use_auth < 0)
	use_auth = GetYesNoDefault("auth", "Auth", default_use_auth);

    if (max_client < 0)
	max_client = GetIntDefault("maxClient", "MaxClient",
						FXIM_MAX_CLIENT_NO);
    if (kanji_buff_size < 0)
	kanji_buff_size = GetIntDefault("workSize", "WorkSize", 4096);

    if(!((server_major_transport_version == 0 &&
           (server_minor_transport_version >= 0 &&
            server_minor_transport_version <= 2)) ||
       (server_major_transport_version == 1 &&
            server_minor_transport_version == 0) ||
       (server_major_transport_version == 2 &&
           (server_minor_transport_version >= 0 &&
            server_minor_transport_version <= 1)))) {
	trans_version = GetTransVersionDefault("transVersion", "TransVersion",
						default_trans_version);
	server_major_transport_version = (CARD32)((int)(trans_version * 0.1) % 10);
	server_minor_transport_version = (CARD32)(trans_version % 10);
    }
    if (boundary_size < 0)
	boundary_size = GetIntDefault("divideSize", "DivideSize",
						default_boundary_size);


#ifdef CONTROL_MODIFIERS_EXTENSION
    if (control_modifiers_grab < 0)
	control_modifiers_grab = GetYesNoDefault("keyGrab", "KeyGrab",
								def_key_grab);
#endif /*CONTROL_MODIFIERS_EXTENSION*/

    return;
}

Static char *
MakeBaseFontName(font_name1, font_name2, font_name3)
    char	*font_name1, *font_name2, *font_name3;
{
    int		 len = 0, no = 0;
    char	*base_font_name;
    char	*form, *name[3];

#define SET_NAME(font_name)		\
    if (font_name && *font_name) {	\
	len += strlen(font_name) + 1;	\
	name[no++] = font_name;		\
    }

    SET_NAME(font_name1);
    SET_NAME(font_name2);
    SET_NAME(font_name3);
#undef SET_NAME

    switch (no) {
    case 1: form = "%s";	break;
    case 2: form = "%s,%s";	break;
    case 3: form = "%s,%s,%s";	break;
    default:
	return (char *)0;
    }
    if (!(base_font_name = (char *)malloc(len)))
	return (char *)0;

    (void)sprintf(base_font_name, form, name[0], name[1], name[2]);
    return base_font_name;
}

Static void
CalculateRootSize( current_font, x_cood, y_cood, width, height)
    FXimFont	  *current_font;
    int           x_cood, y_cood, width, height;
{
    Xim_StatusPropRec  *current_status;
    Xim_PreeditPropRec	*current_preedit;

    fxim_server.big.y = y_cood;
    fxim_server.big.width =
		width - fxim_server.status_width - bd_width * 4 - DELTA30;
    fxim_server.big.height = height;

    fxim_server.small.width  = fxim_server.status_width + DRAW_XOFFSET * 2;
    fxim_server.small.height = height;
    fxim_server.small.y = y_cood;
    if( fxim_server.status_direction == ROOT_STATUS_RIGHT ) {
	fxim_server.big.x = x_cood;
        fxim_server.small.x = x_cood + width - fxim_server.small.width;
    } else {
	fxim_server.big.x = x_cood + width - fxim_server.big.width;
        fxim_server.small.x = x_cood;
    }

    fxim_server.window_height = height - DRAW_YOFFSET * 2 + UNDER_LINE_HEIGHT;
    fxim_server.preedit_width = fxim_server.big.width - DRAW_XOFFSET * 2;

    fxim_server.small_status.x = DRAW_FIRST_OFFSET;
    fxim_server.small_status.y = DRAW_YOFFSET;
    fxim_server.pre_edit.x     = DRAW_FIRST_OFFSET;
    fxim_server.pre_edit.y = DRAW_YOFFSET;

    current_status = root_client->status;
    current_status->AreaNeeded.width  = fxim_server.status_width;
    current_status->AreaNeeded.height = fxim_server.area_needed_height;
    current_status->Area.width  = fxim_server.status_width;
    current_status->Area.height = fxim_server.window_height;
    current_status->Area.x      = fxim_server.small_status.x;
    current_status->Area.y      = fxim_server.small_status.y;
    current_status->Foreground  = foreground_pixel;
    current_status->Background  = root_background_pixel;

    current_preedit = root_client->preedit;
    current_preedit->AreaNeeded.width = fxim_server.font_width;
    current_preedit->AreaNeeded.height = fxim_server.area_needed_height;
    current_preedit->Area.width  = fxim_server.preedit_width;
    current_preedit->Area.height = fxim_server.window_height;
    current_preedit->Area.x      = fxim_server.pre_edit.x;
    current_preedit->Area.y      = fxim_server.pre_edit.y;
    current_preedit->Foreground  = text_font_pixel;
    current_preedit->Background  = text_background_pixel;

    return;
}

Static int
CalculateRootWindow( current_font)
    FXimFont	  *current_font;
{
    int           x_cood, y_cood, y;
    int	  	  big_width, height;
    int		  root_width;
    int		  geometry_mask, gravity;
    XSizeHints    hints;

    root_width  = DisplayWidth( display, screen);
    big_width  = root_width  - bd_width * 2;
    height = current_font->max_font_height + DRAW_YOFFSET * 2;
    fxim_server.area_needed_height = current_font->max_font_height
				    + UNDER_LINE_HEIGHT;

    x_cood = bd_width;
    y_cood = DisplayHeight( display, screen) - height - bd_width * 2;
    gravity = SouthEastGravity;

    fxim_server.font_width  = current_font->kanji_font_width / 2;
    if( fxim_server.font_width < current_font->ascii_kana_font_width )
	fxim_server.font_width = current_font->ascii_kana_font_width;

    fxim_server.status_message_length =
			GetMaxStatusMessageLength(current_client);
    fxim_server.status_width = fxim_server.font_width
				* fxim_server.status_message_length + 2;
    if( geometry ) {
        XSizeHints    normal_hints;
	char	def_geom[64];
	sprintf( def_geom, "%dx%d-0-0", big_width, height );
	normal_hints.flags = PMinSize | PMaxSize | PBaseSize;
	normal_hints.min_width = fxim_server.status_width + DRAW_BIG_OFFSET
		+ (fxim_server.font_width * MIN_PREEDIT_LENGTH);
	normal_hints.min_height = height;
	normal_hints.max_width = root_width;
	normal_hints.max_height = height * 2;
	normal_hints.base_width = 0;
	normal_hints.base_height = 0;
	geometry_mask = XWMGeometry( display, screen, geometry,
		def_geom, bd_width, &normal_hints,
		&hints.x, &hints.y, &hints.width, &hints.height, &gravity);

	big_width = hints.width;
	height    = hints.height;
	x_cood    = hints.x;
	y_cood    = hints.y;
    }
    switch( gravity ) {
    case NorthWestGravity:
    case SouthWestGravity:
        if( fxim_server.status_direction != ROOT_STATUS_LEFT ) {
	    fxim_server.status_direction = ROOT_STATUS_LEFT;
	}
	break;
    case NorthEastGravity:
    case SouthEastGravity:
        if( fxim_server.status_direction != ROOT_STATUS_RIGHT ) {
	    fxim_server.status_direction = ROOT_STATUS_RIGHT;
	}
	break;
    }

    if( y_cood < -DELTA30 ) {
	y_cood = -DELTA30;
    }
    if( y_cood + height > DisplayHeight( display, screen) )
	y_cood = DisplayHeight( display, screen) - height;
    if( x_cood < 0 ) {
	big_width += x_cood;
	x_cood = bd_width;
    }
    if( x_cood + big_width > root_width )
	big_width = root_width - x_cood;
    fxim_server.border_width = bd_width;
    fxim_server.root_display_area = ROOT_DISPLAY_STATUS;
    CalculateRootSize( current_font, x_cood, y_cood, big_width, height);
    fxim_server.gravity = gravity;
    return( gravity );
}

Static int
AllocSHColor( display, screen, base_pixel, shadow_pixel, higlight_pixel)
    Display        	*display;
    int            	screen;
    unsigned long	base_pixel;
    unsigned long	*shadow_pixel;
    unsigned long	*higlight_pixel;
{
    Colormap	  default_colormap;
    XColor        color_val, color_val2;

    default_colormap = DefaultColormap( display, screen);
    color_val.pixel = base_pixel;
    XQueryColor( display, default_colormap, &color_val);

#define Shadow(a)	a / 2
    color_val2.red   = Shadow( color_val.red );
    color_val2.green = Shadow( color_val.green );
    color_val2.blue  = Shadow( color_val.blue );
#undef Shadow
    color_val2.flags = DoRed | DoGreen | DoBlue;
    if( XAllocColor( display, default_colormap, &color_val2) ) {
        *shadow_pixel = color_val2.pixel;
    } else {
        *shadow_pixel = BlackPixel( display, screen);
    }
#define Higlight(a)	a + ((int)((65535 - a) * 2) / 3)
    color_val2.red   = Higlight( color_val.red );
    color_val2.green = Higlight( color_val.green );
    color_val2.blue  = Higlight( color_val.blue );
#undef Higlight
    color_val2.flags = DoRed | DoGreen | DoBlue;
    if( XAllocColor( display, default_colormap, &color_val2) ) {
        *higlight_pixel = color_val2.pixel;
    } else {
        *higlight_pixel = WhitePixel( display, screen);
    }
    return( 0 );
}

Static Bool
CreateRoot()
{
    int		  default_depth;
    XSizeHints    hints;
    XTextProperty icon_name;
    XSetWindowAttributes attributes;
    int		  geometry_mask, gravity;
    int		  rc;
    Xim_StatusPropRec  *current_status;
    Xim_PreeditPropRec	*current_preedit;
    Pixmap	  pixmap = 0;
    char	  *base_font_name;

    if (keybord_type != FKEY_NONE) {
#ifdef FUJITSU_SERVER_EXTENSION
	if (!ControlModifiersQueryExtension(display,
	    &control_modifiers_event, &control_modifiers_error)) {
	WarningMessage(80);
	keybord_type	=  FKEY_NONE;
	control_modifiers_flag = 0;
    } else {
	control_modifiers_flag = 1;
#else /* FUJITSU_SERVER_EXTENSION */
	keybord_type           = FKEY_NONE;
	control_modifiers_flag = 0;
#endif /* FUJITSU_SERVER_EXTENSION */
    }

    root_client = current_client = NewClient( 1);

    current_preedit_font = current_client->preedit_font;
    Fj_InitialMode();
    InitialTranslationTable();

    if (!(base_font_name = MakeBaseFontName( ascii_kana_font_name,
				       kanji_font_name, 0 )))
	return False;
    current_client->status->FontName  = base_font_name;
    current_client->preedit->FontName = base_font_name;
    if (FXLoadQueryBaseFont(display, current_preedit_font, base_font_name) < 0)
	return False;

    current_client->status_font = current_client->preedit_font;

    gravity = CalculateRootWindow( current_client->status_font );
    fxim_server.current = &(fxim_server.small);
    current_client->window = XCreateSimpleWindow( display, root,
			fxim_server.small.x,     fxim_server.small.y,
			fxim_server.small.width, fxim_server.small.height,
			bd_width, border_pixel, root_background_pixel);

    XStoreName( display, current_client->window, (char *)title );
    if (icon_status)
	title_on = False;
    else if(!title_on ) {
        bzero( &attributes, sizeof( XSetWindowAttributes ));
        attributes.override_redirect = True;
        attributes.win_gravity = gravity;
        XChangeWindowAttributes( display, current_client->window,
			CWOverrideRedirect|CWWinGravity, &attributes );
	fxim_server.redirect = 1;	
    }

    default_depth = DefaultDepth( display, screen);
    bzero( &wm_hints, sizeof(XWMHints));
    wm_hints.flags = InputHint | StateHint | IconPixmapHint;
    wm_hints.icon_pixmap = XCreatePixmapFromBitmapData(display,
	    root, (char *)icon_bits, icon_width, icon_height,
	    BlackPixel(display, screen), WhitePixel(display, screen), 1);
    fxim_server_icon_pixmap = wm_hints.icon_pixmap;
    if( icon_status )
	wm_hints.initial_state = IconicState;
    else
	wm_hints.initial_state = NormalState;
    wm_hints.input = True;
    XSetWMHints( display, current_client->window, &wm_hints);

    bzero( &icon_name, sizeof(XTextProperty));
    icon_name.encoding = XA_STRING;
    icon_name.format   = 8;
    icon_name.value    = (unsigned char *)server_name;
    icon_name.nitems   = strlen( (char *)icon_name.value );
    XSetWMIconName( display, current_client->window, &icon_name );

    bzero( &hints, sizeof(XSizeHints));
    hints.flags = USPosition | USSize | PPosition | PSize
	| PMinSize | PMaxSize | PBaseSize | PWinGravity;
    hints.x 	      = fxim_server.small.x;
    hints.y 	      = fxim_server.small.y;
    hints.width       = fxim_server.small.width;
    hints.height      = fxim_server.small.height;
    hints.min_width   = fxim_server.small.width;
    hints.min_height  = fxim_server.small.height;
    hints.max_width   = fxim_server.small.width;
    hints.max_height  = fxim_server.small.height;
    hints.base_width  = fxim_server.small.width;
    hints.base_height = fxim_server.small.height;
    hints.win_gravity = gravity;
    XSetWMNormalHints( display, current_client->window, &hints);
    current_client->style = (XIMPreeditArea | XIMStatusArea);

    current_status = current_client->status;
    current_status->Bg_Pixmap   = pixmap;

    current_client->status_window
    	= XCreateSimpleWindow( display, current_client->window,
		fxim_server.small_status.x, fxim_server.small_status.y,
		fxim_server.status_width,   fxim_server.window_height,
		0, border_pixel, root_background_pixel);
    XStoreName( display, current_client->status_window, "XIMStatusNothing");

    preedit_root = XCreateSimpleWindow( display, root,
		fxim_server.big.x,     fxim_server.big.y,
		fxim_server.big.width, fxim_server.big.height,
		bd_width, border_pixel, text_background_pixel);
    XStoreName( display, preedit_root, "RootPreedit");

    bzero( &hints, sizeof(XSizeHints));
    hints.flags = USPosition | USSize | PPosition | PSize
	| PMinSize | PMaxSize | PBaseSize | PWinGravity;
    hints.x 	      = fxim_server.big.x;
    hints.y 	      = fxim_server.big.y;
    hints.width       = fxim_server.big.width;
    hints.height      = fxim_server.big.height;
    hints.min_width   = fxim_server.big.width;
    hints.min_height  = fxim_server.big.height;
    hints.max_width   = fxim_server.big.width;
    hints.max_height  = fxim_server.big.height;
    hints.base_width  = fxim_server.big.width;
    hints.base_height = fxim_server.big.height;
    hints.win_gravity = gravity;
    XSetWMNormalHints( display, preedit_root, &hints);

    current_preedit = current_client->preedit;
    current_preedit->Bg_Pixmap   = pixmap;

    current_client->preedit_window
    	= XCreateSimpleWindow( display, preedit_root,
		fxim_server.pre_edit.x,    fxim_server.pre_edit.y,
		fxim_server.preedit_width, fxim_server.window_height,
		0, border_pixel, text_background_pixel);
    XStoreName( display, current_client->preedit_window, "XIMPreeditNothing");
    ChangeWindowAttributes( display, current_client->preedit_window,
							 current_preedit );

    FXCreateGC( display, current_client->preedit_window, current_preedit_font,
		foreground_pixel, text_background_pixel );

    if( fxim_server.root_form == ROOT_FORM_2D ) {
        pixmap = XCreatePixmapFromBitmapData( display,
			current_client->window, (char *)gray_bits, gray_width,
			gray_height, root_background_pixel,
			foreground_pixel, default_depth );
        if( pixmap ) {
            XSetWindowBackgroundPixmap( display,
					 current_client->window, pixmap );
            XSetWindowBackgroundPixmap( display, preedit_root, pixmap );
	    fxim_server.pixmap = pixmap;
	}
    }
    else
    if( fxim_server.root_form == ROOT_FORM_3D ) {
        unsigned long higlight_pixel, shadow_pixel;
	XGCValues	  gcvalues;
	AllocSHColor( display, screen, root_background_pixel,
			&shadow_pixel, &higlight_pixel);
	gcvalues.foreground = shadow_pixel;
	fxim_server.shadow_gc
		= XCreateGC( display, root, GCForeground, &gcvalues);
	gcvalues.foreground = higlight_pixel;
	fxim_server.higlight_gc
		= XCreateGC( display, root, GCForeground, &gcvalues);
    }

    return True;
}

Static void
SignalExitFunc()
{
    WarningMessage(8);
    ExitProcess();
}

Static void
SetSignals()
{
#ifdef USE_SIGNAL
    signal(SIGHUP,  SIG_IGN);
    signal(SIGINT,  SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGTERM, (void(*)())SignalExitFunc);
    signal(SIGCLD,  SIG_IGN);
#endif /*USE_SIGNAL*/
    return;
}

Static void
XimEventLoop()
{
    for (;;) {
	XimTransportCheck();
    }
}

Static void
MainLoop()
{
    unsigned long  mask;
    unsigned long  dummy;

    mask = KeyPressMask | FocusChangeMask;
    mask |= EnterWindowMask | LeaveWindowMask | StructureNotifyMask;
    if ((!control_modifiers_flag) || (!control_modifiers_grab))
	mask |= KeyReleaseMask;
    XSelectInput(display, root_client->window, mask);
    XSelectInput(display, root_client->status_window, ExposureMask);
    XSelectInput(display, root_client->preedit_window, ExposureMask);
    XMapWindow(display, root_client->window);
    XMapRaised(display, root_client->status_window);
    root_client->status_visible = 1;

#ifdef CONTROL_MODIFIERS_EXTENSION
    if (control_modifiers_flag) {
	ControlModifiersGetVersion(display, &dummy, &dummy,
				&control_modifiers_state, &dummy);
	ControlModifiersSetMask(display, root_client->window,
				control_modifiers_state);
    }
#endif /*CONTROL_MODIFIERS_EXTENSION*/

    if (control_modifiers_grab) 
        control_modifiers_error_flag = True;

    XimEventLoop();
    return;
}

int
main(argc, argv)
    int		 argc;
    char	*argv[];
{
    register int i;
    int          rc;

    command_name = argv[0];	
    resource_name = argv[0];

    for (i = 1; i < argc; i++) {
	if (!strcmp(argv[i], "-display")) {
	    if(argc > (i + 1) && *argv[i+1] != '-')
		display_name = argv[++i];
	}
        else if (!strcmp(argv[i], "-geometry")) {
	    if(argc > (i + 1) && *argv[i+1] != '-')
        	geometry = argv[++i];
	}

        else if (!strcmp(argv[i], "-none"))
            keybord_type = FKEY_NONE;
        else if (!strcmp(argv[i], "-jis"))
            keybord_type = FKEY_G_JIS;
        else if (!strcmp(argv[i], "-oya"))
            keybord_type = FKEY_G_OYA;
        else if (!strcmp(argv[i], "-jisG"))
            keybord_type = FKEY_G_JIS;
        else if (!strcmp(argv[i], "-jisDS"))
            keybord_type = FKEY_DS_JIS;
        else if (!strcmp(argv[i], "-oyaG"))
            keybord_type = FKEY_G_OYA;
        else if (!strcmp(argv[i], "-oyaDS"))
            keybord_type = FKEY_DS_OYA;
        else if (!strcmp(argv[i], "-oyayubi"))
            keybord_type = FKEY_G_OYA;

        else if (!strcmp(argv[i], "-text_fg")) {
	    if(argc > (i + 1) && *argv[i+1] != '-')
        	text_font_color = argv[++i];
	}
        else if (!strcmp(argv[i], "-text_bg")) {
	    if(argc > (i + 1) && *argv[i+1] != '-')
        	text_background_color = argv[++i];
	}
        else if (!strcmp(argv[i], "-fg")) {
	    if(argc > (i + 1) && *argv[i+1] != '-')
        	foreground_color = argv[++i];
	}
        else if (!strcmp(argv[i], "-bg")) {
	    if(argc > (i + 1) && *argv[i+1] != '-')
        	root_background_color = argv[++i];
	}
        else if (!strcmp(argv[i], "-bd")) {
	    if(argc > (i + 1) && *argv[i+1] != '-')
        	border_color = argv[++i];
	}
        else if (!strcmp(argv[i], "-bw")) {
	    if(argc > (i + 1) && *argv[i+1] != '-')
        	bd_width = atoi( argv[++i]);
	}
        else if (!strcmp(argv[i], "-bp")) {
	    if(argc > (i + 1) && *argv[i+1] != '-')
                background_pixmap = argv[++i];
	}

        else if (!strcmp(argv[i], "-fn")) {
	    if(argc > (i + 1))
        	ascii_kana_font_name = argv[++i];
	}
        else if (!strcmp(argv[i], "-fk")) {
	    if(argc > (i + 1))
        	kanji_font_name = argv[++i];
	}

        else if (!strcmp(argv[i], "-nobeep"))
            no_beep = 1;
        else if (!strcmp(argv[i], "-v"))
            verbose_flag = 1;
        else if (!strcmp(argv[i], "-verbose"))
            verbose_flag = 1;

        else if (!strcmp(argv[i], "-serverSync"))
            server_sync = True;
        else if (!strcmp(argv[i], "-serverSyncNone"))
            server_sync = False;
        else if (!strcmp(argv[i], "-clientSync"))
            client_sync = True;
        else if (!strcmp(argv[i], "-clientSyncNone"))
            client_sync = False;
        else if (!strcmp(argv[i], "-static"))
            event_flow = 0;
        else if (!strcmp(argv[i], "-dynamic"))
            event_flow = 1;
        else if (!strcmp(argv[i], "-backend"))
            back_front = 0;
        else if (!strcmp(argv[i], "-frontend"))
            back_front = 1;

        else if (!strcmp(argv[i], "-proto")) {
	    if(argc > (i + 1) && *argv[i+1] != '-')
        	transport_name = argv[++i];
	}
        else if (!strcmp(argv[i], "-locale")) {
	    if(argc > (i + 1) && *argv[i+1] != '-')
        	xfeoak_locale = argv[++i];
	}

        else if (!strcmp(argv[i], "-Ext")) {
            use_ext_set_event_mask = True;
#ifdef EXT_FORWARD
            use_ext_forward_keyevent = True;
#endif
#ifdef EXT_MOVE
            use_ext_move = True;
#endif
	}
        else if (!strcmp(argv[i], "-noExt")) {
            use_ext_set_event_mask = False;
#ifdef EXT_FORWARD
            use_ext_forward_keyevent = False;
#endif
#ifdef EXT_MOVE
            use_ext_move = False;
#endif
	}
        else if (!strcmp(argv[i], "-ExtSetEventMask"))
            use_ext_set_event_mask = True;
        else if (!strcmp(argv[i], "-noExtSetEventMask"))
            use_ext_set_event_mask = False;
#ifdef EXT_FORWARD
        else if (!strcmp(argv[i], "-ExtForwardKey"))
            use_ext_forward_keyevent = True;
        else if (!strcmp(argv[i], "-noExtForwardKey"))
            use_ext_forward_keyevent = False;
#endif
#ifdef EXT_MOVE
        else if (!strcmp(argv[i], "-ExtMove"))
            use_ext_move = True;
        else if (!strcmp(argv[i], "-noExtMove"))
            use_ext_move = False;
#endif

	else if (!strcmp(argv[i], "-iconic"))
	    icon_status = 1;
        else if (!strcmp(argv[i], "-title")) {
	    title_on = 1;
	    if(argc > (i + 1) && *argv[i+1] != '-')
                title = argv[++i];
	}
        else if (!strcmp(argv[i], "-notitle"))
	    title_on = 0;
        else if (!strcmp(argv[i], "-name")) {
	    if(argc > (i + 1) && *argv[i+1] != '-')
     		resource_name = argv[++i];
	}

        else if (!strcmp(argv[i], "-nofree"))
	    fsm_free = 0;

        else if (!strcmp( argv[i], "-auth"))
            use_auth = True;
#ifdef CONTROL_MODIFIERS_EXTENSION
        else if (!strcmp(argv[i], "-nograb"))
            control_modifiers_grab = 0;
#endif /*CONTROL_MODIFIERS_EXTENSION*/

	else if(!strcmp(argv[i], "-maxClient")) {
	    if(argc > (i + 1) && *argv[i+1] != '-')
		max_client = atoi(argv[++i]);
	}
	else if(!strcmp(argv[i], "-workSize")) {
	    if(argc > (i + 1) && *argv[i+1] != '-')
		kanji_buff_size = atoi(argv[++i]);
	}

	else if(!strcmp(argv[i], "-transVersion")) {
	    if(argc > (i + 1) && *argv[i+1] != '-') {
		i++;
		if((strlen(argv[i]) == 3) &&
		  ((argv[i][0] == '0' &&
		   (argv[i][2] >= '0' && argv[i][2] <= '2')) ||
		   (argv[i][0] == '1' && argv[i][2] == '0')  ||
		   (argv[i][0] == '2' &&
		   (argv[i][2] >= '0' && argv[i][2] <= '1')))){
		    server_major_transport_version = (CARD32)(argv[i][0] - '0');
		    server_minor_transport_version = (CARD32)(argv[i][2] - '0');
		}
	    }
	}
	else if(!strcmp(argv[i], "-divideSize")) {
	    if(argc > (i + 1) && *argv[i+1] != '-')
		boundary_size = atoi(argv[++i]);
	}

	else if (rc = Dic_Argv(argc - i, &argv[i])) {
	    if (--rc > 0)
		i += rc;
	}
	else {
	      ErrorMessage(25, argv[i]);
	      Usage();
	      exit(1);
	}
    }

    XSetErrorHandler(X_ErrorHandler);
    XSetIOErrorHandler(X_IOErrorHandler);

    if (!(display = XOpenDisplay(display_name))) {
	ErrorMessage(1, display_name);
	return -1;
    }
    screen  = DefaultScreen(display);
    root    = RootWindow(display, screen);

    GetDefaults();
    SetColor();

    if (!xfeoak_locale)
	xfeoak_locale = setlocale(LC_CTYPE, "");


    if (!InitialConnectTable(max_client)) {
	fprintf(stderr, "xfeoak: InitialConnectTable error\n");
	return 4;
    }

    if (!InitialIMRecTable(max_client)) {
	fprintf(stderr, "xfeoak: InitialIMRecTable error\n");
	return 4;
    }

    if (!InitialClientTable(max_client)) {
	fprintf(stderr, "xfeoak: InitialClientTable error\n");
	return 4;
    }

    if (InitialKanjiBuff( kanji_buff_size) < 0) {
	fprintf(stderr, "xfeoak: InitialKanjiBuff error\n");
	return 4;
    }

    if (Dic_Open() < 0) {
	fprintf(stderr, "xfeoak: Dic_Open error\n");
	return 4;
    }

    if (!CreateRoot()) {
	fprintf(stderr, "xfeoak: CreateRoot error\n");
	return 4;
    }

    FD_ZERO(&server_fd_mask);
    server_max_fd = 0;
    if (!InitialTransport()) {
	fprintf(stderr, "xfeoak: InitialTransport error\n");
	return 4;
    }

    InitializeProtocol();

    InitializeExtension();

#ifdef XJP
    CreateXjpAtom();
#endif /*XJP*/

    SetSignals();

    MainLoop();
}

Static void
DrawPopRectangle( window, higlight_gc, shadow_gc, x1, y1, w, h )
    GC		higlight_gc, shadow_gc;
    Window	window;
{
    int		x2=x1+w-1, y2=y1+h-1;

    if( fxim_server.root_form == ROOT_FORM_3D ) {
        XDrawLine(display, window, higlight_gc, x1  , y1  , x2  , y1  );
        XDrawLine(display, window, higlight_gc, x1  , y1+1, x1  , y2  );
        XDrawLine(display, window, higlight_gc, x1+1, y1+1, x1+1, y2-1);
        XDrawLine(display, window, shadow_gc, x1  , y2  , x2  , y2    );
        XDrawLine(display, window, shadow_gc, x2-1, y1+1, x2-1, y2    );
        XDrawLine(display, window, shadow_gc, x2  , y1  , x2  , y2    );
    }
    return;
}

Static void
DrawPushRectangle( window, higlight_gc, shadow_gc, x1, y1, w, h )
    GC		higlight_gc, shadow_gc;
    Window	window;
    int		x1,y1,w,h;
{
    int		x2=x1+w-1, y2=y1+h-1;

    if( fxim_server.root_form == ROOT_FORM_3D ) {
        XDrawLine( display, window, shadow_gc, x1  , y1  , x2  , y1   );
        XDrawLine( display, window, shadow_gc, x1+1, y1+1, x2-1, y1+1 );
        XDrawLine( display, window, shadow_gc, x1  , y1+1, x1  , y2   );
        XDrawLine( display, window, shadow_gc, x1+1, y1+1, x1+1, y2-1 );
        XDrawLine( display, window, higlight_gc, x1+1, y2-1, x2  , y2-1 );
        XDrawLine( display, window, higlight_gc, x1  , y2  , x2  , y2   );
        XDrawLine( display, window, higlight_gc, x2-1, y1+1, x2-1, y2   );
        XDrawLine( display, window, higlight_gc, x2  , y1  , x2  , y2   );
    } else {
        XFillRectangle( display, window, higlight_gc, x1+1, y1+1, w-3, h-3 );
        XDrawRectangle( display, window, shadow_gc, x1, y1+1, w-1, h-3 );
    }
    return;
}

void
DisplayRootWidow()
{
    Window	window = root_client->window;
    GC		higlight_gc, shadow_gc;
    FXimFont   *current_status_font;
    Xim_PreeditPropRec *current_preedit;
    Xim_StatusPropRec  *current_status;
    int		root_width  = fxim_server.current->width;
    int		root_height = fxim_server.current->height;

    current_status_font = root_client->status_font;
    current_preedit	 = root_client->preedit;
    current_status	 = root_client->status;
    if( fxim_server.root_form == ROOT_FORM_3D ) {
        higlight_gc = fxim_server.higlight_gc;
        shadow_gc   = fxim_server.shadow_gc;
    } else {
        higlight_gc = current_status_font->reverse_gc;
        shadow_gc   = current_status_font->normal_gc;
    }

    DrawPopRectangle( window, higlight_gc, shadow_gc,
			0,0, root_width, root_height );
    if( fxim_server.root_display_area & ROOT_DISPLAY_PREEDIT )
        DrawPushRectangle( window, higlight_gc, shadow_gc,
			current_preedit->Area.x - DRAW_XOFFSET1,
			current_preedit->Area.y - DRAW_YOFFSET1,
			current_preedit->Area.width  + DRAW_XOFFSET1*2,
			current_preedit->Area.height + DRAW_YOFFSET1*2
						- UNDER_LINE_HEIGHT);

    if( fxim_server.root_display_area & ROOT_DISPLAY_STATUS )
        DrawPushRectangle( window, higlight_gc, shadow_gc, 
			current_status->Area.x - DRAW_XOFFSET1,
			current_status->Area.y - DRAW_YOFFSET1,
			current_status->Area.width + DRAW_XOFFSET1*2,
			current_status->Area.height + DRAW_YOFFSET1*2
						- UNDER_LINE_HEIGHT);
    DisplayStatus( root_client );
    return;
}

void
DisplayRootPreedit()
{
    GC          higlight_gc, shadow_gc;

    if( fxim_server.root_form == ROOT_FORM_3D ) {
	higlight_gc = fxim_server.higlight_gc;
	shadow_gc   = fxim_server.shadow_gc;
    } else {
	higlight_gc = root_client->preedit_font->reverse_gc;
	shadow_gc   = root_client->preedit_font->normal_gc;
    }
    root_client->preedit_visible = 1;
    DrawPopRectangle( preedit_root, higlight_gc, shadow_gc,
	 	0, 0, fxim_server.big.width, fxim_server.big.height );
    DrawPushRectangle( preedit_root, higlight_gc, shadow_gc,
		root_client->preedit->Area.x - DRAW_XOFFSET1,
		root_client->preedit->Area.y - DRAW_YOFFSET1,
		root_client->preedit->Area.width  + DRAW_XOFFSET1 * 2,
		root_client->preedit->Area.height + DRAW_YOFFSET1 * 2
						 - UNDER_LINE_HEIGHT);
    return;
}

#ifdef CONTROL_MODIFIERS_EXTENSION
Bool
CheckControlModifiers(event)
    XEvent	*event;
{
    if (control_modifiers_flag && event->type == control_modifiers_event) {
	if ((!focus_in_client) || (!focus_in_client->henkan_mode))
	    DisplayStatus(root_client);
	return True;
    }
    return False;
}

int
GetKeyMapMode()
{
    int			 shift_mode;

    ControlModifiersGetState(display, &control_modifiers_state);
    if(control_modifiers_state & CModKanaLockMask)
	shift_mode = 1;
    else
	shift_mode = (control_modifiers_state & LockMask) ? 2: 3; 
    return shift_mode;
}
#endif /*CONTROL_MODIFIERS_EXTENSION*/

int
ChangeShiftMode(sw)
    int			sw;	/* 0: reset Map, 1: set Map 2: change Map */
{
#ifdef CONTROL_MODIFIERS_EXTENSION
    unsigned long	mask, change;

    mask = (CModKanaLockMask | LockMask);

    if(sw > 0) {		/* set Map  1 or 2		*/
	if (sw == 1) {
 	    if (control_modifiers_grab)
	        ControlModifiersGrab(display);
            ControlModifiersGetState(display, &(current_client->old_state));
	}

	change = 0;
	if (current_client->romaji_mode == 0)
	    change = current_client->siftmode & FS_ASCII ? 0: CModKanaLockMask;
	if (current_client->siftmode == FS_UPPER_ASCII)
	    change |= LockMask;
    } else {
 	if (control_modifiers_grab)
	     ControlModifiersUngrab(display);
	change = current_client->old_state;
    }
    ControlModifiersChangeState(display, mask, change);
#endif /*CONTROL_MODIFIERS_EXTENSION*/
}
