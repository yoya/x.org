/* @(#)KanjiWork.c      6.1 94/06/01 20:04:28 FUJITSU LIMITED. */
/*

Copyright (c) 1991, 1993, 1994  FUJITSU LIMITED

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

*/

/*
 *  xfeoak : Input Server (kana-kanji covertion IM Server)
 */

#include <stdio.h>
#include <malloc.h>

#ifndef       AIXV3
typedef unsigned short ushort;
#endif
#include "KanjiTbl.h"

char	*top_kanji_buff;
char	*end_kanji_buff;
char	*current_kanji_buff;

int
InitialKanjiBuff(size)
int	size;
{
	extern char *command_name;
	top_kanji_buff = (char *)malloc(size);
	if(top_kanji_buff == 0) {
		fprintf(stderr, "%s : error malloc (kanji buff)\n",command_name);
		return( -1);
		}
	current_kanji_buff = top_kanji_buff;
	end_kanji_buff = top_kanji_buff + size;
	return(0);
}

char *
GetKanjiBuff(size)
int	size;
{
	char	*new_kanji_bufp;

	size = (size + 7) & ~7;

	if( (current_kanji_buff + size) >= end_kanji_buff )
		return((char *)0);
	new_kanji_bufp = current_kanji_buff;
	current_kanji_buff += size;
	return(new_kanji_bufp);
}

int
FreeKanjiBuff()
{
	current_kanji_buff = top_kanji_buff;
}


int
InitialKanjiItem()
{
	top_select_kanji_line = (SelectKanjiLine *)GetKanjiBuff(sizeof(SelectKanjiLine));
	end_select_kanji_line = top_select_kanji_line;
	top_select_kanji_line->item_no  = 0;
	top_select_kanji_line->next_line = (SelectKanjiLine *)0;
	top_select_kanji_line->previous_line = (SelectKanjiLine *)0;

	select_kanji_item_no = 0;
	top_select_kanji_item = (SelectKanjiItem *)0;
	end_select_kanji_item = (SelectKanjiItem *)0;
}

int
AddKanjiItem(length,wbuff)
int	length;
ushort	*wbuff;
{
	SelectKanjiItem	*get_kanji_item, *current_kanji_item;
	int	size;

	for( current_kanji_item=top_select_kanji_item;
	     current_kanji_item>(SelectKanjiItem *)0;
	     current_kanji_item=current_kanji_item->next_item ) {
	    if( current_kanji_item->kanji_length == length ) {
		if( memcmp( current_kanji_item->kanji_data, wbuff, length*2) == 0 ) {
		    return( 1);
		    }
		}
	    }

	size = length*2 - 2 + sizeof(SelectKanjiItem);
	get_kanji_item = (SelectKanjiItem *)GetKanjiBuff(size);
	if(!get_kanji_item)
	    return(-1);	
	if( end_select_kanji_item )
	    end_select_kanji_item->next_item = get_kanji_item;
	else {
	    top_select_kanji_item = get_kanji_item;
	    top_select_kanji_line->top_item = get_kanji_item;
	    }
	get_kanji_item->next_item = (SelectKanjiItem *)0;
	get_kanji_item->kanji_length = length;
	CopyUshort( get_kanji_item->kanji_data, wbuff, length);
	select_kanji_item_no++;
	end_select_kanji_item = get_kanji_item;
	return(0);
}
