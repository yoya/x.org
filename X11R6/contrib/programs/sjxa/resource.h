/* $SonyId: resource.h,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
/******************************************************************

Copyright (c) 1992, 1993, 1994  Sony Corporation

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
#ifndef	_Resource_H
#define	_Resource_H
#include	"sjxa.h"
#include	"Ximp.h"

typedef struct {
    XtTranslations	 accelerators;
    XtTranslations	 base_accelerators;
    String		 sjxa;
    String		 function_items;
    int			 function_par_length;
    String		 function_label[10];
    String 		 hankaku_alpha;
    String		 zenkaku_alpha;
    String		 hankaku_katakana;
    String		 zenkaku_katakana;
    String		 zenkaku_hiragana;
    String 		 lock_hankaku_alpha;
    String		 lock_zenkaku_alpha;
    String		 lock_hankaku_katakana;
    String		 lock_zenkaku_katakana;
    String		 lock_zenkaku_hiragana;
    String		 sjis;
    String		 euc;
    String		 jis;
    String		 kuten;
    String		 supplementary_euc;
    String		 supplementary_jis;
    String		 supplementary_kuten;
    String		 code_sjis;
    String		 code_euc;
    String		 code_jis;
    String		 code_kuten;
    String		 server;
    int			 wrap_to_select;
    CharacterCode	 code;
    ConversionMode	 initial_mode;
    String		 malloc_error;
    String		 supplementary_kanji_error;
    String		 died_error;
    String		 open_error1;
    String		 open_error2;
    String		 open_error3;
    String		 open_error4;
    String		 open_error5;
    String		 open_error6;
    String		 open_error7;
    String		 close_error1;
    String		 close_error2;
    String		 close_error3;
    String		 close_error4;
    String		 close_error5;
    String		 close_error6;
    String		 close_error7;
    String		 close_error8;
    String		 add_error1;
    String		 add_error2;
    String		 add_error3;
    String		 add_error4;
    String		 add_error5;
    String		 add_error6;
    String		 add_error7;
    String		 delete_error1;
    String		 delete_error2;
    String		 delete_error3;
    String		 delete_error4;
    String		 delete_error5;
    String		 delete_error6;
    String		 learning_error1;
    String		 learning_error2;
    String		 conversion_error1;
    String		 hinshi[35];
    XimpKey		*process_start_keys;
    int			 debug;
    Boolean		 sjx_protocol;
    Boolean		 xlc_protocol;
    int			 xim_protocol;
    Boolean		 ximc_protocol;
    Boolean		 auto_replace;
    Boolean		 ignore_preedit_color;
    Boolean		 ignore_status_color;
    Boolean		 conversion_message;
    Boolean		 reset_message;
    Boolean		 kana_mode;
    Boolean		 auto_raise;
} Sjxa, *SjxaP;


#if NeedFunctionPrototypes
#define __P(protos)	protos
#else
#define __P(protos)	()
#endif

Boolean	is_started_key __P(( XKeyEvent * ));
#endif	/* _Resource_H */
