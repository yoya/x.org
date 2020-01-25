/* @(#)Display.c      6.1 94/06/01 20:04:27 FUJITSU LIMITED. */
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

#include "Xfeoak.h"
#include <X11/Xutil.h>

int
SetFocusClient()
    {
    FXim_Client		*old_client;
    FXim_Client		*save_current_client;
    if( current_client == focus_in_client ) {
	return( 0 );
	}

    current_preedit_font = current_client->preedit_font;

    if( focus_in_client && focus_in_client->henkan_mode == 1 ) {
	if( control_modifiers_flag ) {
	    save_current_client = current_client;
	    current_client = focus_in_client;
	    ChangeShiftMode( 0);
	    current_client = save_current_client;
	    }
	}

    if( focus_in_client && (focus_in_client->style & XIMStatusNothing) ) {
	old_client = focus_in_client;
	}
    else {
	old_client = root_client;
	}
    if( control_modifiers_flag )
    	XMapRaised( display, root_client->window );

    if( current_client->henkan_mode == 1 ) {
        if( control_modifiers_flag )
	    ChangeShiftMode( focus_in_client ? 2 : 1);
        if( current_client->status_visible == 0 ) {
    	    XMapRaised( display, current_client->status_window);
	    current_client->status_visible = 1;
            }
	MapPreeditWindow();
        focus_in_client = current_client;
        DisplayStatus( current_client );
	Redisplay();
        }
    else {
        XMapRaised( display, root_client->status_window); 
        focus_in_client = (FXim_Client *)0;
        DisplayStatus( current_client );
	root_client->status_visible = 1;
        }
    XFlush( display );
    return( 0);
    }

int
UnsetFocusClient()
    {
    if( focus_in_client == (FXim_Client *)0 ) {
	return( 0);
	}
    if( focus_in_client != current_client ) {
	return( 0);
	}
    XMapRaised( display, root_client->status_window); 
    root_client->status_visible = 1;
    if( current_client->henkan_mode == 1 ) {
    	if( control_modifiers_flag ) 
	    ChangeShiftMode( 0);
    }
    UnmapPreeditWindow();
    focus_in_client = (FXim_Client *)0;
    DisplayStatus( root_client );
    XFlush( display );
    return( 0);
    }

int
MapPreeditWindow()
    {
    if( current_client->henkan_mode == 0
     || ( current_client->preedit_visible != 0
       && current_client->prot_type == FPRT_XIM ) ) 
	return(0);

    if( current_client->select_kanji_mod == 0 ) {
        current_client->preedit_visible = 1;
        if( current_client->style & XIMPreeditPosition) {
            XMapRaised( display, current_client->preedit_window);
	    if( current_client->used_preedit_window_no >= 2 )
                XMapRaised( display, current_client->preedit_window2);
	    }
        else if(!(current_client->style & XIMPreeditNothing)) {
            XMapRaised( display, current_client->preedit_window);
	    }
	}
    return(0);
    }

int
UnmapPreeditWindow()
{
    if (current_client->style & XIMPreeditNothing) {
	XClearWindow( display, root_client->preedit_window);
	return(0);
    }
    if( current_client->preedit_visible == 0 )
	return(0);
    current_client->preedit_visible = 0;

    XUnmapWindow( display, current_client->preedit_window);
    if(current_client->style & XIMPreeditPosition) {
        XUnmapWindow( display, current_client->preedit_window2);
    }
    return(0);
}

int
Redisplay()
    {
    if(current_client->select_kanji_mod == 0) {
        DisplayOutputBuff(0);
	DisplayCursor();
	}
    else if( current_client == focus_in_client ) {
	DisplaySelectKanji();
	DisplaySelectCursor(current_client->select_kanji_no);
	}
    return(0);
    }

int
DisplayOutputBuff()
    {
    int	lx, ly, width;
    int	length, reverseLength;
    Xim_PreeditPropRec	*current_preedit;
    current_preedit_font = current_client->preedit_font;
    if( current_client->style & XIMPreeditPosition) {
	ResizePreeditArea();
	DisplayOutputBuffInPosition();
	return( 0 );
	}
    else
    if(current_client->style & XIMPreeditNothing) 
	current_preedit = root_client->preedit;
    else
	current_preedit = root_client->preedit;

    XClearWindow( display, current_client->preedit_window);
    length = current_client->reverse_colums;
    lx = 0;	
    ly = (int)(current_preedit->Area.height - current_preedit_font->max_font_height + 1) / 2;
    ly += current_preedit_font->max_font_ascent;
    if( length > 0 ) {	
        lx = FXDrawImageString( display, current_client->preedit_window,
		current_preedit_font, 0, lx, ly,
		&(current_client->output_buff[0]), length);
	}
    length = current_client->last_colums - current_client->reverse_colums;
    if( length > 0 ) {	
	lx = FXTextWidth( current_preedit_font, current_client->output_buff,
			  current_client->reverse_colums); 
	FXDrawImageString( display, current_client->preedit_window,
		current_preedit_font, 1, lx, ly,
		&(current_client->output_buff[current_client->reverse_colums]),
		length);
	}
    return( 0 );
    }

int
ResizePreeditArea()
    {
    int		lx;
    Xim_PreeditPropRec	*current_preedit;
    XRectangle	preeddit_area[2], *current_area;
    short	preedit_line_length[2];
    int		area_no, line_no;
    Window	preedit_window[2];

    if( current_client->check_preedit_area_size ) {
        Xim_PreeditPropRec preedit;
        current_preedit = current_client->preedit;
        preedit.Area.width  = 0;
        preedit.Area.height = 0;
        CheckPreeditWidowSize( &preedit );
        if( ( preedit.Area.width  != current_preedit->Area.width )
         || ( preedit.Area.height != current_preedit->Area.height ) ) {
    	    current_preedit->Area.width  = preedit.Area.width;
	    current_preedit->Area.height = preedit.Area.height;
	    current_client->input_flag = FINF_MODY;
	    }
	}

    if((current_client->style & XIMPreeditPosition)
    && (current_client->input_flag != FINF_NONE) ) {
        area_no = FXTextExtentsArea( current_preedit_font,
				     current_client->preedit,
				     current_client->output_buff,
				     current_client->last_colums,
				     preeddit_area,
				     preedit_line_length, &line_no);
	if( area_no == 0 ) {
	    return( -1 );
	    }

        preedit_window[0] = current_client->preedit_window;
        preedit_window[1] = current_client->preedit_window2;

	for( lx = 0; lx < area_no; lx++ ) {
	    if( lx > current_client->used_preedit_window_no 
	    || (preeddit_area[lx].x      != current_client->preedit_area[lx].x)
	    || (preeddit_area[lx].y      != current_client->preedit_area[lx].y)
	    || (preeddit_area[lx].width  != current_client->preedit_area[lx].width)
	    || (preeddit_area[lx].height != current_client->preedit_area[lx].height)) {
	        XMoveResizeWindow( display, preedit_window[lx],
			preeddit_area[lx].x,     preeddit_area[lx].y,
			preeddit_area[lx].width, preeddit_area[lx].height);
		current_client->preedit_area[lx] = preeddit_area[lx];
		}
            if( current_client->henkan_mode == 1 ) {
		current_client->preedit_visible = 1;
	        XMapRaised( display, preedit_window[lx] );
		}
	    else {
	        XRaiseWindow( display, preedit_window[lx] );
		}
	    }

	for( lx = area_no; lx < 2; lx++ ) {
	    XUnmapWindow( display, preedit_window[lx] );
	    bzero( &preeddit_area[lx], sizeof(preeddit_area[0]) );
	    }
	 
        current_client->input_flag = FINF_NONE;
	XFlush( display );
	current_client->used_preedit_window_no = area_no;
	current_client->preedit_line_no = line_no;
	for( lx = 0; lx < 2; lx++ ) {
	    current_client->preedit_line_length[lx] = preedit_line_length[lx];
	    }
	}
    return(0);
    }

int
DisplayOutputBuffInPosition()
	{
	int	ip, line, lx, ly, width, height;
	int	display_length, area_length, draw_length, current_colums;
	Window	preedit_window[3];

        preedit_window[0] = current_client->preedit_window;
        preedit_window[1] = current_client->preedit_window2;
	height = current_preedit_font->max_font_height
	       + current_client->preedit->LineSpacing;

	for( ip = 0; ip < current_client->used_preedit_window_no; ip++ ) {
	    XClearWindow( display, preedit_window[ip] );
	    }

	lx = 0;	
	ly = current_preedit_font->max_font_ascent;
	ip = 0;
	line = 0;
	area_length = current_client->preedit_line_length[ ip ];

	display_length = current_client->reverse_colums;
	current_colums  = 0;
	for( ; ( display_length > 0) && (ip < current_client->used_preedit_window_no); ) {
	    if( display_length > area_length) 
	        draw_length = area_length;
	    else
	        draw_length = display_length;

	    lx = FXDrawImageString( display, preedit_window[ip],
	    		current_preedit_font, 0, lx, ly,
	    		&(current_client->output_buff[current_colums]),
			draw_length);
	    current_colums += draw_length;
	    display_length -= draw_length;
	    area_length -= draw_length;
	    if( area_length <= 0 ) {
	        lx = 0;
	        ip++;
	        if( ip < 2 )
	       	    area_length = current_client->preedit_line_length[ ip ];
		}
	    }

	display_length = current_client->last_colums
		       - current_client->reverse_colums;
	for( ; ( display_length > 0) && (ip < current_client->used_preedit_window_no); ) {
	    if( display_length > area_length) 
	        draw_length = area_length;
	    else
	        draw_length = display_length;

	    lx = FXDrawImageString( display, preedit_window[ip],
	    		current_preedit_font, 1, lx, ly,
			&(current_client->output_buff[current_colums]),
			draw_length);
	    current_colums += draw_length;
	    display_length -= draw_length;
	    area_length -= draw_length;
	    if( area_length <= 0 ) {
	        lx = 0;
	        ip++;
	        if( ip < 2 )
	       	    area_length = current_client->preedit_line_length[ ip ];
		}
	    }
	}

int
DrawStatus( client, status_message, length )
    FXim_Client *client;
    ushort	*status_message;
    int		length;
    {
    int		xx, yy, ww;
    FXimFont	*status_font;
    Xim_StatusPropRec	*status;

    if( client->style & XIMStatusArea ) {
	status = client->status;
	status_font = client->status_font;
	}
    else{
	status = root_client->status;
	status_font = root_client->status_font;
	XMapWindow( display, root_client->window );
        }
    XClearWindow( display, client->status_window);
    ww = FXTextWidth( status_font, status_message, length );
    xx = status->Area.width - ww - bd_width;
    yy = (int)(status->Area.height - status_font->max_font_height + 1) / 2;
    yy += status_font->max_font_ascent;
    FXDrawImageString( display, client->status_window,
			   status_font, 0, xx, yy,
			   status_message, length );
    }

int
DisplayStatus( client )
    FXim_Client *client;
    {
    FXim_Client	*current_client;
    ushort	status_message[21];
    int		i, length;

    current_client = client;
    if( client == root_client ) {
	if( focus_in_client != (FXim_Client *)0 )
	    current_client = focus_in_client;
	}
    if( client->style & XIMStatusNone ) {
	return(0);
    }
    length = GetStatusMessage( client, status_message );

    if( (current_client->select_kanji_mod == 0 )
	|| (current_client->style & XIMPreeditNothing) ) {
	DrawStatus( client, status_message, length);
	}
    if((current_client == focus_in_client )
    && (current_client->status_window != root_client->status_window)) {
	DrawStatus( root_client, status_message, length);
	}
    if(( client == root_client )
    &&  focus_in_client == (FXim_Client *)0
    &&  control_modifiers_flag == 1 ) {                 
	current_client = 0;
	while( current_client = GetClient( current_client )) {
	    if(( current_client->style & XIMStatusArea )
	    && ( current_client->henkan_mode == 0)) {
		DrawStatus( current_client, status_message, length);
		}
	    }
	}
    return(0);
    }

int
DisplayCursor()
    {
    int 	width, hight;
    int 	cx, cy;
    Window	preedit_window;
    Xim_PreeditPropRec	*current_preedit;

    if( focus_in_client != current_client )
	return( 0 );

    if(current_client->style & XIMPreeditPosition) {
        int 	colums, length, lno, wno;
	current_preedit = current_client->preedit;

	length = current_client->current_colums;
	colums = current_client->preedit_line_length[0];
	if( length >= colums ) {
	    length -= colums;
	    preedit_window = current_client->preedit_window2;
	    }
	else {
	    preedit_window = current_client->preedit_window;
	    }

	colums = current_client->current_colums - length;
        cx = FXTextWidth( current_preedit_font,
			 &(current_client->output_buff[colums]), length);
	cy = 0;
	}
    else {
	current_preedit = root_client->preedit;
        cx = FXTextWidth( current_preedit_font, current_client->output_buff,
			  current_client->current_colums);
        cy = (int)(current_preedit->Area.height - current_preedit_font->max_font_height + 1) / 2;
	preedit_window = current_client->preedit_window;
	}
    hight = current_preedit_font->max_font_height;

    if(current_client->current_colums == current_client->last_colums ) {
         width = (current_client->hankakumode == 0) ?
		  current_preedit_font->kanji_font_width :
		  current_preedit_font->ascii_kana_font_width;
    } else
	 width = (current_client->output_buff[current_client->current_colums] > 0xff) ?
		  current_preedit_font->kanji_font_width :
		  current_preedit_font->ascii_kana_font_width;
    width-=2;
    hight-=2;
    cx++;
    cy++;
    XFillRectangle( display, preedit_window, current_preedit_font->invert_gc,
			cx, cy, width, hight);
    return(0);
    }

int
CreatePreeditWindow()
{
    Xim_PreeditPropRec	*current_preedit;

    if ((current_client->style & XIMPreeditCallbacks)
     || (current_client->style & XIMPreeditNone))
	return 0;

    current_preedit_font = current_client->preedit_font;
    current_preedit = current_client->preedit;
    if (!(current_client->window_type & FC_SAME_ROOT_PREEDIT_FONT)
     && !(FXLoadQueryBaseFont(display, current_preedit_font,
			current_client->preedit->FontName))) {
	current_preedit->AreaNeeded.height =
				 current_preedit_font->max_font_height;
	current_preedit->AreaNeeded.width =
				 current_preedit_font->kanji_font_width * 2;
    } else {
	current_client->window_type |= FC_SAME_ROOT_PREEDIT_FONT;
        *current_preedit_font = *root_client->preedit_font;
        current_preedit_font->status = 0;
    }

    if (current_preedit->Foreground == current_preedit->Background) {
	current_preedit->Foreground = root_client->preedit->Foreground;
	current_preedit->Background = root_client->preedit->Background;
    }

    if (current_client->style & XIMPreeditArea) {
        current_client->preedit_window =
	        XCreateSimpleWindow(display, current_client->window,
		  current_preedit->Area.x, current_preedit->Area.y,
		  current_preedit->Area.width, current_preedit->Area.height,
		  0, current_preedit->Foreground, current_preedit->Background);
	XStoreName(display, current_client->preedit_window, "XIMPreedit");
	ChangeWindowAttributes(display,
			 current_client->preedit_window, current_preedit);

    } else if (current_client->style & XIMPreeditNothing) {
	current_client->preedit_window = root_client->preedit_window;
	*current_client->preedit = *root_client->preedit;

    } else if (current_client->style & XIMPreeditPosition) {
	current_client->input_flag = FINF_MODY;
	CreatePreeditPositionWindow(current_client, current_preedit_font);
    }

    if (!(current_client->window_type & FC_SAME_ROOT_PREEDIT_GC)) {
        FXCreateGC(display, current_client->preedit_window,
		 current_preedit_font, current_preedit->Foreground,
		 current_preedit->Background);
    }
    XSelectInput(display, current_client->preedit_window, ExposureMask);
    return 0;
}

int
ChangePreeditWindow(preedit)
    Xim_PreeditPropRec *preedit;
{
    Xim_PreeditPropRec	*current_preedit;
    Window		status_window;
    Bool		case_same_color = False;

    if ((current_client->window_type & FC_SAME_ROOT_PREEDIT)
			  	    == FC_SAME_ROOT_PREEDIT)
        return 0;

    if ((current_client->style & XIMPreeditCallbacks)
     || (current_client->style & XIMPreeditNone))
	return 0;

    current_preedit_font = current_client->preedit_font;
    current_preedit  = current_client->preedit;
    if (current_preedit->Foreground == current_preedit->Background) {
	current_preedit->Foreground = root_client->preedit->Foreground;
	current_preedit->Background = root_client->preedit->Background;
	case_same_color = True;
    }

    if (current_client->style & XIMPreeditArea) {
	if ((preedit->Area.x      != current_preedit->Area.x)
	 || (preedit->Area.y      != current_preedit->Area.y)
	 || (preedit->Area.width  != current_preedit->Area.width)
	 || (preedit->Area.height != current_preedit->Area.height)) {
	    XMoveResizeWindow( display, current_client->preedit_window,
		preedit->Area.x,     preedit->Area.y,
		preedit->Area.width, preedit->Area.height);
	    current_preedit->Area = preedit->Area;
	    current_preedit->SpotLocation.x = current_preedit->Area.x;
	    current_preedit->SpotLocation.y =
			 current_client->preedit_font->max_font_ascent;
	    ResizePreeditArea();
	}
    } else if( current_client->style & XIMPreeditPosition ) {
        CheckPreeditWidowSize(preedit);
	if ((preedit->Area.x         != current_preedit->Area.x)
	 || (preedit->Area.y         != current_preedit->Area.y)
	 || (preedit->Area.width     != current_preedit->Area.width)
	 || (preedit->Area.height    != current_preedit->Area.height)
	 || (preedit->SpotLocation.x != current_preedit->SpotLocation.x)
	 || (preedit->SpotLocation.y != current_preedit->SpotLocation.y)
	 || (preedit->LineSpacing    != current_preedit->LineSpacing)) {
	    if (preedit->SpotLocation.x != 0 || preedit->SpotLocation.y != 0)
	        current_preedit->SpotLocation = preedit->SpotLocation;
	    current_preedit->LineSpacing  = preedit->LineSpacing;
	    current_preedit->Area         = preedit->Area;
	    current_client->input_flag = FINF_MODY;
	    ResizePreeditArea();
	}
    }

    if (current_client->style &
      (XIMPreeditNothing | XIMPreeditArea | XIMPreeditPosition)) {
	if ((preedit->Background  != current_preedit->Background)
	 || (preedit->Foreground  != current_preedit->Foreground)
	 || case_same_color ) {
	    current_preedit->Background = preedit->Background;
	    current_preedit->Foreground = preedit->Foreground;
	    XSetWindowBackground(display, current_client->preedit_window,
					preedit->Background);
    	    FXChangeGC(display, current_client->preedit_window,
			current_preedit_font, current_preedit->Foreground,
			current_preedit->Background);
	}
	current_preedit->colormap = preedit->colormap;
	current_preedit->StdColormap = preedit->StdColormap;
	current_preedit->Bg_Pixmap = preedit->Bg_Pixmap;
	current_preedit->cursor = preedit->cursor;
	current_preedit->AreaNeeded = preedit->AreaNeeded;
	ChangeWindowAttributes(display, current_client->preedit_window,
				current_preedit);
    }
    XClearArea(display, current_client->preedit_window, 0, 0, 0, 0, True);
    return 0;
}

int
ChangePreeditFont()
    {
    char	*work_ascii_kana_font_name, *work_kanji_font_name,
		*work_gaiji_font_name;
    Xim_PreeditPropRec	*reference_preedit, *current_preedit;
    int		rc;
    Window	status_window;

    if( current_client->window_type & ( FC_SAME_ROOT_PREEDIT_GC
				      | FC_SAME_ROOT_PREEDIT_FONT) ) {
        return( 0 );
        }
    if(( current_client->style & XIMPreeditCallbacks )
    || ( current_client->style & XIMPreeditNone )) {
	return( 0 );
	}

    current_preedit_font = current_client->preedit_font;
    current_preedit = current_client->preedit;
    FXFreeFont( display, current_preedit_font );
    rc = FXLoadQueryBaseFont( display, current_preedit_font,
			current_client->preedit->FontName );
    if( rc ) {
	*current_preedit_font = *root_client->preedit_font;
	current_preedit_font->status = 0;
	}
    else {
	current_preedit->AreaNeeded.height = current_preedit_font->max_font_height;
	current_preedit->AreaNeeded.width = current_preedit_font->kanji_font_width * 2;
	}
    FXChangeGC( display, current_client->preedit_window, current_preedit_font,
		current_preedit->Foreground, current_preedit->Background );
    XClearArea( display, current_client->preedit_window, 0, 0, 0, 0, True );
    return( 0 );
    }

int
_CheckStatusArea(client)
    FXim_Client	*client;
{
    Window		 root_return;
    int			 x_return, y_return;
    unsigned int	 width_return, height_return,
			 border_width_return, depth_return;
    int			status_x, status_y, status_width, status_height;

    status_x = client->status->Area.x;
    status_y = client->status->Area.y;
    status_width = client->status->Area.width;
    status_height = client->status->Area.height;
    if (status_width && status_height)
        return 0;

    if (status_height == 0) {
        status_height = client->status_font->max_font_height + 2;
    }
    if (XGetGeometry(display, (Drawable)client->window,
			&root_return, &x_return, &y_return,
			&width_return, &height_return,
			&border_width_return, &depth_return)) {
	if (status_width == 0) {
	    status_width = width_return;
	    status_x = 0;
        }
	status_y = height_return - status_height;
	client->status->Area.x = status_x;
	client->status->Area.y = status_y;
	client->status->Area.width = status_width;
	client->status->Area.height = status_height;
	return 0;
    }
    return -1;
}

int
CreateStatusWindow()
{
    Xim_StatusPropRec	*root_status, *current_status;
    FXimFont		*current_status_font;
    Window		 parent;

    if (current_client->style & XIMStatusNothing) {
	XWMHints    wm_hints;

	bzero(&wm_hints, sizeof(XWMHints));
	wm_hints.initial_state = NormalState;
	wm_hints.flags = StateHint;
	XSetWMHints(display, root_client->window, &wm_hints);
	XMapRaised(display, root_client->window);
    }

    if ((current_client->style & XIMStatusCallbacks)
     || (current_client->style & XIMStatusNone))
	return 0;

    current_status_font = current_client->status_font;
    current_status = current_client->status;
    root_status = root_client->status;
    if (current_status->Foreground == current_status->Background) {
	current_status->Foreground = root_client->status->Foreground;
	current_status->Background = root_client->status->Background;
    }

    if (!(current_client->window_type & FC_SAME_ROOT_STATUS_FONT)
     && !(FXLoadQueryBaseFont(display, current_status_font,
			current_client->status->FontName))) {
	current_status->AreaNeeded.height =
		 current_status_font->max_font_height;
	current_status->AreaNeeded.width =
		 GetMaxStatusMessageLength(current_client)
		 * current_status_font->kanji_font_width / 2;
    } else {
	current_client->window_type |= FC_SAME_ROOT_STATUS_FONT;
	*current_status_font = *root_client->status_font;
	current_status_font->status = 0;
    }

    if (current_client->style & XIMStatusNothing) {
	current_client->status_window = root_client->status_window;
    } else if (current_client->style & XIMStatusArea) {
	if (current_client->ext_ui_statuswindow && current_status->window)
	    parent = current_status->window;
	else
	    parent = current_client->window;

	_CheckStatusArea(current_client);
        current_client->status_window =
	    XCreateSimpleWindow(display, parent,
		current_status->Area.x,	current_status->Area.y,
		current_status->Area.width, current_status->Area.height,
		0, current_status->Foreground, current_status->Background);
	XStoreName(display, current_client->status_window, "XIMStatusArea");
        ChangeWindowAttributes(display, current_client->status_window,
				current_status);
        FXCreateGC(display, current_client->status_window, current_status_font,
		current_status->Foreground, current_status->Background);
    	XMapRaised( display, current_client->status_window);
	current_client->status_visible = 1;
    }

    XSelectInput( display, current_client->status_window, ExposureMask);
    return 0;
}

int
ChangeStatusWindow(status)
    Xim_StatusPropRec	*status;
{
    Xim_StatusPropRec	*current_status;
    FXimFont		*current_status_font;
    Window		 parent;
    Bool		 case_same_color = False;

    if ((current_client->window_type & FC_SAME_ROOT_STATUS)
			  	    == FC_SAME_ROOT_STATUS)
        return 0;

    if ((current_client->style & XIMStatusCallbacks)
     || (current_client->style & XIMStatusNone))
	return 0;

    current_status_font = current_client->status_font;
    current_status = current_client->status;
    if (current_status->Foreground == current_status->Background) {
	current_status->Foreground = root_client->status->Foreground;
	current_status->Background = root_client->status->Background;
	case_same_color = True;
    }

    if (current_client->style & XIMStatusArea) {
	if (current_client->ext_ui_statuswindow
	&& (status->window || status->window != current_status->window)) {
	    current_status->window = status->window;
	    if (status->window) {
	        parent = status->window;
	        current_status->Area.x = status->Area.x;
	        current_status->Area.y = status->Area.y;
	    } else {
	        parent = current_client->window;
	    }
	    XReparentWindow(display, current_client->status_window,
		parent, current_status->Area.x, current_status->Area.y);
	}

	XClearArea(display, current_client->status_window, 0, 0, 0, 0, True);
	if ((status->Area.x      != current_status->Area.x)
	 || (status->Area.y      != current_status->Area.y)
	 || (status->Area.width  != current_status->Area.width)
	 || (status->Area.height != current_status->Area.height)) {
	    current_status->Area.x      = status->Area.x;
	    current_status->Area.y      = status->Area.y;
	    current_status->Area.width  = status->Area.width;
	    current_status->Area.height = status->Area.height;
	    XMoveResizeWindow(display, current_client->status_window,
		status->Area.x,	    status->Area.y,
		status->Area.width, status->Area.height);
	}
    }

    if (current_client->style & (XIMStatusNothing | XIMStatusArea)) {
	if ((status->Foreground != current_status->Foreground)
	 || (status->Background != current_status->Background)
	 || case_same_color) {
	    current_status->Foreground = status->Foreground;
	    current_status->Background = status->Background;
	    XSetWindowBackground(display,
			 current_client->status_window, status->Background);
    	    FXCreateGC(display, current_client->status_window,
			 current_status_font, current_status->Foreground,
			 current_status->Background);
	}
	current_status->colormap = status->colormap;
	current_status->StdColormap = status->StdColormap;
	current_status->Bg_Pixmap = status->Bg_Pixmap;
	current_status->LineSpacing = status->LineSpacing;
	current_status->cursor = status->cursor;
        ChangeWindowAttributes(display,
			 current_client->status_window, current_status);
    }
    return 0;
}

int
ChangeStatusFont()
    {
    FXimFont	*current_status_font;
    Xim_StatusPropRec	*current_status;
    int		rc;

    if( current_client->window_type & ( FC_SAME_ROOT_STATUS_GC
				      | FC_SAME_ROOT_STATUS_FONT) ) {
        return( 0 );
        }
    if(( current_client->style & XIMStatusCallbacks )
    || ( current_client->style & XIMStatusNone )) {
	return( 0 );
	}

    current_status_font = current_client->status_font;
    current_status = current_client->status;
    FXFreeFont( display, current_status_font );
    rc = FXLoadQueryBaseFont( display, current_status_font,
			current_client->status->FontName );
    if( rc != 0 ) {
	*current_status_font = *root_client->status_font;
	current_status_font->status = 0;
	}
    else {
	current_status->AreaNeeded.height = current_status_font->max_font_height;
	current_status->AreaNeeded.width = GetMaxStatusMessageLength(current_client) * current_status_font->kanji_font_width / 2;
	}
    FXChangeGC( display, current_client->status_window, current_status_font,
	current_status->Foreground, current_status->Background );
    XClearArea( display, current_client->status_window, 0, 0, 0, 0, True );
    return( 0 );
    }

int
ChangeWindowAttributes(display, window, preedit)
    Display		*display;
    Window		 window;
    Xim_PreeditPropRec	*preedit;
{
    unsigned long	 valuemask;
    XSetWindowAttributes attributes;

    valuemask = 0;
    bzero(&attributes, sizeof(attributes));

    if (preedit->Bg_Pixmap) {
	attributes.background_pixmap = preedit->Bg_Pixmap;
	valuemask |= CWBackPixmap;
    }
    if (preedit->cursor) {
	attributes.cursor = preedit->cursor;
	valuemask |= CWCursor;
    }
    if (preedit->colormap) {
	attributes.colormap = preedit->colormap;
	valuemask |= CWColormap;
    }
    if (valuemask) {
	XChangeWindowAttributes(display, window, valuemask, &attributes);
    }
    return 0;
}

int
CreatePreeditPositionWindow(current_client, current_preedit_font)
    FXim_Client		*current_client;
    FXimFont		*current_preedit_font;
{
    Xim_PreeditPropRec	*current_preedit;
    Window		 focus_window;
    unsigned long	 fgpixel, bgpixel;
    long		 xx, yy, ww, hh;

    current_preedit = current_client->preedit;
    focus_window = current_client->focus_window;

    xx = current_preedit->SpotLocation.x;
    yy = current_preedit->SpotLocation.y
				 - current_preedit_font->max_font_ascent;
    ww = current_preedit_font->kanji_font_width;
    hh = current_preedit_font->max_font_height;

    fgpixel = current_preedit->Foreground;
    bgpixel = current_preedit->Background;

    current_client->preedit_window =
		 XCreateSimpleWindow(display, focus_window,
					xx, yy, ww, hh, 0, fgpixel, bgpixel);

    current_client->preedit_window2 =
		 XCreateSimpleWindow(display, focus_window,
					xx, yy, ww, hh, 0, fgpixel, bgpixel);

    XStoreName(display, current_client->preedit_window, "XIMPreeditPosition");
    XStoreName(display, current_client->preedit_window2, "XIMPreeditPosition2");
    ChangeWindowAttributes(display,
			 current_client->preedit_window,  current_preedit);
    ChangeWindowAttributes(display,
			 current_client->preedit_window2, current_preedit);
    CheckPreeditWidowSize(current_preedit);
    return 0;
}

int
CheckPreeditWidowSize(preedit)
    Xim_PreeditPropRec *preedit;
{
    Window		root_return, window;
    int			x_return, y_return;
    unsigned int  	width_return, height_return,
			border_width_return, depth_return;

    if (current_client->style & XIMPreeditPosition) {
	if (current_client->focus_window == current_client->window)
	    return -1;
	window = current_client->focus_window;
    } else {
	window = current_client->window;
    }

    if (preedit->Area.width == 0
    &&  preedit->Area.height == 0) {
        current_client->check_preedit_area_size = 1;
	if (XGetGeometry(display, (Drawable)window,
		&root_return, &x_return, &y_return, &width_return,
		 &height_return, &border_width_return, &depth_return)) {
	    preedit->Area.width  = width_return;
	    preedit->Area.height = height_return;
	    return 0;
	}
	return -1;
    }
    return 0;
}

int
DestroyPreeditWindow( flag)
    int	flag;
    {
    if(current_client->window_type & FC_SAME_ROOT_PREEDIT)
        return( 0 );

    current_preedit_font = current_client->preedit_font;
    if( current_client->style & XIMPreeditNothing ) {
    	if( !(current_client->window_type & FC_SAME_ROOT_PREEDIT_WINDOW) )
	    XDestroyWindow( display, current_client->preedit_window );
	}

    if( flag == 0 )
    if( current_client->style & XIMPreeditPosition ) {
	XDestroyWindow( display, current_client->preedit_window );
	XDestroyWindow( display, current_client->preedit_window2 );
	}

    if( !(current_client->window_type & FC_SAME_ROOT_PREEDIT_GC) )
        FXFreeGC( display, current_preedit_font );
    if( !(current_client->window_type & FC_SAME_ROOT_PREEDIT_FONT) )
        FXFreeFont( display, current_preedit_font );
    return( 0 );
    }

int
DestroyStatusWindow( flag)
    int	flag;
    {
    if(current_client->window_type & FC_SAME_ROOT_STATUS)
        return( 0 );
    if( !(current_client->window_type & FC_SAME_ROOT_STATUS_FONT) )
        FXFreeFont( display, current_client->status_font );

    if( current_client->style & (XIMStatusNothing | XIMStatusArea) ) {
        if( flag == 0 )
    	    if( !(current_client->window_type & FC_SAME_ROOT_STATUS_WINDOW) )
	        XDestroyWindow( display, current_client->status_window );
	}
    if( !(current_client->window_type & FC_SAME_ROOT_STATUS_GC) )
        FXFreeGC( display, current_client->status_font );
    return( 0 );
    }
