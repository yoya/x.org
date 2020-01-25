/* @(#)TransTbl.c      6.1 94/06/01 20:04:28 FUJITSU LIMITED. */
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

*/

/*
 *  xfeoak : Input Server (kana-kanji covertion IM Server)
 */

#include <X11/Xlib.h>
#include <X11/keysym.h>

#include "Xfeoak.h" 
#include "TransTbl.h"

#ifndef NULL
#define NULL	0
#endif  /* NULL */

FtTransTable	def_common_func3[] = {
    {&def_common_func3[1],
			"FtKeyUpDown@BackSpace", 0, 0,
    			Fj_CursorMoveBackward	,0L	},
    {&def_common_func3[2],
			"FtKeyUpDown@Left", 0, 0,
    			Fj_CursorMoveBackward	,0L	},
    {&def_common_func3[3],
			"FtKeyUpDown@Right", 0, 0,
    			Fj_CursorMoveFoward	,0L	},
    {&def_common_func3[4],
			"FtKeyUpDown@Up", 0, 0,
    		 	Fj_CursorMoveTop	,0L	},
    {&def_common_func3[5],
			"FtKeyUpDown@Down", 0, 0,
    			Fj_CursorMoveBottom	,0L	},
    {&def_common_func3[6],
	/* TAB */	"FtKeyUpDown@KP_Tab", 0, 0,
    			Fj_CursorMoveBottom	,0L	},
    {NULL,
    	/* TAB */	"FtKeyUpDown@Tab", 0, 0,
    			Fj_CursorMoveBottom	,0L	},
};

FtTransTable	def_common_func2[] = {
    {&def_common_func2[1],
    	          	"@Kanji", 0, 0,
    			Fj_Henkan	,0L	},
    {&def_common_func2[2],
    	            	"@Muhenkan", 0, 0,
    			Fj_Muhenkan	,0L	},
    {&def_common_func2[3],
     	              	"Ctrl@Kanji", 0, 0,
    			Fj_SelectKanji	,0L	},
    {&def_common_func2[4],
    	            	"Ctrl@Muhenkan", 0, 0,
    			Fj_Maekouho	,0L	},
    {&def_common_func2[5],
    			"@Cancel", 0, 0,
    			Fj_HenkanCancel	,0L	},
    {&def_common_func2[6],
    	/* ESC */	"@Escape", 0, 0,
    			Fj_HenkanCancel	,0L	},
    {&def_common_func2[7],
    	/* DEL */	"@Delete", 0, 0,
    			Fj_DeleteChar	,0L	},
    {&def_common_func2[8],	
    			"@F28", 0, 0,
    			Fj_DeleteChar	,0L	},
    {&def_common_func2[9],	
    			"@Insert", 0, 0,
    			Fj_InsertChar	,0L	},
    {&def_common_func2[10],	
    			"Ctrl@Insert", 0, 0,
    			Fj_InsertChar	,0L	},
    {&def_common_func2[11],	
    			"FtKeyUpDown@Linefeed", 0, 0,
    			Fj_HenkanExecuteLinefeed	,0L	},
    {&def_common_func2[12],	
    			"@Execute", 0, 0,
    			Fj_HenkanExecute		,0L	},
    {&def_common_func2[13],	
    			"Ctrl@space", 0, 0,
    			Fj_HenkanExecute		,0L	},
    {&def_common_func2[14],	
    			"@KP_Enter", 0, 0,
    			Fj_HenkanExecute		,0L	},
    {&def_common_func2[15],
    			"FtKeyUpDown@Return", 0, 0,
    			Fj_HenkanExecuteLinefeed	,0L	},
    {&def_common_func2[16],
     	              	"Shift@Kanji", 0, 0,
    			Fj_SelectKanji	, 1L	},
    {def_common_func3,	
    			"#ConversionSW", 0, 0,
    			Fj_HenkanExecute		,0L	},
};

FtTransTable	def_common_func[] = {
    {&def_common_func[1],
    			"@FT_ASCII", 0, 0,
    			Fj_PutAscii,	0L	},
    {&def_common_func[2],
    			"Shift@FT_ASCII", 0, 0,
    			Fj_PutAscii,	1L	},
    {&def_common_func[3],
    			"@FT_KANA", 0, 0,
    			Fj_PutKana,	0L	},
    {def_common_func2,
    			"Shift@FT_KANA", 0, 0,
    			Fj_PutKana,	1L	},
};


FtTransTable	def_common_mod[] = {
    {&def_common_mod[1],
    			"@Zenkaku_Hankaku", 0, 0,
    			Fj_ChangeMode,	FK_HANKAKU	},
    {&def_common_mod[2],
    			"RomajiMask@Romaji", 0, 0,
    			Fj_ChangeMode,	FK_ROMA		},
    {&def_common_mod[3],
    			"@Kanji", 0, 0,
    			Fj_ChangeMode,	FK_HIRAGANA	},
    {def_common_func,
    			"@Muhenkan", 0, 0,
    			Fj_ChangeMode,	FK_HIRAGANA	},
};

FtTransTable	def_non_henkanoff_mode[] = {
    {&def_non_henkanoff_mode[1],
    			"@FT_ASCII", 0, 0,
    			Fj_ChangeMode,	FK_KANA_OFF	},
    {&def_non_henkanoff_mode[2],
    			"Shift@FT_ASCII", 0, 0,
    			Fj_ChangeMode,	FK_KANA_OFF	},
    {&def_non_henkanoff_mode[3],
    			"FtKeyUp@Kana_Lock", 0, 0,
    			Fj_ChangeMode,	FK_KANA_OFF	},
    {&def_non_henkanoff_mode[4],
    			"@XK_Kana_Lock", 0, 0,
    			Fj_ChangeMode,	FK_KANA_ON	},
    {&def_non_henkanoff_mode[5],
    			"@FT_KANA", 0, 0,
    			Fj_ChangeMode,	FK_KANA_ON	},
    {NULL,
    			"Shift@FT_KANA", 0, 0,
    			Fj_ChangeMode,	FK_KANA_ON	},
};

FtTransTable	def_non_common_mod[] = {
    {&def_non_common_mod[1],
    			"@F2", 0, 0,
    			Fj_ChangeMode,	FK_HANKAKU	},
    {&def_non_common_mod[2],
    			"@F3", 0, 0,
    			Fj_ChangeMode,	FK_ROMA		},
    {&def_non_common_mod[3],
    			"@FT_ASCII", 0, 0,
    			Fj_ChangeMode,	FK_KANA_OFF	},
    {&def_non_common_mod[4],
    			"Shift@FT_ASCII", 0, 0,
    			Fj_ChangeMode,	FK_KANA_OFF	},
    {&def_non_common_mod[5],
    			"@FT_KANA", 0, 0,
    			Fj_ChangeMode,	FK_KANA_ON	},
    {&def_non_common_mod[6],
    			"Shift@FT_KANA", 0, 0,
    			Fj_ChangeMode,	FK_KANA_ON	},

    {&def_non_common_mod[7],
    			"Ctrl@h", 0, 0,
    			Fj_Henkan	,0L	},
    {&def_non_common_mod[8],
    			"Ctrl@m", 0, 0,
    			Fj_Muhenkan	,0L	},
    {&def_non_common_mod[9],
     			"Ctrl@w", 0, 0,
    			Fj_SelectKanji	,0L	},
    {&def_non_common_mod[10],
    			"Ctrl@p", 0, 0,
    			Fj_Maekouho	,0L	},
    {&def_non_common_mod[11],
    			"Ctrl@u", 0, 0,
    			Fj_HenkanCancel	,0L	},
    {&def_non_common_mod[12],
    			"Ctrl@d", 0, 0,
    			Fj_DeleteChar	,0L	},
    {&def_non_common_mod[13],
    			"Ctrl@i", 0, 0,
    			Fj_InsertChar	,0L	},
    {&def_non_common_mod[14],
    			"Ctrl@l", 0, 0,
    			Fj_HenkanExecute	,0L	},

    {&def_non_common_mod[15],
    			"FtKeyUpDown:Ctrl@b", 0, 0,
    			Fj_CursorMoveBackward	,0L	},
    {&def_non_common_mod[16],
    			"FtKeyUpDown:Ctrl@f", 0, 0,
    			Fj_CursorMoveFoward	,0L	},
    {&def_non_common_mod[17],
    			"FtKeyUpDown:Ctrl@a", 0, 0,
    		 	Fj_CursorMoveTop	,0L	},
    {def_common_mod,
    			"FtKeyUpDown:Ctrl@e", 0, 0,
    			Fj_CursorMoveBottom	,0L	},
};

FtTransTable	def_non_hira[] = {
    {&def_non_hira[1],
    			"@F4", 0, 0,
    			Fj_ChangeMode,	FK_UPPER_ASCII	},
    {def_non_common_mod,
    			"@F1", 0, 0,
    			Fj_ChangeMode,	FK_KATAKANA	},
};

FtTransTable	def_non_kana[] = {
    {&def_non_kana[1],
    			"@F4", 0, 0,
    			Fj_ChangeMode,	FK_UPPER_ASCII	},
    {def_non_common_mod,
    			"@F1", 0, 0,
    			Fj_ChangeMode,	FK_HIRAGANA	},
};

FtTransTable	def_non_upper[] = {
    {&def_non_upper[1],
    			"@F4", 0, 0,
    			Fj_ChangeMode,	FK_LOWER_ASCII	},
    {def_non_common_mod,
    			"@F1", 0, 0,
    			Fj_ChangeMode,	FK_HIRAGANA	},
};

FtTransTable	def_non_lower[] = {
    {&def_non_lower[1],
    			"@F4", 0, 0,
    			Fj_ChangeMode,	FK_UPPER_ASCII	},
    {def_non_common_mod,
    			"@F1", 0, 0,
    			Fj_ChangeMode,	FK_HIRAGANA	},
};

FtTransTable	def_jis_hira[] = {
    {&def_jis_hira[1],
    			"@Eisu_toggle",	 0, 0,
    			Fj_ChangeMode,	FK_UPPER_ASCII	},
    {def_common_mod,
    			"@Hiragana_Katakana", 0, 0,
    			Fj_ChangeMode,	FK_KATAKANA	},
};

FtTransTable	def_jis_kana[] = {
    {&def_jis_kana[1],
    			"@Eisu_toggle", 0, 0,
    			Fj_ChangeMode,	FK_UPPER_ASCII	},
    {def_common_mod,
    			"@Hiragana_Katakana", 0, 0,
    			Fj_ChangeMode,	FK_HIRAGANA	},
};

FtTransTable	def_jis_upper[] = {
    {&def_jis_upper[1],
    			"@Eisu_toggle", 0, 0,
    			Fj_ChangeMode,	FK_LOWER_ASCII	},
    {&def_jis_upper[2],
    			"Ctrl:Shift@Eisu_toggle", 0, 0,
    			Fj_ChangeMode,	FK_LOWER_ASCII	},
#ifdef FUJITSU_UXP
    {&def_jis_upper[3],
    			"@Caps_Lock", 0, 0,
    			Fj_ChangeMode,	FK_LOWER_ASCII	},
#endif /*FUJITSU_UXP*/
    {def_common_mod,
    			"@Hiragana_Katakana", 0, 0,
    			Fj_ChangeMode,	FK_HIRAGANA	},
};

FtTransTable	def_jis_lower[] = {
    {&def_jis_lower[1],
    			"@Eisu_toggle", 0, 0,
    			Fj_ChangeMode,	FK_UPPER_ASCII	},
    {&def_jis_lower[2],
    			"Ctrl:Shift@Eisu_toggle", 0, 0,
    			Fj_ChangeMode,	FK_UPPER_ASCII	},
#ifdef FUJITSU_UXP
    {&def_jis_lower[3],
    			"@Caps_Lock", 0, 0,
    			Fj_ChangeMode,	FK_UPPER_ASCII	},
#endif /*FUJITSU_UXP*/
    {def_common_mod,
    			"@Hiragana_Katakana", 0, 0,
    			Fj_ChangeMode,	FK_HIRAGANA	},
};

FtTransTable	def_oak_hira[] = {
    {&def_oak_hira[1],
    			"@Eisu_toggle",	 0, 0,
    			Fj_ChangeMode,	FK_UPPER_ASCII	},
    {def_common_mod,
    			"@Hiragana_Katakana", 0, 0,
    			Fj_ChangeMode,	FK_KATAKANA	},
};

FtTransTable	def_oak_kana[] = {
    {def_common_mod,
    			"@Eisu_toggle", 0, 0,
    			Fj_ChangeMode,	FK_UPPER_ASCII	},
};

FtTransTable	def_oak_upper[] = {
    {&def_oak_upper[1],
    			"Ctrl@Hiragana_Katakana", 0, 0,
    			Fj_ChangeMode,	FK_LOWER_ASCII	},
#ifdef FUJITSU_UXP
    {&def_oak_upper[2],
    			"@Caps_Lock", 0, 0,
    			Fj_ChangeMode,	FK_LOWER_ASCII	},
#endif /*FUJITSU_UXP*/
    {def_common_mod,
    			"@Hiragana_Katakana", 0, 0,
    			Fj_ChangeMode,	FK_LOWER_ASCII	},
};

FtTransTable	def_oak_lower[] = {
    {&def_oak_lower[1],
    			"Ctrl@Eisu_toggle", 0, 0,
    			Fj_ChangeMode	,FK_UPPER_ASCII	},
#ifdef FUJITSU_UXP
    {&def_oak_lower[2],
    			"@Caps_Lock", 0, 0,
    			Fj_ChangeMode,	FK_UPPER_ASCII	},
#endif /*FUJITSU_UXP*/
    {def_common_mod,
    			"@Eisu_toggle", 0, 0,
    			Fj_ChangeMode	,FK_UPPER_ASCII	},
};

FtTransMode	default_non_mode =
    {def_non_hira, def_non_kana, def_non_upper, def_non_lower,
    	 def_non_henkanoff_mode };

FtTransMode	default_jis_mode =			/* JIS*/
    {def_jis_hira, def_jis_kana, def_jis_upper, def_jis_lower, NULL};

FtTransMode	default_oak_mode =			/* oyaubi	*/
    {def_oak_hira, def_oak_kana, def_oak_upper, def_oak_lower, NULL};

FtTransMode	*transmode = &default_non_mode;

ConversionSW	conversionSW[] = {
     /* modifier */	/* modifier mask */	/* keysym */
    {"",		"Ctrl:Shift", 		"ConversionSW"},
    {"Ctrl",		"Ctrl:Shift",		"space"},
    {(char *)0,		(char *)0,		(char *)0},
};

unsigned long	C_key[(sizeof(conversionSW)/sizeof(conversionSW[0])) * 3];
