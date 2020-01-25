
/************************************************************************
 *
 * conflict.h --
 *
 *
 * Copyright 1992, 1993 Data General Corporation;
 * Copyright 1992, 1993 OMRON Corporation  
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that neither the name OMRON or DATA GENERAL be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission of the party whose name is to be used.  Neither 
 * OMRON or DATA GENERAL make any representation about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.  
 *
 * OMRON AND DATA GENERAL EACH DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,
 * IN NO EVENT SHALL OMRON OR DATA GENERAL BE LIABLE FOR ANY SPECIAL, INDIRECT
 * OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 *
 ************************************************************************/

/* $XConsortium: conflict.h,v 1.1 94/03/17 11:35:13 dpw Exp $ */

#ifndef CONFLICT_H
#define CONFLICT_H


/***********************************************
 * Aliases for Read/Write conflict bits so that 
 * a write lock also implies a read lock.
 ***********************************************/

#define CM_R_HIERARCHY        POQ_R_HIERARCHY
#define CM_W_HIERARCHY        POQ_R_HIERARCHY | POQ_W_HIERARCHY
#define CM_R_GEOMETRY         POQ_R_GEOMETRY
#define CM_W_GEOMETRY         POQ_R_GEOMETRY | POQ_W_GEOMETRY
#define CM_R_COLORMAP         POQ_R_COLORMAP
#define CM_W_COLORMAP         POQ_R_COLORMAP | POQ_W_COLORMAP
#define CM_R_EVENT_PROP       POQ_R_EVENT_PROP
#define CM_W_EVENT_PROP       POQ_R_EVENT_PROP | POQ_W_EVENT_PROP
#define CM_R_SCREENSAVER      POQ_R_SCREENSAVER
#define CM_W_SCREENSAVER      POQ_R_SCREENSAVER | POQ_W_SCREENSAVER
#define CM_R_FONTPATH         POQ_R_FONTPATH
#define CM_W_FONTPATH         POQ_R_FONTPATH | POQ_W_FONTPATH

/*********************************
 * Write only conflict mask bits.
 *********************************/

#ifdef USE_SOFTWARE_CURSOR
#define CM_X_CURSOR           POQ_X_CURSOR | POQ_X_RENDER
#else
#define CM_X_CURSOR           POQ_X_CURSOR
#endif
#define CM_X_RENDER           POQ_X_RENDER
#define CM_X_ICCCM            POQ_X_ICCCM
#define CM_X_SERVER           POQ_X_SERVER
#define CM_X_FONT             POQ_X_FONT

#define CM_CONFIGURE          POQ_CONFIGURE
#define CM_NO_CONFLICT        POQ_NO_CONFLICT
#define CM_GRAB_SERVER        ((Mask)~0L)

/*********************************************
 * Used to indicate that the extension
 * conflict bits should be checked. 
 *********************************************/

#define CM_EXTENSION_BIT      POQ_EXTENSION_BIT


/***************************************************************
 * The following are commonly referenced conflicts groups.
 ***************************************************************/

#define CM_XRender      (CM_R_GEOMETRY | CM_X_RENDER)

#define CM_XEvent	(CM_R_HIERARCHY | CM_R_GEOMETRY | CM_R_EVENT_PROP)

#define CM_XEventCursor (CM_XEvent | CM_X_CURSOR)

#define CM_XExposure    (CM_W_HIERARCHY | CM_W_GEOMETRY | CM_W_EVENT_PROP | \
			 CM_X_RENDER)


/**************************************************************************
 * Conflict masks for individual protocol requests (catagorized by object) 
 **************************************************************************/

/*------------------------ Window ---------------------------------------*/

#define CM_XCreateWindow           (CM_W_HIERARCHY | CM_R_COLORMAP |\
				    CM_X_CURSOR | CM_R_SCREENSAVER)
#define CM_XChangeWindowAttributes (CM_XExposure | CM_W_COLORMAP | CM_X_CURSOR)
#define CM_XGetWindowAttributes    (CM_R_GEOMETRY | CM_W_EVENT_PROP |\
				    CM_R_COLORMAP | CM_X_CURSOR)
#define CM_XDestroyWindow	   (CM_XExposure)
#define CM_XDestroySubwindows      (CM_XExposure)
#define CM_XChangeSaveSet	   (CM_W_EVENT_PROP)
#define CM_XReparentWindow	   (CM_W_HIERARCHY | CM_W_GEOMETRY |\
				    CM_X_RENDER | CM_R_SCREENSAVER)
#define CM_XMapWindow		   (CM_XExposure)
#define CM_XMapSubwindows	   (CM_XExposure)
#define CM_XUnmapWindow		   (CM_XExposure)
#define CM_XUnmapSubwindows	   (CM_XExposure | CM_R_SCREENSAVER)
#define CM_XConfigureWindow	   (CM_XExposure | CM_CONFIGURE)
#define CM_XCirculateWindow	   (CM_XExposure | CM_R_SCREENSAVER)
#define CM_XGetGeometry		   (CM_R_GEOMETRY)
#define CM_XQueryTree		   (CM_W_HIERARCHY | CM_R_SCREENSAVER)
#define CM_XTranslateCoords	   (CM_R_HIERARCHY)

/*------------------------ Atom -----------------------------------------*/

#define CM_XInternAtom		   (CM_X_ICCCM)
#define CM_XGetAtomName		   (CM_X_ICCCM)

/*------------------------ Property -------------------------------------*/

#define CM_XDeleteProperty	   (CM_X_ICCCM)
#define CM_XRotateProperties	   (CM_X_ICCCM | CM_W_EVENT_PROP)
#define CM_XChangeProperty	   (CM_X_ICCCM)
#define CM_XGetProperty		   (CM_X_ICCCM | CM_W_EVENT_PROP)
#define CM_XListProperties	   (CM_X_ICCCM | CM_W_EVENT_PROP)

/*------------------------ Selection ------------------------------------*/

#define CM_XSetSelectionOwner	   (CM_X_ICCCM | CM_W_EVENT_PROP)
#define CM_XGetSelectionOwner	   (CM_X_ICCCM | CM_W_EVENT_PROP)
#define CM_XConvertSelection	   (CM_X_ICCCM | CM_W_EVENT_PROP)

/*------------------------ Event ----------------------------------------*/

#define CM_XSendEvent              (CM_XEventCursor)
#define CM_XAllowEvents            (CM_XEventCursor)

/*------------------------ Pointer --------------------------------------*/

#define CM_XWarpPointer            (CM_XEventCursor)
#define CM_XChangeActivePointerGrab (CM_XEventCursor)
#define CM_XGrabPointer            (CM_XEventCursor)
#define CM_XUngrabPointer          (CM_XEventCursor)
#define CM_XQueryPointer	   (CM_XEvent)
#define CM_XSetPointerMapping      (CM_NO_CONFLICT)
#define CM_XGetPointerMapping      (CM_NO_CONFLICT)
#define CM_XChangePointerControl   (CM_NO_CONFLICT)
#define CM_XGetPointerControl      (CM_NO_CONFLICT)

/*------------------------ Button ---------------------------------------*/

#define CM_XGrabButton             (CM_X_CURSOR)
#define CM_XUngrabButton           (CM_W_EVENT_PROP)

/*------------------------ Keyboard -------------------------------------*/

#define CM_XGrabKeyboard           (CM_XEventCursor)
#define CM_XUngrabKeyboard         (CM_XEventCursor)
#define CM_XUngrabKey              (CM_W_EVENT_PROP)
#define CM_XGrabKey                (CM_W_EVENT_PROP)
#define CM_XSetModifierMapping     (CM_NO_CONFLICT)
#define CM_XGetModifierMapping	   (CM_NO_CONFLICT)
#define CM_XGetKeyboardMapping     (CM_NO_CONFLICT)
#define CM_XChangeKeyboardMapping  (CM_NO_CONFLICT)
#define CM_XChangeKeyboardControl  (CM_NO_CONFLICT)
#define CM_XGetKeyboardControl     (CM_NO_CONFLICT)
#define CM_XBell                   (CM_NO_CONFLICT)
#define CM_XQueryKeymap            (CM_NO_CONFLICT)
#define CM_XGetMotionEvents        (CM_NO_CONFLICT)

/*------------------------ Focus ----------------------------------------*/

#define CM_XSetInputFocus          (CM_XEventCursor)
#define CM_XGetInputFocus          (CM_R_GEOMETRY)

/*------------------------ Server ---------------------------------------*/

#define CM_XGrabServer	           (CM_GRAB_SERVER)
#define CM_XUngrabServer           (CM_NO_CONFLICT)

/*------------------------ Font -----------------------------------------*/

#define CM_XOpenFont		   (CM_X_FONT | CM_R_FONTPATH)
#define CM_XCloseFont		   (CM_NO_CONFLICT)
#define CM_XQueryFont		   (CM_NO_CONFLICT)
#define CM_XQueryTextExtents	   (CM_NO_CONFLICT)
#define CM_XListFonts		   (CM_R_FONTPATH)
#define CM_XListFontsWithInfo	   (CM_R_FONTPATH)
#define CM_XSetFontPath		   (CM_X_FONT | CM_W_FONTPATH)
#define CM_XGetFontPath		   (CM_R_FONTPATH)

/*------------------------ Pixmap ---------------------------------------*/

#define CM_XCreatePixmap	   (CM_NO_CONFLICT)
#define CM_XFreePixmap		   (CM_NO_CONFLICT)

/*------------------------ GC -------------------------------------------*/

#define CM_XCreateGC		   (CM_NO_CONFLICT)
#define CM_XChangeGC		   (CM_NO_CONFLICT)
#define CM_XCopyGC		   (CM_NO_CONFLICT)
#define CM_XSetDashes		   (CM_NO_CONFLICT)
#define CM_XSetClipRectangles	   (CM_NO_CONFLICT)
#define CM_XFreeGC		   (CM_NO_CONFLICT)

/*------------------------ GC Area --------------------------------------*/

#define CM_XClearToBackground	   (CM_XRender)
#define CM_XCopyArea		   (CM_XRender)
#define CM_XCopyPlane		   (CM_XRender)

/*------------------------ GC 2D ----------------------------------------*/

#define CM_XPolyPoint		   (CM_XRender)
#define CM_XPolyLine		   (CM_XRender)
#define CM_XPolySegment		   (CM_XRender)
#define CM_XPolyRectangle 	   (CM_XRender)
#define CM_XPolyArc		   (CM_XRender)
#define CM_XFillPoly		   (CM_XRender)
#define CM_XPolyFillRectangle	   (CM_XRender)
#define CM_XPolyFillArc		   (CM_XRender)

/*------------------------ Image ----------------------------------------*/

#define CM_XPutImage		   (CM_XRender)
#define CM_XGetImage		   (CM_XRender)

/*------------------------ Text -----------------------------------------*/

#define CM_XPolyText		   (CM_XRender)
#define CM_XImageText8		   (CM_XRender)
#define CM_XImageText16		   (CM_XRender)

/*------------------------ Colormap -------------------------------------*/

#define CM_XCreateColormap	   (CM_R_COLORMAP)
#define CM_XFreeColormap	   (CM_W_COLORMAP)
#define CM_XCopyColormapAndFree	   (CM_W_COLORMAP)
#define CM_XInstallColormap	   (CM_W_COLORMAP)
#define CM_XUninstallColormap	   (CM_W_COLORMAP)
#define CM_XListInstalledColormaps (CM_R_COLORMAP)
#define CM_XAllocColor		   (CM_W_COLORMAP)
#define CM_XAllocNamedColor	   (CM_W_COLORMAP)
#define CM_XAllocColorCells	   (CM_W_COLORMAP)
#define CM_XAllocColorPlanes	   (CM_W_COLORMAP)
#define CM_XFreeColors		   (CM_W_COLORMAP)
#define CM_XStoreColors		   (CM_W_COLORMAP)
#define CM_XStoreNamedColor	   (CM_W_COLORMAP)
#define CM_XQueryColors		   (CM_W_COLORMAP)
#define CM_XLookupColor		   (CM_W_COLORMAP)

/*------------------------ Cursor ---------------------------------------*/

#define CM_XCreateCursor	   (CM_X_CURSOR)
#define CM_XCreateGlyphCursor	   (CM_X_CURSOR)
#define CM_XFreeCursor		   (CM_X_CURSOR)
#define CM_XRecolorCursor          (CM_X_CURSOR | CM_W_EVENT_PROP)
#define CM_XQueryBestSize   	   (CM_R_GEOMETRY)

/*------------------------ Extension ------------------------------------*/

#define CM_XQueryExtension	   (CM_NO_CONFLICT)
#define CM_XListExtensions	   (CM_NO_CONFLICT)

/*------------------------ Screen Saver ---------------------------------*/

#define CM_XSetScreenSaver         (CM_W_SCREENSAVER)
#define CM_XGetScreenSaver	   (CM_R_SCREENSAVER)
#define CM_XForceScreenSaver	   (CM_W_SCREENSAVER)

/*------------------------ Access Control -------------------------------*/

#define CM_XChangeHosts		   (CM_X_SERVER)
#define CM_XListHosts		   (CM_X_SERVER)
#define CM_XChangeAccessControl	   (CM_X_SERVER)

/*------------------------ Client ---------------------------------------*/

#define CM_XKillClient		   (CM_GRAB_SERVER)
#define CM_XChangeCloseDownMode    (CM_X_SERVER)

/*------------------------ Misc -----------------------------------------*/

#define CM_XNoOperation		   (CM_NO_CONFLICT)
#define CM_XBadRequest		   (CM_NO_CONFLICT)

#define CM_XDeviceInputThread	   (CM_X_CURSOR | CM_W_EVENT_PROP | \
				    CM_W_SCREENSAVER)


#endif /* CONFLICT_H */
