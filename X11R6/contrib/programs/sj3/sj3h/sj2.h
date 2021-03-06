/*
 * Copyright (c) 1994  Sony Corporation
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL SONY CORPORATION BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * Except as contained in this notice, the name of Sony Corporation
 * shall not be used in advertising or otherwise to promote the sale, use
 * or other dealings in this Software without prior written authorization
 * from Sony Corporation.
 *
 */

/*
 * $SonyRCSfile: sj2.h,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:03:51 $
 */



#include "wchar16.h"
#include "sj.h"


#define MODE_ZEN	0x10		
#define MODE_CONV	0x04		
#define MODE_ALPHA	0x02		
#define MODE_KATA	0x01		
#define MODE_CODE	0x08 + MODE_ZEN	
#define CODE_SJIS	0		
#define CODE_JIS	1		
#define CODE_JIS2       2               
#define CODE_EUC	3		
#define CODE_EUC2       4               
#define CODE_KUTEN	5		
#define CODE_KUTEN2	6		
#define OMODE_ZEN	0x80		
#define OMODE_MASK	0x1f		

#define MODE_HALPHA	MODE_ALPHA		
#define MODE_ZALPHA	MODE_ZEN + MODE_ALPHA	
#define MODE_HKATA	MODE_CONV		
#define MODE_ZHIRA	MODE_ZEN + MODE_CONV	
#define MODE_ZKATA	MODE_ZHIRA + MODE_KATA	



#ifdef NOTDEF	
#define	OTHER	0
#define	NEWS	1
#define	ONEWS	2
#define	JTERM	3
#define OJTERM	4
#define	NWP511	5
#define	NWP517	6
#endif 

#define OTHER_KB	0
#define NEWS_KB		1

#define MINCOL	72



#define STDIN	0
#define STDOUT	1



#define BS	'\010'
#define ESC	'\033'
#define DEL	'\177'



#define UNBUFF	0
#define BUFF	1



#define TRUE	0
#define FALSE	-1



struct	wordent {
		wchar16_t word_str[MAXWORD];
	};


struct valtbl {
	wchar16_t *keyword;
	int  ivalue;
};

#define aprintf	printf
