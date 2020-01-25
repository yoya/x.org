/* @(#)KanjiTbl.h      6.1 94/06/01 20:04:28 FUJITSU LIMITED. */
/*

Copyright (c) 1991  FUJITSU LIMITED

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

*/

/*
 *  xfeoak : Input Server (kana-kanji covertion IM Server)
 */

typedef struct _SelectKanjiItem {
	struct _SelectKanjiItem *next_item;
	short			kanji_length;	
	unsigned short		kanji_data[1];
	} SelectKanjiItem;

typedef struct _SelectKanjiLine {
	SelectKanjiItem		*top_item;	
	struct _SelectKanjiLine *next_line;
	struct _SelectKanjiLine *previous_line;	
	int			item_no;	
	} SelectKanjiLine;

typedef struct _SelectKanjiDisplayItem {
	int			x;
	int			y;
	int			width;
	SelectKanjiItem		*select_kanji_item_pointer;
	} SelectKanjiDisplayItem;

extern int 			height_select_kanji_display_item;
extern int 			select_kanji_item_no;
extern SelectKanjiItem 		*top_select_kanji_item;
extern SelectKanjiItem 		*end_select_kanji_item;
extern int 			select_kanji_display_item_no;
extern SelectKanjiDisplayItem 	select_kanji_display_item[10];

extern SelectKanjiLine 		*top_select_kanji_line;
extern SelectKanjiLine 		*end_select_kanji_line;
extern SelectKanjiLine 		*current_select_kanji_line;

extern char  			*top_kanji_buff;
extern char  			*end_kanji_buff;
