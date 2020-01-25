/* @(#)FXlib.c      6.1 94/06/01 20:04:27 FUJITSU LIMITED. */
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

#include <malloc.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include "Xfeoak.h"
#include <X11/Xresource.h>
#include <X11/Xatom.h>

#define FSM_BLOCK_NO      10
#define FSM_START_INDEX    1

typedef struct _Fsm {
    XrmQuark    qname;
    XrmQuark    qname2;
    XFontStruct *fs;
    int         link;
} Fsm_t;

int			 fsm_free = 1;
Static Fsm_t		*fsm_head;
Static int		 fsm_max_ind = 0;

#define REG_ASCII	1
#define REG_KANA	2
#define REG_ASCII_KANA	3
#define REG_KANJI	4
#define REG_GAIJI	8

typedef struct _cahrset_registry {
    char	*name;
    int		cahrset;
} Fcr;

Static Fcr	cahrset_tbl[] = {
    {"JISX0201.1976",	REG_ASCII_KANA	},
    {"JISX0208.1983",	REG_KANJI	},
    {"ISO8859",		REG_ASCII	},
};

#define MAX_CARSET	sizeof(cahrset_tbl)/sizeof(Fcr)

int
CheckBaseCahrasetRegistry(font_name)
    char	*font_name;
{
    register int i;
    char	*charsetName, *str;

    for (i = 0, str = font_name; *str; str++) {
	if (!(str = strchr(str, '-')))
	    return( 0 );

	i++;
	if (i == 13) {
	    str++;
	    break;
	}
    }

    if (!(*str))
	return( 0 );

    charsetName = str;
    for(; *str; str++ ) {
	if( islower(*str) ) *str = toupper( *str );
    }

    for(i=0;i<MAX_CARSET;i++) {
	if( strncmp( cahrset_tbl[i].name, charsetName,
				strlen(cahrset_tbl[i].name) ) == 0 )
	    return( cahrset_tbl[i].cahrset );
    }
    return( 0 );
}

int
CheckCahrasetRegistry(font_struct)
    XFontStruct *font_struct;
{
    int		i;
    unsigned long charsetAtom;
    char	*charsetName, *str;
    Atom	XA_CHARSET_REGISTRY;

    XA_CHARSET_REGISTRY = XInternAtom( display, "CHARSET_REGISTRY", False);
    if( XGetFontProperty( font_struct, XA_CHARSET_REGISTRY, &charsetAtom ) ) {
	charsetName = XGetAtomName(display, charsetAtom);
    } else {
	return( 0 );
    }
    for( str = charsetName; *str; str++ ) {
	if( islower(*str) ) *str = toupper( *str );
    }
    for(i=0;i<MAX_CARSET;i++) {
	if( strcmp( cahrset_tbl[i].name, charsetName ) == 0 ) {
	    XFree( charsetName );
	    return( cahrset_tbl[i].cahrset );
	}
    }
    XFree( charsetName );
    return( 0 );
}

Fsm_t *
_EmptyFsm()
{
    int		ix;
    Fsm_t	*fsmp;
    char	*new;

    ix = FSM_START_INDEX;
    if( fsm_head == (Fsm_t *)NULL) {
        new = (char *)malloc( FSM_BLOCK_NO*sizeof(Fsm_t));
    } else {
        for( fsmp = &fsm_head[FSM_START_INDEX];
		ix < fsm_max_ind; ix++, fsmp++) {
	    if (fsmp->qname == 0) {
    	        return( &fsm_head[ix]);
	    }
        }
	new = (char *)realloc( fsm_head, (FSM_BLOCK_NO+fsm_max_ind)*sizeof(Fsm_t));
    }
    if( new == NULL ) return( (Fsm_t *)NULL);
    fsm_head = (Fsm_t *)new;
    bzero( &fsm_head[fsm_max_ind], FSM_BLOCK_NO*sizeof(Fsm_t));
    fsm_max_ind += FSM_BLOCK_NO;
    return( &fsm_head[ix]);
}

XFontStruct *
_FXLoadQueryFont( display, name )
    Display     *display;
    char	*name;
{
    XFontStruct *font_struct;
    XrmQuark	qname;
    int		ix;
    Fsm_t	*fsmp;
    unsigned int fontAtom;
    char	*fontName;

    qname = XrmStringToQuark( name);
    for( ix=FSM_START_INDEX, fsmp = &fsm_head[FSM_START_INDEX];
		ix < fsm_max_ind; ix++, fsmp++) {
	if( fsmp->qname == qname
	 || fsmp->qname2 == qname) {
	    fsmp->link++;
	    return( fsmp->fs);
	}
    }

    font_struct = XLoadQueryFont( display, XrmQuarkToString(qname));
    if( font_struct ) {
	fontName = 0;
        fsmp = _EmptyFsm();
        if( fsmp ) {
	    fsmp->fs = font_struct;
            fsmp->qname = qname;
            fsmp->link++;
	    if( XGetFontProperty( font_struct, XA_FONT, (unsigned long *)&fontAtom ) ) {
		fontName = XGetAtomName(display, fontAtom);
		}
	    fsmp->qname2 = fontName ? XrmStringToQuark( fontName ) : qname ;
        }
	if( fontName ) XFree( fontName );
        return( font_struct);
    }
    return( (XFontStruct *)NULL);
}

int
_FXFreeFont( display, font_struct )
    Display     *display;
    XFontStruct *font_struct;
{
    int		ix;
    Fsm_t	*fsmp;

    for( ix=FSM_START_INDEX, fsmp = &fsm_head[FSM_START_INDEX];
		ix < fsm_max_ind; ix++, fsmp++) {
	if( fsmp->fs == font_struct) {
	    fsmp->link--;
	    if( fsm_free && fsmp->link <= 0) {
		fsmp->link = 0;
		fsmp->qname = 0;
		fsmp->qname2 = 0;
		if (fsmp->fs) {
		    XFreeFont( display, fsmp->fs);
		    fsmp->fs = NULL;
		}
	    }
            return(0);
	}
    }
    return(-1);
}

int
FXLoadQueryBaseFont( display, load_font, base_font_name )
    Display	*display;
    FXimFont	*load_font;
    char	*base_font_name;
{
    XFontStruct *ascii_kana_font_struct=0, *kanji_font_struct=0,
		*gaiji_font_struct=0;
    char	*ascii_kana_font_name=0, *kanji_font_name=0,
		*gaiji_font_name=0;
    int		wfont_h, no = 0, ii;
    int		error = 0;
    char	*cfn, cc, *none, *none2, null=0;
    XFontStruct *font_struct, *font_struct_tbl[4];
    char	*cfnt[10], work[512];
    int		charset;

    load_font->fixe_width = 1;
    if( !base_font_name || !*base_font_name )
	return( -2);

    cfn = work;
    cfnt[no++] = cfn;
    while( (cc = *base_font_name++) != '\0' ) {
	if( cc == ',' ) {
	    *cfn++ = '\0';
	    cfnt[no++] = cfn;
	}
	else
	    *cfn++ = cc;
    }
    *cfn = '\0';

    none  = &null;
    none2 = &null;
    error = 0;
    for( ii=0; ii<no; ii++ ) {
	if( cfnt[ii] == 0 || *cfnt[ii] == 0 )
	    continue;
	font_struct = _FXLoadQueryFont( display, cfnt[ii] );
        if( !font_struct ) {
	    ErrorMessage( 30, cfnt[ii] );
	    continue;
        }
	if (!(charset = CheckBaseCahrasetRegistry( cfnt[ii] )))
	    charset = CheckCahrasetRegistry( font_struct );
	switch( charset ) {
	case REG_ASCII_KANA :
	    if( ascii_kana_font_struct ) {
		_FXFreeFont( display, ascii_kana_font_struct );
	    }
	    ascii_kana_font_struct = font_struct;
	    ascii_kana_font_name   = cfnt[ii];
	    break;
	case REG_KANJI :
	    if( kanji_font_struct ) {
	        _FXFreeFont( display, kanji_font_struct );
	    }
	    kanji_font_struct = font_struct;
	    kanji_font_name   = cfnt[ii];
	    break;
	case REG_GAIJI :
	    if( gaiji_font_struct ) {
	        _FXFreeFont( display, gaiji_font_struct );
	    }
	    gaiji_font_struct = font_struct;
	    gaiji_font_name   = cfnt[ii];
	    break;
	case REG_ASCII :
	case REG_KANA :
	    none = cfnt[ii];
	    _FXFreeFont( display, font_struct );
	    break;
	}
    }

    if( !ascii_kana_font_struct ) {
	ErrorMessage( 32 );
	error = 1;
    }
    if( !kanji_font_struct ) {
	ErrorMessage( 34 );
	error = 1;
    }
    if( error ) {
	if( ascii_kana_font_struct )
	    _FXFreeFont( display, ascii_kana_font_struct );
	if( kanji_font_struct )
	    _FXFreeFont( display, kanji_font_struct );
	if( gaiji_font_struct )
	    _FXFreeFont( display, gaiji_font_struct );
	return( -1 );
    }

    if( ascii_kana_font_struct->per_char )
	load_font->fixe_width = 0;
    if( kanji_font_struct->per_char )
	load_font->fixe_width = 0;
    if( gaiji_font_struct && gaiji_font_struct->per_char )
	load_font->fixe_width = 0;
    
    load_font->ascii_kana_font_struct = ascii_kana_font_struct;
    load_font->status = FF_FONT_LOAD;
    load_font->kanji_font_struct = kanji_font_struct;
    load_font->gaiji_font_struct = gaiji_font_struct;
    load_font->ascii_kana_font_width
      = ascii_kana_font_struct->max_bounds.width;
    load_font->max_font_ascent
      = (ascii_kana_font_struct->ascent > kanji_font_struct->ascent)?
	 ascii_kana_font_struct->ascent : kanji_font_struct->ascent;
    load_font->max_font_descent
      = (ascii_kana_font_struct->descent > kanji_font_struct->descent)?
	 ascii_kana_font_struct->descent : kanji_font_struct->descent;
    load_font->max_font_height
      = ascii_kana_font_struct->ascent + ascii_kana_font_struct->descent;
    wfont_h = kanji_font_struct->ascent + kanji_font_struct->descent;
    if( load_font->max_font_height < wfont_h )
	load_font->max_font_height = wfont_h;
    load_font->kanji_font_width = kanji_font_struct->max_bounds.width;
    return( 0);
}

int
FXFreeFont( display, load_font )
    Display	*display;
    FXimFont	*load_font;
{
    if( !(load_font->status & FF_FONT_LOAD) )
	return( 0 );

    if( load_font->ascii_kana_font_struct ) {
	_FXFreeFont( display, load_font->ascii_kana_font_struct );
	load_font->ascii_kana_font_struct = 0;
    }
    if( load_font->kanji_font_struct ) {
	_FXFreeFont( display, load_font->kanji_font_struct );
	load_font->kanji_font_struct = 0;
    }
    if( load_font->gaiji_font_struct ) {
	_FXFreeFont( display, load_font->gaiji_font_struct );
	load_font->gaiji_font_struct = 0;
    }
    return( 0);
}

int
FXCreateGC( display, window, load_font,
		foreground_pixel, background_pixel )
    Display	*display;
    Window	window;
    FXimFont	*load_font;
    unsigned long  foreground_pixel, background_pixel;
{
    unsigned long  gcmask;
    XGCValues      gcvalues, reverse_gcvalues;

    gcmask  = GCBackground | GCForeground | GCFont;
    gcvalues.foreground = foreground_pixel;
    gcvalues.background = background_pixel;
    reverse_gcvalues.foreground = background_pixel;
    reverse_gcvalues.background = foreground_pixel;

    gcvalues.font         = load_font->ascii_kana_font_struct->fid;
    reverse_gcvalues.font = load_font->ascii_kana_font_struct->fid;
    load_font->normal_gc
      = XCreateGC( display, window, gcmask, &gcvalues);
    load_font->reverse_gc
      = XCreateGC( display, window, gcmask, &reverse_gcvalues);


    gcvalues.function = GXxor;
    gcvalues.foreground = background_pixel ^ foreground_pixel;
    gcvalues.plane_mask = background_pixel ^ foreground_pixel;
    gcmask  = GCForeground | GCFunction | GCPlaneMask;
    load_font->invert_gc
      = XCreateGC( display, window, gcmask, &gcvalues);
    load_font->status |= FF_CREATE_GC;
    return( 0);
}

int
FXFreeGC( display, load_font )
    Display	*display;
    FXimFont	*load_font;
{
    if( load_font->status & FF_CREATE_GC ) {
#define M_FreeGC( gc ) if( gc ) { XFreeGC( display, gc ); gc = 0; }

        M_FreeGC( load_font->normal_gc );
        M_FreeGC( load_font->reverse_gc );
        M_FreeGC( load_font->invert_gc );
#undef  M_FreeGC
    }
    return( 0);
}

int
FXChangeGC( display, window, load_font,
		foreground_pixel, background_pixel )
    Display	*display;
    Window	window;
    FXimFont	*load_font;
    unsigned long  foreground_pixel, background_pixel;
{
    unsigned long  gcmask;
    XGCValues      gcvalues, reverse_gcvalues;
    if( (load_font->status & FF_CREATE_GC) == (unsigned char)0 ) {
	return FXCreateGC( display, window, load_font,
	   			foreground_pixel, background_pixel );
	}
    gcmask  = GCBackground | GCForeground | GCFont;
    gcvalues.foreground = foreground_pixel;
    gcvalues.background = background_pixel;
    reverse_gcvalues.foreground = background_pixel;
    reverse_gcvalues.background = foreground_pixel;

    gcvalues.font         = load_font->ascii_kana_font_struct->fid;
    reverse_gcvalues.font = load_font->ascii_kana_font_struct->fid;
    XChangeGC( display, load_font->normal_gc, gcmask, &gcvalues);
    XChangeGC( display, load_font->reverse_gc, gcmask,
	&reverse_gcvalues);

    gcvalues.function = GXxor;
    gcvalues.foreground = background_pixel ^ foreground_pixel;
    gcvalues.plane_mask = background_pixel ^ foreground_pixel;
    gcmask  = GCForeground | GCFunction | GCPlaneMask;
    XChangeGC( display, load_font->invert_gc, gcmask, &gcvalues);
    return( 0);
}

int
FXTextWidth( load_font, input_data, length)
    FXimFont	*load_font;
    ushort	*input_data;
    int		length;
{
    int	j, width=0;
    for( j=length; j>0; j--, input_data++) {
	switch(*input_data & 0x8080) {
	case 0x0000:
	case 0x0080:
	     width += load_font->ascii_kana_font_width;
	    break;
	case 0x8080:
	case 0x8000:
	    width += load_font->kanji_font_width;
	    break;
	}
    }
    return( width );
}

int
FXTextExtentsArea( load_font, preedit, input_data, length,
			ret_area, ret_len, ret_lineno)
    FXimFont	*load_font;
    Xim_PreeditPropRec	*preedit;
    ushort	*input_data;
    int		length;
    XRectangle	*ret_area;
    short	*ret_len;
    int		*ret_lineno;
{
    int		j, x2, y2;
    int		sy, over = 0;
    int		area_no = 0;
    int		x, y, width, height;
    int		max_x2;
    int		edit_x1, edit_y1, edit_x2, edit_y2;
    ushort	save, *savep;

    savep = &input_data[length];
    save  = *savep;
    *savep = 0xA1A1;

    edit_x1 = preedit->Area.x;
    edit_y1 = preedit->Area.y;
    edit_x2 = edit_x1 + preedit->Area.width;
    edit_y2 = edit_y1 + preedit->Area.height;

    width = 0;
    max_x2 = 0;
    height = load_font->max_font_height;
    x = preedit->SpotLocation.x;
    y = preedit->SpotLocation.y - load_font->max_font_ascent;
    if( x < edit_x1 )
	x = edit_x1;
    if( x > edit_x2 ) {
	x = edit_x1;
	y = y + height + preedit->LineSpacing;
    }
    if( y < edit_y1 )
	y = edit_y1;
    if( preedit->SpotLocation.y > edit_y2 )
	y = edit_y1;
    sy = y;
    x2 = x;
    y2 = y + height;

    ret_len[0] = 0;
    ret_len[1] = 0;
    *ret_lineno = 0;

    for( j=length+1; j>0; j--, input_data++) {
	width = FXTextWidth( load_font, input_data, 1);
	if( (x2 + width) <= edit_x2 ) {
	    x2 = x2 + width;
        } else {
	    if( *ret_len )  {
		(*ret_lineno)++;
		area_no++;
    		ret_area->x = x;
    		ret_area->y = y;
   		ret_area->width  = x2 - x;
		ret_area->height = y2 - y;
		ret_area++;
		if( area_no >= 2 ) {
    		    *savep = save;
		    return( area_no );
		}
		ret_len++;
	    }
	    x = edit_x1;
	    y = y2 + preedit->LineSpacing;
	    x2 = x + width;
	    y2 = y2 + height + preedit->LineSpacing;
	    if( y2 > edit_y2 ) {
	        y = edit_y1;
    	        y2 = y + height;
	    }
        }
    	(*ret_len)++;
    }

    if( *ret_len ) {
    	(*ret_lineno)++;
	area_no++;
    	ret_area->x = x;
    	ret_area->y = y;
   	ret_area->width  = x2 - x;
	ret_area->height = y2 - y;
    }

    *savep = save;
    return( area_no );
}

FXDrawImageString( display, window, load_font,
			reverse, x, y, buff, length)
    Display *display;
    Window   window;
    FXimFont	*load_font;
    int	     reverse;
    int      x, y;
    ushort   *buff;
    int      length;
{
    GC		gc;
    XFontStruct	*font_struct;
    int		sv_x = x;
    unsigned int  c2, cmask, ctype;
    ushort	*buff_end;
    unsigned char  *outp, *out_end;
#define outbufsize 256
    unsigned char  outbuf[outbufsize];
    int	     	(*DrawString16)(), (*DrawString)();

    if( !reverse ) {
	gc = load_font->normal_gc;
	DrawString16 = XDrawString16;
	DrawString   = XDrawString;
    } else {
	gc = load_font->reverse_gc;
	DrawString16 = XDrawImageString16;
	DrawString   = XDrawImageString;
    }

    outp = outbuf;
    out_end = outbuf + outbufsize - 2;
    buff_end = buff + length;

    while( buff < buff_end ) {
	ctype = *buff & 0x8080;
	switch( ctype ) {
	case 0x0000:
	    font_struct = load_font->ascii_kana_font_struct;
	    cmask = 0x007f;
	    break;
	case 0x0080:
	    font_struct = load_font->ascii_kana_font_struct;
	    cmask = 0x00ff;
	    break;
	case 0x8080:
	    font_struct = load_font->kanji_font_struct;
	    cmask = 0x7f7f;
	    break;
	case 0x8000:
	    font_struct = load_font->gaiji_font_struct;
	    cmask = 0x7f7f;
	    break;
	}

	if( !font_struct ) {
	    while( ( buff < buff_end )
	       && ((*buff & 0x8080) == ctype) ) {
	        buff++;
	    }
	} else
	while( ( buff < buff_end )
	   &&  ( outp < out_end )
	   &&  (((c2 = *buff) & 0x8080) == ctype) ) {
	    if( cmask & 0x7f00 ) {
		*outp++ = (c2 & cmask) >> 8;
		*outp++ =  c2 & 0x007f;
	    } else {
		*outp++ = c2 & cmask;
	    }
	    buff++;
	}

        length = outp - outbuf;
	if( length > 0 ) {
	    XSetFont( display, gc, font_struct->fid );
	    if( cmask & 0x7f00 ) {
		length = length / 2;
        	(*DrawString16)( display, window, gc, x, y,
					(XChar2b *)outbuf, length );
    		x += load_font->kanji_font_width * length;
            }
	    else {
	        (*DrawString)( display, window, gc, x, y,
					(char *)outbuf, length);
	    	x += load_font->ascii_kana_font_width * length;
            }
    	    outp = outbuf;
        }
    }
    return(x - sv_x);
}

int FXDefaultError( display, event)
    Display	*display;
    XErrorEvent	*event;
{
#define	MSG_SIZE	64
    char	xerr_msg[MSG_SIZE];
    if(verbose_flag) {
        XGetErrorText( display, event->error_code, xerr_msg, MSG_SIZE );
        fprintf( stderr, "%s: X Error: %s\n", command_name, xerr_msg );
        fprintf( stderr, "  req.: %d - %d, res.ID: %#x, error #%d - #%d\n",
		event->request_code, event->minor_code,
		event->resourceid, event->serial, NextRequest(display) - 1);
    }
}

int FXDefaultIOError( display )
    Display	*display;
{
    extern char *sys_errlist[];
    extern int sys_nerr;
    char *SysErrorMsg;
    SysErrorMsg = (errno >= 0 && errno < sys_nerr) ?
			sys_errlist[errno] : "unknown error";
    SysErrorMsg = SysErrorMsg ? SysErrorMsg : "no such error";

    fprintf (stderr, "XIO:  fatal IO error %d (%s) on X server \"%s\"\r\n",
		errno, SysErrorMsg, DisplayString (display));
    fprintf (stderr, "      after %lu requests (%lu known processed) with %d events remaining.\r\n",
		NextRequest(display) - 1, LastKnownRequestProcessed(display),
		QLength(display));
}
