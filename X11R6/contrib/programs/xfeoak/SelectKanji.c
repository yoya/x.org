/* @(#)SelectKanji.c      6.1 94/06/01 20:04:28 FUJITSU LIMITED. */
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

#include <ctype.h>

#include "Xfeoak.h"
#include "KanjiTbl.h"

FXim_Client *select_kanji_lock_client = (FXim_Client *)0;
SelectKanjiDisplayItem	select_kanji_display_item[10];
int	select_kanji_display_item_no = 0;
int	select_kanji_item_no;
int	sel_t_size = 2;
int	height_select_kanji_display_item;

SelectKanjiItem	*top_select_kanji_item;
SelectKanjiItem	*end_select_kanji_item;

SelectKanjiLine	*current_select_kanji_line;
SelectKanjiLine	*top_select_kanji_line;
SelectKanjiLine	*end_select_kanji_line;

int
fj_SelectKanji( type)
	int	type;
	{
	int	num;
	ushort	*buff;

	if( current_client->convert_count != 0 ) {
	    Muhenkan();
	    }

	if( current_client->current_colums <= 0 )
	    return( 0);

	if( select_kanji_lock_client ) {
	    FXim_Client *save_client;
	    save_client = current_client;

	    current_client = select_kanji_lock_client;
	    current_preedit_font = current_client->preedit_font;
	    fj_CancelSelectKanji();

	    current_client = save_client;
	    current_preedit_font = current_client->preedit_font;
	    }
	select_kanji_lock_client = current_client;

	num = Dic_Zenkouho( type, current_client->output_buff,
				  current_client->current_colums);
	if( num == -1 || num == 0 ) {
	    fj_CancelSelectKanji();
	    Beep();
	    }
	else {
	    current_client->select_kanji_mod = 1;
	    UnmapPreeditWindow();
	    SelectKanjiDisplayInit();
	    DisplaySelectKanji();
	    DisplaySelectCursor( current_client->select_kanji_no);
	    }
	}

int
SelectKanjiDisplayInit()
	{
int	max_size;
	int		ii, dx, size;
	SelectKanjiLine	*select_kanji_line_pointer, *select_kanji_line_pointer2;
	SelectKanjiItem	*get_kanji_item;
	Xim_StatusPropRec  *current_status;
	FXimFont    	*current_status_font;

	current_status_font = current_client->status_font;
	if( current_client->style & XIMStatusArea ) {
	    current_status = current_client->status;
	    }
	else{
	    current_status = root_client->status;
	    }

	dx = current_status_font->ascii_kana_font_width / 4;
	max_size = (int)(current_status->Area.width - dx)
		 / (int)current_status_font->kanji_font_width;

	select_kanji_line_pointer = top_select_kanji_line;
	get_kanji_item = select_kanji_line_pointer->top_item;
	for(; get_kanji_item;) {
	    size = 0;
	    select_kanji_line_pointer->item_no = 0;
	    for( ii=1; get_kanji_item&&(ii<=10); ii++) {
		size += sel_t_size + get_kanji_item->kanji_length;
		if( size > max_size ) {
		     break;
		     }
		select_kanji_line_pointer->item_no++;
		get_kanji_item = get_kanji_item->next_item;
		}
	    if( get_kanji_item ) {
		select_kanji_line_pointer2
		  = (SelectKanjiLine *)GetKanjiBuff( sizeof(SelectKanjiLine));
		if( !select_kanji_line_pointer2 )
			return( -1 );
		select_kanji_line_pointer->next_line = select_kanji_line_pointer2;
		select_kanji_line_pointer2->previous_line = select_kanji_line_pointer;
		end_select_kanji_line = select_kanji_line_pointer2;
		select_kanji_line_pointer = select_kanji_line_pointer2;
		select_kanji_line_pointer->top_item = get_kanji_item;
	        select_kanji_line_pointer->next_line = (SelectKanjiLine *)0;
		}
	    }
	SelectCurrentKanji( top_select_kanji_line);
	return(0);
	}

int
SelectCurrentKanji( select_kanji_line_pointer)
	SelectKanjiLine	*select_kanji_line_pointer;
	{
	SelectKanjiItem	*get_kanji_item;
	int		size, size2;
	int		select_y, ii;
	int		dx, dy;
	FXimFont    	*current_status_font;
	Xim_StatusPropRec  *current_status;

	current_status_font = current_client->status_font;
	if( current_client->style & XIMStatusArea ) {
	    current_status = current_client->status;
	    }
	else{
	    current_status = root_client->status;
	    }

	current_select_kanji_line = select_kanji_line_pointer;
	get_kanji_item = select_kanji_line_pointer->top_item;
	current_client->select_kanji_no = 1;
	size = 0;
	dx = current_status_font->ascii_kana_font_width / 4;
	if( dx < 4 ) dx = 4;
	height_select_kanji_display_item = current_status_font->max_font_height;
	dy = (int)(current_status->Area.height - current_status_font->max_font_height + 1) / 2;
	select_y = current_status_font->max_font_ascent + dy;
	select_kanji_display_item_no = current_select_kanji_line->item_no;

	for( ii=0; ii<select_kanji_display_item_no; ii++) {
	    size2 = sel_t_size + get_kanji_item->kanji_length;
	    select_kanji_display_item[ii].x
	      = size * current_status_font->kanji_font_width + dx;
	    select_kanji_display_item[ii].y = select_y;
	    select_kanji_display_item[ii].width
	      = get_kanji_item->kanji_length
	      * current_status_font->kanji_font_width;
	    select_kanji_display_item[ii].select_kanji_item_pointer
	      = get_kanji_item;
	    size += size2;
	    get_kanji_item = get_kanji_item->next_item;
	    }
	}

int
DisplaySelectKanji()
	{
	SelectKanjiItem	*select_kanji_item_pointer;
	char	kanji_length[8];
	int	ii, xx, yy, ww, ii2;
	GC	normal_gc;
	Window	window;
	Xim_StatusPropRec  *current_status;
	FXimFont    *current_status_font;

	current_status_font = current_client->status_font;
	if( current_client->style & XIMStatusArea ) {
	    current_status = current_client->status;
	    }
	else{
	    current_status = root_client->status;
	    }

	if (current_client->style & XIMPreeditNothing) {
	    window = root_client->preedit_window;
	} else {
	    window  = current_client->status_window;
	}
	normal_gc = current_status_font->normal_gc;
	ww = current_status_font->kanji_font_width;

	if (!(current_client->style & XIMPreeditNothing)) {
	    XMapRaised( display, window );
	    XClearWindow( display, window );
	}

	for( ii=0; ii<select_kanji_display_item_no; ii++) {
	    ii2 = ii+1;
	    if( ii2 == 10 ) ii2 = 0;
	    sprintf( kanji_length,"%1d",ii2);
	    xx   = select_kanji_display_item[ii].x;
	    yy   = select_kanji_display_item[ii].y
		 - current_status_font->max_font_descent;
	    select_kanji_item_pointer
	      = select_kanji_display_item[ii].select_kanji_item_pointer;
	    XSetFont( display, normal_gc,
		current_status_font->ascii_kana_font_struct->fid );
	    XDrawImageString( display, window,
			normal_gc, xx, yy, kanji_length, 1);
	    FXDrawImageString( display, window,
			current_status_font, 0, xx+ww, yy,
			select_kanji_item_pointer->kanji_data,
			select_kanji_item_pointer->kanji_length);
	    }
	}

int
DisplaySelectCursor( no)
int	no;
	{
	int 		yy, ww;
	FXimFont    	*current_status_font;
	Window 		 window;

	if (current_client->style & XIMPreeditNothing) {
	    window = root_client->preedit_window;
	} else {
	    window  = current_client->status_window;
	}
	    
	current_status_font = current_client->status_font;
	no--;
	if( no < 0 || no >= select_kanji_display_item_no )  return( -1);
	yy = select_kanji_display_item[no].y - height_select_kanji_display_item;
	ww = current_status_font->kanji_font_width;
	XFillRectangle( display, window, current_status_font->invert_gc,
		select_kanji_display_item[no].x+ww, yy,
		select_kanji_display_item[no].width,
		height_select_kanji_display_item+1);
	return( 0);
	}

int
fj_ExecuteSelectKanji( no)
int	no;
	{
	SelectKanjiItem	*select_kanji_item_pointer;
	int   length;

	if( no == 0 ) no = 10;
	if( no <= 0 || no > select_kanji_display_item_no ) {
	    Beep();
	    return( -1);
	    }
	no--;
	select_kanji_item_pointer
	  = select_kanji_display_item[no].select_kanji_item_pointer;
	if( current_client != root_client)
	    SendBuffByLength( current_client->window,
		select_kanji_item_pointer->kanji_data,
		select_kanji_item_pointer->kanji_length);

	length = current_client->last_colums - current_client->current_colums;
	current_client->last_colums = length;
	length;
	if( length > 0) {
	    ushort   *linebuff1= &(current_client->output_buff[0]);
	    ushort   *linebuff2= &(current_client->output_buff[
						current_client->current_colums]);
	    for( ; length>0; length--)
		*linebuff1++ = *linebuff2++;
	    }
	current_client->reverse_colums = 0;
	current_client->current_colums = 0;

	fj_CancelSelectKanji();
	return(0);
	}

int
fj_CancelSelectKanji()
	{
	current_client->current_colums = current_client->reverse_colums;
	select_kanji_lock_client = (FXim_Client *)0;
	FreeKanjiBuff();
	MapPreeditWindow();
	current_client->select_kanji_mod = 0;
	DisplayStatus( current_client );
	DisplayOutputBuff( 0);
	DisplayCursor();
	return(0);
	}

int
fj_MoveCursorSelectKanji( action)
int	action;
	{
	DisplaySelectCursor( current_client->select_kanji_no);
	switch( action) {
	case KS_TOP:
		current_client->select_kanji_no = 1;
		break;
	case KS_BOTTOM:
		current_client->select_kanji_no = select_kanji_display_item_no;
		break;
	case KS_FOWARD:
		if( (int)current_client->select_kanji_no < select_kanji_display_item_no ) 
		    current_client->select_kanji_no++;
		else {
		    current_client->select_kanji_no = 1;
		    }
		break;
	case KS_BACKWARD:
		if( current_client->select_kanji_no > 1 )
		    current_client->select_kanji_no--;
		else {
		    current_client->select_kanji_no = select_kanji_display_item_no;
		    }
		break;
	case KS_NEXT:
		if( current_select_kanji_line != end_select_kanji_line ) {
		    current_select_kanji_line = current_select_kanji_line->next_line;
		    }
		else {
		    current_select_kanji_line = top_select_kanji_line;
		    }
		SelectCurrentKanji( current_select_kanji_line);
	        DisplaySelectKanji();
		break;
	case KS_PREV:
		if( current_select_kanji_line != top_select_kanji_line ) {
		    current_select_kanji_line = current_select_kanji_line->previous_line;
		    }
		SelectCurrentKanji( current_select_kanji_line);
	        DisplaySelectKanji();
		break;
	    }
	DisplaySelectCursor( current_client->select_kanji_no);
	return(0);
	}

int
fj_InputCode()
	{
	/*** Not Supported !!! ***/
	return(0);
	}
