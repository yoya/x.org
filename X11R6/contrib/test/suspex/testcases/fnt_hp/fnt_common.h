/* $XConsortium: fnt_common.h,v 1.0 93/11/22 12:28:11 rws Exp $ */

/******************************************************************************/
/*                                                                            */
/*  (c) Copyright Hewlett-Packard Company, 1993, Fort Collins, Colorado       */
/*                                                                            */
/*                            All Rights Reserved                             */
/*                                                                            */
/*  Permission to use, copy, modify, and distribute this software and its     */
/*  documentation for any purpose and without fee is hereby granted,          */
/*  provided that the above copyright notices appear in all copies and that   */
/*  both the copyright notices and this permission notice appear in           */
/*  supporting documentation, and that the name of Hewlett-Packard not be     */
/*  used in advertising or publicity pertaining to distribution of the        */
/*  software without specific, written prior permission.                      */
/*                                                                            */
/*  HEWLETT-PACKARD MAKES NO WARRANTY OF ANY KIND WITH REGARD TO THIS         */
/*  SOFTWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF        */
/*  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  Hewlett-Packard    */
/*  shall not be liable for errors contained herein or direct, indirect,      */
/*  special, incidental or consequential damages in connection with the       */
/*  furnishing, performance or use of this software.                          */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#ifdef SYSV
#include <string.h>
#else
#include <strings.h>
#endif
#include <X11/Xlib.h>
#include <X11/PEX5/PEXlib.h>
#include <X11/Xutil.h>

void setup_PEX(Display *display, Window *window, PEXRenderer *rdr)
{
    PEXRendererAttributes rdr_attrs;

    {
	unsigned long count;
	char **names =
	    PEXListFonts(display, "*-p-*", 1, &count);
	PEXFont font = PEXLoadFont(display, *names);
	PEXTextFontEntry font_entry;

	PEXFreeFontNames(count, names);

	rdr_attrs.text_font_table =
	    PEXCreateLookupTable(display, *window, PEXLUTTextFont);

	font_entry.count = 1;
	font_entry.fonts = &font;
	PEXSetTableEntries(display, rdr_attrs.text_font_table,
			   2, 1, PEXLUTTextFont, &font_entry);

	PEXUnloadFont(display, font);
    }

    {
	PEXColorRGB background = {0.3, 0.0, 0.4};

	rdr_attrs.background_color.type = PEXColorTypeRGB;
	rdr_attrs.background_color.value.rgb = background;
    }
    
    rdr_attrs.clear_image = True;

    *rdr = PEXCreateRenderer(display, *window, 
			    PEXRATextFontTable |
			    PEXRAClearImage | PEXRABackgroundColor,
			    &rdr_attrs);

    PEXFreeLookupTable(display, rdr_attrs.text_font_table);
}

static XID strs[100];
static int depth = 0;

void begin_structure(Display *display, XID *res,
		     PEXOCRequestType req_type)
{
    if (req_type == PEXOCRender)
	PEXBeginStructure(display, *res, 1);
    else
    {
	strs[depth] = *res;
	*res = PEXCreateStructure(display);
	PEXExecuteStructure(display, strs[depth], PEXOCStore, *res);
	depth++;
    }
}

void end_structure(Display *display, XID *res,
		   PEXOCRequestType req_type)
{
    if (req_type == PEXOCRender)
	PEXEndStructure(display, *res);
    else
    {
	depth--;
	*res = strs[depth];
    }
}

