/* $XConsortium: lut.c,v 1.0 93/11/22 12:25:35 rws Exp $ */

/*
        NOTICE TO USER: The source code in this file is copyrighted under
        U.S. and international laws.  SunSoft, Inc., a Sun Microsystems,
        Inc. business of Mountain View, California owns the copyright. 
        Users and possessors of this source code are hereby granted a
        nonexclusive, royalty-free copyright and design patent license
        to use this code in individual and commercial software. 

        Any use of this source code must include, in the user documentation 
        and internal comments to the code, notices to the end user as follows:

(c) Copyright 1992, 1993 SunSoft, Inc., a Sun Microsystems, Inc. business.

        SUNSOFT, INC. AND SUN MICROSYSTEMS, INC. MAKE NO REPRESENTATIONS
        ABOUT THE SUITABILITY OF THIS SOURCE CODE FOR ANY PURPOSE. IT IS
        PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY OF ANY KIND. 
        SUNSOFT, INC., AND SUN MICROSYSTEMS, INC., SEVERALLY AND
        INDIVIDUALLY, DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOURCE
        CODE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
        FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL SUNSOFT, INC.
        OR SUN MICROSYSTEMS, INC., BE LIABLE FOR ANY SPECIAL, INDIRECT,
        INCIDENTAL, OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER
        RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
        OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
        OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOURCE CODE.
*/
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/PEX5/PEXlib.h>
#include <stdio.h>

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
		fprintf (stderr,  "%s\n%s\n%s\n%s\n",
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
				fprintf (stderr,  "X Error: Bad Request\n");
				break;
		
			case BadValue:
				fprintf (stderr,  "X Error: Bad Value\n");
				break;
		
			case BadWindow:
				fprintf (stderr,  "X Error: Bad Window\n");
				break;
		
			case BadPixmap:
				fprintf (stderr,  "X Error: Bad Pixmap\n");
				break;
		
			case BadAtom:
				fprintf (stderr,  "X Error: Bad Atom\n");
				break;
		
			case BadCursor:
				fprintf (stderr,  "X Error: Bad Cursor\n");
				break;
		
			case BadFont:
				fprintf (stderr,  "X Error: Bad Font\n");
				break;
		
			case BadMatch:
				fprintf (stderr,  "X Error: Bad Match\n");
				break;
		
			case BadDrawable:
				fprintf (stderr,  "X Error: Bad Drawable\n");
				break;
		
			case BadAccess:
				fprintf (stderr,  "X Error: Bad Access\n");
				break;
		
			case BadAlloc:
				fprintf (stderr,  "X Error: Bad Alloc\n");
				break;
		
			case BadColor:
				fprintf (stderr,  "X Error: Bad Color\n");
				break;
		
			case BadGC:
				fprintf (stderr,  "X Error: Bad GC\n");
				break;
		
			case BadIDChoice:
				fprintf (stderr,  "X Error: Bad ID Choice\n");
				break;
		
			case BadName:
				fprintf (stderr,  "X Error: Bad Name\n");
				break;
		
			case BadLength:
				fprintf (stderr,  "X Error: Bad Length\n");
				break;
		
			case BadImplementation:
				fprintf (stderr,  "X Error: Bad Implementation\n");
				break;
		
			default:
				fprintf (stderr,  "X Error: Unknown error\n");
				break;
			}
			fprintf (stderr, "Request code: %d\n\n", error->minor_code);
		}
    }
}

PEXLookupTable	colorLUT=None, colorApproxLUT=None; 
PEXLookupTable	lineLUT=None, markerLUT=None;
PEXLookupTable	textLUT=None, interiorLUT=None, edgeLUT=None, patternLUT=None;
PEXLookupTable	fontLUT=None, lightLUT=None, dcueLUT=None;

PEXWorkstation
setup_LUT(dpy, window, capx_info)
    Display		*dpy;
    Window		window;
    PEXColorApproxEntry *capx_info;
{
    int (*old_error_handler)();

    static PEXLineBundleEntry  lineEntry = {
	PEXLineTypeDashDot, PEXPolylineInterpColor, 
	{PEXApproxImpDep, 0, 0.0}, 5.0,
    };
    static PEXMarkerBundleEntry  markerEntry = {
	PEXMarkerCircle, 0, 2.0,
    };
    static PEXEdgeBundleEntry  edgeEntry = {
	PEXOn, 0, PEXSurfaceEdgeDashDot, 5.0,
    };
    static PEXInteriorBundleEntry  interiorEntry = {
	PEXInteriorStyleSolid, 0, 
	PEXReflectionAmbient, PEXSurfaceInterpColor,
	PEXInteriorStyleSolid, 0, 
	PEXReflectionAmbient, PEXSurfaceInterpColor,
	{PEXApproxImpDep, 0, 0.0, 0.0},
    };
    static PEXReflectionAttributes  reflattrs = {
	0.75, 0.0, 0.0, 0.0, 0.0, 
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
	PEXOn, {0,0,0}, 1.0, 0.0, 1.0, 0.0,
    };

    PEXColorSpecifier colorRed, colorWhite;

    colorRed.type = PEXColorTypeRGB;
    colorRed.value.rgb.red = 1.0;
    colorRed.value.rgb.green = 0.0;
    colorRed.value.rgb.blue = 0.0;

    colorWhite.type = PEXColorTypeRGB;
    colorWhite.value.rgb.red = 1.0;
    colorWhite.value.rgb.green = 1.0;
    colorWhite.value.rgb.blue = 1.0;

    colorApproxLUT = PEXCreateLookupTable(dpy, window, PEXLUTColorApprox);
    PEXSetTableEntries(dpy, colorApproxLUT, 0, 1, PEXLUTColorApprox, capx_info);

    lineLUT = PEXCreateLookupTable(dpy, window, PEXLUTLineBundle);
    lineEntry.color = colorRed;
    PEXSetTableEntries(dpy, lineLUT, 1, 1, PEXLUTLineBundle, &lineEntry);

    markerLUT = PEXCreateLookupTable(dpy, window, PEXLUTMarkerBundle);
    markerEntry.color = colorRed;
    PEXSetTableEntries(dpy, markerLUT, 1, 1, PEXLUTMarkerBundle, &markerEntry);

    edgeLUT = PEXCreateLookupTable(dpy, window, PEXLUTEdgeBundle);
    edgeEntry.color = colorRed;
    PEXSetTableEntries(dpy, edgeLUT, 1, 1, PEXLUTEdgeBundle, &edgeEntry);

    interiorLUT = PEXCreateLookupTable(dpy, window, PEXLUTInteriorBundle);
    interiorEntry.color = interiorEntry.bf_color = colorRed;
    reflattrs.specular_color = colorRed;
    interiorEntry.reflection_attr= interiorEntry.bf_reflection_attr = reflattrs;
    PEXSetTableEntries(dpy, interiorLUT, 1, 1, PEXLUTInteriorBundle, 
	&interiorEntry);

    textLUT = PEXCreateLookupTable(dpy, window, PEXLUTTextBundle);
    textEntry.color = colorRed;
    PEXSetTableEntries(dpy, textLUT, 1, 1, PEXLUTTextBundle, &textEntry);

    lightLUT = PEXCreateLookupTable(dpy, window, PEXLUTLight);
    ambLightEntry.color = colorWhite;
    vecLightEntry.color = colorWhite;
    PEXSetTableEntries(dpy, lightLUT, 1, 1, PEXLUTLight, &ambLightEntry);
    PEXSetTableEntries(dpy, lightLUT, 2, 1, PEXLUTLight, &vecLightEntry);

    dcueLUT = PEXCreateLookupTable(dpy, window, PEXLUTDepthCue);
    dcueEntry.color = colorRed;
    PEXSetTableEntries(dpy, dcueLUT, 1, 1, PEXLUTDepthCue, &dcueEntry);

    fontLUT = PEXCreateLookupTable(dpy, window, PEXLUTTextFont);

    old_error_handler = XSetErrorHandler (local_error_handler);
    patternLUT = PEXCreateLookupTable(dpy, window, PEXLUTPattern);
    XSync(dpy,0);
    XSetErrorHandler (old_error_handler);

    colorLUT = PEXCreateLookupTable(dpy, window, PEXLUTColor);
    {
    /*
     *  Set Entries 0-7 in the Color LUT to Sun Defaults
	 Sun's default color LUT has the following values:
	 0: black
	 1: white
	 2: red
	 3: green
	 4: blue
	 5: yellow
	 6: cyan
	 7: magenta
	 8: white
     */
    PEXColorRGB Black  ={0.0, 0.0, 0.0};
    PEXColorRGB White  ={1.0, 1.0, 1.0};
    PEXColorRGB Red    ={1.0, 0.0, 0.0};
    PEXColorRGB Green  ={0.0, 1.0, 0.0};
    PEXColorRGB Blue   ={0.0, 0.0, 1.0};
    PEXColorRGB Yellow ={1.0, 1.0, 0.0};
    PEXColorRGB Cyan   ={0.0, 1.0, 1.0};
    PEXColorRGB Magenta={1.0, 0.0, 1.0};
  
    PEXColorEntry lut_colors[9];
  
    lut_colors[0].type      = PEXColorTypeRGB;
    lut_colors[0].value.rgb = Black;
  
    lut_colors[1].type      = PEXColorTypeRGB;
    lut_colors[1].value.rgb = White;
  
    lut_colors[2].type      = PEXColorTypeRGB;
    lut_colors[2].value.rgb = Red;
  
    lut_colors[3].type      = PEXColorTypeRGB;
    lut_colors[3].value.rgb = Green;
  
    lut_colors[4].type      = PEXColorTypeRGB;
    lut_colors[4].value.rgb = Blue;
  
    lut_colors[5].type      = PEXColorTypeRGB;
    lut_colors[5].value.rgb = Yellow;
  
    lut_colors[6].type      = PEXColorTypeRGB;
    lut_colors[6].value.rgb = Cyan;
  
    lut_colors[7].type      = PEXColorTypeRGB;
    lut_colors[7].value.rgb = Magenta;
  
    lut_colors[8].type      = PEXColorTypeRGB;
    lut_colors[8].value.rgb = White;
      
    PEXSetTableEntries(dpy, colorLUT, 0, 9, PEXLUTColor, 
		       (PEXPointer) lut_colors);

    }

}
