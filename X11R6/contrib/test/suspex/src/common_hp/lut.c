/* $XConsortium: lut.c,v 1.0 93/11/22 12:21:42 rws Exp $ */

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

#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/PEX5/PEXlib.h>
#include <stdio.h>
#include <misc.h>

PEXLookupTable	colorLUT=None, colorApproxLUT=None; 
PEXLookupTable	lineLUT=None, markerLUT=None;
PEXLookupTable	textLUT=None, interiorLUT=None, edgeLUT=None, patternLUT=None;
PEXLookupTable	fontLUT=None, lightLUT=None, dcueLUT=None;
PEXLookupTable	viewLUT=None;

#define NO_ERR 0
#define X_ERR 1
#define PEX_ERR 2

#define ERROR_CODE   0x1
#define REQUEST_CODE 0x2
#define SUB_CODE     0x4
#define OC_CODE      0x8

static int local_error_handler (Display *dpy, XErrorEvent *error)
{
    PEXOCErrorEvent  *pex_error;
    PEXExtensionInfo *pexinfo;
       
    int pex_error_code;
	int first_pex_error;
	int error_type;
	int foobar;

    /*
     * See what kind of error we have
     */

     error_type=X_ERR;
     foobar = 0;

     pex_error = (PEXOCErrorEvent *) error;

	 if (pexinfo = PEXGetExtensionInfo(dpy))
	   {
	    first_pex_error =  pexinfo->first_error;

	 	if (((int)pex_error->error_code >= first_pex_error) &&
			((int)pex_error->error_code <= first_pex_error + PEXMaxError))
			{
				error_type=PEX_ERR;
			}
       }
     else /* PEX not initialized on this connection!!! */
	   {
			foobar = 1;
	   }

	if (foobar)
	{
		_hppex_stderr_print( "%s\n%s\n%s\n%s\n",
			"*** Test Error Handler Warning: ",
			"\tReceived following error from connection that has not had PEX",
			"\tInitialized! This may indicate either a bug in Xlib error handling or ",
			"\ta defect in PEXlib internal logic. Something is probably foobar.");
	}

     if (error_type == PEX_ERR )
     {
	    /*
         * It's a PEX error so handle it as such
         */

	     pex_error_code = pex_error->error_code - first_pex_error;

		/*
			Silently trap a PEX error in this error handler since
			it's only used to mask the fact that Pattern LUTs cannot
			be created on some systems, including HP.
		*/
	}
    else
    {
		/*
         * It's not a PEX error. Treat the error in the "default" manner.
         */
		{
			/*
			 * Print small message.
				 * See X.h for definitions of opcodes used below
				 */
			switch (error->error_code)
			{
			case BadRequest:
				_hppex_stderr_print( "X Error: Bad Request\n");
				break;
		
			case BadValue:
				_hppex_stderr_print( "X Error: Bad Value\n");
				break;
		
			case BadWindow:
				_hppex_stderr_print( "X Error: Bad Window\n");
				break;
		
			case BadPixmap:
				_hppex_stderr_print( "X Error: Bad Pixmap\n");
				break;
		
			case BadAtom:
				_hppex_stderr_print( "X Error: Bad Atom\n");
				break;
		
			case BadCursor:
				_hppex_stderr_print( "X Error: Bad Cursor\n");
				break;
		
			case BadFont:
				_hppex_stderr_print( "X Error: Bad Font\n");
				break;
		
			case BadMatch:
				_hppex_stderr_print( "X Error: Bad Match\n");
				break;
		
			case BadDrawable:
				_hppex_stderr_print( "X Error: Bad Drawable\n");
				break;
		
			case BadAccess:
				_hppex_stderr_print( "X Error: Bad Access\n");
				break;
		
			case BadAlloc:
				_hppex_stderr_print( "X Error: Bad Alloc\n");
				break;
		
			case BadColor:
				_hppex_stderr_print( "X Error: Bad Color\n");
				break;
		
			case BadGC:
				_hppex_stderr_print( "X Error: Bad GC\n");
				break;
		
			case BadIDChoice:
				_hppex_stderr_print( "X Error: Bad ID Choice\n");
				break;
		
			case BadName:
				_hppex_stderr_print( "X Error: Bad Name\n");
				break;
		
			case BadLength:
				_hppex_stderr_print( "X Error: Bad Length\n");
				break;
		
			case BadImplementation:
				_hppex_stderr_print( "X Error: Bad Implementation\n");
				break;
		
			default:
				_hppex_stderr_print( "X Error: Unknown error\n");
				break;
			}
			_hppex_stderr_print("Request code: %d\n\n", error->minor_code);
		}
    }
}

void
setup_LUT(dpy, window, capx_info)
    Display		*dpy;
    Window		window;
    PEXColorApproxEntry *capx_info;
{

    int (*old_error_handler)();

    static PEXLineBundleEntry  lineEntry = {
	PEXLineTypeDotted, PEXPolylineInterpNone, 
	{PEXApproxImpDep, 0, 0.0}, 2.0,
    };
    static PEXMarkerBundleEntry  markerEntry = {
	PEXMarkerCross, 0, 10.0,
    };
    static PEXEdgeBundleEntry  edgeEntry = {
	PEXOn, 0, PEXSurfaceEdgeDashed, 5.0,
    };
    static PEXInteriorBundleEntry  interiorEntry = {
	PEXInteriorStyleSolid, 0, 
	PEXReflectionAmbient, PEXSurfaceInterpNone,
	PEXInteriorStyleSolid, 0, 
	PEXReflectionAmbient, PEXSurfaceInterpNone,
	{PEXApproxImpDep, 0, 0.0, 0.0},
    };
    static PEXReflectionAttributes  reflattrs = {
	0.7, 0.0, 0.0, 0.0, 0.0, 
    };
    static PEXReflectionAttributes  bf_reflattrs = {
	0.4, 0.0, 0.0, 0.0, 0.0, 
    };
    static PEXTextBundleEntry  textEntry = {
	1, PEXStrokePrecision, 0.8, 1.0, 
    };
    static PEXLightEntry  ambLightEntry = {
	PEXLightAmbient, 0, {0.0,0.0,0.0}, {0.0,0.0,0.0}, 0.0, 0.0, 0.0, 0.0,
    };
    static PEXLightEntry  vecLightEntry = {
	PEXLightWCVector, 0,{-0.5,-0.5,-0.5}, {0.0,0.0,0.0}, 0.0, 0.0, 0.0, 0.0,
    };
    static PEXDepthCueEntry  dcueEntry = {
	PEXOn, {0,0,0}, 1.0, 0.0, 1.0, 0.2,
    };

    PEXColorSpecifier colorRed, colorYellow, colorWhite;
    PEXColorSpecifier indexedColorYellow, indexedColorWhite;
    PEXColorSpecifier indexedColorCyan;

    colorRed.type            = PEXColorTypeRGB;
    colorRed.value.rgb.red   = 1.0;
    colorRed.value.rgb.green = 0.0;
    colorRed.value.rgb.blue  = 0.0;

    colorYellow.type            = PEXColorTypeRGB;
    colorYellow.value.rgb.red   = 1.0;
    colorYellow.value.rgb.green = 1.0;
    colorYellow.value.rgb.blue  = 0.0;

    colorWhite.type            = PEXColorTypeRGB;
    colorWhite.value.rgb.red   = 1.0;
    colorWhite.value.rgb.green = 1.0;
    colorWhite.value.rgb.blue  = 1.0;

    indexedColorYellow.type                = PEXColorTypeIndexed;
    indexedColorYellow.value.indexed.index = YELLOW;

    indexedColorWhite.type                = PEXColorTypeIndexed;
    indexedColorWhite.value.indexed.index = WHITE;

    indexedColorCyan.type                = PEXColorTypeIndexed;
    indexedColorCyan.value.indexed.index = CYAN;

    colorApproxLUT = PEXCreateLookupTable(dpy, window, PEXLUTColorApprox);
    PEXSetTableEntries(dpy, colorApproxLUT, 0, 1, PEXLUTColorApprox, capx_info);

    lineLUT = PEXCreateLookupTable(dpy, window, PEXLUTLineBundle);
    lineEntry.color = colorYellow;
    PEXSetTableEntries(dpy, lineLUT, 2, 1, PEXLUTLineBundle, &lineEntry);

    markerLUT = PEXCreateLookupTable(dpy, window, PEXLUTMarkerBundle);
    markerEntry.color = colorYellow;
    PEXSetTableEntries(dpy, markerLUT, 2, 1, PEXLUTMarkerBundle, &markerEntry);

    edgeLUT = PEXCreateLookupTable(dpy, window, PEXLUTEdgeBundle);
    edgeEntry.color = indexedColorCyan;
    PEXSetTableEntries(dpy, edgeLUT, 2, 1, PEXLUTEdgeBundle, &edgeEntry);

    interiorLUT = PEXCreateLookupTable(dpy, window, PEXLUTInteriorBundle);
    interiorEntry.color = interiorEntry.bf_color = indexedColorWhite;
    reflattrs.specular_color    = indexedColorWhite;
    bf_reflattrs.specular_color = indexedColorWhite;
    interiorEntry.reflection_attr    = reflattrs;
    interiorEntry.bf_reflection_attr = bf_reflattrs;
    PEXSetTableEntries(dpy, interiorLUT, 2, 1, PEXLUTInteriorBundle, 
	&interiorEntry);

    textLUT = PEXCreateLookupTable(dpy, window, PEXLUTTextBundle);
    textEntry.color = colorYellow;
    PEXSetTableEntries(dpy, textLUT, 1, 1, PEXLUTTextBundle, &textEntry);

    lightLUT = PEXCreateLookupTable(dpy, window, PEXLUTLight);
    ambLightEntry.color = colorWhite;
    vecLightEntry.color = colorWhite;
    PEXSetTableEntries(dpy, lightLUT, 1, 1, PEXLUTLight, &ambLightEntry);
    PEXSetTableEntries(dpy, lightLUT, 2, 1, PEXLUTLight, &vecLightEntry);

    dcueLUT = PEXCreateLookupTable(dpy, window, PEXLUTDepthCue);
    dcueEntry.color = colorRed;
    PEXSetTableEntries(dpy, dcueLUT, 2, 1, PEXLUTDepthCue, &dcueEntry);

    fontLUT = PEXCreateLookupTable(dpy, window, PEXLUTTextFont);
    load_two_fonts (dpy, fontLUT);

    old_error_handler = XSetErrorHandler (local_error_handler);
    patternLUT = PEXCreateLookupTable(dpy, window, PEXLUTPattern);
    XSetErrorHandler (old_error_handler);

    colorLUT = PEXCreateLookupTable(dpy, window, PEXLUTColor);

    {
    /*
     *  Set Entries 0-7 in the Color LUT to HP Defaults
     */
    PEXColorRGB Black  ={0.0, 0.0, 0.0};
    PEXColorRGB White  ={1.0, 1.0, 1.0};
    PEXColorRGB Red    ={1.0, 0.0, 0.0};
    PEXColorRGB Yellow ={1.0, 1.0, 0.0};
    PEXColorRGB Green  ={0.0, 1.0, 0.0};
    PEXColorRGB Cyan   ={0.0, 1.0, 1.0};
    PEXColorRGB Blue   ={0.0, 0.0, 1.0};
    PEXColorRGB Magenta={1.0, 0.0, 1.0};
  
    PEXColorEntry lut_colors[8];
  
    lut_colors[0].type      = PEXColorTypeRGB;
    lut_colors[0].value.rgb = Black;
  
    lut_colors[1].type      = PEXColorTypeRGB;
    lut_colors[1].value.rgb = White;
  
    lut_colors[2].type      = PEXColorTypeRGB;
    lut_colors[2].value.rgb = Red;
  
    lut_colors[3].type      = PEXColorTypeRGB;
    lut_colors[3].value.rgb = Yellow;
  
    lut_colors[4].type      = PEXColorTypeRGB;
    lut_colors[4].value.rgb = Green;
  
    lut_colors[5].type      = PEXColorTypeRGB;
    lut_colors[5].value.rgb = Cyan;
  
    lut_colors[6].type      = PEXColorTypeRGB;
    lut_colors[6].value.rgb = Blue;
  
    lut_colors[7].type      = PEXColorTypeRGB;
    lut_colors[7].value.rgb = Magenta;
      
    PEXSetTableEntries(dpy, colorLUT, 0, 8, PEXLUTColor, 
		       (PEXPointer) lut_colors);

    viewLUT = PEXCreateLookupTable(dpy, window, PEXLUTView);
    }

}


int load_two_fonts (Display *display, PEXLookupTable font_lut)
{
    int				i, first_font_index;
    unsigned long		num_fonts;
    char			**font_names;
    PEXFont			font_ids[2];
    PEXTextFontEntry		entries[2];

    /* 
	Get the font names and store the first and last loadable ones 
	in the font_lut table. 
    */

    if ( font_names = PEXListFonts( display, "*", 32, &num_fonts ) ) {
	    if (num_fonts < 2) {
		    _hppex_stderr_print ( "\t%s\n\t%s\n",
			    "WARNING:  Only one text font found,",
			    "images may not be distinguishable.\n");
	    }

	    /* search forward to the first loadable font */
	    first_font_index = -1;
	    for ( i = 0; i < num_fonts; i++ ) {

		    if ((font_ids[0] = PEXLoadFont (display, font_names[i])) != None) {

			    first_font_index = i;

			    entries[0].count = 1;
			    entries[0].fonts = &(font_ids[0]);

			    break;
		    }
	    }
	    if (first_font_index == -1) {
		    _hppex_stderr_print ("PROCESS ERROR:  No loadable fonts.\n");
		    return 1;
	    }

	    for ( i = (num_fonts-1); i > first_font_index; i-- ) {

		    if ((font_ids[1] = PEXLoadFont (display, font_names[i])) != None) {

			    if (i == first_font_index) {
				    _hppex_stderr_print ( "\t%s\n\t%s\n",
					    "WARNING:  Only one loadable text font found,",
					    "images may not be distinguishable.\n");
			    }

			    entries[1].count = 1;
			    entries[1].fonts = &(font_ids[1]);

			    break;
		    }
	    }

	    /*
		Now actually load the entries.
	    */
	    PEXSetTableEntries (display, font_lut, 1, 2, PEXLUTTextFont, (PEXPointer) entries);

	    PEXUnloadFont (display, font_ids[0]);
	    PEXUnloadFont (display, font_ids[1]);
	    PEXFreeFontNames(  num_fonts, font_names );
    }
    else {
	    _hppex_stderr_print ("PROCESS ERROR:  No available font names.\n");
	    return 1;
    }

    return 0;
}

