/* @(#)TransTbl.h      6.1 94/06/01 20:04:28 FUJITSU LIMITED. */
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

typedef int (*FtActionProc)();
typedef unsigned long FtArg;

typedef struct _FtActionRec {
	char		*name;
	FtActionProc	proc;
} FtActionRec;

typedef struct _FtModify {
	char	*name;
	int	state;
} FtModify;

#define	FtKey		0		/* KeyPress		*/
#define	FtKeyUp		(1<<16)		/* KeyRelease		*/
#define	FtKeyUpDown	(1<<17)		/* KeyPress | KeyRelease*/

#define FT_ASCII	0x7FFF01
#define FT_KANA		0x7FFF02
#define FT_NUM		0x7FFF03

typedef struct _FtTransTable {
	struct _FtTransTable *next_item;
	char	*key;
	unsigned int state;
	KeySym	keysym;
	FtActionProc proc;
	FtArg	arg;
} FtTransTable;

typedef struct _FtTransMode {			
	struct _FtTransTable *hiragana;
	struct _FtTransTable *katakana;
	struct _FtTransTable *bigascii;
	struct _FtTransTable *smallascii;
	struct _FtTransTable *henkanoff;
} FtTransMode;

typedef struct _ConversionSW {
    char	*modifier_name;
    char	*modifierMask_name;
    char	*keysym_name;
} ConversionSW;

#define FK_HIRAGANA	1
#define FK_KATAKANA	2
#define FK_UPPER_ASCII	3
#define FK_LOWER_ASCII	4
#define FK_ROMA		6
#define FK_HANKAKU	7
#define FK_NIHONGO	8
#define FK_ROMA_ON	10
#define FK_ROMA_OFF	11
#define FK_NIHONGO_ON	12
#define FK_NIHONGO_OFF	13
#define FK_KANA_ON	14
#define FK_KANA_OFF	15

extern int	Fj_PutKana();
extern int	Fj_PutAscii();
extern int	Fj_Henkan();
extern int	Fj_Muhenkan();
extern int	Fj_Maekouho();
extern int	Fj_HenkanCancel();
extern int	Fj_HenkanExecute();
extern int	Fj_HenkanExecuteLinefeed();
extern int	Fj_BackSpace();
extern int	Fj_Tab();
extern int	Fj_CursorMoveTop();
extern int	Fj_CursorMoveBottom();
extern int	Fj_CursorMoveFoward();
extern int	Fj_CursorMoveBackward();
extern int	Fj_DeleteChar();
extern int	Fj_InsertChar();
extern int	Fj_SelectKanji();
extern int	Beep();
extern int	Fj_ChangeMode();
extern int	Fj_SendEvent();

extern FtTransMode    *transmode;
extern FtTransMode    default_jis_mode;
extern FtTransMode    default_oak_mode;
extern FtTransMode    default_non_mode;

extern ConversionSW   conversionSW[];
extern unsigned long  C_key[];
extern int            C_key_count;
