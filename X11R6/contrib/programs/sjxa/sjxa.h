/* $SonyId: sjxa.h,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
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
#ifndef		_Sjxa_H
#define		_Sjxa_H

#include	<sj3lib.h>
#include	<X11/Intrinsic.h>

#define	CLASS_NAME		"Sjxa"

#define MAX_KEY			512
#define MAX_KANA		256
#define MAX_KANJI		256
#define MAX_DOUON_KANA		64
#define MAX_TOUROKU_YOMI	31
#define MAX_TOUROKU_KANJI	32


#ifndef	Min
#define Min( a, b )     (((a) < (b)) ? (a) : (b))
#endif

typedef enum {
    XIM_COMMON_PROTOCOL,
    XIM_PROTOCOL,
    XLC_PROTOCOL,
    SJX_PROTOCOL
} Protocol;

typedef enum {
    BIGENDIAN		= 0x42,
    LITTLEENDIAN	= 0x6c
} Endian;

typedef enum {
    _SJIS,
    _EUC,
    _JIS,
    _KUTEN,
    _SUPP_EUC,
    _SUPP_JIS,
    _SUPP_KUTEN
} CharacterCode;

typedef enum {
    ZENKAKU_HIRAGANA	= 0x0001,
    HANKAKU_HIRAGANA	= 0x0002,
    ZENKAKU_KATAKANA	= 0x0004,
    HANKAKU_KATAKANA	= 0x0008,
    ZENKAKU_ALPHA	= 0x0010,
    HANKAKU_ALPHA	= 0x0020,
    SJIS		= 0x0040,
    EUC			= 0x0080,
    JIS			= 0x0100,
    KUTEN		= 0x0200,
    SUPP_EUC		= 0x0400,
    SUPP_JIS		= 0x0800,
    SUPP_KUTEN		= 0x1000,
    EXTEND		= 0x8000,
    ZENKAKU		= ZENKAKU_HIRAGANA | ZENKAKU_KATAKANA | ZENKAKU_ALPHA,
    HANKAKU		= HANKAKU_HIRAGANA | HANKAKU_KATAKANA | HANKAKU_ALPHA,
    BASIC_CODE		= SJIS | EUC | JIS | KUTEN,
    SUPP_CODE		= SUPP_EUC | SUPP_JIS | SUPP_KUTEN
} ConversionMode;

typedef enum {
    LC_CTYPE_EUC,
    LC_CTYPE_SHIFTJIS
} LcCtype;

typedef enum {
    /* INIT		*/	INIT		= 0x0001,
    /* INTERMEDIATE	*/	INTERMEDIATE	= 0x0002,
    /* CONVERSION	*/	CONVERSION	= 0x0004,
    /* EDIT		*/	EDIT		= 0x0008,
    /* ADD		*/	ADD		= 0x0010,
    /* DELETE		*/	DELETE		= 0x0020,
    /* SELECT		*/	SELECT		= 0x0040,
    /* CODE		*/	JISX0208	= 0x0080,
				JISX0212	= 0x0100,
				CODE		= 0x0180,
    /* WRAP		*/	WRAP		= 0x0200,
    /* SERVER		*/	SERVER		= 0x0400,
    /* ERROR		*/	ERROR		= 0x0800
} State;

typedef enum {
    BUNSETSU_END		= 0x0000,
    BUNSETSU_KANJI		= 0x0001,
    BUNSETSU_MUHENKAN		= 0x0002,
    BUNSETSU_EDIT		= 0x0004,
    BUNSETSU_ZENKAKU_HIRAGANA	= 0x0010,
    BUNSETSU_HANKAKU_HIRAGANA	= 0x0020,
    BUNSETSU_ZENKAKU_KATAKANA	= 0x0040,
    BUNSETSU_HANKAKU_KATAKANA	= 0x0080,
    BUNSETSU_ZENKAKU_ALPHA	= 0x0100,
    BUNSETSU_HANKAKU_ALPHA	= 0x0200
} Bunsetsu;

typedef struct bunsetu_wcs {
    int			 srclen;
    int			 destlen;
    wchar_t		*srcstr;
    wchar_t		*deststr;
    SJ3_STUDYREC	 dcid;
} SJ3_BUNSETU_WCS;

typedef struct douon_wcs {
    wchar_t		*ddata;
    int			 dlen;
    int			 width;
    SJ3_STUDYREC	 dcid;
} SJ3_DOUON_WCS;

typedef struct syoukyo_wcs {
    wchar_t	*yomi;
    wchar_t	*kanji;
    int		 code;
} SJ3_SYOUKYO_WCS;

typedef struct _RkTable {
    struct _RkTable	*previous;
    struct _RkTable	*next;
    unsigned char	*key;
    wchar_t		*yomi;
    unsigned char	*str;
} RkTable;

typedef struct {
    unsigned int	 point;
    wchar_t		 kanji[MAX_KANJI];
    SJ3_BUNSETU_WCS	 bun[MAX_KANJI];
    Bunsetsu		 bunsetsu[MAX_KANJI];
    unsigned int	 bunsetsu_length[MAX_KANJI];

    int			 wrap_length;

    int			 douon_count;
    int			 douon_point;
    SJ3_DOUON_WCS	*dou;
} Henkan;

typedef struct _Buffer {
    struct _Buffer	*previous;
    struct _Buffer	*next;
    int			 connect;

    unsigned long	 conversion_style;
    Widget		 preedit_widget;
    Widget		 status_widget;
    Widget		 label, box[2];
    Widget		 shell;
    Widget		 list_widget, code_label;
    Widget		 error_shell;

    State		 state;

    Boolean		 mode_lock;
    ConversionMode	 conversion_mode;
    ConversionMode	 conversion_mode_toggle;

    unsigned int	 point;
    unsigned int	 end_point;
    unsigned char	 key[MAX_KEY];
    unsigned char	 str[MAX_KEY];
    unsigned int	 keyC[MAX_KEY];
    wchar_t		 yomi[MAX_KANA];
    unsigned int	 yomiC[MAX_KANA];
    ConversionMode	 mode[MAX_KEY];
    Boolean		 conversion_flag[MAX_KEY];
    unsigned int	 cursor;
    RkTable		*rt;

    Henkan		*henkan;

    Boolean		 use_jisx0212;
    unsigned short	 jisx0208;
    unsigned short	 jisx0212;
} Buffer;

#endif	/* _Sjxa_H */
